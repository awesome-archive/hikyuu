/*
 * IFilter.h
 * 
 *  Copyright (c) 2019 hikyuu.org
 *
 *  Created on: 2019-5-4
 *      Author: fasiondog
 */

#pragma once
#ifndef INDICATOR_IMP_IFILTER_H_
#define INDICATOR_IMP_IFILTER_H_

#include "../Indicator.h"

namespace hku {

class IFilter: public IndicatorImp {
    INDICATOR_IMP(IFilter)
    INDICATOR_IMP_NO_PRIVATE_MEMBER_SERIALIZATION

public:
    IFilter();
    virtual ~IFilter();
};

} /* namespace hku */
#endif /* INDICATOR_IMP_IFILTER_H_ */
