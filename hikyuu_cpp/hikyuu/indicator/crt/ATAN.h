/*
 * ATAN.h
 *
 *  Copyright (c) 2019 hikyuu.org
 * 
 *  Created on: 2019-5-1
 *      Author: fasiondog
 */

#pragma once
#ifndef INDICATOR_CRT_ATAN_H_
#define INDICATOR_CRT_ATAN_H_

#include "CVAL.h"

namespace hku {

/**
 * 反正切值
 * @ingroup Indicator
 */
Indicator HKU_API ATAN();
Indicator ATAN(price_t);
Indicator ATAN(const Indicator& ind);

inline Indicator ATAN(const Indicator& ind) {
    return ATAN()(ind);
}

inline Indicator ATAN(price_t val) {
    return ATAN(CVAL(val));
}

}

#endif /* INDICATOR_CRT_ATAN_H_ */
