#include <stdlib.h>

#include "rc_ref.h"
#include "rc_ref_list.h"

#include "rc_weak.h"
#include "rc_strong.h"

rc_weak_t* rc_weak_create(rc_resource_t* resource) {
  rc_weak_t* ref = (rc_weak_t*)malloc(sizeof(rc_weak_t));

  if (ref == NULL) {
    return NULL;
  }

  int insert_result = rc_ref_list_insert(resource->weaks, (void*)ref);
  
  if (insert_result != 0) {
    free(ref);
    return NULL;
  }

  ref->resource = resource;

  return ref;
}

rc_strong_t rc_weak_upgrade(rc_weak_t* ref) {
  if (rc_weak_valid(ref) == 0) {
    rc_strong_t invalid = { .resource = NULL };
    return invalid;
  }

  return rc_strong_create(ref->resource);
}

int rc_weak_free(rc_weak_t* ref) {
  if (ref->resource != NULL) {
    rc_ref_list_remove(ref->resource->weaks, ref);
  }

  free(ref);
  return 0;
}

void* rc_weak_get(rc_weak_t* ref) {
  if (rc_weak_valid(ref) == 0) {
    return NULL;
  }
  
  return ref->resource->owned;
}

inline int rc_weak_valid(rc_weak_t* ref) {
  return ref->resource != NULL;
}