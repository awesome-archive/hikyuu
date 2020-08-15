/*
 * test_Signal.cpp
 *
 *  Created on: 2013-3-11
 *      Author: fasiondog
 */

#include "doctest/doctest.h"
#include <hikyuu/StockManager.h>
#include <hikyuu/trade_sys/system/crt/SYS_Simple.h>
#include <hikyuu/trade_sys/selector/crt/SE_Fixed.h>

using namespace hku;

/**
 * @defgroup test_Selector test_Selector
 * @ingroup test_hikyuu_trade_sys_suite
 * @{
 */

/** @par 检测点 */
TEST_CASE("test_SE_Fixed") {
    StockManager& sm = StockManager::instance();

    SYSPtr sys = SYS_Simple();
    SEPtr se = SE_Fixed();

    /** @arg 试图加入一个不存在的stock */
    se->addStock(Stock(), sys);
    SystemList result = se->getSelectedSystemList(Datetime(200001010000L));
    CHECK_EQ(result.size(), 0);

    /** @arg 试图加入一个空的系统策略原型 */
    se->addStock(sm["sh600000"], SYSPtr());
    result = se->getSelectedSystemList(Datetime(200001010000L));
    CHECK_EQ(result.size(), 0);

    /** @arg getSelectedSystemList */
    se->addStock(sm["sh600000"], sys);
    se->addStock(sm["sz000001"], sys);
    se->addStock(sm["sz000002"], sys);

    se->reset();
    result = se->getSelectedSystemList(Datetime(200001010000L));
    CHECK_EQ(result.size(), 3);
    CHECK_EQ(sm["sh600000"], result[0]->getStock());
    CHECK_EQ(sm["sz000001"], result[1]->getStock());
    CHECK_EQ(sm["sz000002"], result[2]->getStock());

    /** @arg clear */
    se->clear();
    result = se->getSelectedSystemList(Datetime(200001010000L));
    CHECK_EQ(result.size(), 0);

    /** @arg reset */
    se->addStock(sm["sh600000"], sys);
    se->addStock(sm["sz000001"], sys);
    se->addStock(sm["sz000002"], sys);
    se->reset();
    result = se->getSelectedSystemList(Datetime(200001010000L));
    CHECK_EQ(result.size(), 3);
    CHECK_EQ(sm["sh600000"], result[0]->getStock());
    CHECK_EQ(sm["sz000001"], result[1]->getStock());
    CHECK_EQ(sm["sz000002"], result[2]->getStock());

    /** @arg 克隆操作 */
    SEPtr se2;
    se2 = se->clone();
    CHECK_NE(se2.get(), se.get());
    result = se2->getSelectedSystemList(Datetime(200001010000L));
    CHECK_EQ(result.size(), 3);
    CHECK_EQ(sm["sh600000"], result[0]->getStock());
    CHECK_EQ(sm["sz000001"], result[1]->getStock());
    CHECK_EQ(sm["sz000002"], result[2]->getStock());
}

/** @} */
