/* For 2-dimensional */
/* Not Generalized k-dimensional range tree */
/* Constructed from a static array of points sorted by first coordinate */

#include <iostream>
#include <algorithm>
#include <vector>
#include <array>

using namespace std;

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
        NodeRange2D(int inf = 0, int sup = 0): count(0), inferior_limit(inf), superior_limit(sup), left(nullptr), right(nullptr) {}
    };

    NodeRange2D* root;
    int NMAX;

    void dimensional_merge(NodeRange2D *parent, const NodeRange2D *left, const NodeRange2D *right) {
        parent->inferior_limit = min(left->inferior_limit, right->inferior_limit);
        parent->superior_limit = max(left->superior_limit, right->superior_limit);
        
        parent->count = left->count + right->count;
        parent->last_dimension.resize(parent->count);
        
        merge(left->last_dimension.begin(), left->last_dimension.end(),
              right->last_dimension.begin(), right->last_dimension.end(),
              parent->last_dimension.begin());
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
        dimensional_merge(node, node->left, node->right);
    }

    Range2D(int n): NMAX(n) {
        root = new NodeRange2D();
        build(root, 0, n);
    }

    int dimensional_query(NodeRange2D* node, int l, int r) {
        int l_res = lower_bound(node->last_dimension.begin(), node->last_dimension.end(), l) - node->last_dimension.begin();
        int r_res = upper_bound(node->last_dimension.begin(), node->last_dimension.end(), r) - node->last_dimension.begin();
        return r_res - l_res;
    }

    int query(NodeRange2D* node, int l1, int r1, int l2, int r2) {
        if (node == nullptr) return 0;
        if (r1 < node->inferior_limit or node->superior_limit < l1) return 0;
        if (l1 <= node->inferior_limit and node->superior_limit <= r1) return dimensional_query(node, l2, r2);
        int a = query(node->left, l1, r1, l2, r2);
        int b = query(node->right, l1, r1, l2, r2);
        return a + b;
    }

    int Que(int l1, int r1, int l2, int r2) {
        return query(root, l1, r1, l2, r2);
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
