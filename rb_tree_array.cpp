#include "rb_tree_array.h"

void RBTree::init(size_t capacity) {
    root_ = new RBTreeNode[capacity];
    root_[0].setBlack();
    capacity_ = capacity;
    root_idx_ = 0;
    curr_idx_ = 0;
    size_ = 0;
    re_stack_.reserve(64);
}

bool RBTree::connect(size_t p_idx, size_t c_idx) {
    if (p_idx > size_ || p_idx == c_idx) {
        return false;
    }

    if (root_[p_idx].key_ > root_[c_idx].key_) {
        root_[p_idx].left_ = c_idx;
    }
    else if (root_[p_idx].key_ < root_[c_idx].key_) {
        root_[p_idx].right_ = c_idx;
    }
    else {
        return false;
    }
    return true;
}

bool RBTree::insert(const Key key, const Value value) {
    if (isFull()) {
        return false;
    }

    if (0 == size_ && 0 == root_idx_) { //empty tree, add node as root.
        size_t idx = allocIndex();
        if (0 == idx) {
            return false;
        }
    
        root_[idx].key_ = key;
        root_[idx].value_ = value;
        root_[idx].setBlack();
        root_idx_ = idx;
        size_++;
        return true;
    }

    re_stack_.clear();
    size_t x = 0;
    size_t p = root_idx_;
    while (Nil_idx != p) {        //find correct parent.
        x = p;
        re_stack_.push_back(p);
        if (root_[p].key_ < key) {
            p = root_[p].right_;
        }
        else if (root_[p].key_ > key) {
            p = root_[p].left_;
        }
        else {
            std::cout << "The element is already in it." << std::endl;
            return false;
        }
    }

    size_t n_idx = allocIndex();
    if (0 == n_idx) {
        return false;
    }

    root_[n_idx].key_ = key;
    root_[n_idx].value_ = value;
    if (!connect(x, n_idx) || !insertFix(n_idx, re_stack_)) {//add into tree.
        releaseIndex();
        return false;
    }

    size_++;
    return true;
}

//插入时一共有6种情况，根据父节点是祖父节点的左孩子还是右孩子对称：
//  1、父节点是左孩子，叔节点是红色；
//  2、父节点是左孩子，叔节点是黑色，要插入节点是父节点的右孩子；
//  3、父节点是左孩子，叔节点是黑色，要插入节点是父节点的左孩子；
//父节点是右孩子的情况对称

bool RBTree::insertFix(size_t idx, std::vector<size_t> &re_stack) {
    size_t height = re_stack.size();
    size_t top = height - 1;

    if (0 == top) { //the parent is root;
        return true;
    }

    size_t idx_bk = idx;
    if (top >= 1) {
        size_t parent = re_stack[top];
        while (root_[parent].isRed()) {
            parent = re_stack[top];
            size_t grand_parent = re_stack[top - 1];
            if (parent == root_[grand_parent].left_) {
                size_t uncle = root_[grand_parent].right_;
                if (Nil_idx != uncle && root_[uncle].isRed()) {
                    root_[uncle].setBlack();
                    root_[parent].setBlack();
                    root_[grand_parent].setRed();
                    if (top < 2) {  // grand_parent is root, break and set root black.
                        break;
                    }
                    idx_bk = grand_parent;
                    top -= 2;
                    parent = re_stack[top];
                }
                else {
                    if (idx_bk == root_[parent].right_) {
                        //assert(top < 1);
                        root_[grand_parent].left_ = idx_bk;
                        root_[parent].right_ = root_[idx_bk].left_;
                        root_[idx_bk].left_ = parent;
                        //size_t temp_idx = idx_bk;
                        idx_bk = parent;
                        re_stack[top] = idx_bk;
                        parent = re_stack[top];
                    }

                    root_[grand_parent].setRed();
                    root_[parent].setBlack();
                    root_[grand_parent].left_ = root_[parent].right_;
                    root_[parent].right_ = grand_parent;

                    if (1 == top) {
                        root_idx_ = parent;
                    }
                    else {
                        size_t gg_parent = re_stack[top - 2];
                        root_[gg_parent].left_ = parent;
                    }
                    break;
                }
            }
            else {
                size_t uncle = root_[grand_parent].left_;
                if (Nil_idx != uncle && root_[uncle].isRed()) {
                    root_[uncle].setBlack();
                    root_[parent].setBlack();
                    root_[grand_parent].setRed();
                    if (top < 2) {  // grand_parent is root, break and set root black.
                        break;
                    }
                    idx_bk = grand_parent;
                    top -= 2;
                    parent = re_stack[top];
                }
                else {
                    if (idx_bk == root_[parent].left_) {
                        //assert(top < 1);
                        root_[grand_parent].right_ = idx_bk;
                        root_[parent].left_ = root_[idx_bk].right_;
                        root_[idx_bk].right_ = parent;
                        //size_t temp_idx = idx_bk;
                        idx_bk = parent;
                        re_stack[top] = idx_bk;
                        parent = re_stack[top];
                    }

                    root_[grand_parent].setRed();
                    root_[parent].setBlack();
                    root_[grand_parent].right_ = root_[parent].left_;
                    root_[parent].left_ = grand_parent;

                    if (1 == top) {
                        root_idx_ = parent;
                    }
                    else {
                        size_t gg_parent = re_stack[top - 2];
                        root_[gg_parent].right_ = parent;
                    }
                }
            }
        }
    }
    root_[root_idx_].setBlack();
    return true;
}

void RBTree::mPrintImpl(size_t idx) {
    if (Nil_idx == idx) {
        return;
    }
    mPrintImpl(root_[idx].left_);
    printf("%d\n", root_[idx].value_);
    mPrintImpl(root_[idx].right_);
}

void RBTree::destroy() {
    delete []root_;
    root_ = NULL;
    capacity_ = 0;
    root_idx_ = 0;
    curr_idx_ = 0;
    size_ = 0;
}

void RBTree::clear() {
    memset(root_, 0, capacity_ * sizeof(RBTreeNode));
    root_idx_ = 0;
    curr_idx_ = 0;
    size_ = 0;
}

RBTreeNode* RBTree::search(const Key key) {
    size_t p = root_idx_;
    //int i = 0;
    while (curr_idx_ >= p && 0 != p) {
        //printf("i = %d, parent = %zu\n", i++, p);
        if (root_[p].key_ < key) {
            p = root_[p].right_;
        } else if (root_[p].key_ > key) {
            p = root_[p].left_;
        } else {
            return &root_[p];
        }
    }
    return NULL;
}
