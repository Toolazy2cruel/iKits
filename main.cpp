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
#include <stack>
#include "threadpool.h"
#include "algorithm.h"

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



//单调栈
vector<int> getRightMax(vector<int>& nums) {
    vector<int> res;
    if (nums.empty()) {
        return res;
    }
   stack<int> monotonous;
    res.assign(nums.size(), -1);
    int end = nums.size() - 1;
    monotonous.push(nums[end]);
    for (int i = end; i >= 0; i--) {
        while (!monotonous.empty()) {
            if (monotonous.top() > nums[i]) {
                res[i] = monotonous.top();
                break;
            }
            monotonous.pop();
        }
        if (monotonous.empty()) {
            res[i] = -1;
        }
        monotonous.push(nums[i]);
    }
    return res;
}

//单调栈
vector<int> getRightMaxV2(vector<int>& nums) {
    vector<int> res;
    if (nums.empty()) {
        return res;
    }
    stack<int> monotonous;
    res.assign(nums.size(), -1);
    int end = nums.size() - 1;
    monotonous.push(end);
    for (int i = end; i >= 0; i--) {
        while (!monotonous.empty()) {
            int index = monotonous.top();
            if (nums[index] > nums[i]) {
                res[i] = index - i;
                break;
            }
            monotonous.pop();
        }
        if (monotonous.empty()) {
            res[i] = -1;
        }
        monotonous.push(i);
    }
    return res;
}

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

class Solution {
public:
    bool isPalindrome(string s) {
        int left = 0;
        int right = s.size() - 1;
        while (left <= right ) {
            if (s[left] != s[right]) {
                return false;
            }
            left++;
            right--;
        }
        return true;
    }
    string res = "";
    string isPalindrome(string& s, int left, int right) {
        while (left >= 0 && right < s.size() && s[left] == s[right]) {
            left--;
            right++;
        }
        return s.substr(left+1, right-1-left-1+1);
    }

    int getPalindrome(string& s, int left, int right) {
        int num = 0;
        while (left >= 0 && right < s.size() && s[left] == s[right]) {
            num++;
            left--;
            right++;
        }
        return num;
    }

    int  countSubstrings(string s) {
        int num = 0;
        for (int i = 0; i < s.size(); ++i) {
            num += getPalindrome(s, i, i) + getPalindrome(s, i, i+1);
        }
        return num;

    }
};

class SolutionV2 {
public:
    int partion(vector<int>& nums, int left, int right) {
        int cur = nums[left];
        while (left < right) {
            while(left < right && nums[right] >= cur) {
                right--;
            }
            nums[left] = nums[right];
            while(left < right && nums[left] < cur) {
                left++;
            }
            nums[right] = nums[left];
        }
        nums[left] = cur;
        return left;
    }

    void quickSort(vector<int>& nums, int left, int right) {
        if (left < right) {
            int mid = partion(nums, left, right);
            quickSort(nums, left, mid);
            quickSort(nums, mid + 1, right);
        }
    }

    int findUnsortedSubarray(vector<int>& nums) {
        return 0;
    }
};

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

string decodeString(string& s) {
    stack<char> record;
    string res = "";
    for (int i = 0; i < s.size(); ++i) {
        if (s[i] == ']') {
            string inner = "";
            while (record.top() != '[') {
                inner = record.top() + inner;
                record.pop();
            }
            record.pop(); //弹出符号
            int seq = 0, base = 1;
            while (!record.empty() && record.top() >= '0' && record.top() <= '9') {
                int next = record.top() - '0';
                seq += next * base;
                base *= 10;
                record.pop(); //可能有多个数字
            }
            for (int j = 0; j < seq; ++j) {
                for (char k : inner) {
                    record.push(k);
                }
            }
            continue;
        }
        record.push(s[i]);
    }
    while (!record.empty()) {
        res = record.top() + res;
        record.pop();
    }

    return res;

}

vector<double> calcEquation(vector<vector<string>>& equations, vector<double>& values, vector<vector<string>>& queries) {
    std::map<string, map<string, double >> memo;
    for (int i = 0; i < equations.size(); ++i) {
        string pre = equations[i][0];
        string  back = equations[i][1];
        memo[pre][back] = values[i];
        memo[back][pre] = 1 / values[i];
    }
    vector<double> res;
    for (auto & querie : queries) {
        string pre = querie[0];
        string  back = querie[1];
        if (pre == back) {
            res.push_back(1);
            continue;
        }
        auto record = memo.find(pre);
        if (record == memo.end()) {
            res.push_back(-1);
            continue;
        }
        auto detail = record->second.find(back);
        if (detail != record->second.end()) {
            res.push_back(detail->second);
            continue;
        } else {
            //上一层去找
            auto front = memo.find(back);
            if (front != memo.end()) {
                if (front->second.find(pre) != front->second.end()) {
                    res.push_back(detail->second/front->second[pre]);
                }
            } else {
                res.push_back(-1);
            }
        }

    }
    return res;
}

int leastInterval(vector<char>& tasks, int n) {
    if (n == 0) {
        return tasks.size();
    }
    //尽可能不重复任务先执行
    std::map<char, int> momo;
    for (int i = 0; i < tasks.size(); ++i) {
        momo[tasks[i]]++;
    }
    int max_task = 0, n_cols = 0;
    for (auto& val : momo) {
        if (val.second > max_task) {
            max_task = val.second;
        }
    }
    for (auto& val : momo) {
        if (val.second == max_task)
            n_cols++;
    }
    int task_num = tasks.size();
    return max(task_num, (max_task - 1) * (n + 1) + n_cols);
}

void reconstructQueue() {
    vector<vector<int>> people = {{7,0}, {4,4}, {7,1}, {5,0}, {6,1}, {5,2}};
    auto cmp = [](vector<int>& a, vector<int>& b) -> bool{
        return a[0] > b[0] || (a[0] == b[0] && a[1] < b[1] );
    };
    sort(people.begin(), people.end(), cmp);
    cout << printVec(people) << endl;
    vector<vector<int>> res;
    map<pair<int,int>, int> memo;
    memo[std::make_pair(2,3)] = 3;
    memo[std::make_pair(3,3)] = 3;

}

namespace maxSquare{
    bool dfs(vector<vector<char>>& in, int i, int j, int& res) {
        if (i < 0 || i >= in.size() || j < 0 || j >= in[0].size() || in[i][j] == '0') {
            return false;
        }
        res += 1;
        bool is_meet = dfs(in, i + 1, j, res) && dfs(in, i, j + 1, res) && dfs(in, i + 1, j + 1, res);
        cout << "is_meet=" <<is_meet <<endl;
        if (is_meet) {
            return true;
        }
        return true;
    }

    int maxSquare(vector<vector<char>>& matrix) {
        int m = matrix.size();
        if (m == 0) {
            return 0;
        }
        int n = matrix[0].size();
        vector<vector<int>> left(m, vector<int>(n, 0));

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (matrix[i][j] == '1') {
                    left[i][j] = (j == 0 ? 0: left[i][j - 1]) + 1;
                }
            }
        }

        int ret = 0;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (matrix[i][j] == '0') {
                    continue;
                }
                int width = left[i][j];
                int area = width;
                for (int k = i - 1; k >= 0; k--) {
                    width = min(width, left[k][j]);
                    area = max(area, (i - k + 1) * width);
                }
                ret = max(ret, area);
            }
        }
        return ret;
    }
}


std::vector<int> nextGreaterElement(const std::vector<int>& nums) {
    std::vector<int> result(nums.size(), -1); // 初始化结果数组为-1
    std::stack<int> st; // 单调递减栈

    for (int i = 0; i < nums.size(); ++i) {
        while (!st.empty() && nums[i] > nums[st.top()]) {
            // 当前元素大于栈顶元素，更新栈顶元素的下一个更大元素为当前元素
            result[st.top()] = nums[i];
            st.pop();
        }
        st.push(i); // 将当前元素下标入栈
    }

    return result;
}

//括号生成
namespace parenthesis{
    vector<string> res;
    map<string, int> memo;
    bool isValid(string &path) {
        stack<char> selector;
        for (char c : path) {
            if (c == ')') {
                if (selector.empty()) {
                    return false;
                }
                selector.pop();
            } else {
                selector.push(c);
            }
        }
        return selector.empty();
    }

    void backtrack(string &choice, string path) {
        if (path.size() == choice.size()) {
            if (isValid(path) && memo.find(path) == memo.end()) {
                memo[path] = 1;
                res.push_back(path);
            }
            return;
        }
        for (int i = 0; i < choice.size(); ++i) {
            path.push_back(choice[i]);
            backtrack(choice, path);
            path.pop_back();
        }
    }

    vector<string> generateParenthesis(int n) {
        string choice(n, '(');
        choice += string (n, ')');
        string path;
        backtrack(choice, path);
        return res;
    }
}

//解数独
namespace solveSudoku{

    bool isValid(vector<vector<char>>& board, int row, int col, char cur) {
        //行
        for (auto & c : board) {
            if (c[col] == cur) {
                return false;
            }
        }
        //列
        for (char c : board[row]) {
            if (c == cur) {
                return false;
            }
        }

        //3*3
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                int curRow = (row / 3) * 3 + i;
                int curCol = (col / 3) * 3 + j;
                if (curRow == row && curCol == col) {
                    continue;
                }

                if (board[curRow][curCol] == cur) {
                    return false;
                }

            }
        }
        return true;
    }

    bool backtrack(vector<vector<char>>& board, int i, int j) {
        int m = 9, n = 9;
        if (j == n) {
            // 穷举到最后一列的话就换到下一行重新开始。
            return backtrack(board, i + 1, 0);
        }
        if (i == m) {
            // 找到一个可行解，触发 base case
            return true;
        }

        if (board[i][j] != '.') {
            // 如果有预设数字，不用我们穷举
            return backtrack(board, i, j + 1);
        }

        for (char ch = '1'; ch <= '9'; ch++) {
            // 如果遇到不合法的数字，就跳过
            if (!isValid(board, i, j, ch))
                continue;

            board[i][j] = ch;
            // 如果找到一个可行解，立即结束
            if (backtrack(board, i, j + 1)) {
                return true;
            }
            board[i][j] = '.';
        }
        // 穷举完 1~9，依然没有找到可行解，此路不通
        return false;
    }

    bool backteack(vector<vector<char>>& board, int row, int col) {
        if (col == board.size()) {
            return backteack(board, row + 1, 0);
        }

        if (row == board.size()) {
            return true;
        }

        if (board[row][col] != '.') {
            return backteack(board, row, col + 1);
        }
        for (int k = 1; k <= 9; ++k) {
            if (!isValid(board, row, col, k + '0')) {
                continue;
            }
            board[row][col] = k + '0';
            if (backteack(board, row, col + 1)) {
                return true;
            }
            board[row][col] = '.';
        }

        return false;
    }

    void solveSudoku(vector<vector<char>>& board) {
        backteack(board, 0, 0);
    }

}

std::string printVec(vector<vector<char>>& res) {
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


int main() {
    vector<vector<char>> board = {{'5','3','.','.','7','.','.','.','.'},{'6','.','.','1','9','5','.','.','.'},{'.','9','8','.','.','.','.','6','.'},{'8','.','.','.','6','.','.','.','3'},{'4','.','.','8','.','3','.','.','1'},{'7','.','.','.','2','.','.','.','6'},{'.','6','.','.','.','.','2','8','.'},{'.','.','.','4','1','9','.','.','5'},{'.','.','.','.','8','.','.','7','9'}};
    solveSudoku::solveSudoku(board);
    cout << printVec(board).c_str() << endl;


//    string s = "1000[a]";
//    cout << decodeString(s) << endl;
//
//    cout << max(2,3) << endl;
//    vector<vector<char>> in = {{'1', '1', '1'}, {'1', '0', '1'}, {'1', '0', '1'}};
//    cout << maxSquare::maxSquare(in) << endl;
//    test_maxDepth();
    //reconstructQueue();
//    Solution s;
//    string a = "abbda";
//    cout << "length=" << s.countSubstrings(a) << endl;
//std::map<int, int> temp = {{1,2}, {3,4}};
//    for (auto val:temp) {
//        cout << val.second;
//    }
//        SolutionV2 ss;
//    std::vector<int> nums = {5,4,6,2,9,0};
//        ss.quickSort(nums, 0, nums.size()-1);
//    for (int i = 0; i < nums.size(); ++i) {
//        cout << "i=" << nums[i] << endl;
//    }

    //testLFU();
//    std::vector<int> nums = {1,1, 3, 0, 0,0};
//    auto res= getRightMaxV2(nums);
//    for (int i = 0; i < res.size(); ++i) {
//        cout << "ele=" << res[i] << endl;
//    }
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
