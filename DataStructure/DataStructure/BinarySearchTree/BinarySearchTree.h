#pragma once

#include "cassert"

using namespace std;

template<typename T>
struct Node {
    T key;
    int height = -1;
    Node* parent = nullptr;
    Node* L = nullptr;
    Node* R = nullptr;

    bool NoChildR() {
        return R == nullptr;
    }
    bool NoChildL() {
        return L == nullptr;
    }
    bool NoChild() {
        return NoChildL() && NoChildR();
    }
    bool OnlyOneChild() {
        return (NoChildR() && !NoChildL()) || (!NoChildR() && NoChildL());
    }
    bool BothChild() {
        return !NoChildL() && !NoChildR();
    }
    int bias() {
        int r = NoChildR() ? 0 : R->height;
        int l = NoChildL() ? 0 : L->height;
        return l - r;
    }
    Node<T>* another_child(Node<T>* one) {
        assert(BothChild() && (one == L || one == R));
        if (R == one) {
            return L;
        }
        else {
            return R;
        }
    }
    bool isR(Node<T>* ptr) {
        return (ptr != nullptr) && (R == ptr);
    }
    bool isL(Node<T>* ptr) {
        return (ptr != nullptr) && (L == ptr);
    }
    bool isChild(Node<T>* ptr) {
        return (ptr != nullptr) && ((L == ptr) || (R == ptr));
    }
    void update_height() {
        int r = NoChildR() ? 0 : R->height;
        int l = NoChildL() ? 0 : L->height;
        height = max(r, l) + 1;
    }
};

template<typename T>
class BinarySearchTree {
public:
    bool exist(T x) {
        return _search(x) != nullptr;
    }
    virtual void insert(T x) {
        _insert(x);
    }
    virtual void erase(T x) {
        _erase(x);
    }

    vector<T> elem() {
        vector<T> list(0);
        if (_n == 0) {
            return list;
        }
        _elem_from_left(root, list);
        return list;
    }

    size_t size() {
        return _n;
    }

    void debug_print_graph() {
        vector<vector<pair<int, int>>> data;
        _debug_print_graph(root, 0, data);

        for (int i = 0; i < data.size(); i++) {
            for (int j = 0; j < data[i].size(); j++) {
                cout << data[i][j].first << "(" << data[i][j].second << ") ";
            }
            cout << endl;
        }
    }
protected:
    size_t _n = 0;
    Node<T>* root = nullptr;

    /*
     * x��ǉ�����D
     * �ǉ�����Node�ւ̃|�C���^��Ԃ�
     */
    Node<T>* _insert(T x) {
        // �T�C�Y��1���₷
        _n++;
        // �����󂾂����ꍇ��root�̒l��ύX
        if (_n == 1) {
            root = new Node<T>;
            root->key = x;
            root->height = 1;
            return root;
        }

        // ���̃T�C�Y��1�ȏ�̏ꍇ�͓񕪒T���؂̒�`�Ɋ�Â��ăm�[�h���ړ�
        Node<T>* looking = root;
        Node<T>* NewNode = new Node<T>;
        NewNode->key = x;

        while (true) {
            if (x >= looking->key) {
                if (!looking->NoChildR()) {
                    looking = looking->R;
                }
                else {
                    looking->R = NewNode;
                    NewNode->parent = looking;
                    break;
                }
            }
            else {
                if (!looking->NoChildL()) {
                    looking = looking->L;
                }
                else {
                    looking->L = NewNode;
                    NewNode->parent = looking;
                    break;
                }
            }
        }

        return NewNode;
    }

    /*
     * x���폜����D
     * �폜����Node�����݂��Ȃ��ꍇ�͉������Ȃ�
     * �폜�������ʁCNode��height�̍X�V�Ɏg�p�����ԍŏ���Node��Ԃ�
     */
    Node<T>* _erase(T x) {
        // �폜����m�[�h
        Node<T>* erasing = _search(x);
        // �폜����m�[�h�����݂��Ȃ��ꍇ�͉������Ȃ�
        if (erasing == nullptr) {
            return nullptr;
        }

        // �폜�����悤 /////////////////////////////
        // �폜���邽��1���炷
        _n--;
        // height�̍X�V�Ɏg�p�����ԍŏ���Node
        Node<T>* Node_beginHeightUpdate = nullptr;
        // �폜�m�[�h�̐e
        Node<T>* parent = erasing->parent;

        // ���̃T�C�Y1�̏ꍇ��root���폜
        // ����͍폜�Ώۂ�root�ŁC�폜�m�[�h���q�������Ȃ��ꍇ�ɂ�����
        if (_n == 0) {
            delete root;
            root = nullptr;
            return nullptr;
        }

        // �ȉ��ł͍폜�m�[�h�̎q�̐��ŏ����𕪊򂷂�D

        // �폜�m�[�h���q�������Ȃ��ꍇ
        if (erasing->NoChild()) {
            Node_beginHeightUpdate = erasing->parent;

            // �폜�m�[�h��key���e�m�[�h��key�ȏ�ł���ΐe�m�[�h��R������
            if (parent->key <= erasing->key) {
                parent->R = nullptr;
            }
            // �����łȂ��ꍇ��L������
            else {
                parent->L = nullptr;
            }
        }
        // �폜�m�[�h���q��2���ꍇ
        // �폜�m�[�h��L��root�Ƃ��镔���؂ɂ�����ő�l�̃m�[�h�𓾂�D
        // �ő�l�m�[�h�ɂ�R�̎q�����Ȃ��D�ő�l�m�[�h��L������Ȃ�ő�l�m�[�h�̂Ƃ���Ɏ����Ă���
        // �ő�l�m�[�h���폜�m�[�h�̏ꏊ�Ɏ����Ă���
        else if (erasing->BothChild()) {
            // L�̕����؂̍ő�l�m�[�h
            Node<T>* MaxNode = _search_max(erasing->L);
            Node<T> initial_MaxNode = *MaxNode;

            // �폜�m�[�h��root�łȂ��ꍇ
            if (root != erasing) {
                // �ő�l�m�[�h���폜�m�[�h�̂Ƃ���Ɏ����Ă���
                // �폜�m�[�h��key���e�m�[�h��key�ȏ�ł���ΐe�m�[�h��R�ɂ���
                if (parent->key <= erasing->key) {
                    parent->R = MaxNode;
                }
                // �����łȂ��ꍇ��L�ɂ���
                else {
                    parent->L = MaxNode;
                }
                MaxNode->height = erasing->height;
            }
            // �폜�m�[�h��root�̏ꍇ��root��MaxNode�ɐݒ�
            else {
                MaxNode->height = root->height;
                root = MaxNode;
            }

            // R�m�[�h�Ɛڑ�
            MaxNode->R = erasing->R;
            erasing->R->parent = MaxNode;

            // L�m�[�h�Ɛڑ�
            if (erasing->L != MaxNode) {
                MaxNode->L = erasing->L;
                erasing->L->parent = MaxNode;
            }

            // �e�m�[�h�Ɛڑ�
            MaxNode->parent = parent;

            // �ő�l�m�[�h���폜�m�[�h��L�ł͂Ȃ��ꍇ
            // �ő�l�m�[�h�̐e�͍ő�l�m�[�h��K��R�̎q�Ƃ��Ď���
            if (MaxNode != erasing->L) {
                // �ő�l�m�[�h��L������ꍇ�͂��̎q����������K�v������
                // �E�ő�l�m�[�h�̐e��R�����̍ő�l�m�[�h��L�Ɛڑ��i�폜�m�[�h��R�͖������Ƃ�������j
                if (!initial_MaxNode.NoChildL()) {
                    initial_MaxNode.parent->R = initial_MaxNode.L;
                    initial_MaxNode.L->parent = initial_MaxNode.parent;
                }
                // �ő�l�m�[�h��L�����݂��Ȃ����Ƃƍő�l�m�[�h�Ɏq�����Ȃ����Ƃ͓��l�ł���D
                // �E�ő�l�m�[�h�̐e��R��null�ɂ���D
                else {
                    initial_MaxNode.parent->R = nullptr;
                }

                Node_beginHeightUpdate = initial_MaxNode.parent;
            }
            else {
                Node_beginHeightUpdate = MaxNode;
            }
        }
        // �폜�m�[�h���q��1���������Ȃ��ꍇ
        // erasing�̐e��child��ڑ�����
        else {
            // ���݂���q
            Node<T>* child = erasing->NoChildR() ? erasing->L : erasing->R;

            // �폜�Ώۂ�root�ł���Ƃ�
            if (root == erasing) {
                child->parent = nullptr;
                root = child;
            }
            else {
                // �e�m�[�h��ڑ�����
                // �폜�m�[�h��key���e�m�[�h��key�ȏ�ł���ΐe�m�[�h��R��t���ւ���
                if (parent->key <= erasing->key) {
                    parent->R = child;
                }
                // �����łȂ��ꍇ��L��t���ւ���
                else {
                    parent->L = child;
                }
                // �q�m�[�h��ڑ�����D
                child->parent = parent;
            }
            Node_beginHeightUpdate = erasing->parent;
        }
        delete erasing;

        return Node_beginHeightUpdate;
    }

    void _elem_from_left(Node<T>* subtree_root, vector<T>& elem) {
        if (!subtree_root->NoChildL()) {
            _elem_from_left(subtree_root->L, elem);
        }
        elem.push_back(subtree_root->key);
        if (!subtree_root->NoChildR()) {
            _elem_from_left(subtree_root->R, elem);
        }
    }

    void _debug_print_graph(Node<T>* subtree_root, int depth, vector<vector<pair<int, int>>>& out) {
        if (depth >= out.size()) {
            out.resize(depth + 1);
        }

        if (!subtree_root->NoChildL()) {
            _debug_print_graph(subtree_root->L, depth + 1, out);
        }

        if (subtree_root != root) {
            out[depth].push_back({ subtree_root->key, subtree_root->height });
        }
        else {
            out[depth].push_back({ subtree_root->key, subtree_root->height });
        }

        if (!subtree_root->NoChildR()) {
            _debug_print_graph(subtree_root->R, depth + 1, out);
        }
    }

    Node<T>* _search(T& x) {
        if (_n == 0) {
            return nullptr;
        }
        Node<T>* looking = root;

        while (true) {
            if (looking->key == x) {
                return looking;
            }
            if (x >= looking->key) {
                if (!looking->NoChildR()) {
                    looking = looking->R;
                }
                else {
                    return nullptr;
                }
            }
            else {
                if (!looking->NoChildL()) {
                    looking = looking->L;
                }
                else {
                    return nullptr;
                }
            }
        }
    }

    Node<T>* _search_max(Node<T>* subtree_root) {
        if (_n == 0) {
            return nullptr;
        }
        Node<T>* looking = subtree_root;
        while (!looking->NoChildR()) {
            looking = looking->R;
        }
        return looking;
    }
    Node<T>* _search_min(Node<T>* subtree_root) {
        if (_n == 0) {
            return nullptr;
        }
        Node<T>* looking = subtree_root;
        while (!looking->NoChildL()) {
            looking = looking->L;
        }
        return looking;
    }

    bool isRoot(Node<T>* Node) {
        return Node == root;
    }
};