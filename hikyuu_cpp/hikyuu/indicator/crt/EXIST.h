/*
 * EXIST.h
 *  
 *  Copyright (c) 2019 hikyuu.org
 * 
 *  Created on: 2019-4-19
 *      Author: fasiondog
 */

#pragma once
#ifndef INDICATOR_CRT_EXIST_H_
#define INDICATOR_CRT_EXIST_H_

#include "../Indicator.h"

namespace hku {

/**
 * 存在
 * @details
 * <pre>
 * 用法：EXIST(X,N) 表示条件X在N周期有存在
 * 例如：EXIST(C>O,10) 表示前10日内存在着阳线
 * </pre>
 * @ingroup Indicator
 */
Indicator HKU_API EXIST(int n = 20);
Indicator EXIST(const Indicator& ind, int n = 20);

inline Indicator EXIST(const Indicator& ind, int n) {
    return EXIST(n)(ind);
}

} /* namespace */

#endif /* INDICATOR_CRT_EXIST_H_ */
