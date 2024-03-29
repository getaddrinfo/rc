#ifndef _RC_WEAK_H
#define _RC_WEAK_H

#include "rc_resource.h"
#include "rc_ref.h"

rc_weak_t* rc_weak_create(rc_resource_t* resource); // creates a weak ref to a resource
rc_strong_t rc_weak_upgrade(rc_weak_t* ref); // creates a strong reference from a weak reference, if possible
void* rc_weak_get(rc_weak_t* ref); // gets a weak value
int rc_weak_free(rc_weak_t* ref); // drops the weakref from the list that needs to be dropped
int rc_weak_valid(rc_weak_t* ref); // returns 1 if weakref->resource != NULL

#endif // _RC_WEAK_H