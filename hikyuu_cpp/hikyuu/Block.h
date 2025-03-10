/*
 * Block.h
 *
 *  Created on: 2015年2月8日
 *      Author: fasiondog
 */

#pragma once
#ifndef BLOCK_H_
#define BLOCK_H_

#include "StockMapIterator.h"

namespace hku {

/**
 * 板块类，可视为证券的容器
 * @ingroup StockManage
 */
class HKU_API Block {
public:
    Block();
    Block(const string& category, const string& name);
    Block(const Block&);
    Block& operator=(const Block&);
    virtual ~Block();

    typedef StockMapIterator const_iterator;
    const_iterator begin() const {
        const_iterator iter;
        if (m_data)
            iter = StockMapIterator(m_data->m_stockDict.begin());
        return iter;
    }

    const_iterator end() const {
        const_iterator iter;
        if (m_data)
            iter = StockMapIterator(m_data->m_stockDict.end());
        return iter;
    }

    bool operator==(const Block& blk) {
        return m_data == blk.m_data;
    }

    bool operator!=(const Block& blk) {
        return m_data != blk.m_data;
    }

    /** 获取板块类别 */
    string category() const {
        return m_data ? m_data->m_category : "";
    }

    /** 获取板块名称 */
    string name() const {
        return m_data ? m_data->m_name : "";
    }

    /** 设置板块类别 */
    void category(const string& category) {
        if (!m_data)
            m_data = shared_ptr<Data>(new Data);
        m_data->m_category = category;
    }

    /** 设置名称 */
    void name(const string& name) {
        if (!m_data)
            m_data = shared_ptr<Data>(new Data);
        m_data->m_name = name;
    }

    /** 是否包含指定的证券 */
    bool have(const string& market_code) const;

    /** 是否包含指定的证券 */
    bool have(const Stock& stock) const;

    /** 获取指定的证券 */
    Stock get(const string& market_code) const;

    /** 获取指定的证券 */
    Stock operator[](const string& market_code) const {
        return get(market_code);
    }

    /** 加入指定证券 */
    bool add(const Stock& stock);

    /** 加入指定证券 */
    bool add(const string& market_code);

    /** 移除指定证券 */
    bool remove(const string& market_code);

    /** 移除指定证券 */
    bool remove(const Stock& stock);

    /** 包含的证券数量 */
    size_t size() const {
        return m_data ? m_data->m_stockDict.size() : 0;
    }

    /** 是否为空 */
    bool empty() const {
        return size() ? false : true;
    }

    /** 清除包含的所有证券 */
    void clear() {
        if (m_data) m_data->m_stockDict.clear();
    }

private:
    struct HKU_API Data {
        string m_category;
        string m_name;
        StockMapIterator::stock_map_t m_stockDict;
    };
    shared_ptr<Data> m_data;
};


/** @ingroup StockManage */
typedef vector<Block> BlockList;

HKU_API std::ostream& operator <<(std::ostream &os, const Block&);

} /* namespace hku */

#endif /* BLOCK_H_ */
