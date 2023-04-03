//
// Created by chenzhi on 2023/3/31.
//

#ifndef IKITS_LFUCACHE_H
#define IKITS_LFUCACHE_H

#include <unordered_map>
#include "DListNode.h"
#include <map>



class LFUCache {
public:
    explicit LFUCache(int cap): cap_(cap) {
        min_freq_ = 0;
    }
    int get(int key);
    void incrFreq(int key);
    void newHashDoubleList(int freq);
    void put(int key, int value);

private:
    int cap_;
    int min_freq_;
    std::unordered_map<int, int> key_value_;
    std::unordered_map<int, int> key_freq_;
    std::map<int, HashDoubleList*> freq_2_key_list_;
};

#endif //IKITS_LFUCACHE_H
