/*
 * _log.cpp
 *
 *  Created on: 2019-2-11
 *      Author: fasiondog
 */

#include <boost/python.hpp>
#include <hikyuu/Log.h>

using namespace boost::python;
using namespace hku;

void export_log() {
    enum_<LOG_LEVEL>("LOG_LEVEL")
      .value("DEBUG", LOG_LEVEL::DEBUG)
      .value("TRACE", LOG_LEVEL::TRACE)
      .value("INFO", LOG_LEVEL::INFO)
      .value("WARN", LOG_LEVEL::WARN)
      .value("ERROR", LOG_LEVEL::ERROR)
      .value("FATAL", LOG_LEVEL::FATAL)
      .value("OFF", LOG_LEVEL::OFF);

    def("get_log_level", get_log_level, "获取当前日志级别");
    def("set_log_level", set_log_level, "设置当前日志级别");
}
