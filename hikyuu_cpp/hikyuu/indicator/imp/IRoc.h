/*
 * IRoc.h
 *
 *  Copyright (c) 2019, hikyuu.org
 * 
 *  Created on: 2019-5-18
 *      Author: fasiondog
 */

#pragma once
#ifndef INDICATOR_IMP_IROC_H_
#define INDICATOR_IMP_IROC_H_

#include "../Indicator.h"

namespace hku {

class IRoc: public hku::IndicatorImp {
    INDICATOR_IMP(IRoc)
    INDICATOR_IMP_NO_PRIVATE_MEMBER_SERIALIZATION

public:
    IRoc();
    virtual ~IRoc();
};

} /* namespace hku */
#endif /* INDICATOR_IMP_IROC_H_ */
