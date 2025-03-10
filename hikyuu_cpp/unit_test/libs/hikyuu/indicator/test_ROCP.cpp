/*
 * test_ROCP.cpp
 *
 *  Copyright (c) 2019, hikyuu.org
 * 
 *  Created on: 2019-5-18
 *      Author: fasiondog
 */

#ifdef TEST_ALL_IN_ONE
    #include <boost/test/unit_test.hpp>
#else
    #define BOOST_TEST_MODULE test_hikyuu_indicator_suite
    #include <boost/test/unit_test.hpp>
#endif

#include <fstream>
#include <hikyuu/StockManager.h>
#include <hikyuu/indicator/crt/ROCP.h>
#include <hikyuu/indicator/crt/KDATA.h>
#include <hikyuu/indicator/crt/PRICELIST.h>

using namespace hku;

/**
 * @defgroup test_indicator_ROCP test_indicator_ROCP
 * @ingroup test_hikyuu_indicator_suite
 * @{
 */

/** @par 检测点 */
BOOST_AUTO_TEST_CASE( test_ROCP ) {
    Indicator result;

    PriceList a;
    for (int i = 0; i < 10; ++i) {
        a.push_back(i);
    }

    Indicator data = PRICELIST(a);
    result = ROCP(data, 1);
    BOOST_CHECK(result.name() == "ROCP");
    BOOST_CHECK(result.size() == data.size());
    BOOST_CHECK(result.discard() == 1);
    BOOST_CHECK(std::isnan(result[0]));
    BOOST_CHECK(result[1] == 0);
    BOOST_CHECK(result[2] == 1);
    BOOST_CHECK(result[3] == 0.5);
    BOOST_CHECK(result[5] == 0.25);
    BOOST_CHECK(result[9] == 0.125);

    result = ROCP(data, 2);
    BOOST_CHECK(result.name() == "ROCP");
    BOOST_CHECK(result.size() == data.size());
    BOOST_CHECK(result.discard() == 2);
    BOOST_CHECK(std::isnan(result[0]));
    BOOST_CHECK(std::isnan(result[1]));
    BOOST_CHECK(result[2] == 0);
    BOOST_CHECK(result[3] == 2);
    BOOST_CHECK(result[4] == 1);
}


//-----------------------------------------------------------------------------
// test export
//-----------------------------------------------------------------------------
#if HKU_SUPPORT_SERIALIZATION

/** @par 检测点 */
BOOST_AUTO_TEST_CASE( test_ROCP_export ) {
    StockManager& sm = StockManager::instance();
    string filename(sm.tmpdir());
    filename += "/ROCP.xml";

    Stock stock = sm.getStock("sh000001");
    KData kdata = stock.getKData(KQuery(-20));
    Indicator x1 = ROCP(CLOSE(kdata));
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

    BOOST_CHECK(x2.name() == "ROCP");
    BOOST_CHECK(x1.size() == x2.size());
    BOOST_CHECK(x1.discard() == x2.discard());
    BOOST_CHECK(x1.getResultNumber() == x2.getResultNumber());
    for (size_t i = x1.discard(); i < x1.size(); ++i) {
        BOOST_CHECK_CLOSE(x1[i], x2[i], 0.00001);
    }
}
#endif /* #if HKU_SUPPORT_SERIALIZATION */

/** @} */


