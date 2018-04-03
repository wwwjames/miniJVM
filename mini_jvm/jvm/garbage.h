

#ifndef _GARBAGE_H
#define _GARBAGE_H

#include "../utils/hashtable.h"
#include "../utils/hashset.h"
#include "../utils/linkedlist.h"
#include "jvm.h"
#include "jvm_util.h"


#ifdef __cplusplus
extern "C" {
#endif

//回收线程
extern s64 GARBAGE_PERIOD_MS;//
extern GcCollector *collector;

extern s64 MAX_HEAP_SIZE;


//每个线程一个回收站，线程多了就是灾难
struct _GcCollectorType {
    //
    Hashset *objs_holder; //法外之地，防回收的持有器，放入其中的对象及其引用的其他对象不会被回收
    MemoryBlock *header, *tmp_header;
    s64 obj_count;
    //
    MemoryBlock *free_list_header;
    spinlock_t lock_reuse;
    s32 volatile flag_reuse;//control reuse mode=1 if heap using less than  MAX_HEAP_SIZE

    //
    thrd_t _garbage_thread;//垃圾回收线程
    ThreadLock garbagelock;

    spinlock_t lock;
    //
    ArrayList *runtime_refer_copy;
    //
    s64 _garbage_count;
    u8 _garbage_thread_status;
    u8 flag_refer;
    u8 isgc;
};

enum {
    GARBAGE_THREAD_NORMAL,
    GARBAGE_THREAD_PAUSE,
    GARBAGE_THREAD_STOP,
    GARBAGE_THREAD_DEAD,
};

s32 _collect_thread_run(void *para);

void garbage_thread_lock(void);

void garbage_thread_unlock(void);

void garbage_thread_stop(void);

void garbage_thread_pause(void);

void garbage_thread_resume(void);

void garbage_thread_wait(void);

void garbage_thread_timedwait(s64 ms);

void garbage_thread_notify(void);

void garbage_thread_notifyall(void);

//其他函数

s32 garbage_collector_create(void);

void garbage_collector_destory(void);

s64 garbage_collect(void);

MemoryBlock *garbage_is_alive(__refer obj);

void garbage_refer_hold(__refer ref);

void garbage_refer_release(__refer ref);

s32 garbage_refer_reg(__refer ref);

void garbage_dump_runtime();

Instance *get_instance_mb();

void put_instance_mb(MemoryBlock *mb);

#ifdef __cplusplus
}
#endif

#endif //_GARBAGE_H
