#include <iostream>
using namespace std;

#include "DataStructure/Segtree.h"

int op(int a, int b) {
	return min(a, b);
}
int e() {
	return INT_MAX;
}

int main() {
	int N, Q;
	cin >> N >> Q;
	segtree<int, op, e> ST(N);
	for (int i = 0; i < N; i++) {
		int in;
		cin >> in;
		ST.set(in, i);
	}

	for (int q = 0; q < Q; q++) {
		int l, r;
		cin >> l >> r;
		cout << ST.prod_NotRecursive(l, r) << endl;
	}
}