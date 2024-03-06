# rc

A simple reference counting library to experiment with implementing my own reference counting system. Implements both strong and weak references. Atomic references not implemented -- this library is not thread safe.

## Usage

```c
// include the necessary headers
#include "rc_resource.h"
#include "rc_strong.h"
#include "rc_weak.h"

void free_rc_managed_resource(void* ptr) {
  free(ptr);
}

int main() {
  int* example = calloc(1024, sizeof(int));
  if (example == NULL) return 1;

  rc_resource_t* resource = rc_create((void*)example, free_rc_managed_resource);
  if (resource == NULL) return 2;

  // get a strong reference
  rc_strong_t* strong = rc_strong_create(resource);
  if (strong == NULL) return 3;

  int* rc_strong_managed_example = (int*)rc_strong_get(strong);
  // use rc_strong_managed_example, but drop from scope once done

  rc_weak_t* weak = rc_weak_create(resource);
  if (weak == NULL) return 4;

  int* rc_weak_managed_example = (int*)rc_weak_get(weak);
  // use rc_weak_maanged_example

  rc_strong_free(strong);

  // the reference count has dropped to 0, meaning that the underlying resourve has been released, and the
  // weak reference is no longer valid. weak references allow you to use a resource without keeping it alive
  // longer than needed
  printf("%d\n", rc_weak_valid(weak)); // 0
}
```