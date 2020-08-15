/*
 * IReverse.cpp
 *
 *  Copyright (c) 2019 hikyuu.org
 *
 *  Created on: 2019-5-1
 *      Author: fasiondog
 */

#include "IReverse.h"

#if HKU_SUPPORT_SERIALIZATION
BOOST_CLASS_EXPORT(hku::IReverse)
#endif

namespace hku {

IReverse::IReverse() : IndicatorImp("REVERSE", 1) {}

IReverse::~IReverse() {}

bool IReverse::check() {
    return true;
}

void IReverse::_calculate(const Indicator& data) {
    size_t total = data.size();
    m_discard = data.discard();
    if (m_discard >= total) {
        m_discard = total;
        return;
    }

    for (size_t i = m_discard; i < total; ++i) {
        _set(-data[i], i);
    }
}

Indicator HKU_API REVERSE() {
    return Indicator(make_shared<IReverse>());
}

} /* namespace hku */
