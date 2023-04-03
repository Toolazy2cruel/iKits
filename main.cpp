#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <map>
#include <algorithm>
#include <memory>
#include "test_template.h"
#include "unistd.h"
#include "LRUCache.h"
#include "LFUCache.h"
#include "LFUCache.cpp"
#include "threadpool.h"

using namespace std;

int mult_mod(int a, int b, int m) {
    int res = 0;
    while (b) {
        if (b & 1) {
            res = (res + a) % m;
        }
        a = (a + a) % m;
        b >>= 1;
    }
    return res;
}

int mult_mod_v2(int a, int b, int m) {
    int c = a * b - (int)((double)a * b / m + 0.5) * m;
    return c < 0 ? c + m : c;  //就是算的a*b%m;
}

//1，2，3 => 1，3，2
void nextpermutation(std::vector<int>& nums) {

    int right = nums.size();


    //从右往左找1个

}

namespace Combine{
    vector<vector<int>> res;

    void dfs(vector<int>& nums, vector<int>& temp, vector<int>& memo) {
        if (nums.size() == temp.size()) {
            res.push_back(temp);
            return ;
        }
        for (int i = 0; i < nums.size(); ++i) {
            if (memo[i] == 1) {
                continue;
            }
            cout << nums[i] << endl;
            temp.push_back(nums[i]);
            memo[i] = 1;
            dfs(nums, temp, memo);
            temp.pop_back();
            memo[i] = 0;
        }
    }
    vector<vector<int>> combine(vector<int>& nums) {
        vector<int> temp;
        std::vector<int> memo(nums.size(), 0);
        dfs(nums, temp, memo);
        return res;
    }
}

std::string printVec(vector<vector<int>>& res) {
    std::stringstream ss;
    for (int i = 0; i < res.size(); ++i) {
        ss << "[";
        for (int j = 0; j < res[i].size(); ++j) {
            ss << res[i][j];
            if (j != res[i].size()) {
                ss << ",";
            }
        }
        ss << "]";
    }
    return ss.str();
}

//判断
std::string minWindow(string s, string t) {
    std::map<char, int> window, goals;
    for (char i : t) {
        goals[i]++;
    }

    int left = 0, right = 0, start = 0;
    int minor = INT_MAX;
    int valid = 0;
    while(right < s.size()) {
        //扩窗口
        char c = s[right];
        right++;
        if (goals.count(c)) {
            window[c]++;
            if (window[c] == goals[c]) {
                valid++;
            }
        }
        while (valid == goals.size()) {
            char d = s[left];
            //更新长度
            if (right - left < minor) {
                start = left;
                minor = right - left;
            }
            left++;
            if (goals.count(d)) {
                if (window[d] == goals[d]) {
                    valid--;
                }
                window[d]--;
            }
        }
    }
    return minor == INT_MAX ? "":s.substr(start, minor);
}


vector<int> maxSlidingWindow(vector<int>& nums, int k) {
    if (k == 1) {
        return nums;
    }
    int most = INT_MIN, minor = INT_MIN;
    vector<int> ans;
    int left = 0, right = 0;
    map<int, int> window;
    while (left < nums.size() && right < nums.size()) {
        //放入window
        if (nums[right] > most) {
            most = nums[right]; //最大
        } else{
            if (nums[right] > minor) {
                minor = nums[right] ; //次大
            }
        }
        cout << "most=" << most << "minor=" << minor << endl;
        //window数到k + 1个
        if (right - left == k - 1) {
            //push当前一个最大值
            ans.push_back(most);
            //update 左边
            window[nums[left]]--;
            //左值就是唯一的上个最大值
            if (window[nums[left]] == 0 && nums[left] == most) {
                most = minor;
                minor = INT_MIN;
            }
            left++;
        }
        window[nums[right]]++;
        right++; //扩大窗口
    }
    return ans;
}

bool res = false;
std::map<string, int> memo;
void dfs(string s, vector<string>& wordDict) {
    if (res) {
        return;
    }
    if (s.empty()) {
        if (!res) {
            res = true;
            return;
        }
    }
    for (int i = 0; i < wordDict.size(); i++) {
        string cur_s = s;
        memo.insert(std::make_pair(wordDict[i], 1));
        if (s.size() < wordDict[i].size())
            continue;
        bool is_meet = false;
        int pos = s.find(wordDict[i]);
        while (pos != s.npos && pos ==0) {
            s.erase(pos, wordDict[i].size());
            if (!is_meet) {
                is_meet = true;
            }
            pos = s.find(wordDict[i]);
            if (memo.find(s) != memo.end()) {
                if (!res) {
                    res = true;
                    return;
                }
            }
        }
        cout << "s=" << s << ",d=" << wordDict[i] << endl;
        if (is_meet) {
            dfs(s, wordDict);
        }
        s = cur_s;
    }
}

bool wordBreak(string s, vector<string>& wordDict) {
    dfs(s, wordDict);
    return res;
}

int glob_number = 0;
std::mutex mutex_;

void threadJobs() {
    time_t now = time(nullptr);
    //mutex_.lock();
    std::lock_guard<std::mutex> locker(mutex_);

    cout << "start_time=" << now << endl;
    for (int i = 0; i < 100; ++i) {
        glob_number += 1;
    }
    //mutex_.unlock();
}

void testThreadPool() {
    operations_research::ThreadPool pool("test", 100);
    pool.StartWorkers();
    for (int i = 0; i < 50; ++i) {
        pool.Schedule(threadJobs);
    }
    sleep(3);
    cout << "glob_number=" << glob_number << endl;
}

struct DopFilterInfo {
    void clear() {
        dopDetailInfo.clear();
    }
    //dop维度common信息
    struct DopDetailInfo{
        DopDetailInfo(): filter_ruler(0), filter_line(0), fcx_match(-1) {}
        int filter_ruler;
        int filter_line;
        int fcx_match; //当前dop的车型信息
    };

    //dop外层索引
    std::map<int, DopDetailInfo> dopDetailInfo;

};

void testMapStruct() {
    DopFilterInfo dop[23][24];
    for (auto & i : dop) {
        for (auto & j : i) {
            j.dopDetailInfo.insert(std::make_pair(2021, DopFilterInfo::DopDetailInfo()));
        }
    }

    for (auto & i : dop) {
        for (auto & j : i) {
            for (auto & info: j.dopDetailInfo) {
                cout << "reason=" << info.first << "value=" << info.second.filter_ruler << "fcx=" << info.second.fcx_match << endl;
            }

        }
    }


}

template<typename T, typename U>
auto add(T x, U y) -> decltype(x+y) {
    return x+y;
}

template< typename T, typename U, int value>
class SuckType {
public:
    T a;
    U b;
    SuckType():a(value),b(value){}
};
template <typename T>
using NewType = SuckType<int, T, 1>;    // 合法

void testC11() {
    auto x = 1;
    auto y = 2;
    decltype(x+y) z;
    z = x + y + 10;
    cout << "z=" << z << endl;

    add(x, y);
    NewType<int> ss;
    cout << "ss=" << ss.b;

}

struct A
{
    explicit A(int i): a(i) {}
    A(double d,int i): a(d),b (i){}
    A(float f,int i,char* c):a(f),b (i),c(c){}
    //...等等系列的构造函数版本

    int a;
    int b;
    char* c;
};
struct B:A
{
    using A::A;
    //关于基类各构造函数的继承一句话搞定
    //......
};

class AP
{
public:
    int i;
    virtual void func() {
        cout << "Ap func1" << endl;
    }
    virtual void func2() {
        cout << "Ap func2" << endl;
    }
};
class BP : public AP
{
    int j;
    void func() {
        cout << "Bp func1" << endl;
    }
};

struct TestObj{
    TestObj(int a_, int b_): a(a_), b(b_) {}
    int a;
    int b;
};

int sumClac(vector<int>& nums) {
    if (nums.empty()) {
        return 0;
    }
    int res = 0;
    int left = 0, right = nums.size() -1;
    sort(nums.begin(), nums.end());
    int min = nums[left];
    int max = nums[right];
    while(min != max) {
        //前n-1个数+1
        int bias = nums[right] - nums[left];
        res += bias;
        for (int i = 0; i < right; ++i) {
            nums[i] += bias;
            if (nums[i] > max) {
                max = nums[i];
            } else if (nums[i] < min) {
                min = nums[i];
            }
        }
    }
    return res;
}

void testLRU() {
    LRUCache cache(2);
    cache.Put(2,3);
    cache.Put(3,4);
    cache.Put(4,1);
    cout << cache.Get(2) << endl;
    cout << cache.Get(3) << endl;
    cout << cache.Get(4) << endl;
    cache.Put(5,6);
    cout << cache.Get(5) << endl;
}

void testLFU() {
    LFUCache cache(2);
    cache.put(2,3);
    cache.put(3,4);
    cache.put(4,1);
    cout << cache.get(2) << endl;
    cout << cache.get(3) << endl;
    cout << cache.get(4) << endl;
    cache.put(5,6);
    cout << cache.get(5) << endl;
}

int main() {
    testLFU();
//    std::unique_ptr<TestObj> pt(new TestObj(3,4));
//    std::unique_ptr<TestObj> ps = std::move(pt);
//    std::vector<int> nums = {1};
//    cout << sumClac(nums) << endl;
//    cout << pt->a << endl;
//    pt->a = 5;
    //能查到BP的虚函数指针
//    AP* p = new BP();
//    p->func();

    //TestTemplate<int> a;
    //a.f();
    //TestTemplateV2();
//    B b(2);
//    cout << "b=" << b.c << endl;
//    testC11();
//    std::vector<int> nums = {1,2,3};
//    Combine::combine(nums);
//    cout << printVec(Combine::res).c_str() << endl;

    //cout << minWindow("BANCCCCC", "ABC") << endl;
    //testMapStruct();
    //testThreadPool();
//    vector<int> nums = {9,10,9,-7,-4,-8,2,-6};
//    maxSlidingWindow(nums, 5);
//
//    std::cout << (10>>1) << std::endl;
//    std::string ss = "catskicatcats";
//    std::vector<string> dict = {"cats","cat","dog","ski"};
//    cout << wordBreak(ss, dict);
//    ss.erase(1,4);
//    std::cout << ss  << std::endl;
//    std::cout << "find_res=" << ss.find("6")  << std::endl;
//    std::cout << mult_mod(2, 0.5, INT_MAX) << std::endl;
    return 0;
}
