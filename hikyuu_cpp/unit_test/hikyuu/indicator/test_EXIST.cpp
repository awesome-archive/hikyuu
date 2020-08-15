/*
 * test_EXIST.cpp
 *
 *  Created on: 2019-4-19
 *      Author: fasiondog
 */
#include "doctest/doctest.h"
#include <fstream>
#include <hikyuu/StockManager.h>
#include <hikyuu/indicator/crt/CVAL.h>
#include <hikyuu/indicator/crt/EXIST.h>
#include <hikyuu/indicator/crt/KDATA.h>
#include <hikyuu/indicator/crt/PRICELIST.h>

using namespace hku;

/**
 * @defgroup test_indicator_EXIST test_indicator_EXIST
 * @ingroup test_hikyuu_indicator_suite
 * @{
 */

/** @par 检测点 */
TEST_CASE("test_EXIST") {
    Indicator result;

    PriceList a;
    a.push_back(0);
    a.push_back(1);
    a.push_back(0);
    a.push_back(1);
    a.push_back(1);
    a.push_back(0);

    Indicator data = PRICELIST(a);

    /** @arg n=0 */
    result = EXIST(data, 0);
    CHECK_EQ(result.discard(), data.size() - 1);
    CHECK_EQ(result.size(), data.size());
    CHECK_EQ(result[5], 1);
    for (int i = 0; i < data.discard(); ++i) {
        CHECK_UNARY(std::isnan(result[i]));
    }

    /** @arg n=1, total>1 */
    result = EXIST(data, 1);
    CHECK_EQ(result.name(), "EXIST");
    CHECK_EQ(result.discard(), 0);
    CHECK_EQ(result.size(), data.size());
    for (int i = 0; i < data.size(); ++i) {
        CHECK_EQ(result[i], data[i]);
    }

    /** @arg n=1, total=1 */
    result = EXIST(CVAL(1), 1);
    CHECK_EQ(result.size(), 1);
    CHECK_EQ(result.discard(), 0);
    CHECK_EQ(result[0], 1);

    result = EXIST(CVAL(0), 1);
    CHECK_EQ(result.size(), 1);
    CHECK_EQ(result.discard(), 0);
    CHECK_EQ(result[0], 0);

    /** @arg n > 1, total = n */
    result = EXIST(data, 6);
    CHECK_EQ(result.name(), "EXIST");
    CHECK_EQ(result.size(), data.size());
    CHECK_EQ(result.discard(), 5);
    CHECK_EQ(result[5], 1);

    /** @arg n > 1, total < n */
    result = EXIST(data, 7);
    CHECK_EQ(result.name(), "EXIST");
    CHECK_EQ(result.size(), data.size());
    CHECK_EQ(result.discard(), 6);

    /** @arg n > 1, total > n */
    result = EXIST(data, 3);
    CHECK_EQ(result.name(), "EXIST");
    CHECK_EQ(result.size(), data.size());
    CHECK_EQ(result.discard(), 2);
    for (int i = 0; i < result.discard(); i++) {
        CHECK_UNARY(std::isnan(result[i]));
    }
    CHECK_EQ(result[2], 1);
    CHECK_EQ(result[3], 1);
    CHECK_EQ(result[4], 1);
    CHECK_EQ(result[5], 1);

    a.push_back(0);
    a.push_back(0);
    a.push_back(0);
    a.push_back(0);

    data = PRICELIST(a);
    result = EXIST(data, 3);
    CHECK_EQ(result.name(), "EXIST");
    CHECK_EQ(result.size(), data.size());
    CHECK_EQ(result.discard(), 2);
    for (int i = 0; i < result.discard(); i++) {
        CHECK_UNARY(std::isnan(result[i]));
    }
    CHECK_EQ(result[2], 1);
    CHECK_EQ(result[3], 1);
    CHECK_EQ(result[4], 1);
    CHECK_EQ(result[5], 1);
    CHECK_EQ(result[6], 1);
    CHECK_EQ(result[7], 0);
    CHECK_EQ(result[8], 0);
    CHECK_EQ(result[9], 0);
}

//-----------------------------------------------------------------------------
// test export
//-----------------------------------------------------------------------------
#if HKU_SUPPORT_SERIALIZATION

/** @par 检测点 */
TEST_CASE("test_EXIST_export") {
    StockManager& sm = StockManager::instance();
    string filename(sm.tmpdir());
    filename += "/EXIST.xml";

    Stock stock = sm.getStock("sh000001");
    KData kdata = stock.getKData(KQuery(-20));
    Indicator x1 = EXIST(CLOSE(kdata) > OPEN(kdata));
    {
        std::ofstream ofs(filename);
        boost::archive::xml_oarchive oa(ofs);
        oa << BOOST_SERIALIZATION_NVP(x1);
    }

    Indicator x2;
    {
        std::ifstream ifs(filename);
        boost::archive::xml_iarchive ia(ifs);
        ia >> BOOST_SERIALIZATION_NVP(x2);
    }

    CHECK_EQ(x2.name(), "EXIST");
    CHECK_EQ(x1.size(), x2.size());
    CHECK_EQ(x1.discard(), x2.discard());
    CHECK_EQ(x1.getResultNumber(), x2.getResultNumber());
    for (size_t i = x1.discard(); i < x1.size(); ++i) {
        CHECK_EQ(x1[i], doctest::Approx(x2[i]));
    }
}
#endif /* #if HKU_SUPPORT_SERIALIZATION */

/** @} */
