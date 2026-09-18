#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
const long long MOD = 1e9;
const int MAX_N = 450005;
const int MAX_NODES = 9500000;

vector<long long> vorigin;
vector<long long> vals;

int get_compressed(long long x){
  return lower_bound(vals.begin(), vals.end(), x) - vals.begin() + 1;
}

long long get_original(int cmpi){
  return vals[cmpi - 1];
}

struct SegmentTreeNode {
  int data; // frecuencia
  int left, right; //indices
};

SegmentTreeNode tree[MAX_NODES];
int node_cnt = 0;

int clone_node(int prev_idx) {
  node_cnt++;
  tree[node_cnt] = tree[prev_idx];
  return node_cnt;
}

struct PersistentSegmentTree {
  int n;
  int U;
  vector<int> version_roots;

  PersistentSegmentTree(int uval, int nv) {
    n = nv;
    U = uval;
    version_roots[0] = 0;
    build();
  }

  void build(){
    for (int p = 1; p <= n; p++) {
      insert_freq(p);
    }
  }

  int update(int val, int last_node, int l, int r) {
    int curr_node = clone_node(last_node);
    tree[curr_node].data +=1;
    if ( l == r) {
      return curr_node;
    }

    int mi = l + (r-l) / 2;
    if (val <= mi) {
      tree[curr_node].left = update(val, tree[last_node].left, l, mi);
    }
    else {
      tree[curr_node].right = update(val, tree[last_node].right, mi+1, r);
    }
    return curr_node;
  }

  void insert_freq(int p) {
    int val_comp = get_compressed(vorigin[p]);
    int new_root = update(val_comp, version_roots[p-1], 1, U);
    version_roots.emplace_back(new_root);
  }

  int query_solve(int l_idx, int r_idx, int k){
    int start = version_roots[l_idx -1];
    int end = version_roots[r_idx];
    int l = 1; int r = U;
    while (l < r) {
      int mi = l + (r-l)/2;
      int cizq = tree[tree[end].left].data - tree[tree[start].left].data;
      if (k <= cizq) {
        start = tree[start].left;
        end = tree[end].left;
        r = mi;
      } else {
        k -= cizq;
        start = tree[start].right;
        end = tree[end].right;
        l = mi+1;
      }
    }
    return get_original(l);
  }
};


int main (){
  ios::sync_with_stdio(0); cin.tie(0);
  int n, b;
  long long a, l, m;
  cin>>n;
  cin>>a>>l>>m;
  cin>>b;
  vorigin.resize(n+1);
  // generar arreglo
  vorigin[0] = 0;
  vorigin[1] = a;
  for (int i = 2; i <= n; i++) {
    vorigin[i] = (vorigin[i-1]*l + m)%MOD;
  }
  vals.assign(vorigin.begin()+1, vorigin.end());
  sort(vals.begin(), vals.end());
  vals.erase(unique(vals.begin(), vals.end()), vals.end());
  int U = vals.size();
  PersistentSegmentTree st(U, n);
  long long sum_total = 0;
  for (int i = 0; i < b; i++) {
    int g,x1,lx,mx,y1,ly,my,k1,lk,mk;
    cin>>g>>x1>>lx>>mx>>y1>>ly>>my>>k1>>lk>>mk;
    long long xgb, ygb, kgb, xg, yg, ig, jg, kg;
    for(int gval = 1; gval <= g; gval++){
      if (gval > 1){
        xg = (((xgb-1)*lx+mx)%n)+1;
        yg = (((ygb-1)*ly+my)%n)+1;
        ig = min(xg, yg);
        jg = max(xg, yg);
        kg = (((kgb-1)*lk+mk)%(jg-ig+1))+1;
        xgb = xg;
        ygb = yg;
        kgb = kg;
      } else {
        ig = min(x1,y1);
        jg = max(x1,y1);
        kg = k1;
        xgb = x1;
        ygb = y1;
        kgb = k1;
      }
      sum_total += st.query_solve(ig, jg, kg);
    }
  }
  cout<<sum_total<<"\n";
}

