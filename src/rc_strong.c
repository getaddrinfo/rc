#include <stdlib.h>

#include "rc_resource.h"
#include "rc_ref.h"
#include "rc_weak.h"
#include "rc_strong.h"

rc_strong_t rc_strong_create(rc_resource_t* resource) { 
  resource->count++;

  rc_strong_t strong = { .resource = resource, .valid = 1 };
  return strong;
}

rc_strong_t rc_strong_clone(rc_strong_t ref) {
  return rc_strong_create(ref.resource);
}

rc_weak_t* rc_strong_weak_clone(rc_strong_t ref) {
  return rc_weak_create(ref.resource);
}

void* rc_strong_get(rc_strong_t strongref) {
  return strongref.resource;
}

int rc_strong_free(rc_strong_t* strongref) {
  rc_release(strongref->resource);
  strongref->valid = 0;
  
  return 0;
}

int rc_strong_valid(rc_strong_t strongref) {
  return strongref.valid;
}
