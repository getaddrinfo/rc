#include <stdlib.h>
#include "rc_resource.h"
#include "rc_weak.h"

rc_resource_t* rc_create(void* value, rc_resource_freer_t free_impl_fn) {
  if (value == NULL) {
    return NULL;
  }

  if (free_impl_fn == NULL) {
    return NULL;
  }

  rc_resource_t* resource = (rc_resource_t*)malloc(sizeof(rc_resource_t));
  if (resource == NULL) {
    return NULL;
  }

  rc_ref_list_t* weak_ref_list = rc_ref_list_create();
  if (weak_ref_list == NULL) {
    free(resource);
    return NULL;
  }

  resource->count = 0;
  resource->owned = value;
  resource->_free = free_impl_fn;
  resource->weaks = weak_ref_list;

  return resource;
}

int rc_release(rc_resource_t* resource) {
  resource->count--;
  if (resource->count != 0) return 0;

  int i, seen;
  rc_ref_list_t* weaks = resource->weaks;

  for(i = 0; i < weaks->capacity; i++) {
    void* curr = weaks->items[i];

    // if the element is NULL, it was a used space that was released.
    if (curr == NULL) continue;

    // invalidate the ref, so when calling `rc_weak_get` it returns NULL.
    ((rc_weak_t*)curr)->resource = NULL;
    seen++;

    if(seen == weaks->size) break;
  } 

  resource->_free(resource->owned);
  free(resource);

  return 0;
}