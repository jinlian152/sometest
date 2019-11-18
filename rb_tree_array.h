// 2018-11-06 by jinlian
// 基于数组实现的红黑树，利用数组内存连续和cpu cache的特性，
// 在查找操作时速度比标准库map快30%以上，适合用于只有插入
// 和查找的场景，比如解决hash冲突。
#include <vector>
#include <iostream>

typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

using std::size_t;

#define     RED                     0
#define     BLACK                   1
#define     Key                     uint64_t
#define     Value                   uint32_t
#define     Default_capacity        1000000
#define     Nil_idx                 0               // as common leaf node.

struct RBTreeNode {
    int             color_;
    size_t          left_;
    size_t          right_;
    Key             key_;
    Value           value_;

public:
    RBTreeNode() : color_(RED), left_(0), right_(0) {}
    RBTreeNode(Key key, Value value) : color_(RED), left_(0), right_(0), key_(key), value_(value) {}
    void setRed() { color_ = RED; }
    void setBlack() { color_ = BLACK; }
    bool isRed() { return color_ == RED; }
    bool isBlack() { return color_ == BLACK; }

private:
    RBTreeNode& operator=(RBTreeNode&);
    RBTreeNode(RBTreeNode&);
};

class RBTree {
public:
    RBTree() {}

    ~RBTree() { destroy(); }

    void init(size_t capacity = Default_capacity);

    bool insert(const Key key, const Value value);

    RBTreeNode* search(const Key key);

    bool empty() {
        return (0 == size_);
    }
    
    void destroy();
    
    void clear();
    
    RBTreeNode& operator[] (size_t key);
    
    void mPrint() {
        mPrintImpl(root_idx_);
    }
    
private:
    
    bool isFull() {     //idx 0 used as leaf;
        return (size_ == (capacity_ - 1));
    }
    
    RBTreeNode* getRoot() {
        return (0 == root_idx_) ? NULL : &root_[root_idx_];
    }
    
    void mPrintImpl(size_t idx);

    bool insertFix(size_t idx, std::vector<size_t> &re_stack);

    bool connect(size_t p_idx, size_t c_idx);

    bool isLeftChild(size_t p_node, size_t c_node) {
        return (root_[p_node].left_ == c_node);
    }

    bool isRightChild(size_t p_node, size_t c_node) {
        return (root_[p_node].right_ == c_node);
    }

    size_t allocIndex() {
        return (curr_idx_ < (capacity_ - 1)) ? ++curr_idx_ : 0;
    }

    void releaseIndex() {
        --curr_idx_;
    }

private:
    size_t              capacity_;
    size_t              size_;
    size_t              nil_idx_;
    size_t              curr_idx_;
    size_t              root_idx_;
    RBTreeNode*         root_;
    std::vector<size_t> re_stack_;
};
