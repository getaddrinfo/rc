#ifndef _RC_STRONG_H
#define _RC_STRONG_H

#include "rc_resource.h"
#include "rc_ref.h"

rc_strong_t* rc_strong_create(rc_resource_t* resource); // creates a strong ref
rc_strong_t* rc_strong_clone(rc_strong_t* ref); // clones a strong reference, creating a new strong reference to it
rc_weak_t* rc_strong_weak_clone(rc_strong_t* ref); // creates a weak reference from a strong reference

void* rc_strong_get(rc_strong_t* strongref); // gets the value owned by the resource
int rc_strong_free(rc_strong_t* strongref); // frees the strong ref

#endif // _RC_STRONG_H