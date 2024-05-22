#pragma once

#include <vector>

template<class S, S(*op)(S, S), S(*e)()>
class segtree {
public:
    segtree(size_t size) {
        // size以上の2^kとなる最小のkを求め，2^(k+1)で配列サイズを決定する．（配列の最後の1要素は使用しない）
        uint32_t k = (size == 1) ? 1u : (uint32_t)log2((double)(size - 1u)) + 1;
        _n = (1 << (k + 1));
        _data.resize(_n, e());
    }

    void set(S x, int pos) {
        // seg-treeにおいて，元の配列[pos]はsegtree[_n/2 - 1 + pos]にある．
        // おおもとの場所から処理していく．
        int looking = _n / 2 - 1 + pos;
        _data[looking] = x;
        // 根の方に更新していく．
        // lookingが奇数であれば右側と，偶数であれば左側と演算する．
        // 親は(looking-1)/2である．
        while (looking != 0) {
            if (looking % 2 == 0) {
                x = op(_data[looking - 1], x);
            }
            else {
                x = op(x, _data[looking + 1]);
            }
            looking = (looking - 1) / 2;
            if (_data[looking] == x) {
                break;
            }
            _data[looking] = x;
        }
    }
    // query([l, r))
    S prod(int l, int r) {
        S val = e();
        _prod_recursive(l, r, 0, 0, val);

        return val;
    }
    // query[l, r)
    S prod_NotRecursive(int l, int r) {
        l += _n / 2 - 1;
        r += _n / 2 - 1;
        S val_l = e();
        S val_r = e();

        while (l < r) {
            if ((l & 1) == 0) {
                val_l = op(val_l, _data[l]);
                l++;
            }
            if ((r & 1) == 0) {
                r--;
                val_r = op(_data[r], val_r);
            }
            l = (l - 1) >> 1;
            r = (r - 1) >> 1;
        }

        return op(val_l, val_r);
    }

private:
    size_t _n;
    vector<S> _data;

    void _prod_recursive(int l, int r, int NodeID, int depth, S& val) {
        // Nodeの，現在のDepthでの左から何個めのNodeであるか
        int CurDepthNodeID = NodeID - ((1 << depth) - 1);
        int CurDepthNodeWidth = _n / 2 / (1 << depth);
        // [RangeL, RangeR): 現在注目しているNodeが網羅しているRange
        int CurNodeRangeL = CurDepthNodeWidth * CurDepthNodeID;
        int CurNodeRangeR = CurDepthNodeWidth * (CurDepthNodeID + 1);

        // [RangeL, RangeR) AND [l, r) が空集合の場合（区間が一切被らない場合）
        // 何もしない
        if (CurNodeRangeR <= l || r <= CurNodeRangeL) {
            return;
        }
        // [RangeL, RangeR) AND [l, r) が[RangeL, RangeR)の場合
        // 左から注目しているので左にvalを置く
        else if (l <= CurNodeRangeL && CurNodeRangeR <= r) {
            val = op(val, _data[NodeID]);
            return;
        }
        // 2区間がお互いに一部だけ被っている場合
        // 左から子ノードを見ていく
        else {
            _prod_recursive(l, r, NodeID * 2 + 1, depth + 1, val);
            _prod_recursive(l, r, NodeID * 2 + 2, depth + 1, val);
        }
    }
};