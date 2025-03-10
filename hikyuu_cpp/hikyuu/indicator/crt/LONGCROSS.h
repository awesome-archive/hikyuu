/*
 * LONGCROSS.h
 *  
 *  Copyright (c) 2019 hikyuu.org
 * 
 *  Created on: 2019-5-2
 *      Author: fasiondog
 */

#pragma once
#ifndef INDICATOR_CRT_LONGCROSS_H_
#define INDICATOR_CRT_LONGCROSS_H_

#include "CVAL.h"
#include "EVERY.h"
#include "REF.h"

namespace hku {

/**
 * 两条线维持一定周期后交叉
 * @details
 * <pre>
 * LONGCROSS(A,B,N) 表示 A 在 N 周期内都小于 B，
 * 本周期从下方向上穿过 B 时返回 1，否则返回 0。
 * </pre>
 * @ingroup Indicator
 */
Indicator LONGCROSS(const Indicator& x, const Indicator& y, int n=3);
Indicator LONGCROSS(const Indicator& x, price_t, int n=3);
Indicator LONGCROSS(price_t, const Indicator& y, int n=3);
Indicator LONGCROSS(price_t, price_t, int n=3);

inline Indicator LONGCROSS(const Indicator& x, const Indicator& y, int n) {
    Indicator result = EVERY((REF(x, 1) < REF(y, 1)), n) & (x > y);
    result.name("LONGCROSS");
    return result;
}

inline Indicator LONGCROSS(const Indicator& x, price_t y, int n) {
    return LONGCROSS(x, CVAL(x, y), n);
}

inline Indicator LONGCROSS(price_t x, const Indicator& y, int n) {
    return LONGCROSS(CVAL(y, x), y, n);
}

inline Indicator LONGCROSS(price_t x, price_t y, int n) {
    return LONGCROSS(CVAL(x), CVAL(y), n);
}

} /* namespace */

#endif /* INDICATOR_CRT_LONGCROSS_H_ */
