#include <stdlib.h>

#include "rc_resource.h"
#include "rc_ref.h"
#include "rc_weak.h"
#include "rc_strong.h"

rc_strong_t* rc_strong_create(rc_resource_t* resource) {
  rc_strong_t* strong = (rc_strong_t*)malloc(sizeof(rc_strong_t));
  
  if (strong == NULL) {
    return NULL;
  }

  strong->resource = resource;
  strong->resource->count++;

  return strong;
}

rc_strong_t* rc_strong_clone(rc_strong_t* ref) {
  if (ref == NULL) {
    return NULL;
  }

  if (ref->resource == NULL) {
    return NULL;
  }

  return rc_strong_create(ref->resource);
}

rc_weak_t* rc_strong_weak_clone(rc_strong_t* ref) {
  if (ref == NULL) {
    return NULL;
  }

  if (ref->resource == NULL) {
    return NULL;
  }

  return rc_weak_create(ref->resource);
}

void* rc_strong_get(rc_strong_t* strongref) {
  if (strongref == NULL) {
    return NULL;
  }

  if (strongref->resource == NULL) {
    return NULL;
  }


  return strongref->resource->owned;
}

int rc_strong_free(rc_strong_t* strongref) {
  rc_release(strongref->resource);
  free(strongref);

  return 0;
}

