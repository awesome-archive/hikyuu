﻿/*
 * test_Indicator.cpp
 *
 *  Created on: 2013-4-11
 *      Author: fasiondog
 */

#include "doctest/doctest.h"
#include <hikyuu/indicator/Indicator.h>
#include <hikyuu/indicator/crt/PRICELIST.h>
#include <hikyuu/indicator/crt/KDATA.h>
#include <hikyuu/StockManager.h>

using namespace hku;

/**
 * @defgroup test_indicator_Indicator test_indicator_Indicator
 * @ingroup test_hikyuu_indicator_suite
 * @{
 */

/** @par 检测点 */
TEST_CASE("test_operator_add") {
    /** @arg 正常相加*/
    PriceList d1, d2;
    for (size_t i = 0; i < 10; ++i) {
        d1.push_back(i);
        d2.push_back(i + 1);
    }

    Indicator data1 = PRICELIST(d1);
    Indicator data2 = PRICELIST(d2);
    Indicator result = data1 + data2;

    CHECK_EQ(result.size(), 10);
    CHECK_EQ(result.getResultNumber(), 1);
    CHECK_EQ(result.discard(), 0);
    for (size_t i = 0; i < 10; ++i) {
        CHECK_EQ(result[i], i + i + 1);
    }

    /** @arg 两个待加的ind的size不同，其中一个为0 */
    Indicator data3;
    result = data1 + data3;
    CHECK_UNARY(result.empty());
    CHECK_EQ(result.size(), 0);

    /** @arg 两个待加的ind的size不同，其中一个为0 */
    PriceList d3;
    for (size_t i = 0; i < 20; ++i) {
        d3.push_back(i);
    }
    data3 = PRICELIST(d3);
    result = data1 + data3;
    CHECK_EQ(data1.size(), 10);
    CHECK_EQ(data3.size(), 20);
    CHECK_EQ(result.empty(), false);
    CHECK_EQ(result.size(), 20);
    CHECK_EQ(result.discard(), 10);
    for (size_t i = 0; i < result.discard(); ++i) {
        CHECK_UNARY(std::isnan(result[i]));
    }
    for (size_t i = result.discard(); i < 20; ++i) {
        CHECK_EQ(result[i], i + i - 10);
    }

    /** @arg 两个待加的ind的size相同，但result_number不同 */
    StockManager& sm = StockManager::instance();
    Stock stock = sm.getStock("sh600000");
    KQuery query(0, 10);
    KData kdata = stock.getKData(query);
    Indicator k = KDATA(kdata);
    CHECK_EQ(k.size(), data1.size());
    result = k + data1;
    CHECK_EQ(result.size(), k.size());
    CHECK_EQ(result.getResultNumber(), 1);
    for (size_t i = 0; i < result.size(); ++i) {
        CHECK_EQ(result[i], (k[i] + data1[i]));
    }
}

/** @par 检测点 */
TEST_CASE("test_operator_reduce") {
    /** @arg 正常相减*/
    PriceList d1, d2;
    for (size_t i = 0; i < 10; ++i) {
        d1.push_back(i);
        d2.push_back(i + 1);
    }

    Indicator data1 = PRICELIST(d1);
    Indicator data2 = PRICELIST(d2);
    Indicator result = data1 - data2;
    CHECK_EQ(result.size(), 10);
    CHECK_EQ(result.getResultNumber(), 1);
    CHECK_EQ(result.discard(), 0);
    for (size_t i = 0; i < 10; ++i) {
        CHECK_EQ(result[i], data1[i] - data2[i]);
    }

    /** @arg 两个待减的ind的size不同 */
    Indicator data3;
    result = data1 - data3;
    CHECK_UNARY(result.empty());
    CHECK_EQ(result.size(), 0);

    /** @arg 两个待减的ind的size相同，但result_number不同 */
    StockManager& sm = StockManager::instance();
    Stock stock = sm.getStock("sh600000");
    KQuery query(0, 10);
    KData kdata = stock.getKData(query);
    Indicator k = KDATA(kdata);
    CHECK_EQ(k.size(), data1.size());
    result = k - data1;
    CHECK_EQ(result.size(), k.size());
    for (size_t i = 0; i < result.size(); ++i) {
        CHECK_EQ(result[i], (k[i] - data1[i]));
    }
}

/** @par 检测点 */
TEST_CASE("test_operator_multi") {
    /** @arg 正常相乘*/
    PriceList d1, d2;
    for (size_t i = 0; i < 10; ++i) {
        d1.push_back(i);
        d2.push_back(i + 1);
    }

    Indicator data1 = PRICELIST(d1);
    Indicator data2 = PRICELIST(d2);
    Indicator result = data1 * data2;
    CHECK_EQ(result.size(), 10);
    CHECK_EQ(result.getResultNumber(), 1);
    CHECK_EQ(result.discard(), 0);
    for (size_t i = 0; i < 10; ++i) {
        CHECK_EQ(result[i], data1[i] * data2[i]);
    }

    /** @arg 两个待乘的ind的size不同 */
    Indicator data3;
    result = data1 * data3;
    CHECK_UNARY(result.empty());
    CHECK_EQ(result.size(), 0);

    /** @arg 两个待乘的ind的size相同，但result_number不同 */
    StockManager& sm = StockManager::instance();
    Stock stock = sm.getStock("sh600000");
    KQuery query(0, 10);
    KData kdata = stock.getKData(query);
    Indicator k = KDATA(kdata);
    CHECK_EQ(k.size(), data1.size());
    result = k * data1;
    CHECK_EQ(result.size(), k.size());
    for (size_t i = 0; i < result.size(); ++i) {
        CHECK_EQ(result[i], (k[i] * data1[i]));
    }
}

/** @par 检测点 */
TEST_CASE("test_operator_division") {
    /** @arg 正常相除*/
    PriceList d1, d2;
    for (size_t i = 0; i < 10; ++i) {
        d1.push_back(i);
        d2.push_back(i + 1);
    }

    Indicator data1 = PRICELIST(d1);
    Indicator data2 = PRICELIST(d2);
    Indicator result = data2 / data1;
    CHECK_EQ(result.size(), 10);
    CHECK_EQ(result.getResultNumber(), 1);
    CHECK_EQ(result.discard(), 0);
    for (size_t i = 0; i < 10; ++i) {
        if (data1[i] == 0.0) {
            CHECK_UNARY(std::isnan(result[i]));
        } else {
            CHECK_EQ(result[i], data2[i] / data1[i]);
        }
    }

    /** @arg 两个待除的ind的size不同 */
    Indicator data3;
    result = data1 / data3;
    CHECK_UNARY(result.empty());
    CHECK_EQ(result.size(), 0);

    /** @arg 两个待除的ind的size相同，但result_number不同 */
    StockManager& sm = StockManager::instance();
    Stock stock = sm.getStock("sh600000");
    KQuery query(0, 10);
    KData kdata = stock.getKData(query);
    Indicator k = KDATA(kdata);
    CHECK_EQ(k.size(), data1.size());
    result = k / data1;
    CHECK_EQ(result.size(), k.size());
    for (size_t i = 0; i < result.size(); ++i) {
        if (data1[i] == 0.0) {
            CHECK_UNARY(std::isnan(result[i]));
        } else {
            CHECK_EQ(result[i], (k[i] / data1[i]));
        }
    }
}

/** @par 检测点 */
TEST_CASE("test_operator_mod") {
    /** @arg 正常取模*/
    PriceList d1, d2;
    for (size_t i = 0; i < 10; ++i) {
        d1.push_back(i);
        d2.push_back(i + 2);
    }

    Indicator data1 = PRICELIST(d1);
    Indicator data2 = PRICELIST(d2);
    Indicator result = data2 % data1;
    CHECK_EQ(result.size(), 10);
    CHECK_EQ(result.getResultNumber(), 1);
    CHECK_EQ(result.discard(), 0);
    CHECK_UNARY(std::isnan(result[0]));
    CHECK_EQ(result[1], 0);
    CHECK_EQ(result[2], 0);
    CHECK_EQ(result[3], 2);
    CHECK_EQ(result[4], 2);
    CHECK_EQ(result[5], 2);
    CHECK_EQ(result[6], 2);
    CHECK_EQ(result[7], 2);
    CHECK_EQ(result[8], 2);
    CHECK_EQ(result[9], 2);

    /** @arg 两个待除的ind的size不同 */
    Indicator data3;
    result = data1 % data3;
    CHECK_UNARY(result.empty());
    CHECK_EQ(result.size(), 0);

    /** @arg 两个待除的ind的size相同，但result_number不同 */
    StockManager& sm = StockManager::instance();
    Stock stock = sm.getStock("sh600000");
    KQuery query(0, 10);
    KData kdata = stock.getKData(query);
    Indicator k = KDATA(kdata);
    CHECK_EQ(k.size(), data1.size());
    result = k % data1;
    CHECK_EQ(result.size(), k.size());
    CHECK_UNARY(std::isnan(result[0]));
    CHECK_EQ(result[1], 0);
    CHECK_EQ(result[2], 1);
    CHECK_EQ(result[3], 1);
    CHECK_EQ(result[4], 3);
    CHECK_EQ(result[5], 1);
    CHECK_EQ(result[6], 3);
    CHECK_EQ(result[7], 6);
    CHECK_EQ(result[8], 2);
    CHECK_EQ(result[9], 8);
}

/** @par 检测点 */
TEST_CASE("test_operator_eq") {
    /** @arg 正常相等*/
    PriceList d1, d2;
    for (size_t i = 0; i < 10; ++i) {
        d1.push_back(i);
        d2.push_back(i);
    }

    Indicator data1 = PRICELIST(d1);
    Indicator data2 = PRICELIST(d2);
    Indicator result = (data2 == data1);
    CHECK_EQ(result.size(), 10);
    CHECK_EQ(result.getResultNumber(), 1);
    CHECK_EQ(result.discard(), 0);
    for (size_t i = 0; i < 10; ++i) {
        CHECK_EQ(result[i], true);
    }

    /** @arg 两个ind的size不同 */
    Indicator data3;
    result = (data1 == data3);
    CHECK_UNARY(result.empty());
    CHECK_EQ(result.size(), 0);

    /** @arg 两个ind的size相同，但result_number不同 */
    StockManager& sm = StockManager::instance();
    Stock stock = sm.getStock("sh600000");
    KQuery query(0, 10);
    KData kdata = stock.getKData(query);
    Indicator k = KDATA(kdata);
    CHECK_EQ(k.size(), data1.size());
    result = (k == data1);
    CHECK_EQ(result.size(), k.size());
    for (size_t i = 0; i < result.size(); ++i) {
        CHECK_EQ(result[i], false);
    }
}

/** @par 检测点 */
TEST_CASE("test_operator_ne") {
    /** @arg 正常不相等 */
    PriceList d1, d2;
    for (size_t i = 0; i < 10; ++i) {
        d1.push_back(i);
        d2.push_back(i);
    }

    Indicator data1 = PRICELIST(d1);
    Indicator data2 = PRICELIST(d2);
    Indicator result = (data2 != data1);
    CHECK_EQ(result.size(), 10);
    CHECK_EQ(result.getResultNumber(), 1);
    CHECK_EQ(result.discard(), 0);
    for (size_t i = 0; i < 10; ++i) {
        CHECK_EQ(result[i], false);
    }

    /** @arg 两个ind的size不同  */
    Indicator data3;
    result = (data1 != data3);
    CHECK_UNARY(result.empty());
    CHECK_EQ(result.size(), 0);

    /** @arg 两个ind的size相同，但result_number不同 */
    StockManager& sm = StockManager::instance();
    Stock stock = sm.getStock("sh600000");
    KQuery query(0, 10);
    KData kdata = stock.getKData(query);
    Indicator k = KDATA(kdata);
    CHECK_EQ(k.size(), data1.size());
    result = (k != data1);
    CHECK_EQ(result.size(), k.size());
    for (size_t i = 0; i < result.size(); ++i) {
        CHECK_EQ(result[i], true);
    }
}

/** @par 检测点 */
TEST_CASE("test_operator_gt") {
    PriceList d1, d2, d3;
    for (size_t i = 0; i < 10; ++i) {
        d1.push_back(i);
        d2.push_back(i);
        d3.push_back(i + 1);
    }

    Indicator data1 = PRICELIST(d1);
    Indicator data2 = PRICELIST(d2);
    Indicator data3 = PRICELIST(d3);

    /** @arg ind1 > ind2*/
    Indicator result = (data3 > data1);
    CHECK_EQ(result.size(), 10);
    CHECK_EQ(result.getResultNumber(), 1);
    CHECK_EQ(result.discard(), 0);
    for (size_t i = 0; i < 10; ++i) {
        CHECK_EQ(result[i], 1.0);
    }

    /** @arg ind1 < ind2 */
    result = (data1 > data3);
    for (size_t i = 0; i < 10; ++i) {
        CHECK_EQ(result[i], 0.0);
    }

    /** @arg ind1 == ind2 */
    result = (data1 > data2);
    for (size_t i = 0; i < 10; ++i) {
        CHECK_EQ(result[i], 0.0);
    }

    /** @arg 两个ind的size不同 */
    Indicator data4;
    result = data1 > data4;
    CHECK_UNARY(result.empty());
    CHECK_EQ(result.size(), 0);

    /** @arg 两个ind的size相同，但result_number不同 */
    StockManager& sm = StockManager::instance();
    Stock stock = sm.getStock("sh600000");
    KQuery query(0, 10);
    KData kdata = stock.getKData(query);
    Indicator k = KDATA(kdata);
    CHECK_EQ(k.size(), data1.size());
    result = (k > data1);
    CHECK_EQ(result.size(), k.size());
    for (size_t i = 0; i < result.size(); ++i) {
        CHECK_EQ(result[i], 1.0);
    }
}

/** @par 检测点 */
TEST_CASE("test_operator_ge") {
    PriceList d1, d2, d3;
    for (size_t i = 0; i < 10; ++i) {
        d1.push_back(i);
        d2.push_back(i);
        d3.push_back(i + 1);
    }

    Indicator data1 = PRICELIST(d1);
    Indicator data2 = PRICELIST(d2);
    Indicator data3 = PRICELIST(d3);

    /** @arg ind1 > ind2*/
    Indicator result = (data3 >= data1);
    CHECK_EQ(result.size(), 10);
    CHECK_EQ(result.getResultNumber(), 1);
    CHECK_EQ(result.discard(), 0);
    for (size_t i = 0; i < 10; ++i) {
        CHECK_EQ(result[i], 1.0);
    }

    /** @arg ind1 < ind2 */
    result = (data1 >= data3);
    for (size_t i = 0; i < 10; ++i) {
        CHECK_EQ(result[i], 0.0);
    }

    /** @arg ind1 == ind2 */
    result = (data1 >= data2);
    for (size_t i = 0; i < 10; ++i) {
        CHECK_EQ(result[i], 1.0);
    }

    /** @arg 两个ind的size不同 */
    Indicator data4;
    result = data1 >= data4;
    CHECK_UNARY(result.empty());
    CHECK_EQ(result.size(), 0);

    /** @arg 两个ind的size相同，但result_number不同 */
    StockManager& sm = StockManager::instance();
    Stock stock = sm.getStock("sh600000");
    KQuery query(0, 10);
    KData kdata = stock.getKData(query);
    Indicator k = KDATA(kdata);
    CHECK_EQ(k.size(), data1.size());
    result = (k >= data1);
    CHECK_EQ(result.size(), k.size());
    for (size_t i = 0; i < result.size(); ++i) {
        CHECK_EQ(result[i], 1.0);
    }
}

/** @par 检测点 */
TEST_CASE("test_operator_lt") {
    PriceList d1, d2, d3;
    for (size_t i = 0; i < 10; ++i) {
        d1.push_back(i);
        d2.push_back(i);
        d3.push_back(i + 1);
    }

    Indicator data1 = PRICELIST(d1);
    Indicator data2 = PRICELIST(d2);
    Indicator data3 = PRICELIST(d3);

    /** @arg ind1 > ind2*/
    Indicator result = (data3 < data1);
    CHECK_EQ(result.size(), 10);
    CHECK_EQ(result.getResultNumber(), 1);
    CHECK_EQ(result.discard(), 0);
    for (size_t i = 0; i < 10; ++i) {
        CHECK_EQ(result[i], 0.0);
    }

    /** @arg ind1 < ind2 */
    result = (data1 < data3);
    for (size_t i = 0; i < 10; ++i) {
        CHECK_EQ(result[i], 1.0);
    }

    /** @arg ind1 == ind2 */
    result = (data1 < data2);
    for (size_t i = 0; i < 10; ++i) {
        CHECK_EQ(result[i], 0.0);
    }

    /** @arg 两个ind的size不同 */
    Indicator data4;
    result = data1 < data4;
    CHECK_UNARY(result.empty());
    CHECK_EQ(result.size(), 0);

    /** @arg 两个ind的size相同，但result_number不同 */
    StockManager& sm = StockManager::instance();
    Stock stock = sm.getStock("sh600000");
    KQuery query(0, 10);
    KData kdata = stock.getKData(query);
    Indicator k = KDATA(kdata);
    CHECK_EQ(k.size(), data1.size());
    result = (k < data1);
    CHECK_EQ(result.size(), k.size());
    for (size_t i = 0; i < result.size(); ++i) {
        CHECK_EQ(result[i], 0.0);
    }
}

/** @par 检测点 */
TEST_CASE("test_operator_le") {
    PriceList d1, d2, d3;
    for (size_t i = 0; i < 10; ++i) {
        d1.push_back(i);
        d2.push_back(i);
        d3.push_back(i + 1);
    }

    Indicator data1 = PRICELIST(d1);
    Indicator data2 = PRICELIST(d2);
    Indicator data3 = PRICELIST(d3);

    /** @arg ind1 > ind2*/
    Indicator result = (data3 <= data1);
    CHECK_EQ(result.size(), 10);
    CHECK_EQ(result.getResultNumber(), 1);
    CHECK_EQ(result.discard(), 0);
    for (size_t i = 0; i < 10; ++i) {
        CHECK_EQ(result[i], 0.0);
    }

    /** @arg ind1 < ind2 */
    result = (data1 <= data3);
    for (size_t i = 0; i < 10; ++i) {
        CHECK_EQ(result[i], 1.0);
    }

    /** @arg ind1 == ind2 */
    result = (data1 <= data2);
    for (size_t i = 0; i < 10; ++i) {
        CHECK_EQ(result[i], 1.0);
    }

    /** @arg 两个ind的size不同 */
    Indicator data4;
    result = data1 <= data4;
    CHECK_UNARY(result.empty());
    CHECK_EQ(result.size(), 0);

    /** @arg 两个ind的size相同，但result_number不同 */
    StockManager& sm = StockManager::instance();
    Stock stock = sm.getStock("sh600000");
    KQuery query(0, 10);
    KData kdata = stock.getKData(query);
    Indicator k = KDATA(kdata);
    CHECK_EQ(k.size(), data1.size());
    result = (k <= data1);
    CHECK_EQ(result.size(), k.size());
    for (size_t i = 0; i < result.size(); ++i) {
        CHECK_EQ(result[i], 0.0);
    }
}

/** @par 检测点 */
TEST_CASE("test_getResult_getResultAsPriceList") {
    StockManager& sm = StockManager::instance();
    Stock stock = sm.getStock("sh600000");
    KQuery query;
    KData kdata;
    Indicator ikdata, result1;
    PriceList result2;

    /** @arg 源数据为空 */
    ikdata = KDATA(kdata);
    result1 = ikdata.getResult(0);
    result2 = ikdata.getResultAsPriceList(0);
    CHECK_EQ(result1.size(), 0);
    CHECK_EQ(result2.size(), 0);

    /** @arg result_num参数非法 */
    query = KQuery(0, 10);
    kdata = stock.getKData(query);
    ikdata = KDATA(kdata);
    CHECK_EQ(ikdata.size(), 10);
    result1 = ikdata.getResult(6);
    result2 = ikdata.getResultAsPriceList(6);
    CHECK_EQ(result1.size(), 0);
    CHECK_EQ(result2.size(), 0);

    /** @arg 正常获取 */
    result1 = ikdata.getResult(0);
    result2 = ikdata.getResultAsPriceList(1);
    CHECK_EQ(result1.size(), 10);
    CHECK_EQ(result2.size(), 10);
    CHECK_EQ(result1[0], 29.5);
    CHECK_LT(std::fabs(result1[1] - 27.58), 0.0001);
    CHECK_EQ(result1[9], 26.45);

    CHECK_EQ(result2[0], 29.8);
    CHECK_EQ(result2[1], 28.38);
    CHECK_EQ(result2[9], 26.55);
}

/** @par 检测点 */
TEST_CASE("test_LOGIC_AND") {
    PriceList d1, d2, d3;
    for (size_t i = 0; i < 10; ++i) {
        d1.push_back(0);
        d2.push_back(1);
        d3.push_back(i);
    }

    Indicator data1 = PRICELIST(d1);
    Indicator data2 = PRICELIST(d2);
    Indicator data3 = PRICELIST(d3);

    /** @arg ind1为全0， ind2为全1 */
    Indicator result = data1 & data2;
    CHECK_EQ(result.size(), 10);
    CHECK_EQ(result.getResultNumber(), 1);
    CHECK_EQ(result.discard(), 0);
    for (size_t i = 0; i < 10; ++i) {
        CHECK_EQ(result[i], 0.0);
    }

    /** @arg ind为全0， val为1 */
    /*result = IND_AND(data1, 1.0);
    BOOST_CHECK(result.size() == 10);
    BOOST_CHECK(result.getResultNumber() == 1);
    BOOST_CHECK(result.discard() == 0);
    for (size_t i = 0; i < 10; ++i) {
        BOOST_CHECK(result[i] == 0.0);
    }*/

    /** @arg ind1为全0， ind2为从0开始的整数 */
    result = data1 & data3;
    CHECK_EQ(result.size(), 10);
    CHECK_EQ(result.getResultNumber(), 1);
    CHECK_EQ(result.discard(), 0);
    for (size_t i = 0; i < 10; ++i) {
        CHECK_EQ(result[i], 0.0);
    }

    /** @arg ind1为全1， ind2为从0开始的整数 */
    result = data2 & data3;
    CHECK_EQ(result.size(), 10);
    CHECK_EQ(result.getResultNumber(), 1);
    CHECK_EQ(result.discard(), 0);
    CHECK_EQ(result[0], 0.0);
    for (size_t i = 1; i < 10; ++i) {
        CHECK_EQ(result[i], 1.0);
    }

    /** @arg 两个ind的size不同 */
    Indicator data4;
    result = data1 & data4;
    CHECK_UNARY(result.empty());
    CHECK_EQ(result.size(), 0);
}

/** @par 检测点 */
TEST_CASE("test_LOGIC_OR") {
    PriceList d1, d2, d3;
    for (size_t i = 0; i < 10; ++i) {
        d1.push_back(0);
        d2.push_back(1);
        d3.push_back(i);
    }

    Indicator data1 = PRICELIST(d1);
    Indicator data2 = PRICELIST(d2);
    Indicator data3 = PRICELIST(d3);

    /** @arg ind1为全0， ind2为全1 */
    Indicator result = data1 | data2;
    CHECK_EQ(result.size(), 10);
    CHECK_EQ(result.getResultNumber(), 1);
    CHECK_EQ(result.discard(), 0);
    for (size_t i = 0; i < 10; ++i) {
        CHECK_EQ(result[i], 1.0);
    }

    /** @arg ind1为全0， ind2为从0开始的整数 */
    result = data1 | data3;
    CHECK_EQ(result.size(), 10);
    CHECK_EQ(result.getResultNumber(), 1);
    CHECK_EQ(result.discard(), 0);
    CHECK_EQ(result[0], 0.0);
    for (size_t i = 1; i < 10; ++i) {
        CHECK_EQ(result[i], 1.0);
    }

    /** @arg ind1为全1， ind2为从0开始的整数 */
    result = data2 | data3;
    CHECK_EQ(result.size(), 10);
    CHECK_EQ(result.getResultNumber(), 1);
    CHECK_EQ(result.discard(), 0);
    for (size_t i = 0; i < 10; ++i) {
        CHECK_EQ(result[i], 1.0);
    }

    /** @arg 两个ind的size不同 */
    Indicator data4;
    result = data1 | data4;
    CHECK_UNARY(result.empty());
    CHECK_EQ(result.size(), 0);
}

/** @} */
