#pragma once

#include <vector>

template<class S, S(*op)(S, S), S(*e)()>
class segtree {
public:
    segtree(size_t size) {
        // size�ȏ��2^k�ƂȂ�ŏ���k�����߁C2^(k+1)�Ŕz��T�C�Y�����肷��D�i�z��̍Ō��1�v�f�͎g�p���Ȃ��j
        uint32_t k = (size == 1) ? 1u : (uint32_t)log2((double)(size - 1u)) + 1;
        _n = (1 << (k + 1));
        _data.resize(_n, e());
    }

    void set(S x, int pos) {
        // seg-tree�ɂ����āC���̔z��[pos]��segtree[_n/2 - 1 + pos]�ɂ���D
        // �������Ƃ̏ꏊ���珈�����Ă����D
        int looking = _n / 2 - 1 + pos;
        _data[looking] = x;
        // ���̕��ɍX�V���Ă����D
        // looking����ł���ΉE���ƁC�����ł���΍����Ɖ��Z����D
        // �e��(looking-1)/2�ł���D
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
        // Node�́C���݂�Depth�ł̍����牽�߂�Node�ł��邩
        int CurDepthNodeID = NodeID - ((1 << depth) - 1);
        int CurDepthNodeWidth = _n / 2 / (1 << depth);
        // [RangeL, RangeR): ���ݒ��ڂ��Ă���Node���ԗ����Ă���Range
        int CurNodeRangeL = CurDepthNodeWidth * CurDepthNodeID;
        int CurNodeRangeR = CurDepthNodeWidth * (CurDepthNodeID + 1);

        // [RangeL, RangeR) AND [l, r) ����W���̏ꍇ�i��Ԃ���ؔ��Ȃ��ꍇ�j
        // �������Ȃ�
        if (CurNodeRangeR <= l || r <= CurNodeRangeL) {
            return;
        }
        // [RangeL, RangeR) AND [l, r) ��[RangeL, RangeR)�̏ꍇ
        // �����璍�ڂ��Ă���̂ō���val��u��
        else if (l <= CurNodeRangeL && CurNodeRangeR <= r) {
            val = op(val, _data[NodeID]);
            return;
        }
        // 2��Ԃ����݂��Ɉꕔ��������Ă���ꍇ
        // ������q�m�[�h�����Ă���
        else {
            _prod_recursive(l, r, NodeID * 2 + 1, depth + 1, val);
            _prod_recursive(l, r, NodeID * 2 + 2, depth + 1, val);
        }
    }
};