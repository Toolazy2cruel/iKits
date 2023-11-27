//
// Created by chenzhi on 2023/11/27.
//

#ifndef IKITS_FENWICKTREE_H
#define IKITS_FENWICKTREE_H


#include <iostream>
#include <vector>

class FenwickTree {
public:
    FenwickTree(int size) : tree(size + 1, 0) {}

    // 单点更新：将索引 i 的元素增加 delta
    void update(int i, int delta) {
        while (i < tree.size()) {
            tree[i] += delta;
            i += lowbit(i);
        }
    }

    // 前缀和查询：计算区间 [1, i] 的和
    int query(int i) {
        int sum = 0;
        while (i > 0) {
            sum += tree[i];
            i -= lowbit(i);
        }
        return sum;
    }

private:
    // 计算 x 的二进制表示中末尾连续 1 所代表的值
    int lowbit(int x) {
        return x & -x;
    }

    std::vector<int> tree;
};


#endif //IKITS_FENWICKTREE_H
