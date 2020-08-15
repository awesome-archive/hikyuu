/*
 * IIntpart.cpp
 *
 *  Copyright (c) 2019 hikyuu.org
 *
 *  Created on: 2019-04-18
 *      Author: fasiondog
 */

#include "../../utilities/util.h"
#include "IIntpart.h"

#if HKU_SUPPORT_SERIALIZATION
BOOST_CLASS_EXPORT(hku::IIntpart)
#endif

namespace hku {

IIntpart::IIntpart() : IndicatorImp("INTPART", 1) {}

IIntpart::~IIntpart() {}

bool IIntpart::check() {
    return true;
}

void IIntpart::_calculate(const Indicator& data) {
    size_t total = data.size();
    m_discard = data.discard();
    if (m_discard >= total) {
        m_discard = total;
        return;
    }

    for (size_t i = m_discard; i < total; ++i) {
        _set(int(data[i]), i);
    }
}

Indicator HKU_API INTPART() {
    return Indicator(make_shared<IIntpart>());
}

} /* namespace hku */
