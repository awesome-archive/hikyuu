/*
 * SelectorBase.cpp
 *
 *  Created on: 2016年2月21日
 *      Author: fasiondog
 */

#include "SelectorBase.h"

namespace hku {

HKU_API std::ostream& operator<<(std::ostream& os, const SelectorBase& st) {
    os << "Selector(" << st.name() << ", " << st.getParameter() << ")";
    return os;
}

HKU_API std::ostream& operator<<(std::ostream& os, const SelectorPtr& st) {
    if (st) {
        os << *st;
    } else {
        os << "Selector(NULL)";
    }

    return os;
}

SelectorBase::SelectorBase() : m_name("SelectorBase"), m_count(0), m_pre_date(Datetime::min()) {}

SelectorBase::SelectorBase(const string& name)
: m_name(name), m_count(0), m_pre_date(Datetime::min()) {}

SelectorBase::~SelectorBase() {}

void SelectorBase::clear() {
    m_count = 0;
    m_pre_date = Datetime::min();
    m_sys_list.clear();
}

void SelectorBase::reset() {
    SystemList::const_iterator iter = m_sys_list.begin();
    for (; iter != m_sys_list.end(); ++iter) {
        (*iter)->reset(true, false);
    }

    m_count = 0;
    m_pre_date = Datetime::min();

    _reset();
}

SelectorPtr SelectorBase::clone() {
    SelectorPtr p;
    try {
        p = _clone();
    } catch (...) {
        HKU_ERROR("Subclass _clone failed!");
        p = SelectorPtr();
    }

    if (!p || p.get() == this) {
        HKU_ERROR("Failed clone! Will use self-ptr!");
        return shared_from_this();
    }

    p->m_params = m_params;

    p->m_name = m_name;
    p->m_count = m_count;
    p->m_pre_date = m_pre_date;

    SystemList::const_iterator iter = m_sys_list.begin();
    for (; iter != m_sys_list.end(); ++iter) {
        // TODO
        p->m_sys_list.push_back((*iter)->clone());
    }

    return p;
}

void SelectorBase::addStock(const Stock& stock, const SystemPtr& protoSys) {
    if (stock.isNull()) {
        HKU_WARN("Try add Null stock, will be discard!");
        return;
    }

    if (!protoSys) {
        HKU_WARN("Try add Null protoSys, will be discard!");
        return;
    }

    SYSPtr sys = protoSys->clone();
    sys->setStock(stock);
    m_sys_list.push_back(sys);
}

void SelectorBase::addStockList(const StockList& stkList, const SystemPtr& protoSys) {
    if (!protoSys) {
        HKU_WARN("Try add Null protoSys, will be discard!");
        return;
    }

    StockList::const_iterator iter = stkList.begin();
    for (; iter != stkList.end(); ++iter) {
        if (iter->isNull()) {
            HKU_WARN("Try add Null stock, will be discard!");
            continue;
        }

        SYSPtr sys = protoSys->clone();
        m_sys_list.push_back(sys);
    }
}

} /* namespace hku */
