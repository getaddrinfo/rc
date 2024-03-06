#ifndef _RC_RESOURCE_H
#define _RC_RESOURCE_H

#include "rc_ref_list.h"

typedef void (*rc_resource_freer_t)(void* ptr);

typedef struct {  
  void* owned;

  int count; 
  rc_resource_freer_t _free;
  rc_ref_list_t* weaks;
} rc_resource_t;

rc_resource_t* rc_create(void* value, rc_resource_freer_t free_impl_fn);
int rc_release(rc_resource_t* resource);

#endif // _RC_RESOURCE_H