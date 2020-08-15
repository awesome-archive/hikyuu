/*
 * _StockManager.cpp
 *
 *  Created on: 2011-12-4
 *      Author: fasiondog
 */

#include <boost/python.hpp>
#include <hikyuu/StockManager.h>

using namespace boost::python;
using namespace hku;

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(getTradingCalendar_overloads, getTradingCalendar, 1, 2)

BlockList (StockManager::*getBlockList_1)(const string&) = &StockManager::getBlockList;
BlockList (StockManager::*getBlockList_2)() = &StockManager::getBlockList;

void export_StockManager() {
    class_<StockManager>("StockManager", "证券信息管理类", no_init)
      .def("instance", &StockManager::instance, return_value_policy<reference_existing_object>(),
           "获取StockManager单例实例")
      .staticmethod("instance")

      .def("init", &StockManager::init,
           R"(init(self, base_info_param, block_param, kdata_param, preload_param, hikyuu_param)
              
    初始化函数，必须在程序入口调用
    
    :param base_info_param 基础信息驱动参数
     param block_param 板块信息驱动参数
     param kdata_param K线数据驱动参数
     param preload_param 预加载参数
     param hikyuu_param 其他参数)")

      .def("tmpdir", &StockManager::tmpdir, R"(tmpdir(self) -> str

    获取用于保存零时变量等的临时目录，如未配置则为当前目录 由m_config中的“tmpdir”指定)")

      .def("get_base_info_parameter", &StockManager::getBaseInfoDriverParameter,
           "获取当前基础信息驱动参数")
      .def("get_block_parameter", &StockManager::getBlockDriverParameter,
           "获取当前板块信息驱动参数")
      .def("get_kdata_parameter", &StockManager::getKDataDriverParameter, "获取当前K线数据驱动参数")
      .def("get_preload_parameter", &StockManager::getPreloadParameter, "获取当前预加载参数")
      .def("get_hikyuu_parameter", &StockManager::getHikyuuParameter, "获取当前其他参数")

      .def("get_market_list", &StockManager::getAllMarket, R"(get_market_list(self)

    获取市场简称列表

    :rtype: StringList)")

      .def("get_market_info", &StockManager::getMarketInfo, R"(get_market_info(self, market)

    获取相应的市场信息

    :param string market: 指定的市场标识（市场简称）
    :return: 相应的市场信息，如果相应的市场信息不存在，则返回Null<MarketInfo>()
    :rtype: MarketInfo)")

      .def("get_stock_type_info", &StockManager::getStockTypeInfo,
           R"(get_stock_type_info(self, stk_type)

    获取相应的证券类型详细信息

    :param int stk_type: 证券类型，参见： :py:data:`constant`
    :return: 对应的证券类型信息，如果不存在，则返回Null<StockTypeInfo>()
    :rtype: StockTypeInfo)")

      .def("get_stock", &StockManager::getStock, R"(get_stock(self, querystr)

    根据"市场简称证券代码"获取对应的证券实例

    :param str querystr: 格式：“市场简称证券代码”，如"sh000001"
    :return: 对应的证券实例，如果实例不存在，则Null<Stock>()，不抛出异常
    :rtype: Stock)")

      .def("get_block", &StockManager::getBlock, R"(get_block(self, category, name)

    获取预定义的板块

    :param str category: 板块分类
    :param str name: 板块名称
    :return: 板块，如找不到返回空Block
    :rtype: Block)")

      .def("get_block_list", getBlockList_1)
      .def("get_block_list", getBlockList_2, R"(get_block_list(self[, category])

    获取指定分类的板块列表

    :param str category: 板块分类
    :return: 板块列表
    :rtype: BlockList)")

      .def("get_trading_calendar", &StockManager::getTradingCalendar,
           (arg("query"), arg("market") = "SH"),
           R"(get_trading_calendar(self, query[, market='SH'])

    获取指定市场的交易日日历

    :param KQuery query: Query查询条件
    :param str market: 市场简称
    :return: 日期列表
    :rtype: DatetimeList)")

      .def(
        "add_temp_csv_stock", &StockManager::addTempCsvStock,
        (arg("code"), arg("day_filename"), arg("min_filename"), arg("tick") = 0.01,
         arg("tick_value") = 0.01, arg("precision") = 2, arg("min_trade_num") = 1,
         arg("max_trade_num") = 1000000),
        R"(add_temp_csv_stock(code, day_filename, min_filename[, tick=0.01, tick_value=0.01, precision=2, min_trade_num = 1, max_trade_num=1000000])

    从CSV文件（K线数据）增加临时的Stock，可用于只有CSV格式的K线数据时，进行临时测试。        
        
    CSV文件第一行为标题，需含有 Datetime（或Date、日期）、OPEN（或开盘价）、HIGH（或最高价）、LOW（或最低价）、CLOSE（或收盘价）、AMOUNT（或成交金额）、VOLUME（或VOL、COUNT、成交量）。
        
    :param str code: 自行编号的证券代码，不能和已有的Stock相同，否则将返回Null<Stock>
    :param str day_filename: 日线CSV文件名
    :param str min_filename: 分钟线CSV文件名
    :param float tick: 最小跳动量，默认0.01
    :param float tick_value: 最小跳动量价值，默认0.01
    :param int precision: 价格精度，默认2
    :param int min_trade_num: 单笔最小交易量，默认1
    :param int max_trade_num: 单笔最大交易量，默认1000000
    :return: 加入的Stock
    :rtype: Stock)")

      .def("removeTempCsvStock", &StockManager::removeTempCsvStock,
           R"(removeTempCsvStock(self, code)

    移除增加的临时Stock

    :param str code: 创建时自定义的编码)")

      .def("__len__", &StockManager::size, "返回证券数量")
      .def("__getitem__", &StockManager::getStock, "同 get_stock")
      .def("__iter__", iterator<const StockManager>());
}
