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
     * xを追加する．
     * 追加したNodeへのポインタを返す
     */
    Node<T>* _insert(T x) {
        // サイズを1増やす
        _n++;
        // 元が空だった場合はrootの値を変更
        if (_n == 1) {
            root = new Node<T>;
            root->key = x;
            root->height = 1;
            return root;
        }

        // 元のサイズが1以上の場合は二分探索木の定義に基づいてノードを移動
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
     * xを削除する．
     * 削除するNodeが存在しない場合は何もしない
     * 削除した結果，Nodeのheightの更新に使用する一番最初のNodeを返す
     */
    Node<T>* _erase(T x) {
        // 削除するノード
        Node<T>* erasing = _search(x);
        // 削除するノードが存在しない場合は何もしない
        if (erasing == nullptr) {
            return nullptr;
        }

        // 削除をしよう /////////////////////////////
        // 削除するため1減らす
        _n--;
        // heightの更新に使用する一番最初のNode
        Node<T>* Node_beginHeightUpdate = nullptr;
        // 削除ノードの親
        Node<T>* parent = erasing->parent;

        // 元のサイズ1の場合はrootを削除
        // これは削除対象がrootで，削除ノードが子を持たない場合にあたる
        if (_n == 0) {
            delete root;
            root = nullptr;
            return nullptr;
        }

        // 以下では削除ノードの子の数で処理を分岐する．

        // 削除ノードが子を持たない場合
        if (erasing->NoChild()) {
            Node_beginHeightUpdate = erasing->parent;

            // 削除ノードのkeyが親ノードのkey以上であれば親ノードのRを消す
            if (parent->key <= erasing->key) {
                parent->R = nullptr;
            }
            // そうでない場合はLを消す
            else {
                parent->L = nullptr;
            }
        }
        // 削除ノードが子を2つ持つ場合
        // 削除ノードのLをrootとする部分木における最大値のノードを得る．
        // 最大値ノードにはRの子がいない．最大値ノードのLがいるなら最大値ノードのところに持ってくる
        // 最大値ノードを削除ノードの場所に持ってくる
        else if (erasing->BothChild()) {
            // Lの部分木の最大値ノード
            Node<T>* MaxNode = _search_max(erasing->L);
            Node<T> initial_MaxNode = *MaxNode;

            // 削除ノードがrootでない場合
            if (root != erasing) {
                // 最大値ノードを削除ノードのところに持ってくる
                // 削除ノードのkeyが親ノードのkey以上であれば親ノードのRにつける
                if (parent->key <= erasing->key) {
                    parent->R = MaxNode;
                }
                // そうでない場合はLにつける
                else {
                    parent->L = MaxNode;
                }
                MaxNode->height = erasing->height;
            }
            // 削除ノードがrootの場合はrootをMaxNodeに設定
            else {
                MaxNode->height = root->height;
                root = MaxNode;
            }

            // Rノードと接続
            MaxNode->R = erasing->R;
            erasing->R->parent = MaxNode;

            // Lノードと接続
            if (erasing->L != MaxNode) {
                MaxNode->L = erasing->L;
                erasing->L->parent = MaxNode;
            }

            // 親ノードと接続
            MaxNode->parent = parent;

            // 最大値ノードが削除ノードのLではない場合
            // 最大値ノードの親は最大値ノードを必ずRの子として持つ
            if (MaxNode != erasing->L) {
                // 最大値ノードにLがいる場合はその子を処理する必要がある
                // ・最大値ノードの親のRを元の最大値ノードのLと接続（削除ノードにRは無いことが言える）
                if (!initial_MaxNode.NoChildL()) {
                    initial_MaxNode.parent->R = initial_MaxNode.L;
                    initial_MaxNode.L->parent = initial_MaxNode.parent;
                }
                // 最大値ノードのLが存在しないことと最大値ノードに子がいないことは同値である．
                // ・最大値ノードの親のRをnullにする．
                else {
                    initial_MaxNode.parent->R = nullptr;
                }

                Node_beginHeightUpdate = initial_MaxNode.parent;
            }
            else {
                Node_beginHeightUpdate = MaxNode;
            }
        }
        // 削除ノードが子を1つしか持たない場合
        // erasingの親とchildを接続する
        else {
            // 存在する子
            Node<T>* child = erasing->NoChildR() ? erasing->L : erasing->R;

            // 削除対象がrootであるとき
            if (root == erasing) {
                child->parent = nullptr;
                root = child;
            }
            else {
                // 親ノードを接続する
                // 削除ノードのkeyが親ノードのkey以上であれば親ノードのRを付け替える
                if (parent->key <= erasing->key) {
                    parent->R = child;
                }
                // そうでない場合はLを付け替える
                else {
                    parent->L = child;
                }
                // 子ノードを接続する．
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