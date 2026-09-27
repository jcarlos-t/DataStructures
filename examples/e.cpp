#include <algorithm>
#include <iostream>
#include <vector>

using namespace::std;

const int MOD = 1e9;

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
      tree[u].data += val;
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

  int kthi(int k, int l, int r, int last, int cur){
    if (l == r) {
      return l;
    }
    int left_size = tree[tree[cur].left].data - tree[tree[last].left].data;
    if (left_size >= k) return kthi(k, l, l + (r-l)/2, tree[last].left, tree[cur].left);
    return kthi(k -  left_size, l + (r-l)/2 + 1, r, tree[last].right, tree[cur].right);
  }

  int kth(int last_ver, int cur_ver, int k){
    return kthi(k, 0, n-1, version_roots[last_ver], version_roots[cur_ver]);
  }

};


int main() {
    cin.tie(0) -> sync_with_stdio(false);
    int n;
    cin >> n;
    vector<int> a(n);
    cin >> a[0];
    int l, m;
    cin >> l >> m;
    for (int i = 1; i < n; ++i) {
        a[i] = (1ll * a[i - 1] * l + m) % MOD;
    }
    vector<int> values(a.begin(), a.end());
    sort(values.begin(), values.end());
    values.erase(unique(values.begin(), values.end()), values.end());
    for (int i = 0; i < n; ++i) a[i] = lower_bound(values.begin(), values.end(), a[i]) - values.begin();
    l = values.size();
    PersistentSegmentTree<int> data(l);
    for (int i = 0; i < n; ++i) {
        data.update(i, a[i], 1);
    }
    int b;
    cin >> b;
    long long res = 0;
    while (b--) {
        int q;
        cin >> q;
        int x1, lx, mx;
        cin >> x1 >> lx >> mx;
        int y1, ly, my;
        cin >> y1 >> ly >> my;
        int k1, lk, mk;
        cin >> k1 >> lk >> mk;
        int ig = min(x1, y1), jg = max(x1, y1);
        int cur = data.kth(ig - 1, jg, k1);
        res += values[cur];
        for (int i = 1; i < q; ++i) {
            x1 = (1ll * (x1 - 1) * lx + mx) % n + 1;
            y1 = (1ll * (y1 - 1) * ly + my) % n + 1;
            ig = min(x1, y1);
            jg = max(x1, y1);
            k1 = (1ll * (k1 - 1) * lk + mk) % (jg - ig + 1) + 1;
            int cur = data.kth(ig - 1, jg, k1);
            res += values[cur];
        }
    }
    cout << res << '\n';
    return 0;
}
