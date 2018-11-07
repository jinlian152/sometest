//
//  main.cpp
//  rbtree
//
//  Created by didi on 2018/11/1.
//  Copyright © 2018年 didi. All rights reserved.
//

#include "rb_tree_array.h"
#include <iostream>
#include <map>
#include <cstdlib>
#include <stdio.h>
#include <sys/time.h>


int64_t getCurrentTime()
{
    struct timeval tv;
    ::gettimeofday(&tv,NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

int main() {
    srand(getCurrentTime());
    {
        RBTree rb_tree;
        rb_tree.init(1000);
        int64_t rbtree_begin = getCurrentTime();
        for (int i = 1; i != 10000; ++i) {
            rb_tree.insert(i, i);
        }
        printf("rbtree insert cost=%ld\n", getCurrentTime() - rbtree_begin);
        
        rb_tree.mPrint(); //中序遍历校验
        
        int64_t rb_find_begin = getCurrentTime();
        for (int i = 0; i != 10000000; ++i) {
            rb_tree.search((i % 9999) + 1);
        }
        printf("rb_tree find cost=%ld\n", getCurrentTime() - rb_find_begin);
    }
    
    
    {
        int64_t map_begin = getCurrentTime();
        std::map<Key, Value> stl_tree;
        for (int i = 1; i != 10000; ++i) {
            stl_tree.insert(std::make_pair(i, i));
        }
        printf("map insert cost=%ld\n", getCurrentTime() - map_begin);
        
        int64_t map_find_begin = getCurrentTime();
        for (int i = 0; i != 10000000; ++i) {
            stl_tree.find((i % 9999) + 1);
        }
        printf("map find cost=%ld\n", getCurrentTime() - map_find_begin);
    }
    
    
    
//  ---------------------------------------------
//    RBTree rb_tree;
//    rb_tree.init(10000000);
//    for (int j = 0; j != 10; ++j) {
//        rb_tree.clear();
//        for (int i = 1; i != 10000000; ++i) {
//            rb_tree.insert(i, i);
//        }

//        RBTreeNode* ret = rb_tree.search(9999999);
//        if (NULL == ret) {
//            printf("not found.");
//            return -1;
//        }
//        std::cout << "key=" << ret->key_ << "value=" << ret->value_ <<std::endl;
//    }
    return 0;
}
