/*
 * RightShift.h
 *
 *  Created on: 2015年3月21日
 *      Author: fasiondog
 */

#pragma once
#ifndef INDICATOR_IMP_RIGHTSHIFT_H_
#define INDICATOR_IMP_RIGHTSHIFT_H_

#include "../Indicator.h"

namespace hku {

/*
 * REF 向前引用 （即右移）
 * 引用若干周期前的数据。
 * 用法：　REF(X，A)　引用A周期前的X值。
 * 例如：　REF(CLOSE，1)　表示上一周期的收盘价，在日线上就是昨收。
 */
class RightShift: public IndicatorImp {
    INDICATOR_IMP(RightShift)
    INDICATOR_IMP_NO_PRIVATE_MEMBER_SERIALIZATION

public:
    RightShift();
    virtual ~RightShift();
};

} /* namespace hku */

#endif /* INDICATOR_IMP_RIGHTSHIFT_H_ */
