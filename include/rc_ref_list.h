#ifndef _RC_REF_LIST_H
#define _RC_REF_LIST_H

#ifndef RC_REF_LIST_INITIAL_CAPACITY
  #define RC_REF_LIST_INITIAL_CAPACITY 16
#endif // RC_REF_LIST_INITIAL_CAPACITY

typedef struct {
  void** items;
  int size;
  int capacity;
} rc_ref_list_t;

rc_ref_list_t* rc_ref_list_create();
int rc_ref_list_count(rc_ref_list_t* list);
int rc_ref_list_insert(rc_ref_list_t* list, void* ref);
int rc_ref_list_remove(rc_ref_list_t* list, void* target_ref);
int rc_ref_list_destroy(rc_ref_list_t* list);

int _rc_ref_list_copy(rc_ref_list_t* from, void** to);
int _rc_ref_list_grow(rc_ref_list_t* list);
int _rc_ref_list_shrink(rc_ref_list_t* list);
int _rc_ref_list_should_grow(rc_ref_list_t* list);
int _rc_ref_list_should_shrink(rc_ref_list_t* list);

#endif // _RC_REF_LIST_H