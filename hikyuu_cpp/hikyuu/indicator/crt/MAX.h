/*
 * MAX.h
 *
 *  Created on: 2019年4月8日
 *      Author: fasiondog
 */

#pragma once
#ifndef INDICATOR_CRT_MAX_H_
#define INDICATOR_CRT_MAX_H_

#include "CVAL.h"

namespace hku {

/**
 * 求最大值
 * @details
 * <pre>
 * 用法: MAX(A,B)返回A和B中的较大值
 * 例如: MAX(CLOSE-OPEN,0)表示若收盘价大于开盘价返回它们的差值，否则返回0
 * </pre>
 * @ingroup Indicator
 */
Indicator MAX(const Indicator&, const Indicator&);
Indicator MAX(const Indicator&, price_t val);
Indicator MAX(price_t val, const Indicator& ind);

inline Indicator MAX(const Indicator& ind1, const Indicator& ind2) {
    Indicator result = IF(ind1 > ind2, ind1, ind2);
    result.name("MAX");
    return result;
}

inline Indicator MAX(const Indicator& ind, price_t val) {
    Indicator result = IF(ind > val, ind, val);
    result.name("MAX");
    return result;
}

inline Indicator MAX(price_t val, const Indicator& ind) {
    Indicator result = IF(val > ind, val, ind);
    result.name("MAX");
    return result;
}

}


#endif /* INDICATOR_CRT_MAX_H_ */
