/*
 * test_SUM.cpp
 *
 *  Created on: 2019年4月2日
 *      Author: fasiondog
 */

#include "doctest/doctest.h"
#include <fstream>
#include <hikyuu/StockManager.h>
#include <hikyuu/indicator/crt/SUM.h>
#include <hikyuu/indicator/crt/KDATA.h>
#include <hikyuu/indicator/crt/PRICELIST.h>

using namespace hku;

/**
 * @defgroup test_indicator_SUM test_indicator_SUM
 * @ingroup test_hikyuu_indicator_suite
 * @{
 */

/** @par 检测点 */
TEST_CASE("test_SUM") {
    Indicator result;

    PriceList a;
    for (int i = 0; i < 10; ++i) {
        a.push_back(i);
    }

    Indicator data = PRICELIST(a);

    /** @arg n = 0 */
    result = SUM(data, 0);
    CHECK_EQ(result.name(), "SUM");
    CHECK_EQ(result.size(), 10);
    CHECK_EQ(result.discard(), 0);
    price_t sum = 0;
    for (int i = 0; i < 10; ++i) {
        sum += data[i];
        CHECK_EQ(result[i], sum);
    }

    /** @arg n = 1 */
    result = SUM(data, 1);
    CHECK_EQ(result.size(), 10);
    CHECK_EQ(result.discard(), 0);
    for (int i = 0; i < 10; ++i) {
        CHECK_EQ(result[i], data[i]);
    }

    /** @arg n = 9 */
    result = SUM(data, 9);
    CHECK_EQ(result.size(), 10);
    CHECK_EQ(result.discard(), 8);
    CHECK_EQ(result[8], 36);
    CHECK_EQ(result[9], 45);

    /** @arg n = 10 */
    result = SUM(data, 10);
    CHECK_EQ(result.size(), 10);
    CHECK_EQ(result.discard(), 9);
    CHECK_EQ(result[9], 45);
}

//-----------------------------------------------------------------------------
// test export
//-----------------------------------------------------------------------------
#if HKU_SUPPORT_SERIALIZATION

/** @par 检测点 */
TEST_CASE("test_SUM_export") {
    StockManager& sm = StockManager::instance();
    string filename(sm.tmpdir());
    filename += "/SUM.xml";

    Stock stock = sm.getStock("sh000001");
    KData kdata = stock.getKData(KQuery(-20));
    Indicator x1 = SUM(CLOSE(kdata), 3);
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

    CHECK_EQ(x1.size(), x2.size());
    CHECK_EQ(x1.discard(), x2.discard());
    CHECK_EQ(x1.getResultNumber(), x2.getResultNumber());
    for (size_t i = x1.discard(); i < x1.size(); ++i) {
        CHECK_EQ(x1[i], doctest::Approx(x2[i]).epsilon(0.00001));
    }
}
#endif /* #if HKU_SUPPORT_SERIALIZATION */

/** @} */
