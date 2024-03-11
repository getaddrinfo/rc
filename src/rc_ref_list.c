#include "rc_ref_list.h"
#include <stdlib.h>

rc_ref_list_t* rc_ref_list_create() {
  rc_ref_list_t* ref_list = (rc_ref_list_t*)malloc(sizeof(rc_ref_list_t));

  if (ref_list == NULL) {
    return NULL;
  }

  void** underlying_array = (void**)calloc(RC_REF_LIST_INITIAL_CAPACITY, sizeof(void*));

  if (underlying_array == NULL) {
    free(ref_list);
    return NULL;
  }

  ref_list->items = underlying_array;
  ref_list->size = 0;
  ref_list->capacity = RC_REF_LIST_INITIAL_CAPACITY;

  return ref_list;
} 

int rc_ref_list_count(rc_ref_list_t* list) {
  return list->size;
}

int rc_ref_list_insert(rc_ref_list_t* list, void* ref) { 
  int i;

  if (_rc_ref_list_should_grow(list)) {
    _rc_ref_list_grow(list);
  }

  // insert into next available space
  for(i = 0; i < list->capacity; i++) {
    if (list->items[i] != NULL) continue;

    list->items[i] = ref;
    list->size++;

    break;
  }

  return 0;
}

int rc_ref_list_remove(rc_ref_list_t* list, void* target_ref) {
  void* curr;
  int i;
  int result = 1;

  for(i = 0; i < list->size; i++) {
    if (list->items[i] != target_ref) {
      continue;
    }

    list->items[i] = NULL;
    list->size--;
    result = 0;

    break;
  }

  if (_rc_ref_list_should_shrink(list)) {
    _rc_ref_list_shrink(list);
  }

  return result;
}

int rc_ref_list_destroy(rc_ref_list_t* list) {
  free(list->items);
  free(list);
}


int _rc_ref_list_copy(rc_ref_list_t* from, void** to) {
  int i, j;

  for(i = 0; i < from->capacity; i++) {
    if (from->items[i] == NULL) continue;

    to[j] = from->items[i];
    j++;

    if (j == from->size) break;
  }

  return 0;
}

int _rc_ref_list_grow(rc_ref_list_t* list) {
  int new_capacity = list->capacity * 2;
  void** next = calloc(new_capacity, sizeof(void*));

  if (next == NULL) {
    return 1;
  } 

  _rc_ref_list_copy(list, next);

  free(list->items);
  list->items = next;
  list->capacity = new_capacity;

  return 0;
}

int _rc_ref_list_shrink(rc_ref_list_t* list) {
  if (list->capacity == 1) return 0;

  int new_capacity = list->capacity / 2;
  void** next = calloc(new_capacity, sizeof(void*));

  if (next == NULL) {
    return 1;
  }

  _rc_ref_list_copy(list, next);

  free(list->items);
  list->items = next;
  list->capacity = new_capacity;

  return 0;
} 

inline int _rc_ref_list_should_grow(rc_ref_list_t* list) {
  return list->capacity == list->size;
}

inline int _rc_ref_list_should_shrink(rc_ref_list_t* list) {
  if (list->capacity == 1) return 0;
  return (list->capacity / 2) < list->size;
}
