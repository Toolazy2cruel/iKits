//
// Created by chenzhi on 2023/3/31.
//

#ifndef IKITS_DLISTNODE_H
#define IKITS_DLISTNODE_H

struct DListNode {
    DListNode():pre(nullptr), next(nullptr), val(-1) {}
    DListNode(int key_, int val_):pre(nullptr), next(nullptr), key(key_), val(val_) {}
    DListNode* pre;
    DListNode* next;
    int key, val;
};

struct DoubleList {
    int size_;
    DListNode* head;
    DListNode* tail;
    //init
    DoubleList() {
        size_ = 0;
        head = new DListNode(-1, -1);
        tail = new DListNode(-1, -1);

        head->next = tail;
        head->pre = nullptr;
        tail->pre = head;
        tail->next = nullptr;
    }
    ~DoubleList() {
        delete head;
        delete tail;
    }

    void Remove(DListNode* node) {
        if (size_ == 0) {
            return;
        }
        node->pre->next = node->next;
        node->next->pre = node->pre;
        size_--;
    }

    int RemoveFirst() {
        if (size_ == 0) {
            return -1;
        }
        int key = head->next->key;
        Remove(head->next);
        return key;
    }

    void AddLast(DListNode* node ) {
        if (node == nullptr) {
            return;
        }
        DListNode* cur_last = tail->pre;
        //cur_last和tail之间插入一个
        cur_last->next = node;
        node->pre = cur_last;
        node->next = tail;
        tail->pre = node;
        size_++;
    }

};


struct HashDoubleList {
    int size_;
    DListNode* head;
    DListNode* tail;
    //key-node
    std::map<int, DListNode*> record;
    //init
    HashDoubleList() {
        size_ = 0;
        head = new DListNode(-1, -1);
        tail = new DListNode(-1, -1);

        head->next = tail;
        head->pre = nullptr;
        tail->pre = head;
        tail->next = nullptr;
    }
    ~HashDoubleList() {
        delete head;
        delete tail;
    }

    DListNode* GetListNode(int key) {
        if (record.find(key) ==record.end()) {
            return nullptr;
        }
        return record[key];
    }

    void Remove(DListNode* node) {
        if (size_ == 0) {
            return;
        }
        node->pre->next = node->next;
        node->next->pre = node->pre;
        record.erase(node->key);
        size_--;
    }

    int RemoveFirst() {
        if (head->next == nullptr) {
            return -1;
        }
        int key = head->next->key;
        Remove(head->next);
        //size_--;
        return key;
    }

    void AddLast(DListNode* node ) {
        if (node == nullptr) {
            return;
        }
        DListNode* cur_last = tail->pre;
        //cur_last和tail之间插入一个
        cur_last->next = node;
        node->pre = cur_last;
        node->next = tail;
        tail->pre = node;
        record[node->key] = node;
        size_++;
    }

};


#endif //IKITS_DLISTNODE_H
