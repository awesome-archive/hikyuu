/*
 * test_Slippage.cpp
 *
 *  Created on: 2013-3-21
 *      Author: fasiondog
 */

#include "doctest/doctest.h"
#include <hikyuu/StockManager.h>
#include <hikyuu/trade_sys/slippage/SlippageBase.h>

using namespace hku;

class SlippageTest : public SlippageBase {
public:
    SlippageTest() : SlippageBase("SlippageTest"), m_x(0) {}
    virtual ~SlippageTest() {}

    virtual price_t getRealBuyPrice(const Datetime &datetime, price_t price) {
        return m_x < 10 ? 0.0 : 1.0;
    }

    virtual price_t getRealSellPrice(const Datetime &datetime, price_t price) {
        return m_x < 10 ? 0.0 : 1.0;
    }

    virtual void _reset() {
        m_x = 0;
    }

    virtual SlippagePtr _clone() {
        SlippageTest *p = new SlippageTest;
        p->m_x = m_x;
        return SlippagePtr(p);
    }

    virtual void _calculate() {}

    int getX() const {
        return m_x;
    }
    void setX(int x) {
        m_x = x;
    }

private:
    int m_x;
};

/**
 * @defgroup test_Slippage test_Slippage
 * @ingroup test_hikyuu_trade_sys_suite
 * @{
 */

/** @par 检测点 */
TEST_CASE("test_Slippage") {
    /** @arg 基本操作 */
    SlippagePtr p(new SlippageTest);
    CHECK_EQ(p->name(), "SlippageTest");
    CHECK_EQ(p->getRealBuyPrice(Datetime(200101010000), 1.0), 0.0);
    CHECK_EQ(p->getRealSellPrice(Datetime(200101010000), 1.0), 0.0);
    SlippageTest *p_src = (SlippageTest *)p.get();
    CHECK_EQ(p_src->getX(), 0);

    p_src->setX(10);
    CHECK_EQ(p->getRealBuyPrice(Datetime(200101010000), 1.0), 1.0);
    CHECK_EQ(p->getRealSellPrice(Datetime(200101010000), 1.0), 1.0);
    CHECK_EQ(p_src->getX(), 10);
    p->reset();
    CHECK_EQ(p_src->getX(), 0);

    /** @arg 测试克隆操作 */
    p_src->setX(10);
    SlippagePtr p_clone = p->clone();
    CHECK_EQ(p_clone->name(), "SlippageTest");
    p_src = (SlippageTest *)p_clone.get();
    CHECK_EQ(p_src->getX(), 10);
    CHECK_NE(p, p_clone);
}

/** @} */
