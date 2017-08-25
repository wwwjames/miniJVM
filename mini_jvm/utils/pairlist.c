//
// Created by gust on 2017/8/25.
//
#include "../jvm/garbage.h"

Pairlist *pairlist_create(s32 len) {
    if (len <= 0) {
        len = 8;
    }
    Pairlist *list = (Pairlist *) jvm_alloc(sizeof(Pairlist));//每个位置放两个指针
    if (list) {
        list->ptr = jvm_alloc(sizeof(__refer) * 2 * len);//每个位置放两个指针
        list->_alloced = len;
        list->count = 0;
        return list;
    }
    return NULL;
};

s32 pairlist_put(Pairlist *list, __refer left, __refer right) {
    if (list->count >= list->_alloced) {//空间不足
        void *p = jvm_realloc(list->ptr, list->_alloced * 2 * sizeof(__refer) * 2);
        list->ptr = p;
    }
    (list->ptr)[list->count] = left;
    (list->ptr)[list->count + 1] = right;
};

__refer pairlist_get(Pairlist *list, __refer left) {
    __refer right = NULL;
    s32 i;
    for (i = 0; i < list->count; i++) {
        if ((list->ptr)[list->count] == left) {
            return (list->ptr)[list->count + 1];
        }
    }
    return right;
};

void pairlist_destory(Pairlist *list) {
    if (list) {
        jvm_free(list->ptr);
        jvm_free(list);
    }
};