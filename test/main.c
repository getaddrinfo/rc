#define _PHT_DEBUG

#include <rc_resource.h>
#include <rc_strong.h>
#include <rc_weak.h>

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// we wrap these in do { ... } while(0) to not change semantic meaning of code
#define DEBUG_MSG(ty, msg) do { printf("test(%s): %s\n", ty, msg); } while(0)
#define DEBUG(ty, fmt, ...) do { printf("test(%s): " fmt "\n", ty, __VA_ARGS__); } while(0)
#define DEBUG_FINISH(ty) DEBUG_MSG(ty, "OK\n\n")

#define ARRAY_SIZE(it) sizeof(it)/sizeof(it[0])

void free_test_value(void* ptr) {
  free(ptr);
}

void test_rc_creation_works() {
  void* owned = malloc(sizeof(int*) * 2);

  rc_resource_t* resource = rc_create(owned, free_test_value);
  if (resource == NULL) {
    DEBUG_MSG("rc_creation_works", "rc_create failed");
    goto free_owned;
  }

  DEBUG_FINISH("rc_creation_works");

  rc_release(resource);
  return;

free_owned:
  free(owned);
}

void test_rc_strong_creation_works() {
  void* owned = malloc(sizeof(int*) * 2);

  if (owned == NULL) {
    DEBUG_MSG("rc_strong_creation_works", "malloc owned failed");
    return;
  }

  rc_resource_t* resource = rc_create(owned, free_test_value);
  if (resource == NULL) {
    DEBUG_MSG("rc_strong_creation_works", "rc_create failed");
    goto free_owned;
  }

  rc_strong_t strong = rc_strong_create(resource);
  DEBUG_FINISH("rc_strong_creation_works");

free_resource:
  rc_release(resource);
  return;

free_owned:
  free(owned);
}

void test_rc_weak_creation_works() {
  void* owned = malloc(sizeof(int*) * 2);

  if (owned == NULL) {
    DEBUG_MSG("rc_weak_creation_works", "malloc owned failed");
    return;
  }

  rc_resource_t* resource = rc_create(owned, free_test_value);
  if (resource == NULL) {
    DEBUG_MSG("rc_weak_creation_works", "rc_create failed");
    goto free_owned;
  }

  rc_weak_t* weak = rc_weak_create(resource);
  if (weak == NULL) {
    DEBUG_MSG("rc_weak_creation_works", "rc_weak_create failed");
    goto free_resource;
  }

  DEBUG_FINISH("rc_weak_creation_works");

free_resource:
  rc_release(resource);
  return;

free_owned:
  free(owned);
}

void test_rc_weak_and_strong_behave_correctly() {
  void* owned = malloc(sizeof(int*) * 2);

  if (owned == NULL) {
    DEBUG_MSG("rc_weak_and_strong_behave_correctly", "malloc owned failed");
    return;
  }

  rc_resource_t* resource = rc_create(owned, free_test_value);
  if (resource == NULL) {
    DEBUG_MSG("rc_weak_and_strong_behave_correctly", "rc_create failed");
    goto free_owned;
  }

  rc_strong_t strong[2] = {
    rc_strong_create(resource),
    rc_strong_create(resource)
  };

  rc_weak_t* weak[4] = {
    rc_weak_create(resource),
    rc_weak_create(resource),
    rc_weak_create(resource),
    rc_weak_create(resource)
  };

  DEBUG("rc_weak_and_strong_behave_correctly", "references = %d", resource->count);

  for(int i = 0; i < ARRAY_SIZE(weak); i++) {
    DEBUG("rc_weak_and_strong_behave_correctly", "weak(%d) @ 2 strong (expected = valid): %s", i, rc_weak_valid(weak[i]) == 1 ? "valid" : "invalid");
  }

  rc_strong_free(&strong[0]);
  DEBUG("rc_weak_and_strong_behave_correctly", "references = %d", resource->count);

  for(int i = 0; i < ARRAY_SIZE(weak); i++) {
    DEBUG("rc_weak_and_strong_behave_correctly", "weak(%d) @ 1 strong (expected = valid): %s", i, rc_weak_valid(weak[i]) == 1 ? "valid" : "invalid");
  }

  rc_strong_free(&strong[1]);

  for(int i = 0; i < ARRAY_SIZE(weak); i++) {
    DEBUG("rc_weak_and_strong_behave_correctly", "weak(%d) @ 0 strong (expected = invalid): %s", i, rc_weak_valid(weak[i]) == 1 ? "valid" : "invalid");
  }

  DEBUG_FINISH("rc_weak_and_strong_behave_correctly");
  return;

free_resource:
  rc_release(resource);
  return;

free_owned:
  free(owned);
}

void test_upgrading_weak_ref_to_strong_ref_behaves_correctly() {
  void* owned = malloc(sizeof(void*));

  if (owned == NULL) {
    DEBUG_MSG("upgrading_weak_ref_to_strong_ref_behaves_correctly", "malloc owned failed");
    return;
  }

  rc_resource_t* resource = rc_create(owned, free_test_value);
  if (resource == NULL) {
    DEBUG_MSG("upgrading_weak_ref_to_strong_ref_behaves_correctly", "rc_create failed");
    goto free_owned;
  }

  rc_weak_t* weak = rc_weak_create(resource);
  if (weak == NULL) {
    DEBUG_MSG("upgrading_weak_ref_to_strong_ref_behaves_correctly", "rc_weak_create failed");
    return;
  }

  rc_strong_t strong = rc_weak_upgrade(weak);
  
  if (resource->count != 1 || !rc_strong_valid(strong)) {
    DEBUG("upgrading_weak_ref_to_strong_ref_behaves_correctly", "rc_weak_upgrade failed, ref count @ %d (expected 1)", resource->count);
    return;
  }

  DEBUG("upgrading_weak_ref_to_strong_ref_behaves_correctly", "resource @ %d (expected = 1)", resource->count);
  DEBUG_FINISH("upgrading_weak_ref_to_strong_ref_behaves_correctly");

  rc_strong_free(&strong);
  return;

free_owned:
  free(owned);
}

int main() {
  test_rc_creation_works();
  test_rc_strong_creation_works();
  test_rc_weak_creation_works();
  test_rc_weak_and_strong_behave_correctly();
  test_upgrading_weak_ref_to_strong_ref_behaves_correctly();

  return 0;
}