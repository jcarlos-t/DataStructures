/* For 2-dimensional */
/* Not Generalized k-dimensional range tree */
/* Constructed from a static array of points sorted by first coordinate */
/* with fractional cascading*/

#include <iostream>
#include <algorithm>
#include <vector>
#include <array>

using namespace std;

/* macro for easy reading */
#define all(v) (v).begin(), (v).end()

/* Where we store the points */

const int nm = 3e5 + 5;
const int km = 2;

array<int, km> Points[nm];

/* The data structure */

struct Range2D {
    
    struct NodeRange2D {
        int count;
        int inferior_limit;
        int superior_limit;
        NodeRange2D* left;
        NodeRange2D* right;
        vector<int> last_dimension;

        /* Fractional Cascading bridges */
        /* bridge: lower bound index in respective son (left / right) */
        vector<int> left_bridge;
        vector<int> right_bridge;

        NodeRange2D(int inf = 0, int sup = 0): count(0), inferior_limit(inf), superior_limit(sup), left(nullptr), right(nullptr) {}
    };

    NodeRange2D* root;
    int NMAX;

    void fractional_merge(NodeRange2D* parent, NodeRange2D* left, NodeRange2D* right) {
        parent->inferior_limit = min(left->inferior_limit, right->inferior_limit);
        parent->superior_limit = max(left->superior_limit, right->superior_limit);
        
        parent->count = left->count + right->count;
        parent->last_dimension.resize(parent->count);
        parent->left_bridge.resize(parent->count + 1);
        parent->right_bridge.resize(parent->count + 1);
        

        /* Renaming variables for easy coding */
        int p = 0;
        int l = 0;
        int r = 0;

        int best_l = 0;
        int best_r = 0;

        vector<int>& arr_p = parent->last_dimension;
        vector<int>& arr_l = left->last_dimension;
        vector<int>& arr_r = right->last_dimension;
        vector<int>& bridge_l = parent->left_bridge;
        vector<int>& bridge_r = parent->right_bridge;

        /* Special merge for retain information for bridges */
        /* lower bounds without copy */

        while(l < left->count and r < right->count) {
            bridge_l[p] = l;
            bridge_r[p] = r;
            if (arr_l[l] <= arr_r[r]) arr_p[p++] = arr_l[l++];
            else arr_p[p++] = arr_r[r++];
        }
        while (l < left->count) {
            bridge_l[p] = l;
            bridge_r[p] = r;
            arr_p[p++] = arr_l[l++];
        }
        while (r < right->count) {
            bridge_l[p] = l;
            bridge_r[p] = r;
            arr_p[p++] = arr_r[r++];
        }

        /* Super bounds */
        bridge_l[parent->count] = l;
        bridge_r[parent->count] = r;
    } 

    void build(NodeRange2D* node, int l, int r) {
        if (l + 1 == r) {
            node->inferior_limit = Points[l][0];
            node->superior_limit = Points[l][0];
            node->count = 1;
            node->last_dimension = { Points[l][1] };
            return;
        }
        int mid = (r + l) / 2;
        node->left = new NodeRange2D();
        node->right = new NodeRange2D();
        build(node->left, l, mid);
        build(node->right, mid, r);
        fractional_merge(node, node->left, node->right);
    }

    Range2D(int n): NMAX(n) {
        root = new NodeRange2D();
        build(root, 0, NMAX);
    }

    // lo: lower bound
    // hi: upper bound
    int cascading_query(NodeRange2D* node, int l1, int r1, int lo, int hi) {
        if (node == nullptr) return 0;
        if (r1 < node->inferior_limit or node->superior_limit < l1) return 0;
        if (l1 <= node->inferior_limit and node->superior_limit <= r1) return hi - lo;
        int res = 0;
        if (node->left) 
            res += cascading_query(node->left, l1, r1, node->left_bridge[lo], node->left_bridge[hi]);
        if (node->right) 
            res += cascading_query(node->right, l1, r1, node->right_bridge[lo], node->right_bridge[hi]);
        return res;
    }

    int Que(int l1, int r1, int l2, int r2) {
        int lo = lower_bound(all(root->last_dimension), l2) - root->last_dimension.begin();
        int hi = upper_bound(all(root->last_dimension), r2) - root->last_dimension.begin();
        return cascading_query(root, l1, r1, lo, hi);
    }

};


int main () {

    /* Reading input */
    int n, q; cin >> n >> q;
    for (int i = 0; i < n; i++) {
        cin >> Points[i][0] >> Points[i][1];
    }

    /* Ordering array */
    sort(Points, Points + n);


    /* Creating 2-dimensional range tree */
    Range2D r2d(n);

    /* Template form of queries */
    /* x in [l1,r1] and y in [l2, r2] */
    /* return number of points in that range */

    for (int i = 0; i < q; i++) {
        int l1, r1, l2, r2;
        cin >> l1 >> r1 >> l2 >> r2;
        cout << r2d.Que(l1, r1, l2, r2) << "\n";
    }

    return 0;
}
