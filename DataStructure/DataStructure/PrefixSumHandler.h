#pragma once

#include <vector>

/// 1D Prefix Sum ///////////////////////////////////////////////////////////////////////////////////////////////////////

template<class S>
class PrefixSumHandler1D {
public:
    PrefixSumHandler1D(int n, vector<S>& A) {
        _n = n;
        _data.resize(n + 1);
        for (int i = 0; i < n; i++) {
            _data[i + 1] = _data[i] + A[i];
        }
    }
    // sum: A[L, R)
    S sum(int L, int R) {
        return _data[R] - _data[L];
    }
    // prefix_sum[idx]
    S prefix_sum_at(int idx) {
        return _data[idx];
    }
    // A[idx]
    S original_at(int idx) {
        return sum(idx, idx + 1);
    }

private:
    int _n;
    vector<S> _data;
};

/// END: 1D Prefix Sum ///////////////////////////////////////////////////////////////////////////////////////////


/// 2D Prefix Sum //////////////////////////////////////////////////////////////////////////////////

// A[height][width]‚Ì—İÏ˜aˆ—‚ğs‚¤
template<class S>
class PrefixSumHandler2D {
public:
    PrefixSumHandler2D(int height, int width, vector<vector<S>>& A) {
        _nHeight = height;
        _nWidth = width;
        _data.resize(height + 1);
        for (int h = 0; h < height + 1; h++) {
            _data[h].resize(width + 1);
        }

        for (int h = 0; h < height; h++) {
            for (int w = 0; w < width; w++) {
                _data[h + 1][w + 1] = _data[h + 1][w] + _data[h][w + 1] - _data[h][w] + A[h][w];
            }
        }
    }

    // sum: A[h1, h2)[w1, w2)
    S sum(int h1, int w1, int h2, int w2) {
        return _data[h2][w2] - _data[h1][w2] - _data[h2][w1] + _data[h1][w1];
    }
    S prefix_sum_at(int h, int w) {
        return _data[h][w];
    }
    S original_at(int h, int w) {
        return sum(h, w, h + 1, w + 1);
    }

private:
    int _nHeight;
    int _nWidth;
    vector<vector<S>> _data;
};

/// END: 2D Prefix Sum //////////////////////////////////////////////////////////////////////////

/// 3D Prefix Sum ///////////////////////////////////////////////////////////////////////////////

// A[x][y][z]‚Ì—İÏ˜aˆ—‚ğ‚·‚é
template<class S>
class PrefixSumHandler3D {
public:
    PrefixSumHandler3D(int X, int Y, int Z, vector<vector<vector<S>>>& A) {
        _nx = X;
        _ny = Y;
        _nz = Z;
        _data.resize(X + 1);
        for (int x = 0; x < X + 1; x++) {
            _data[x].resize(Y + 1);
            for (int y = 0; y < Y + 1; y++) {
                _data[x][y].resize(Z + 1);
            }
        }

        for (int x = 0; x < X; x++) {
            for (int y = 0; y < Y; y++) {
                for (int z = 0; z < Z; z++) {
                    _data[x + 1][y + 1][z + 1] = _data[x][y + 1][z + 1] + _data[x + 1][y][z + 1] + _data[x + 1][y + 1][z]
                        - _data[x][y][z + 1] - _data[x][y + 1][z] - _data[x + 1][y][z]
                        + _data[x][y][z] + A[x][y][z];
                }
            }
        }
    }
    // sum: A[x1, x2)[y1, y2)[z1, z2)
    S sum(int x1, int y1, int z1, int x2, int y2, int z2) {
        return _data[x2][y2][z2]
            - _data[x1][y2][z2] - _data[x2][y1][z2] - _data[x2][y2][z1]
            + _data[x1][y1][z2] + _data[x1][y2][z1] + _data[x2][y1][z1]
            - _data[x1][y1][z1];
    }
    S original_at(int x, int y, int z) {
        return sum(x, y, z, x + 1, y + 1, z + 1);
    }
    S prefix_sum_at(int x, int y, int z) {
        return _data[x][y][z];
    }
private:
    int _nx;
    int _ny;
    int _nz;
    vector<vector<vector<S>>> _data;
};


/// END: 3D Prefix Sum ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////