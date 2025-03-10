/*
 * DOWNNDAY.h
 *  
 *  Copyright (c) 2019 hikyuu.org
 * 
 *  Created on: 2019-5-2
 *      Author: fasiondog
 */

#pragma once
#ifndef INDICATOR_CRT_DOWNNDAY_H_
#define INDICATOR_CRT_DOWNNDAY_H_

#include "EVERY.h"
#include "REF.h"

namespace hku {

/**
 * 连跌周期, DOWNNDAY(CLOSE,M)表示连跌M个周期
 * @ingroup Indicator
 */
Indicator DOWNNDAY(const Indicator& ind, int n = 3);

inline Indicator DOWNNDAY(const Indicator& ind, int n) {
    Indicator result = EVERY(REF(ind, 1) > ind, n);
    result.name("DOWNNDAY");
    return result;
}

} /* namespace */

#endif /* INDICATOR_CRT_DOWNNDAY_H_ */
