#include <bits/stdc++.h>
using namespace std;

struct Node {
    int val, id;
    bool operator<(const Node& other) const {
        return val < other.val; 
    }
};

struct CustomCompare {
    bool operator()(const pair<int, int>& a, const pair<int, int>& b) {
        if (a.first != b.first) return a.first > b.first; 
        return a.second < b.second;                     
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // MAX-HEAP
    priority_queue<int> max_heap;
    max_heap.push(10);                   
    max_heap.emplace(20);                
    int top_max = max_heap.top();   
    max_heap.pop();            

    // MIN-HEAP
    priority_queue<int, vector<int>, greater<int>> min_heap;
    min_heap.push(10);
    min_heap.push(5);
    int top_min = min_heap.top();        

    vector<int> nums = {4, 1, 7, 3, 8};
    priority_queue<int> heap_from_vec(nums.begin(), nums.end()); 

    priority_queue<Node> custom_heap;
    custom_heap.push({100, 1});

    priority_queue<pair<int, int>, vector<pair<int, int>>, CustomCompare> pair_heap;
    pair_heap.push({1, 50});

    bool empty_check = max_heap.empty();   
    size_t total_size = max_heap.size();   

    while (!heap_from_vec.empty()) {
        int curr = heap_from_vec.top();
        heap_from_vec.pop();
    }

    return 0;
}
