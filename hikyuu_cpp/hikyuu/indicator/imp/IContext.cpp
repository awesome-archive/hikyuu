/*
 *  Copyright (c) 2024 hikyuu.org
 *
 *  Created on: 2024-12-28
 *      Author: fasiondog
 */

#include "hikyuu/indicator/crt/ALIGN.h"
#include "hikyuu/indicator/crt/CVAL.h"
#include "hikyuu/indicator/crt/SLICE.h"
#include "IContext.h"

#if HKU_SUPPORT_SERIALIZATION
BOOST_CLASS_EXPORT(hku::IContext)
#endif

namespace hku {

IContext::IContext() : IndicatorImp("CONTEXT") {}

IContext::IContext(const Indicator& ref_ind) : IndicatorImp("CONTEXT"), m_ref_ind(ref_ind) {}

IContext::~IContext() {}

IndicatorImpPtr IContext::_clone() {
    auto p = make_shared<IContext>();
    p->m_ref_ind = m_ref_ind.clone();
    return p;
}

void IContext::_calculate(const Indicator& ind) {
    HKU_WARN_IF(!isLeaf() && !ind.empty(),
                "The input is ignored because {} depends on the context!",
                getParam<string>("kdata"));

    auto in_k = getContext();
    auto self_k = m_ref_ind.getContext();
    auto null_k = Null<KData>();

    auto ref_dates = m_ref_ind.getDatetimeList();

    auto ref = m_ref_ind;

    if (in_k != null_k && in_k != self_k) {
        if (ref_dates.empty()) {
            // 上下文无效且无对齐日期，按时间无关序列计算并对齐
            if (ref.size() > in_k.size()) {
                ref = SLICE(ref, ref.size() - in_k.size(), ref.size());
            } else if (ref.size() < in_k.size()) {
                // 右对齐
                ref = CVAL(ind, 0.) + ref;
            }  // else 长度相等无需再处理
        } else {
            // 如果参考指标是时间序列，自按当前上下文日期查询条件查询后按日期对齐
            auto self_stk = self_k.getStock();
            ref = m_ref_ind(self_stk.getKData(in_k.getQuery()));
            ref = ALIGN(ref, in_k, false);
        }
    }

    size_t total = ref.size();
    m_discard = ref.discard();
    if (m_discard >= total) {
        m_discard = total;
        return;
    }

    size_t rtotal = ref.getResultNumber();
    _readyBuffer(total, rtotal);
    size_t len = sizeof(value_t) * (total - m_discard);
    for (size_t r = 0; r < rtotal; ++r) {
        const auto* src = ref.data(r) + m_discard;
        auto* dst = this->data(r) + m_discard;
        memcpy(dst, src, len);
    }
}

Indicator HKU_API CONTEXT() {
    return Indicator(make_shared<IContext>());
}

Indicator HKU_API CONTEXT(const Indicator& ind) {
    return Indicator(make_shared<IContext>(ind));
}

}  // namespace hku