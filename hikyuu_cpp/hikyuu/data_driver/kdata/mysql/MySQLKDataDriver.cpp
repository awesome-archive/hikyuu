/*
 * MySQLKDataDriverImp.cpp
 *
 *  Created on: 2014年9月3日
 *      Author: fasiondog
 */

#include <boost/lexical_cast.hpp>
#include "../../../Log.h"
#include "MySQLKDataDriver.h"

namespace hku {

class MySQLCloser {
public:
    void operator()(MYSQL* db) {
        if (db) {
            mysql_close(db);
            delete db;
            db = nullptr;
            // std::cout << "Closed Sqlite3 database!" << std::endl;
            // Cannot use log output when exiting!
            // HKU_TRACE("Closed MySQL database!");
        }
    }
};

MySQLKDataDriver::MySQLKDataDriver() : KDataDriver("mysql"), m_port(3306) {}

bool MySQLKDataDriver::_init() {
    string default_host("127.0.0.1");
    string default_usr("root");
    string default_pwd("");

    try {
        m_host = m_params.get<string>("host");
    } catch (...) {
        m_host = default_host;
        HKU_WARN("Can't get mysql host!");
    }

    try {
        m_port = m_params.get<int>("port");
    } catch (...) {
        m_port = 3306;
    }

    try {
        m_usr = m_params.get<string>("usr");
    } catch (...) {
        m_usr = default_usr;
    }

    try {
        m_pwd = m_params.get<string>("pwd");
    } catch (...) {
        m_pwd = default_pwd;
    }

    shared_ptr<MYSQL> mysql(new MYSQL, MySQLCloser());
    if (!mysql_init(mysql.get())) {
        HKU_ERROR("Initial MySQL handle error!");
        return false;
    }

    if (!mysql_real_connect(mysql.get(), m_host.c_str(), m_usr.c_str(), m_pwd.c_str(), NULL, m_port,
                            NULL, 0)) {
        HKU_ERROR("Failed to connect to database!");
        return false;
    }

    if (mysql_set_character_set(mysql.get(), "utf8")) {
        HKU_ERROR("mysql_set_character_set error!");
        return false;
    }

    m_mysql = mysql;

    return true;
}

bool MySQLKDataDriver::_query(const string& sql_str) {
    int res = mysql_query(m_mysql.get(), sql_str.c_str());
    if (!res) {
        return true;
    }

    //重新连接数据库
    HKU_INFO("MySQL connect invalid, will retry connect!");
    m_mysql = NULL;

    shared_ptr<MYSQL> mysql(new MYSQL, MySQLCloser());
    if (!mysql_init(mysql.get())) {
        HKU_ERROR(" Initial MySQL handle error!");
        return false;
    }

    if (!mysql_real_connect(mysql.get(), m_host.c_str(), m_usr.c_str(), m_pwd.c_str(), NULL, m_port,
                            NULL, 0)) {
        HKU_ERROR(" Failed to connect to database!");
        return false;
    }

    if (mysql_set_character_set(mysql.get(), "utf8")) {
        HKU_ERROR(" mysql_set_character_set error!");
        return false;
    }

    m_mysql = mysql;

    res = mysql_query(m_mysql.get(), sql_str.c_str());
    if (!res) {
        return true;
    }

    HKU_ERROR("mysql_query error! error no: {} {}", res, sql_str);
    return false;
}

string MySQLKDataDriver ::_getTableName(const string& market, const string& code,
                                        KQuery::KType ktype) {
    string table(market + "_" + KQuery::getKTypeName(ktype) + "." + code);
    to_lower(table);
    return table;
}

MySQLKDataDriver::~MySQLKDataDriver() {}

void MySQLKDataDriver::loadKData(const string& market, const string& code, KQuery::KType kType,
                                 size_t start_ix, size_t end_ix, KRecordListPtr out_buffer) {
    if (!m_mysql) {
        return;
    }

    // if (kType >= KQuery::INVALID_KTYPE || start_ix >= end_ix) {
    if (start_ix >= end_ix) {
        HKU_WARN("ktype({}) is invalid or start_ix({}) >= endix({})", kType, start_ix, end_ix);
        return;
    }

    MYSQL_RES* result;
    MYSQL_ROW row;

    string table(_getTableName(market, code, kType));
    std::stringstream buf(std::stringstream::out);
    buf << "select date, open, high, low, close, amount, count from " << table
        << " order by date limit " << start_ix << ", " << (end_ix - start_ix);
    if (!_query(buf.str())) {
        return;
    }

    result = mysql_store_result(m_mysql.get());
    if (!result) {
        HKU_ERROR("mysql_store_result error!");
        return;
    }

    int i = 0;
    while ((row = mysql_fetch_row(result))) {
        try {
            KRecord k;
            uint64 d = boost::lexical_cast<uint64>(row[0]);
            k.datetime = Datetime(d);
            k.openPrice = boost::lexical_cast<price_t>(row[1]);
            k.highPrice = boost::lexical_cast<price_t>(row[2]);
            k.lowPrice = boost::lexical_cast<price_t>(row[3]);
            k.closePrice = boost::lexical_cast<price_t>(row[4]);
            k.transAmount = boost::lexical_cast<price_t>(row[5]);
            k.transCount = boost::lexical_cast<price_t>(row[6]);
            out_buffer->push_back(k);
            i++;
        } catch (...) {
            HKU_INFO("Can't fecth No.{} record in {}", i, table);
            i++;
            continue;
        }
    }

    mysql_free_result(result);
    return;
}

size_t MySQLKDataDriver::getCount(const string& market, const string& code, KQuery::KType kType) {
    size_t result = 0;
    if (!m_mysql) {
        HKU_ERROR("Null m_mysql!");
        return result;
    }

    /*if (kType >= KQuery::INVALID_KTYPE ) {
        HKU_WARN("ktype(" << kType << ") is invalid" << func_name);
        return result;
    }*/

    MYSQL_RES* mysql_result;
    MYSQL_ROW row;

    string table(_getTableName(market, code, kType));
    std::stringstream buf(std::stringstream::out);
    buf << "select count(1) from " << table;
    if (!_query(buf.str())) {
        HKU_ERROR("mysql_query error! ");
        return result;
    }

    mysql_result = mysql_store_result(m_mysql.get());
    if (!mysql_result) {
        HKU_ERROR("mysql_store_result error!");
        return result;
    }

    while ((row = mysql_fetch_row(mysql_result))) {
        try {
            result = boost::lexical_cast<size_t>(row[0]);
        } catch (...) {
            HKU_INFO("Error get record count of {}", table);
            result = 0;
        }
    }

    mysql_free_result(mysql_result);
    return result;
}

bool MySQLKDataDriver::getIndexRangeByDate(const string& market, const string& code,
                                           const KQuery& query, size_t& out_start,
                                           size_t& out_end) {
    out_start = 0;
    out_end = 0;
    if (query.queryType() != KQuery::DATE) {
        HKU_ERROR("queryType must be KQuery::DATE");
        return false;
    }

    if (query.startDatetime() >= query.endDatetime() || query.startDatetime() > (Datetime::max)()) {
        return false;
    }

    MYSQL_RES* mysql_result;
    MYSQL_ROW row;

    string table(_getTableName(market, code, query.kType()));
    std::stringstream buf(std::stringstream::out);
    buf << "select count(1) from " << table << " where date<" << query.startDatetime().number();

    if (!_query(buf.str())) {
        HKU_ERROR("mysql_query error!");
        return false;
    }

    mysql_result = mysql_store_result(m_mysql.get());
    if (!mysql_result) {
        HKU_ERROR("mysql_store_result error!");
        return false;
    }

    while ((row = mysql_fetch_row(mysql_result))) {
        try {
            out_start = boost::lexical_cast<size_t>(row[0]);
        } catch (...) {
            HKU_INFO("Error boost::lexical_cast<size_t> {}", table);
            out_start = 0;
            mysql_free_result(mysql_result);
            return false;
        }
    }

    mysql_free_result(mysql_result);

    buf.str("");
    buf << "select count(1) from " << table << " where date<=" << query.endDatetime().number();
    if (!_query(buf.str())) {
        HKU_ERROR("mysql_query error!");
        return false;
    }

    mysql_result = mysql_store_result(m_mysql.get());
    if (!mysql_result) {
        HKU_ERROR("mysql_store_result error!");
        return false;
    }

    while ((row = mysql_fetch_row(mysql_result))) {
        try {
            out_end = boost::lexical_cast<size_t>(row[0]) - 1;
        } catch (...) {
            HKU_INFO("Error boost::lexical_cast<size_t> {}", table);
            out_end = 0;
            mysql_free_result(mysql_result);
            return false;
        }
    }

    mysql_free_result(mysql_result);
    return true;
}

KRecord MySQLKDataDriver::getKRecord(const string& market, const string& code, size_t pos,
                                     KQuery::KType kType) {
    KRecord result;
    if (!m_mysql) {
        HKU_ERROR("Null m_mysql!");
        return result;
    }

    /*if (kType >= KQuery::INVALID_KTYPE ) {
        HKU_WARN("ktype(" << kType << ") is invalid" << func_name);
        return result;
    }*/

    MYSQL_RES* mysql_result;
    MYSQL_ROW row;

    string table(_getTableName(market, code, kType));
    std::stringstream buf(std::stringstream::out);
    buf << "select date, open, high, low, close, amount, count from " << table
        << " order by date limit " << pos << ", 1";
    if (!_query(buf.str())) {
        HKU_ERROR("mysql_query error! ");
        return result;
    }

    mysql_result = mysql_store_result(m_mysql.get());
    if (!mysql_result) {
        HKU_ERROR("mysql_store_result error!");
        return result;
    }

    while ((row = mysql_fetch_row(mysql_result))) {
        try {
            uint64 d = boost::lexical_cast<uint64>(row[0]);
            result.datetime = Datetime(d);
            result.openPrice = boost::lexical_cast<price_t>(row[1]);
            result.highPrice = boost::lexical_cast<price_t>(row[2]);
            result.lowPrice = boost::lexical_cast<price_t>(row[3]);
            result.closePrice = boost::lexical_cast<price_t>(row[4]);
            result.transAmount = boost::lexical_cast<price_t>(row[5]);
            result.transCount = boost::lexical_cast<price_t>(row[6]);
        } catch (...) {
            HKU_INFO("Error get record {} {}", pos, table);
            result = Null<KRecord>();
        }
    }

    mysql_free_result(mysql_result);
    return result;
}

} /* namespace hku */
