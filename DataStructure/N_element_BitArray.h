#pragma once

#include <iostream>
#include <vector>
using namespace std;


class N_element_BitArray {
public:
    N_element_BitArray(int len, int dim, uint64_t init = 0) {
        _n = len;
        _dim = dim;
        _bitArray.resize(_n);
        if (init != 0) {
            _calc_bitarray_from_num(init);
        }
    }
    // 0 <= val < dim
    void set(int idx, int val) {
        _bitArray[idx] = val;
    }
    int count(int state) {
        int ret = 0;
        for (int i = 0; i < _n; i++) {
            ret += (_bitArray[i] == state) ? 1 : 0;
        }
        return ret;
    }
    int operator[](int idx) {
        return _bitArray[idx];
    }
    uint64_t to_ullong() {
        uint64_t ret = 0;
        uint64_t coeff = 1;
        for (int i = 0; i < _n; i++) {
            ret += coeff * _bitArray[i];
            coeff *= _dim;
        }
        return ret;
    }
    void print() {
        for (int i = _n - 1; i >= 0; i--) {
            printf("%d ", _bitArray[i]);
        }
        printf("\n");
    }
    void swap(int idxA, int idxB) {
        std::swap(_bitArray[idxA], _bitArray[idxB]);
    }
    void swap_n(int idxA, int idxB, int n) {
        for (int i = 0; i < n; i++) {
            swap(idxA + i, idxB + i);
        }
    }
private:
    // bitArray[i]にはi bit目の状態が入っている．
    // 出力の際には
    int _n;
    int _dim;
    vector<int> _bitArray; // 32bitは不要であるが，このままでいく

    void _calc_bitarray_from_num(uint64_t num) {
        for (int b = 0; b < _n; b++) {
            _bitArray[b] = (int)(num % _dim);
            num /= (uint64_t)_dim;
        }
    }
};
