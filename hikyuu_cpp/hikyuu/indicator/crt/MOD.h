/*
 * MOD.h
 *
 *  Created on: 2019-5-1
 *      Author: fasiondog
 */

#pragma once
#ifndef INDICATOR_IMP_MOD_H_
#define INDICATOR_IMP_MOD_H_

#include "CVAL.h"

namespace hku {

/**
 * 取整后求模
 * @details
 * <pre>
 * 该函数仅为兼容通达信。实际上，指标求模可直接使用 % 操作符
 * </pre>
 * @param ind1 指标1，将取整
 * @param ind2 指标2，将取整
 * @ingroup Indicator
 */
Indicator MOD(const Indicator& ind1, const Indicator& ind2);
Indicator MOD(const Indicator& ind1, price_t ind2);
Indicator MOD(price_t ind1, const Indicator& ind2);
Indicator MOD(price_t ind1, price_t ind2);

inline Indicator MOD(const Indicator& ind1, const Indicator& ind2) {
    return (ind1 % ind2);
}

inline Indicator MOD(const Indicator& ind1, price_t ind2) {
    return ind1 % CVAL(ind1, ind2);
}

inline Indicator MOD(price_t ind1, const Indicator& ind2) {
    return CVAL(ind2, ind1) % ind2;
}

inline Indicator MOD(price_t ind1, price_t ind2) {
    return CVAL(ind1) % CVAL(ind2);
}

} /* namespace */

#endif /* INDICATOR_IMP_MOD_H_ */
