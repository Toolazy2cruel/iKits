//
// Created by chenzhi on 2023/3/31.
//

#include "LFUCache.h"

int LFUCache::get(int key) {
    auto res = key_value_.find(key);
    if (res == key_value_.end()) {
        return -1;
    }
    incrFreq(key);
    return res->second;
}
void LFUCache::put(int key, int value) {
    auto res = key_value_.find(key);
    if (res != key_value_.end()) {
        key_value_[key] = value;
        incrFreq(key);
        return;
    }
    //不存在，新增场景
    DListNode* node = new DListNode(key, value);
    if (key_value_.size() >= cap_) {
        int old = freq_2_key_list_[min_freq_]->RemoveFirst();
        key_value_.erase(old);
        key_freq_.erase(old);

    }
    key_value_[key] = value;
    key_freq_[key] = 1;
    min_freq_ = 1;
    newHashDoubleList(min_freq_);
    freq_2_key_list_[min_freq_]->AddLast(node);
}

void LFUCache::incrFreq(int key) {
    //key_value_存在，key_freq_一定也存在
    auto res = key_freq_.find(key);
    if (res != key_freq_.end()) {
        //+1
        int freq = res->second;
        DListNode* node = freq_2_key_list_[freq]->GetListNode(key);
        if (node == nullptr) {
            return;
        }
        freq_2_key_list_[freq]->Remove(node);
        //删除后无节点
        if (freq_2_key_list_[freq]->size_ == 0) {
            min_freq_ = freq + 1;
        }
        key_freq_[key]++;
        newHashDoubleList(freq + 1);
        freq_2_key_list_[freq + 1]->AddLast(node);
    }
    return;
}
void LFUCache::newHashDoubleList(int freq) {
    if (freq_2_key_list_.find(freq) == freq_2_key_list_.end()
            || freq_2_key_list_[freq] == nullptr) {
        freq_2_key_list_[freq] = new HashDoubleList;
    }
}
