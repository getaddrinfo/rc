#ifndef _RC_STRONG_H
#define _RC_STRONG_H

#include "rc_resource.h"

typedef struct {
  rc_resource_t* resource;
} rc_strong_t;

rc_strong_t* rc_strong_create(rc_resource_t* resource); // creates a strong ref
void* rc_strong_get(rc_strong_t* strongref); // gets the value owned by the resource
int rc_strong_free(rc_strong_t* strongref); // frees the strong ref

#endif // _RC_STRONG_H