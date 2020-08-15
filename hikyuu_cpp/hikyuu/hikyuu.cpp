/*
 * hikyuu.cpp
 *
 *  Created on: 2011-11-13
 *      Author: fasiondog
 */

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <fmt/format.h>
#include "utilities/IniParser.h"
#include "hikyuu.h"
#include "version.h"

namespace hku {

static Parameter g_hikyuu_context;

void hikyuu_init(const string& config_file_name) {
    IniParser config;
    try {
        config.read(config_file_name);

    } catch (std::invalid_argument& e) {
        HKU_FATAL("Reading configure error!\n{}", e.what());
        exit(1);
    } catch (std::logic_error& e) {
        HKU_FATAL("Reading configure error!\n{}", e.what());
        exit(1);
    } catch (...) {
        HKU_WARN("Reading configure error! Don't know  error!");
        exit(1);
    }

    Parameter baseParam, blockParam, kdataParam, preloadParam, hkuParam;

    hkuParam.set<string>("tmpdir", config.get("hikyuu", "tmpdir", "."));
    hkuParam.set<string>("datadir", config.get("hikyuu", "datadir", "."));

    if (!config.hasSection("baseinfo")) {
        HKU_FATAL("Missing configure of baseinfo!");
        exit(1);
    }

    IniParser::StringListPtr option = config.getOptionList("baseinfo");
    for (auto iter = option->begin(); iter != option->end(); ++iter) {
        string value = config.get("baseinfo", *iter);
        baseParam.set<string>(*iter, value);
    }

    IniParser::StringListPtr block_config = config.getOptionList("block");
    for (auto iter = block_config->begin(); iter != block_config->end(); ++iter) {
        string value = config.get("block", *iter);
        blockParam.set<string>(*iter, value);
    }

    option = config.getOptionList("kdata");
    for (auto iter = option->begin(); iter != option->end(); ++iter) {
        kdataParam.set<string>(*iter, config.get("kdata", *iter));
    }

    option = config.getOptionList("preload");
    for (auto iter = option->begin(); iter != option->end(); ++iter) {
        preloadParam.set<bool>(*iter, config.getBool("preload", *iter));
    }

    StockManager& sm = StockManager::instance();
    sm.init(baseParam, blockParam, kdataParam, preloadParam, hkuParam);
}

string getVersion() {
    return HKU_VERSION;
}

std::string HKU_API getVersionWithBuild() {
#if defined(__arm__)
    return fmt::format("{}_{}_arm", HKU_VERSION, HKU_VERSION_BUILD);
#elif defined(__aarch64__)
    return fmt::format("{}_{}_aarch64", HKU_VERSION, HKU_VERSION_BUILD);
#elif defined(__x86_64__) || defined(_WIN64)
    return fmt::format("{}_{}_x64", HKU_VERSION, HKU_VERSION_BUILD);
#elif defined(__i386__) || defined(_WIN32)
    return fmt::format("{}_{}_i386", HKU_VERSION, HKU_VERSION_BUILD);
#else
    return fmt::format("{}_{}_unknow_arch", HKU_VERSION, HKU_VERSION_BUILD);
#endif
}

}  // namespace hku
