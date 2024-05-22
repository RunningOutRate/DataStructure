#pragma once

#include "BinarySearchTree.h"

/// AVT-Tree /////////////////////////////////////////////////////////////////////////////////
template<typename T>
class AVLTree : public BinarySearchTree<T> {
public:
    void insert(T x) override {
        Node<T>* Node_beginHeightUpdate = BinarySearchTree<T>::_insert(x);
        Node_beginHeightUpdate->update_height();
        _update_and_adjust_height_insert(Node_beginHeightUpdate->parent);
        //cout << "";
    }
    void erase(T x) override {
        Node<T>* Node_beginHeightUpdate = BinarySearchTree<T>::_erase(x);
        _update_and_adjust_height_erase(Node_beginHeightUpdate);
    }

    void height_verifier() {
        if (BinarySearchTree<T>::_n == 0) {
            return;
        }
        _height_verifier(BinarySearchTree<T>::root);
    }
    void bias_verifier() {
        if (BinarySearchTree<T>::_n == 0) {
            return;
        }
        _bias_verifier(BinarySearchTree<T>::root);
    }
    size_t height() {
        return BinarySearchTree<T>::root->height;
    }

protected:
    // looking以上の木の高さを更新する．
    // 更新しながらbiasを修正する
    void _update_and_adjust_height_insert(Node<T>* looking) {
        if (looking == nullptr) {
            return;
        }
        // 注目ノードのheightを更新する．（この更新により現時点での正しいheightが得られる）
        looking->update_height();

        int bias = looking->bias();

        // bias = 0
        // biasの調整不要
        // 部分木の高さは不変であり，上位のノードでの調整不要．done
        if (bias == 0) {
            return;
        }
        // bias = pm1
        // biasの調整不要
        // 部分木の高さは+1．上位のノードを確認．continue
        else if (bias == 1 || bias == -1) {
            _update_and_adjust_height_insert(looking->parent);
        }
        // bias = 2
        // biasを調整する
        // vは必ず存在する
        else if (bias == 2) {
            Node<T>* v = looking->L;
            int bias_v = v->bias();

            // 木の高さは不変．done
            if (bias_v == 1) {
                _rotate_R(looking);
                return;
            }
            // 木の高さは不変．done
            else if (bias_v == -1) {
                _rotate_L(v);
                _rotate_R(looking);
                return;
            }
            else {
                printf("debug: bias_v != 1, -1\n");
            }
        }
        // bias = -2
        // biasを調整する
        // vは必ず存在する
        else {
            Node<T>* v = looking->R;
            int bias_v = v->bias();

            // 木の高さは不変．done
            if (bias_v == -1) {
                _rotate_L(looking);
                return;
            }
            // 木の高さは不変．done
            else if (bias_v == 1) {
                _rotate_R(v);
                _rotate_L(looking);
                return;
            }
            else {
                printf("debug: bias_v != 1, -1\n");
            }
        }
    }

    // looking以上の木の高さを更新する．
    // 更新しながらbiasを修正する
    void _update_and_adjust_height_erase(Node<T>* looking) {
        if (looking == nullptr) {
            return;
        }
        // 注目ノードのheightを更新する．（この更新により現時点での正しいheightが得られる）
        looking->update_height();

        int bias = looking->bias();

        // bias = 0
        // biasの調整不要
        // 部分木の高さは-1．上位のノードを確認．continue
        if (bias == 0) {
            _update_and_adjust_height_erase(looking->parent);
        }
        // bias = pm1
        // biasの調整不要
        // 部分木の高さは変化しない．done
        else if (bias == 1 || bias == -1) {
            return;
        }
        // bias = 2
        // biasを調整する
        // vは必ず存在する
        else if (bias == 2) {
            Node<T>* v = looking->L;
            int bias_v = v->bias();

            // 部分木の高さは-1．上位のノードを確認．continue
            if (bias_v == 1) {
                _rotate_R(looking);
                _update_and_adjust_height_erase(looking->parent);
            }
            // 部分木の高さは-1．上位のノードを確認．continue
            else if (bias_v == -1) {
                _rotate_L(v);
                _rotate_R(looking);
                _update_and_adjust_height_erase(looking->parent);
            }
            else if (bias_v == 0) {
                _rotate_R(looking);
                return;
            }
            else {
                printf("debug: bias_v not 0, -1, 1\n");
            }
        }
        // bias = -2
        // biasを調整する
        // vは必ず存在する
        else {
            Node<T>* v = looking->R;
            int bias_v = v->bias();

            // 木の高さは不変．done
            if (bias_v == -1) {
                _rotate_L(looking);
                _update_and_adjust_height_erase(looking->parent);
            }
            // 木の高さは不変．done
            else if (bias_v == 1) {
                _rotate_R(v);
                _rotate_L(looking);
                _update_and_adjust_height_erase(looking->parent);
            }
            else if (bias_v == 0) {
                _rotate_L(looking);
                return;
            }
            else {
                printf("debug: bias_v not 0, -1, 1\n");
            }
        }
    }

    void _rotate_R(Node<T>* u) {
        // 回転が成立しない場合
        // uの左の子が存在する
        assert(!u->NoChildL());

        // uの左の子をvとする
        Node<T>* v = u->L;

        // uがrootである場合，vをrootにする．
        if (BinarySearchTree<T>::isRoot(u)) {
            BinarySearchTree<T>::root = v;
            v->parent = nullptr;
        }
        // そうでない場合，uのparentの子をvにする．
        else {
            if (u->parent->isR(u)) {
                u->parent->R = v;
            }
            else {
                u->parent->L = v;
            }
            v->parent = u->parent;
        }

        // vにRがいる場合，vのRをuのLにつなぐ
        if (!v->NoChildR()) {
            u->L = v->R;
            v->R->parent = u;
        }
        else {
            u->L = nullptr;
        }

        // uをvのRとする．
        v->R = u;
        u->parent = v;

        // heightの調整
        u->update_height();
        v->update_height();
    }

    void _rotate_L(Node<T>* u) {
        // 回転が成立しない場合
        // uの右の子が存在する
        assert(!u->NoChildR());

        // uの右の子をvとする
        Node<T>* v = u->R;

        // uがrootである場合，vをrootにする．
        if (BinarySearchTree<T>::isRoot(u)) {
            BinarySearchTree<T>::root = v;
            v->parent = nullptr;
        }
        // そうでない場合，uのparentの子をvにする．
        else {
            if (u->parent->isR(u)) {
                u->parent->R = v;
            }
            else {
                u->parent->L = v;
            }
            v->parent = u->parent;
        }

        // vにLがいる場合，vのLをuのRにつなぐ
        if (!v->NoChildL()) {
            u->R = v->L;
            v->L->parent = u;
        }
        else {
            u->R = nullptr;
        }

        // uをvのLとする．
        v->L = u;
        u->parent = v;

        // heightの調整
        u->update_height();
        v->update_height();
    }

    void _height_verifier(Node<T>* subtree_root) {
        // height verifier
        if (subtree_root->NoChildR() && subtree_root->NoChildL()) {
            assert(subtree_root->height == 1);
        }
        else if (subtree_root->NoChildR()) {
            assert(subtree_root->height == subtree_root->L->height + 1);
        }
        else if (subtree_root->NoChildL()) {
            assert(subtree_root->height == subtree_root->R->height + 1);
        }
        else {
            assert(subtree_root->height == max(subtree_root->R->height, subtree_root->L->height) + 1);
        }
        if (!subtree_root->NoChildL()) {
            _height_verifier(subtree_root->L);
        }
        if (!subtree_root->NoChildR()) {
            _height_verifier(subtree_root->R);
        }
    }
    void _bias_verifier(Node<T>* subtree_root) {
        // bias verifier
        assert(abs(subtree_root->bias()) < 2);

        if (!subtree_root->NoChildL()) {
            _bias_verifier(subtree_root->L);
        }
        if (!subtree_root->NoChildR()) {
            _bias_verifier(subtree_root->R);
        }
    }
};
