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
    // looking�ȏ�̖؂̍������X�V����D
    // �X�V���Ȃ���bias���C������
    void _update_and_adjust_height_insert(Node<T>* looking) {
        if (looking == nullptr) {
            return;
        }
        // ���ڃm�[�h��height���X�V����D�i���̍X�V�ɂ�茻���_�ł̐�����height��������j
        looking->update_height();

        int bias = looking->bias();

        // bias = 0
        // bias�̒����s�v
        // �����؂̍����͕s�ςł���C��ʂ̃m�[�h�ł̒����s�v�Ddone
        if (bias == 0) {
            return;
        }
        // bias = pm1
        // bias�̒����s�v
        // �����؂̍�����+1�D��ʂ̃m�[�h���m�F�Dcontinue
        else if (bias == 1 || bias == -1) {
            _update_and_adjust_height_insert(looking->parent);
        }
        // bias = 2
        // bias�𒲐�����
        // v�͕K�����݂���
        else if (bias == 2) {
            Node<T>* v = looking->L;
            int bias_v = v->bias();

            // �؂̍����͕s�ρDdone
            if (bias_v == 1) {
                _rotate_R(looking);
                return;
            }
            // �؂̍����͕s�ρDdone
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
        // bias�𒲐�����
        // v�͕K�����݂���
        else {
            Node<T>* v = looking->R;
            int bias_v = v->bias();

            // �؂̍����͕s�ρDdone
            if (bias_v == -1) {
                _rotate_L(looking);
                return;
            }
            // �؂̍����͕s�ρDdone
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

    // looking�ȏ�̖؂̍������X�V����D
    // �X�V���Ȃ���bias���C������
    void _update_and_adjust_height_erase(Node<T>* looking) {
        if (looking == nullptr) {
            return;
        }
        // ���ڃm�[�h��height���X�V����D�i���̍X�V�ɂ�茻���_�ł̐�����height��������j
        looking->update_height();

        int bias = looking->bias();

        // bias = 0
        // bias�̒����s�v
        // �����؂̍�����-1�D��ʂ̃m�[�h���m�F�Dcontinue
        if (bias == 0) {
            _update_and_adjust_height_erase(looking->parent);
        }
        // bias = pm1
        // bias�̒����s�v
        // �����؂̍����͕ω����Ȃ��Ddone
        else if (bias == 1 || bias == -1) {
            return;
        }
        // bias = 2
        // bias�𒲐�����
        // v�͕K�����݂���
        else if (bias == 2) {
            Node<T>* v = looking->L;
            int bias_v = v->bias();

            // �����؂̍�����-1�D��ʂ̃m�[�h���m�F�Dcontinue
            if (bias_v == 1) {
                _rotate_R(looking);
                _update_and_adjust_height_erase(looking->parent);
            }
            // �����؂̍�����-1�D��ʂ̃m�[�h���m�F�Dcontinue
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
        // bias�𒲐�����
        // v�͕K�����݂���
        else {
            Node<T>* v = looking->R;
            int bias_v = v->bias();

            // �؂̍����͕s�ρDdone
            if (bias_v == -1) {
                _rotate_L(looking);
                _update_and_adjust_height_erase(looking->parent);
            }
            // �؂̍����͕s�ρDdone
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
        // ��]���������Ȃ��ꍇ
        // u�̍��̎q�����݂���
        assert(!u->NoChildL());

        // u�̍��̎q��v�Ƃ���
        Node<T>* v = u->L;

        // u��root�ł���ꍇ�Cv��root�ɂ���D
        if (BinarySearchTree<T>::isRoot(u)) {
            BinarySearchTree<T>::root = v;
            v->parent = nullptr;
        }
        // �����łȂ��ꍇ�Cu��parent�̎q��v�ɂ���D
        else {
            if (u->parent->isR(u)) {
                u->parent->R = v;
            }
            else {
                u->parent->L = v;
            }
            v->parent = u->parent;
        }

        // v��R������ꍇ�Cv��R��u��L�ɂȂ�
        if (!v->NoChildR()) {
            u->L = v->R;
            v->R->parent = u;
        }
        else {
            u->L = nullptr;
        }

        // u��v��R�Ƃ���D
        v->R = u;
        u->parent = v;

        // height�̒���
        u->update_height();
        v->update_height();
    }

    void _rotate_L(Node<T>* u) {
        // ��]���������Ȃ��ꍇ
        // u�̉E�̎q�����݂���
        assert(!u->NoChildR());

        // u�̉E�̎q��v�Ƃ���
        Node<T>* v = u->R;

        // u��root�ł���ꍇ�Cv��root�ɂ���D
        if (BinarySearchTree<T>::isRoot(u)) {
            BinarySearchTree<T>::root = v;
            v->parent = nullptr;
        }
        // �����łȂ��ꍇ�Cu��parent�̎q��v�ɂ���D
        else {
            if (u->parent->isR(u)) {
                u->parent->R = v;
            }
            else {
                u->parent->L = v;
            }
            v->parent = u->parent;
        }

        // v��L������ꍇ�Cv��L��u��R�ɂȂ�
        if (!v->NoChildL()) {
            u->R = v->L;
            v->L->parent = u;
        }
        else {
            u->R = nullptr;
        }

        // u��v��L�Ƃ���D
        v->L = u;
        u->parent = v;

        // height�̒���
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
