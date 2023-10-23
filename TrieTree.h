//
// Created by chenzhi on 2023/10/19.
//

#ifndef IKITS_TRIETREE_H
#define IKITS_TRIETREE_H

#include <vector>

using namespace std;

struct TireNode{
    char val;
    bool leaf_node;
    vector<TireNode*> childs;

    TireNode():val('%'),leaf_node(false) {

    }

    TireNode(char val):val(val),leaf_node(false) {

    }

};

class TireTree {
private:
    TireNode* root;
public:
    TireTree() {
        root = new TireNode;
    }
    void insert(string s) {
        TireNode* start = root;
        int index = 0;
        for (char i : s) {
            bool flag = false;
            for (auto & child : start->childs) {
                if (child->val == i) {
                    flag = true;
                    if (index++ == s.size()) {
                        child->leaf_node = true;
                    }
                    start = child;
                    break;
                }
            }
            if (flag) {
                continue;
            }
            auto* child = new TireNode(i);
            if (index++ == s.size()) {
                child->leaf_node = true;
            }
            start->childs.push_back(child);
            start = child;
        }
    }

    bool startwith(TireNode* child, string& s, int i) {
        if (i == s.size()) {
            return true;
        }
        for (auto &ch : child->childs) {
            if (ch->val == s[i]) {
                return startwith(ch, s, i+1);
            }
        }
        return false;
    }

    bool startwith(string s) {
        return startwith(root, s, 0);
    }

    bool search(TireNode* child, string& s, int i) {
        if (i == s.size() && child->leaf_node) {
            return true;
        }
        for (auto &ch : child->childs) {
            if (ch->val == s[i]) {

                return search(ch, s, i+1);
            }
        }
        return false;
    }

    bool search(string s) {
        return search(root, s, 0);
    }

};
#endif //IKITS_TRIETREE_H
