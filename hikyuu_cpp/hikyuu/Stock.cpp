/*
 * Stock.cpp
 *
 *  Created on: 2011-11-9
 *      Author: fasiondog
 */

#include "GlobalInitializer.h"
#include "StockManager.h"
#include "data_driver/KDataDriver.h"
#include "data_driver/HistoryFinanceReader.h"
#include "utilities/util.h"
#include "KData.h"

namespace hku {

const string Stock::default_market;
const string Stock::default_code;
const string Stock::default_market_code;
const string Stock::default_name;
const uint32 Stock::default_type = Null<uint32>();
const bool Stock::default_valid = false;
const Datetime Stock::default_startDate;  // = Null<Datetime>();
const Datetime Stock::default_lastDate;   // = Null<Datetime>();
const price_t Stock::default_tick = 0.01;
const price_t Stock::default_tickValue = 0.01;
const price_t Stock::default_unit = 1.0;
const int Stock::default_precision = 2;
const size_t Stock::default_minTradeNumber = 100;
const size_t Stock::default_maxTradeNumber = 1000000;

HKU_API std::ostream& operator<<(std::ostream& os, const Stock& stock) {
    string strip(", ");
    const StockManager& sm = StockManager::instance();
    StockTypeInfo typeInfo(sm.getStockTypeInfo(stock.type()));
    os << "Stock(" << stock.market() << strip << stock.code() << strip
#if defined(_MSC_VER) && (PY_VERSION_HEX >= 0x03000000)
       << utf8_to_gb(stock.name()) << strip << utf8_to_gb(typeInfo.description()) << strip
#else
       << stock.name() << strip << typeInfo.description() << strip
#endif
       << stock.valid() << strip << stock.startDatetime() << strip << stock.lastDatetime() << ")";
    return os;
}

string Stock::toString() const {
    std::stringstream os;
    string strip(", ");
    const StockManager& sm = StockManager::instance();
    StockTypeInfo typeInfo(sm.getStockTypeInfo(type()));
    os << "Stock(" << market() << strip << code() << strip << name() << strip
       << typeInfo.description() << strip << valid() << strip << startDatetime() << strip
       << lastDatetime() << ")";
    return os.str();
}

Stock::Data::Data()
: m_market(default_market),
  m_code(default_code),
  m_name(default_name),
  m_type(default_type),
  m_valid(default_valid),
  m_startDate(default_startDate),
  m_lastDate(default_lastDate),
  m_tick(default_tick),
  m_tickValue(default_tickValue),
  m_unit(default_unit),
  m_precision(default_precision),
  m_minTradeNumber(default_minTradeNumber),
  m_maxTradeNumber(default_maxTradeNumber) {
    /*for (int i = 0; i < KQuery::INVALID_KTYPE; ++i) {
        pKData[i] = KRecordListPtr();
    }*/
}

Stock::Data::Data(const string& market, const string& code, const string& name, uint32 type,
                  bool valid, const Datetime& startDate, const Datetime& lastDate, price_t tick,
                  price_t tickValue, int precision, size_t minTradeNumber, size_t maxTradeNumber)
: m_market(market),
  m_code(code),
  m_name(name),
  m_type(type),
  m_valid(valid),
  m_startDate(startDate),
  m_lastDate(lastDate),
  m_tick(tick),
  m_tickValue(tickValue),
  m_precision(precision),
  m_minTradeNumber(minTradeNumber),
  m_maxTradeNumber(maxTradeNumber) {
    if (0.0 == m_tick) {
        HKU_WARN("tick should not be zero!");
        m_unit = 1.0;
    } else {
        m_unit = m_tickValue / m_tick;
    }

    to_upper(m_market);
    m_market_code = m_market + m_code;
    /*for (int i = 0; i < KQuery::INVALID_KTYPE; ++i) {
        pKData[i] = KRecordListPtr();
    }*/
}

Stock::Data::~Data() {
    /*for (int i = 0; i < KQuery::INVALID_KTYPE; ++i) {
        delete pKData[i];
    }*/
}

Stock::Stock() {}

Stock::~Stock() {}

Stock::Stock(const Stock& x) : m_data(x.m_data), m_kdataDriver(x.m_kdataDriver) {}

Stock& Stock::operator=(const Stock& x) {
    if (this == &x)
        return *this;
    m_data = x.m_data;
    m_kdataDriver = x.m_kdataDriver;
    return *this;
}

Stock::Stock(const string& market, const string& code, const string& name) {
    m_data =
      shared_ptr<Data>(new Data(market, code, name, default_type, default_valid, default_startDate,
                                default_lastDate, default_tick, default_tickValue,
                                default_precision, default_minTradeNumber, default_maxTradeNumber));
}

Stock::Stock(const string& market, const string& code, const string& name, uint32 type, bool valid,
             const Datetime& startDate, const Datetime& lastDate) {
    m_data = shared_ptr<Data>(new Data(market, code, name, type, valid, startDate, lastDate,
                                       default_tick, default_tickValue, default_precision,
                                       default_minTradeNumber, default_maxTradeNumber));
}

Stock::Stock(const string& market, const string& code, const string& name, uint32 type, bool valid,
             const Datetime& startDate, const Datetime& lastDate, price_t tick, price_t tickValue,
             int precision, size_t minTradeNumber, size_t maxTradeNumber)
: m_data(make_shared<Data>(market, code, name, type, valid, startDate, lastDate, tick, tickValue,
                           precision, minTradeNumber, maxTradeNumber)) {}

bool Stock::operator!=(const Stock& stock) const {
    if (this == &stock)
        return false;

    if (m_data == stock.m_data)
        return false;

    if (!m_data || !stock.m_data)
        return true;

    if (m_data->m_code != stock.code() || m_data->m_market != stock.market())
        return true;

    return false;
}

const string& Stock::market() const {
    return m_data ? m_data->m_market : default_market;
}

const string& Stock::code() const {
    return m_data ? m_data->m_code : default_code;
}

const string& Stock::market_code() const {
    return m_data ? m_data->m_market_code : default_market_code;
}

const string& Stock::name() const {
    return m_data ? m_data->m_name : default_name;
}

uint32 Stock::type() const {
    return m_data ? m_data->m_type : default_type;
}

bool Stock::valid() const {
    return m_data ? m_data->m_valid : default_valid;
}

Datetime Stock::startDatetime() const {
    return m_data ? m_data->m_startDate : default_startDate;
}

Datetime Stock::lastDatetime() const {
    return m_data ? m_data->m_lastDate : default_lastDate;
}

price_t Stock::tick() const {
    return m_data ? m_data->m_tick : default_tick;
}

price_t Stock::tickValue() const {
    return m_data ? m_data->m_tickValue : default_tickValue;
}

price_t Stock::unit() const {
    return m_data ? m_data->m_unit : default_unit;
}

int Stock::precision() const {
    return m_data ? m_data->m_precision : default_precision;
}

size_t Stock::atom() const {
    return m_data ? m_data->m_minTradeNumber : default_minTradeNumber;
}

size_t Stock::minTradeNumber() const {
    return m_data ? m_data->m_minTradeNumber : default_minTradeNumber;
}

size_t Stock::maxTradeNumber() const {
    return m_data ? m_data->m_maxTradeNumber : default_maxTradeNumber;
}

void Stock::setKDataDriver(const KDataDriverPtr& kdataDriver) {
    HKU_CHECK(kdataDriver, "kdataDriver is nullptr!");
    m_kdataDriver = kdataDriver;
    m_data->pKData.clear();
}

void Stock::setWeightList(const StockWeightList& weightList) {
    if (m_data)
        m_data->m_weightList = weightList;
}

KDataDriverPtr Stock::getKDataDriver() const {
    return m_kdataDriver;
}

bool Stock::isBuffer(KQuery::KType ktype) const {
    if (!m_data)
        return false;
    string nktype(ktype);
    to_upper(ktype);
    return m_data->pKData.find(ktype) != m_data->pKData.end() ? true : false;
}

bool Stock::isNull() const {
    if (!m_data || !m_kdataDriver) {
        return true;
    }
    return false;
}

void Stock::releaseKDataBuffer(KQuery::KType inkType) {
    // if (!m_data || kType >= KQuery::INVALID_KTYPE)
    if (!m_data)
        return;

    // if (m_data->pKData[kType])
    // if (m_data->pKData.find(kType) != m_data->pKData.end())
    // m_data->pKData[kType] = KRecordListPtr();
    string kType(inkType);
    to_upper(kType);
    m_data->pKData.erase(kType);
    return;
}

void Stock::loadKDataToBuffer(KQuery::KType inkType) {
    // if (!m_data || kType >= KQuery::INVALID_KTYPE)
    if (!m_data)
        return;

    string kType(inkType);
    to_upper(kType);

    releaseKDataBuffer(kType);
    m_data->pKData[kType] = make_shared<KRecordList>();
    if (m_kdataDriver) {
        m_kdataDriver->loadKData(m_data->m_market, m_data->m_code, kType, 0, Null<size_t>(),
                                 m_data->pKData[kType]);
    }
    return;
}

StockWeightList Stock::getWeight(const Datetime& start, const Datetime& end) const {
    StockWeightList result;
    if (!m_data || start >= end)
        return result;

    StockWeightList::const_iterator start_iter, end_iter;
    start_iter = lower_bound(m_data->m_weightList.begin(), m_data->m_weightList.end(), start,
                             std::less<StockWeight>());
    if (start_iter == m_data->m_weightList.end()) {
        return result;
    }

    end_iter = lower_bound(start_iter, (StockWeightList::const_iterator)m_data->m_weightList.end(),
                           end, std::less<StockWeight>());
    for (; start_iter != end_iter; ++start_iter) {
        result.push_back(*start_iter);
    }

    return result;
}

KData Stock::getKData(const KQuery& query) const {
    return KData(*this, query);
}

size_t Stock::getCount(KQuery::KType kType) const {
    // if (!m_data || kType >= KQuery::INVALID_KTYPE)
    if (!m_data)
        return 0;

    string nktype(kType);
    to_upper(nktype);
    if (m_data->pKData.find(nktype) != m_data->pKData.end()) {
        return m_data->pKData[nktype]->size();
    }

    return m_kdataDriver ? m_kdataDriver->getCount(market(), code(), nktype) : 0;
}

price_t Stock::getMarketValue(const Datetime& datetime, KQuery::KType inktype) const {
    if (isNull()) {
        return 0.0;
    }

    if (!valid()) {
        if (datetime > lastDatetime()) {
            return 0.0;
        }
    }

    string ktype(inktype);
    to_upper(ktype);

    KQuery query = KQueryByDate(datetime, Null<Datetime>(), ktype);
    price_t price = 0.0;
    size_t out_start, out_end;
    KRecord k;
    if (getIndexRange(query, out_start, out_end)) {
        //找到的是>=datetime的记录
        k = getKRecord(out_start, ktype);
        if (k.datetime == datetime) {
            price = k.closePrice;
        } else {
            if (out_start != 0) {
                k = getKRecord(out_start - 1, ktype);
                price = k.closePrice;
            }
        }

    } else {
        //没有找到，则取最后一条记录
        size_t total = getCount(ktype);
        if (total > 0) {
            price = getKRecord(total - 1, ktype).closePrice;
        }
    }

    return price;
}

bool Stock::getIndexRange(const KQuery& query, size_t& out_start, size_t& out_end) const {
    out_start = 0;
    out_end = 0;
    if (!m_data || !m_kdataDriver)
        return false;

    if (KQuery::INDEX == query.queryType())
        return _getIndexRangeByIndex(query, out_start, out_end);

    if ((KQuery::DATE != query.queryType())
        //|| (query.kType() >= KQuery::INVALID_KTYPE)
        || query.startDatetime() >= query.endDatetime())
        return false;

    if (isBuffer(query.kType())) {
        return _getIndexRangeByDateFromBuffer(query, out_start, out_end);
    }

    if (!m_kdataDriver->getIndexRangeByDate(m_data->m_market, m_data->m_code, query, out_start,
                                            out_end)) {
        out_start = 0;
        out_end = 0;
        return false;
    }

    return true;
}

bool Stock::_getIndexRangeByIndex(const KQuery& query, size_t& out_start, size_t& out_end) const {
    assert(query.queryType() == KQuery::INDEX);
    out_start = 0;
    out_end = 0;

    size_t total = getCount(query.kType());
    if (0 == total) {
        return false;
    }

    int64 startix, endix;
    startix = query.start();
    if (startix < 0) {
        startix += total;
        if (startix < 0)
            startix = 0;
    }

    endix = query.end();
    if (endix < 0) {
        endix += total;
        if (endix < 0)
            endix = 0;
    }

    size_t null_size_t = Null<size_t>();
    size_t startpos = 0;
    size_t endpos = null_size_t;

    try {
        startpos = boost::numeric_cast<size_t>(startix);
    } catch (...) {
        startpos = null_size_t;
    }

    try {
        endpos = boost::numeric_cast<size_t>(endix);
    } catch (...) {
        endpos = null_size_t;
    }

    if (endpos > total) {
        endpos = total;
    }

    if (startpos >= endpos) {
        return false;
    }

    out_start = startpos;
    out_end = endpos;
    return true;
}

bool Stock::_getIndexRangeByDateFromBuffer(const KQuery& query, size_t& out_start,
                                           size_t& out_end) const {
    out_start = 0;
    out_end = 0;

    //总数为0，视为失败
    size_t total = getCount(query.kType());
    if (0 == total) {
        return false;
    }

    const KRecordList& kdata = *(m_data->pKData[query.kType()]);
    size_t mid, low = 0, high = total - 1;
    size_t startpos, endpos;
    while (low <= high) {
        if (query.startDatetime() > kdata[high].datetime) {
            mid = high + 1;
            break;
        }

        if (kdata[low].datetime >= query.startDatetime()) {
            mid = low;
            break;
        }

        mid = (low + high) / 2;
        if (query.startDatetime() > kdata[mid].datetime) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    if (mid >= total) {
        return false;
    }

    startpos = mid;

    low = mid;
    high = total - 1;
    while (low <= high) {
        if (query.endDatetime() > kdata[high].datetime) {
            mid = high + 1;
            break;
        }

        if (kdata[low].datetime >= query.endDatetime()) {
            mid = low;
            break;
        }

        mid = (low + high) / 2;
        if (query.endDatetime() > kdata[mid].datetime) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    endpos = (mid >= total) ? total : mid;
    if (startpos >= endpos) {
        return false;
    }

    out_start = startpos;
    out_end = endpos;

    return true;
}

KRecord Stock ::getKRecord(size_t pos, KQuery::KType inkType) const {
    if (!m_data)
        return KRecord();

    string kType(inkType);
    to_upper(kType);

    if (m_data->pKData.find(kType) != m_data->pKData.end())
        return m_data->pKData[kType]->at(pos);

    return m_kdataDriver ? m_kdataDriver->getKRecord(market(), code(), pos, kType) : KRecord();
}

KRecordList Stock ::getKRecordList(size_t start_ix, size_t end_ix, KQuery::KType inktype) const {
    KRecordList result;
    if (!m_data)
        return result;

    string ktype(inktype);
    to_upper(ktype);

    // if (m_data->pKData[ktype]) {
    if (m_data->pKData.find(ktype) != m_data->pKData.end()) {
        size_t total = m_data->pKData[ktype]->size();
        if (start_ix >= end_ix || start_ix > total) {
            HKU_WARN("Invalid param! ({}, {})", start_ix, end_ix);
            return result;
        }

        size_t end = end_ix > total ? total : end_ix;
        result.reserve(end - start_ix);
        for (size_t i = start_ix; i < end; ++i) {
            result.push_back((*m_data->pKData[ktype])[i]);
        }
        return result;
    }

    if (!m_kdataDriver) {
        return result;
    }

    KRecordListPtr plist(new KRecordList);
    m_kdataDriver->loadKData(m_data->m_market, m_data->m_code, ktype, start_ix, end_ix, plist);
    size_t total = plist->size();
    result.reserve(total);
    for (size_t i = 0; i < total; i++) {
        result.push_back((*plist)[i]);
    }
    return result;
}

DatetimeList Stock ::getDatetimeList(size_t start, size_t end, KQuery::KType ktype) const {
    DatetimeList result;
    KRecordList kdata = getKRecordList(start, end, ktype);
    size_t total = kdata.size();
    if (0 == total) {
        return result;
    }

    result.reserve(total);
    for (size_t i = 0; i < kdata.size(); ++i) {
        result.push_back(kdata[i].datetime);
    }
    return result;
}

DatetimeList Stock::getDatetimeList(const KQuery& query) const {
    DatetimeList result;
    size_t start = 0, end = 0;
    if (getIndexRange(query, start, end)) {
        result = getDatetimeList(start, end, query.kType());
    }
    return result;
}

TimeLineList Stock::getTimeLineList(const KQuery& query) const {
    return m_kdataDriver ? m_kdataDriver->getTimeLineList(market(), code(), query) : TimeLineList();
}

TransList Stock::getTransList(const KQuery& query) const {
    return m_kdataDriver ? m_kdataDriver->getTransList(market(), code(), query) : TransList();
}

Parameter Stock::getFinanceInfo() const {
    Parameter result;
    if (type() != STOCKTYPE_A) {
        return result;
    }

    BaseInfoDriverPtr driver = StockManager::instance().getBaseInfoDriver();
    if (driver) {
        result = driver->getFinanceInfo(market(), code());
    }

    return result;
}

PriceList Stock::getHistoryFinanceInfo(const Datetime& date) const {
    if (type() == STOCKTYPE_A) {
        StockManager& sm = StockManager::instance();
        HistoryFinanceReader rd(sm.datadir() + "/downloads/finance");
        return rd.getHistoryFinanceInfo(date, market(), code());
    }

    return PriceList();
}

KRecord Stock::getKRecordByDate(const Datetime& datetime, KQuery::KType ktype) const {
    size_t startix = 0, endix = 0;
    KQuery query = KQueryByDate(datetime, Datetime(datetime.number() + 1), ktype);
    if (getIndexRange(query, startix, endix)) {
        return getKRecord(startix, ktype);
    }

    return Null<KRecord>();
}

void Stock::realtimeUpdate(const KRecord& record) {
    // if (!m_data || !m_data->pKData[KQuery::DAY] ||
    if (!isBuffer(KQuery::DAY) || record.datetime == Null<Datetime>()) {
        return;
    }

    if (m_data->pKData[KQuery::DAY]->empty()) {
        m_data->pKData[KQuery::DAY]->push_back(record);
        return;
    }

    KRecord& tmp = m_data->pKData[KQuery::DAY]->back();
    if (tmp.datetime == record.datetime) {
        tmp = record;
    } else if (tmp.datetime < record.datetime) {
        m_data->pKData[KQuery::DAY]->push_back(record);
    } else {
        HKU_INFO("Ignore record, datetime < last record.datetime!");
    }
}

Stock HKU_API getStock(const string& querystr) {
    const StockManager& sm = StockManager::instance();
    return sm.getStock(querystr);
}

}  // namespace hku
