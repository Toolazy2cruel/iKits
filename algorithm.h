//
// Created by chenzhi on 2023/4/17.
//

#ifndef IKITS_ALGORITHM_H
#define IKITS_ALGORITHM_H

#endif //IKITS_ALGORITHM_H

#include <iostream>
using namespace std;

struct TreeNode{
    TreeNode(int v): val(v), left(nullptr), right(nullptr) {}
    TreeNode(int val, TreeNode* l, TreeNode* r): val(val), left(l), right(r) {}
    int val;
    TreeNode* left;
    TreeNode* right;
};


int maxDepth(TreeNode* root) {
    if (root == nullptr) {
        return 0;
    }

    int leftDepth = maxDepth(root->left);
    int rightDepth = maxDepth(root->right);

    return max(leftDepth, rightDepth) + 1;
}

namespace diam {
    int ans = 0;

    int getDiam(TreeNode* root) {
        if (root == nullptr) {
            return 0;
        }

        int leftDepth = getDiam(root->left);
        int rightDepth = getDiam(root->right);

        ans = max(ans, leftDepth + rightDepth + 1);
        return max(leftDepth, rightDepth) + 1;
    }
}

void test_maxDepth() {
    TreeNode* root = new TreeNode(
        1 ,
        new TreeNode(
            2,
            new TreeNode(
                3
                , new TreeNode (
                    6
                    )
                    , nullptr
                ),
            new TreeNode(
                4
                )

        ),
        new TreeNode(
            5
            )
        );
    cout << maxDepth(root) << endl;
    diam::getDiam(root);
    cout << diam::ans << endl;

}