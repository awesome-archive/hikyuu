/*
 * test_HHV.cpp
 *
 *  Created on: 2019年4月1日
 *      Author: fasiondog
 */

#include "doctest/doctest.h"
#include <fstream>
#include <hikyuu/StockManager.h>
#include <hikyuu/indicator/crt/CVAL.h>
#include <hikyuu/indicator/crt/HHV.h>
#include <hikyuu/indicator/crt/KDATA.h>
#include <hikyuu/indicator/crt/PRICELIST.h>

using namespace hku;

/**
 * @defgroup test_indicator_HHV test_indicator_HHV
 * @ingroup test_hikyuu_indicator_suite
 * @{
 */

/** @par 检测点 */
TEST_CASE("test_HHV") {
    Indicator result;

    PriceList a;
    for (int i = 0; i < 10; ++i) {
        a.push_back(i);
    }

    Indicator data = PRICELIST(a);

    /** @arg n = 0 */
    result = HHV(data, 0);
    CHECK_EQ(result.discard(), 0);
    for (int i = 0; i < 10; ++i) {
        CHECK_EQ(result[i], data[i]);
    }

    /** @arg n = 1 */
    result = HHV(data, 1);
    CHECK_EQ(result.discard(), 0);
    for (int i = 0; i < 10; ++i) {
        CHECK_EQ(result[i], data[i]);
    }

    /** @arg n = 9 */
    result = HHV(data, 9);
    CHECK_EQ(result.discard(), 0);
    CHECK_EQ(result[0], data[0]);
    CHECK_EQ(result[1], data[1]);
    CHECK_EQ(result[7], data[7]);
    CHECK_EQ(result[8], data[8]);
    CHECK_EQ(result[9], data[9]);

    /** @arg n = 10 */
    result = HHV(data, 10);
    CHECK_EQ(result.discard(), 0);
    CHECK_EQ(result[0], data[0]);
    CHECK_EQ(result[1], data[1]);
    CHECK_EQ(result[8], data[8]);
    CHECK_EQ(result[9], data[9]);

    /** ind.size() == 1 */
    result = HHV(CVAL(2));
    CHECK_EQ(result.size(), 1);
    CHECK_EQ(result.discard(), 0);
    CHECK_EQ(result[0], 2);
}

//-----------------------------------------------------------------------------
// test export
//-----------------------------------------------------------------------------
#if HKU_SUPPORT_SERIALIZATION

/** @par 检测点 */
TEST_CASE("test_HHV_export") {
    StockManager& sm = StockManager::instance();
    string filename(sm.tmpdir());
    filename += "/HHV.xml";

    Stock stock = sm.getStock("sh000001");
    KData kdata = stock.getKData(KQuery(-20));
    Indicator x1 = HHV(CLOSE(kdata), 2);
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
    for (size_t i = 0; i < x1.size(); ++i) {
        CHECK_EQ(x1[i], doctest::Approx(x2[i]));
    }
}
#endif /* #if HKU_SUPPORT_SERIALIZATION */

/** @} */
