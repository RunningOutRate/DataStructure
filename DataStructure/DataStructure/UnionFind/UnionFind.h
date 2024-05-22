#pragma once

using namespace std;

#include <cassert>
#include <vector>

class Union_Find {
public:
    Union_Find(int n) {
        assert(n >= 0);
        _n = n;
        _union_size.resize(n, 1);
        _parent.resize(n, -1);
    }
    int root(int v) {
        assert(v >= 0 && v < _n);
        if (_parent[v] != -1) {
            _parent[v] = root(_parent[v]);
            return _parent[v];
        }
        else {
            return v;
        }
    }
    bool isSame(int u, int v) {
        assert(u >= 0 && u < _n && v >= 0 && v < _n);
        return root(u) == root(v);
    }
    // ƒTƒCƒY‚Ì¬‚³‚¢•û‚Ìroot‚Ì
    void merge(int u, int v) {
        assert(u >= 0 && u < _n && v >= 0 && v < _n);

        int root_u = root(u);
        int root_v = root(v);
        if (root_u == root_v) {
            return;
        }
        int us_u = _union_size[root_u];
        int us_v = _union_size[root_v];

        if (us_u > us_v) {
            _parent[root_v] = root_u;
            _union_size[root_u] += _union_size[root_v];
        }
        else {
            _parent[root_u] = root_v;
            _union_size[root_v] += _union_size[root_u];
        }
    }
private:
    size_t _n;
    vector<int> _union_size;
    vector<int> _parent;
};