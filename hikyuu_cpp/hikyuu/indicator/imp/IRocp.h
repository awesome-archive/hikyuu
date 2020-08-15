/*
 * IRocp.h
 *
 *  Copyright (c) 2019, hikyuu.org
 *
 *  Created on: 2019-5-18
 *      Author: fasiondog
 */

#pragma once
#ifndef INDICATOR_IMP_IROCP_H_
#define INDICATOR_IMP_IROCP_H_

#include "../Indicator.h"

namespace hku {

class IRocp : public hku::IndicatorImp {
    INDICATOR_IMP(IRocp)
    INDICATOR_IMP_NO_PRIVATE_MEMBER_SERIALIZATION

public:
    IRocp();
    virtual ~IRocp();
};

} /* namespace hku */
#endif /* INDICATOR_IMP_IROCP_H_ */
