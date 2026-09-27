#include <iostream>
#include <vector>

using namespace std;

template<typename T>
struct PersistentSegmentTree {
  struct Node {
    T data;
    int left = 0;
    int right = 0;
  };

  int n;
  vector<Node> tree;
  vector<int> version_roots;
  
  PersistentSegmentTree(int n): n(n) {
    tree.push_back({T(), 0, 0});
    if (n <= 0) return;
    int root = build(0, n-1);
    version_roots.emplace_back(root);
  }

  PersistentSegmentTree(const vector<T>& a) : n((int)a.size()){
    tree.push_back({T(), 0, 0});
    if (n <= 0) return;
    int root = build(0, n-1, a);
    version_roots.emplace_back(root);
  }

  int new_node(T data = T(), int l=0, int r=0){
    tree.push_back({data, l, r});
    return (int)tree.size()-1;
  }

  int build(int l, int r, const vector<T>& a){
    int u = new_node();
    if (l == r) {
      tree[u].data = a[l];
      return u;
    }
    int mid = l + (r - l)/2;
    tree[u].left = build(l, mid, a);
    tree[u].right = build(mid+1, r, a);
    tree[u].data = tree[tree[u].left].data + tree[tree[u].right].data;
    return u;
  }

  int build(int l, int r){
    int u = new_node();
    if (l == r) {
      tree[u].data = T();
      return u;
    }
    int mid = l + (r-l)/2;
    tree[u].left = build(l, mid);
    tree[u].right = build(mid+1, r);
    tree[u].data = tree[tree[u].left].data + tree[tree[u].right].data;
    return u;
  }

  int update(int prev_u, int l, int r, int pos, T val){
    int u = new_node(tree[prev_u].data, tree[prev_u].left, tree[prev_u].right);
    if (l == r) {
      tree[u].data = val;
      return u;
    }
    int mid = l + (r-l)/2;
    if (pos <= mid) {
      tree[u].left = update(tree[prev_u].left, l, mid, pos, val);
    } else {
      tree[u].right = update(tree[prev_u].right, mid+1, r, pos, val);
    }

    tree[u].data = tree[tree[u].left].data + tree[tree[u].right].data;
    return u;
  }

  int update(int version, int pos, T val){
    int new_root = update(version_roots[version], 0, n-1, pos, val);
    version_roots.emplace_back(new_root);
    return (int)version_roots.size()-1;
  }

  T query(int u, int l, int r, int ql, int qr){
    if (!u || ql > r || qr < l) return T();
    if (ql <= l && r <= qr) return tree[u].data;
    int mid = l + (r-l)/2;
    return query(tree[u].left, l, mid, ql, qr) + query(tree[u].right, mid+1, r, ql, qr);
  }

  T query(int version, int ql, int qr){
    return query(version_roots[version], 0, n-1, ql, qr);
  }

  int get_current_version(){
    return (int)version_roots.size()-1;
  }

};


template<typename data_type>
struct PersistentQueue {
  vector<int> roots;
  vector<int> heads;
  vector<int> tails;
  PersistentSegmentTree<data_type> data;

  PersistentQueue(int max_cap) : data(max_cap){
    roots.emplace_back(data.get_current_version());
    heads.emplace_back(0);
    tails.emplace_back(0);
  }

  void push(int version, data_type value){
    data.update(roots[version], tails[version], value);
    roots.emplace_back(data.get_current_version());
    heads.emplace_back(heads[version]);
    tails.emplace_back(tails[version] + 1);
  }

  data_type pop(int version){
    data_type res = data.query(roots[version], heads[version], heads[version]);
    roots.emplace_back(roots[version]);
    heads.emplace_back(heads[version]+1);
    tails.emplace_back(tails[version]);
    return res;
  }
};
