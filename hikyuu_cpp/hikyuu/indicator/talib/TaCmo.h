/*
 *  Copyright (c) 2024 hikyuu.org
 *
 *  Created on: 2024-12-18
 *      Author: fasiondog
 */

#pragma once

#include "../Indicator.h"

namespace hku {

class TaCmo : public IndicatorImp {
    INDICATOR_IMP(TaCmo)
    INDICATOR_IMP_NO_PRIVATE_MEMBER_SERIALIZATION

public:
    TaCmo();
    virtual ~TaCmo();
    virtual void _checkParam(const string& name) const override;
};

}  // namespace hku