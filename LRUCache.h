//
// Created by chenzhi on 2023/3/29.
//

#ifndef IKITS_LRUCACHE_H
#define IKITS_LRUCACHE_H

#include <map>
#include "DListNode.h"

class LRUCache {
public:
    LRUCache(int cap): cap_(cap) {
        cache_ = new DoubleList;
    }
    ~LRUCache() {
        delete cache_;
    }
    int Get(int key) {
        if (dict_.find(key) != dict_.end()) {
            //更新频次
            DListNode* exist_node = dict_[key];
            cache_->Remove(exist_node);
            cache_->AddLast(exist_node);
            return dict_[key]->val;
        }
        return -1;
    }

    void Put(int key, int val) {
        if (dict_.find(key) != dict_.end()) {
            dict_[key]->val = val;
            cache_->Remove(dict_[key]);
            cache_->AddLast(dict_[key]);
        } else {
            //超长度
            DListNode* node = new DListNode(key, val);
            if (dict_.size() >= cap_) {
                int key = cache_->RemoveFirst();
                dict_.erase(key);
            }
            dict_[key] = node;
            cache_->AddLast(node);
        }
    }

private:
    DoubleList* cache_;
    std::map<int, DListNode*> dict_;
    int cap_; //存储长度

};

#endif //IKITS_LRUCACHE_H
