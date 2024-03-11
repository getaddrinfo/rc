#ifndef _RC_REF_H
#define _RC_REF_H

#include "rc_resource.h"

typedef struct {
  rc_resource_t* resource;
} rc_strong_t;

typedef struct {
  rc_resource_t* resource;
} rc_weak_t;

#endif // _RC_REF_H