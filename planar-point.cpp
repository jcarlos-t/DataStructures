#include <bits/stdc++.h>
using namespace std;

const int ADD = -1e9 - 1;
const int REM = -1e9 - 2;
const int QUERY = -1e9 - 3;

template<typename T>
struct FenwickTree {
	vector<T> ft;
	
	FenwickTree(int n) {
		ft.resize(n + 1);
		fill_n(ft.begin(), n + 1, T(0));
	}
	
	void update(int pos, T val) {
		++pos;
		while (pos < ft.size()) {
			ft[pos] += val;
			pos += (-pos) & pos;
		}
	}
	
	T get_sum(int pos) {
		++pos;
		T res = T(0);
		while (pos > 0) {
			res += ft[pos];
			pos &= pos - 1;
		}
		return res;
	}
	
	T query(int l, int r) {
		return get_sum(r) - get_sum(l - 1);
	}
};

int compress(vector<int> &Y) {
	vector<int> values(Y.begin(), Y.end());
	sort(values.begin(), values.end());
	values.erase(unique(values.begin(), values.end()), values.end());
	for (int &x : Y) {
		x = lower_bound(values.begin(), values.end(), x) - values.begin();
	}
	return values.size();
}

int main() {
	cin.tie(0) -> sync_with_stdio(false);
	int n;
	cin >> n;
	vector<int> Y;
	vector<int> X;
	vector<tuple<int, int, int>> events;
	for (int i = 0; i < n; ++i) {
		int a, b, c, d;
		cin >> a >> b >> c >> d;
		if (a == c) {
			if (b > d) swap(b, d);
			Y.emplace_back(a);
			Y.emplace_back(a + 1);
			X.emplace_back(b);
			X.emplace_back(d + 1);
		}
		else {
			if (a > c) swap(a, c);
			Y.emplace_back(a);
			Y.emplace_back(c + 1);
			X.emplace_back(QUERY);
			X.emplace_back(b);
		}
	}
	int m = compress(Y);
	for (int i = 0; i < X.size(); i += 2) {
		if (X[i] == QUERY) {
			events.emplace_back(X[i + 1], Y[i], Y[i + 1] - 1);
		}
		else {
			events.emplace_back(X[i], ADD, Y[i]);
			events.emplace_back(X[i + 1], REM, Y[i]);
		}
	}
	sort(events.begin(), events.end());
	int res = 0;
	FenwickTree<int> F(m);
	for (auto &e : events) {
		int t, l, r;
		tie(t, l, r) = e;
		if (l == ADD) {
			F.update(r, 1);
		}
		else if (l == REM) {
			F.update(r, -1);
		}
		else {
			res += F.query(l, r);
		}
	}
	cout << res << '\n';
	return 0;
}
