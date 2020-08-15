/*
 * _MoneyManager.cpp
 *
 *  Created on: 2013-3-13
 *      Author: fasiondog
 */

#include <boost/python.hpp>
#include <hikyuu/trade_sys/moneymanager/build_in.h>
#include "../_Parameter.h"
#include "../pickle_support.h"

using namespace boost::python;
using namespace hku;

class MoneyManagerWrap : public MoneyManagerBase, public wrapper<MoneyManagerBase> {
public:
    MoneyManagerWrap() : MoneyManagerBase() {}
    MoneyManagerWrap(const string& name) : MoneyManagerBase(name) {}

    void buyNotify(const TradeRecord& record) {
        if (override buyNotify = this->get_override("buyNotify")) {
            buyNotify(record);
            return;
        }
        MoneyManagerBase::buyNotify(record);
    }

    void default_buyNotify(const TradeRecord& record) {
        MoneyManagerBase::buyNotify(record);
    }

    void sellNotify(const TradeRecord& record) {
        if (override sellNotify = this->get_override("sellNotify")) {
            sellNotify(record);
            return;
        }
        MoneyManagerBase::sellNotify(record);
    }

    void default_sellNotify(const TradeRecord& record) {
        MoneyManagerBase::sellNotify(record);
    }

    double _getSellNumber(const Datetime& datetime, const Stock& stock, price_t price, price_t risk,
                          SystemPart from) {
        if (override _getSellNumber = this->get_override("_getSellNumber")) {
            return _getSellNumber(datetime, stock, price, risk, from);
        }
        return MoneyManagerBase::_getSellNumber(datetime, stock, price, risk, from);
    }

    double default_getSellNumber(const Datetime& datetime, const Stock& stock, price_t price,
                                 price_t risk, SystemPart from) {
        return MoneyManagerBase::_getSellNumber(datetime, stock, price, risk, from);
    }

    double _getBuyNumber(const Datetime& datetime, const Stock& stock, price_t price, price_t risk,
                         SystemPart from) {
        return this->get_override("_getBuyNumber")(datetime, stock, price, risk, from);
    }

    double _getSellShortNumber(const Datetime& datetime, const Stock& stock, price_t price,
                               price_t risk, SystemPart from) {
        if (override _getSellShortNumber = this->get_override("_getSellShortNumber")) {
            return _getSellShortNumber(datetime, stock, price, risk, from);
        }
        return MoneyManagerBase::_getSellShortNumber(datetime, stock, price, risk, from);
    }

    double default_getSellShortNumber(const Datetime& datetime, const Stock& stock, price_t price,
                                      price_t risk, SystemPart from) {
        return MoneyManagerBase::_getSellShortNumber(datetime, stock, price, risk, from);
    }

    double getBuyShortNumber(const Datetime& datetime, const Stock& stock, price_t price,
                             price_t risk, SystemPart from) {
        if (override _getBuyShortNumber = this->get_override("_getBuyShortNumber")) {
            return _getBuyShortNumber(datetime, stock, price, risk, from);
        }
        return MoneyManagerBase::_getBuyShortNumber(datetime, stock, price, risk, from);
    }

    double default_getBuyShortNumber(const Datetime& datetime, const Stock& stock, price_t price,
                                     price_t risk, SystemPart from) {
        return MoneyManagerBase::_getBuyShortNumber(datetime, stock, price, risk, from);
    }

    void _reset() {
        if (override func = get_override("_reset")) {
            func();
        } else {
            MoneyManagerBase::_reset();
        }
    }

    void default_reset() {
        this->MoneyManagerBase::_reset();
    }

    MoneyManagerPtr _clone() {
        return this->get_override("_clone")();
    }
};

string (MoneyManagerBase::*mm_get_name)() const = &MoneyManagerBase::name;
void (MoneyManagerBase::*mm_set_name)(const string&) = &MoneyManagerBase::name;

void export_MoneyManager() {
    class_<MoneyManagerWrap, boost::noncopyable>("MoneyManagerBase", R"(资金管理策略基类

公共参数：

    - auto-checkin=False (bool) : 当账户现金不足以买入资金管理策略指示的买入数量时，自动向账户中补充存入（checkin）足够的现金。
    - max-stock=20000 (int) : 最大持有的证券种类数量（即持有几只股票，而非各个股票的持仓数）
    - disable_ev_force_clean_position=False (bool) : 禁用市场环境失效时强制清仓
    - disable_cn_force_clean_position=False (bool) : 禁用系统有效条件失效时强制清仓

自定义资金管理策略接口：

    - buyNotify : 【可选】接收实际买入通知，预留用于多次增减仓处理
    - sellNotify : 【可选】接收实际卖出通知，预留用于多次增减仓处理
    - _getBuyNumber : 【必须】获取指定交易对象可买入的数量
    - _getSellNumber : 【可选】获取指定交易对象可卖出的数量，如未重载，默认为卖出全部已持仓数量
    - _reset : 【可选】重置私有属性
    - _clone : 【必须】克隆接口)",
                                                 init<>())
      .def(init<const string&>())
      .def(self_ns::str(self))
      .def(self_ns::repr(self))

      .add_property("name", mm_get_name, mm_set_name, "名称")
      .add_property("tm", &MoneyManagerBase::getTM, &MoneyManagerBase::setTM,
                    "设置或获取交易管理对象")
      .add_property("query", &MoneyManagerBase::getQuery, &MoneyManagerBase::setQuery,
                    "设置或获取查询条件")

      .def("get_param", &MoneyManagerBase::getParam<boost::any>, R"(get_param(self, name)

    获取指定的参数

    :param str name: 参数名称
    :return: 参数值
    :raises out_of_range: 无此参数)")

      .def("set_param", &MoneyManagerBase::setParam<object>, R"(set_param(self, name, value)

    设置参数

    :param str name: 参数名称
    :param value: 参数值
    :raises logic_error: Unsupported type! 不支持的参数类型)")

      .def("haveParam", &MoneyManagerBase::haveParam, "是否存在指定参数")

      .def("reset", &MoneyManagerBase::reset, "复位操作")
      .def("clone", &MoneyManagerBase::clone, "克隆操作")

      .def("buy_notify", &MoneyManagerBase::buyNotify, &MoneyManagerWrap::default_buyNotify,
           R"(buy_notify(self, trade_record)

    【重载接口】交易系统发生实际买入操作时，通知交易变化情况，一般存在多次增减仓的情况才需要重载

    :param TradeRecord trade_record: 发生实际买入时的实际买入交易记录)")

      .def("sell_notify", &MoneyManagerBase::sellNotify, &MoneyManagerWrap::default_sellNotify,
           R"(sell_notify(self, trade_record)

    【重载接口】交易系统发生实际卖出操作时，通知实际交易变化情况，一般存在多次增减仓的情况才需要重载

    :param TradeRecord trade_record: 发生实际卖出时的实际卖出交易记录)")

      .def("get_buy_num", &MoneyManagerBase::getBuyNumber,
           R"(get_buy_num(self, datetime, stock, price, risk, part_from)

    获取指定交易对象可买入的数量

    :param Datetime datetime: 交易时间
    :param Stock stock: 交易对象
    :param float price: 交易价格
    :param float risk: 交易承担的风险，如果为0，表示全部损失，即市值跌至0元
    :param System.Part part_from: 来源系统组件
    :return: 可买入数量
    :rtype: float)")

      .def("get_sell_num", &MoneyManagerBase::getSellNumber,
           R"(get_sell_num(self, datetime, stock, price, risk, part_from)

    获取指定交易对象可卖出的数量
    
    :param Datetime datetime: 交易时间
    :param Stock stock: 交易对象
    :param float price: 交易价格
    :param float risk: 新的交易承担的风险，如果为0，表示全部损失，即市值跌至0元
    :param System.Part part_from: 来源系统组件
    :return: 可卖出数量
    :rtype: float)")

      .def("_get_buy_num", pure_virtual(&MoneyManagerBase::_getBuyNumber),
           R"(_get_buy_num(self, datetime, stock, price, risk, part_from)

    【重载接口】获取指定交易对象可买入的数量

    :param Datetime datetime: 交易时间
    :param Stock stock: 交易对象
    :param float price: 交易价格
    :param float risk: 交易承担的风险，如果为0，表示全部损失，即市值跌至0元
    :param System.Part part_from: 来源系统组件
    :return: 可买入数量
    :rtype: float)")

      .def("_get_sell_num", &MoneyManagerBase::_getSellNumber,
           &MoneyManagerWrap::default_getSellNumber,
           R"(_get_sell_num(self, datetime, stock, price, risk, part_from)

    【重载接口】获取指定交易对象可卖出的数量。如未重载，默认为卖出全部已持仓数量。

    :param Datetime datetime: 交易时间
    :param Stock stock: 交易对象
    :param float price: 交易价格
    :param float risk: 新的交易承担的风险，如果为0，表示全部损失，即市值跌至0元
    :param System.Part part_from: 来源系统组件
    :return: 可卖出数量
    :rtype: float)")

      /*.def("getSellShortNumber", &MoneyManagerBase::getSellShortNumber)
      .def("getBuyShortNumber", &MoneyManagerBase::getBuyShortNumber)
      .def("_getSellShortNumber", &MoneyManagerBase::_getSellShortNumber,
           &MoneyManagerWrap::default_getSellShortNumber)
      .def("_getBuyShortNumber", &MoneyManagerBase::_getBuyShortNumber,
           &MoneyManagerWrap::default_getBuyShortNumber)*/

      .def("_reset", &MoneyManagerBase::_reset, &MoneyManagerWrap::default_reset,
           R"(【重载接口】子类复位接口，复位内部私有变量)")
      .def("_clone", pure_virtual(&MoneyManagerBase::_clone), "【重载接口】子类克隆接口")

#if HKU_PYTHON_SUPPORT_PICKLE
      .def_pickle(name_init_pickle_suite<MoneyManagerBase>())
#endif
      ;

    register_ptr_to_python<MoneyManagerPtr>();

    def("MM_Nothing", MM_Nothing, R"(MM_Nothing()

    特殊的资金管理策略，相当于不做资金管理，有多少钱买多少。)");

    def("MM_FixedRisk", MM_FixedRisk, (arg("risk") = 1000.00), R"(MM_FixedRisk([risk = 1000.00])

    固定风险资金管理策略对每笔交易限定一个预先确定的或者固定的资金风险，如每笔交易固定风险1000元。公式：交易数量 = 固定风险 / 交易风险。

    :param float risk: 固定风险
    :return: 资金管理策略实例)");

    def("MM_FixedCapital", MM_FixedCapital, (arg("capital") = 10000.00),
        R"(MM_FixedCapital([capital = 10000.0])

    固定资本资金管理策略

    :param float capital: 固定资本单位
    :return: 资金管理策略实例)");

    def("MM_FixedCount", MM_FixedCount, (arg("n") = 100), R"(MM_FixedCount([n = 100])

    固定交易数量资金管理策略。每次买入固定的数量。
    
    :param float n: 每次买入的数量（应该是交易对象最小交易数量的整数，此处程序没有此进行判断）
    :return: 资金管理策略实例)");

    def("MM_FixedPercent", MM_FixedPercent, (arg("p") = 0.03), R"(MM_FixedPercent([p = 0.03])

    固定百分比风险模型。公式：P（头寸规模）＝ 账户余额 * 百分比 / R（每股的交易风险）。[BOOK3]_, [BOOK4]_ .
    
    :param float p: 百分比
    :return: 资金管理策略实例)");

    def("MM_FixedUnits", MM_FixedUnits, (arg("n") = 33), R"(MM_FixedUnits([n = 33])

    固定单位资金管理策略

    :param int n: n个资金单位
    :return: 资金管理策略实例)");

    def("MM_WilliamsFixedRisk", MM_WilliamsFixedRisk, (arg("p") = 0.1, arg("max_loss") = 1000.0),
        R"( MM_WilliamsFixedRisk([p=0.1, max_loss=1000.0])

    威廉斯固定风险资金管理策略)");
}
