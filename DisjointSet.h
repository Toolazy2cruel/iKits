//
// Created by chenzhi on 2023/10/19.
//

#ifndef IKITS_DISJOINTSET_H
#define IKITS_DISJOINTSET_H

class Disjoint{
private:
    int count; // 联通分量个数
    int *parent;
    int *length;
public:
    Disjoint(int n) {
        parent = new int[n];
        length = new int[n];
        count = n;
        for (int i = 0; i < n; ++i) {
            parent[i] = i;
            length[i] = 1;
        }
    }
    ~Disjoint(){
        delete []parent;
        delete []length;
    }

//    int getParent(int p){
//        if (p == parent[p]) {
//            return p;
//        }
//        return getParent(parent[p]);
//    }

    int getParent(int p){
        while(p != parent[p]) {
            parent[p] = parent[parent[p]];
            p = parent[p];
        }
        return p;
    }

    void Union(int p, int q) {
        int pp = getParent(p);
        int qp = getParent(q);
        if (pp == qp) {
            return;
        }
        if (length[pp] > length[qp]) {
            parent[qp] = pp;
            length[qp] += length[pp];
        } else {
            parent[pp] = qp;
            length[pp] += length[qp];
        }
        count--;
    }

    bool IsConnected(int p, int q) {
        int pp = getParent(p);
        int qp = getParent(q);
        return pp == qp;
    }

    int GetCount() {
        return count;
    }

};

#endif //IKITS_DISJOINTSET_H
