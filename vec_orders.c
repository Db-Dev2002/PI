#include "vec.h"

Orders *vec_orders_new() {
  Order *v = (Order *)malloc(sizeof(Order));
  if (v == NULL) {
    goto error;
  }
  v->size = 0;
  v->capacity = V_ALLOC;
  v->data = (size_t *)malloc(sizeof(size_t) * v->capacity);
  if (v->data == NULL) {
    goto error;
  }
  return v;

error:
  perror("not able to allocate memory!");
  exit(1);
}

void vec_orders_destroy(Orders *v) {
  assert(v);
  free(v->data);
  free(v);
}

int vec_orders_is_empty(Vehicles *v) {
  assert(v);
  return v->size == 0;
}

size_t vec_orders_size(Orders *v) {
  assert(v);
  return v->size;
}

Order *vec_orders_get(Orders *v, size_t i) {
  assert(v);
  if (i < 0 || i >= v->size) {
    perror("not able to reach element");
    exit(1);
  }
  return v->data[i];
}

void vec_orders_expand(Orders *v) {
  assert(v);
  size_t new_capacity = 2 * v->capacity;
  size_t *new_array = (size_t *)malloc(sizeof(size_t) * new_capacity);
  if (new_array == NULL) {
    perror("not able to expand vector on memory!");
    exit(1);
  }
  for (size_t i = 0; i < v->size; i++) {
    new_array[i] = v->data[i];
  }
  free(v->data);
  v->data = new_array;
  v->capacity = new_capacity;
}

void vec_orders_halve(Orders *v) {
  assert(v);
  size_t new_capacity = v->capacity / 2;
  size_t *new_array = (size_t *)malloc(sizeof(size_t) * new_capacity);
  if (new_array == NULL) {
    perror("not able to halve vector on memory!");
    exit(1);
  }
  for (size_t i = 0; i < v->size; i++) {
    new_array[i] = v->data[i];
  }
  free(v->data);
  v->data = new_array;
  v->capacity = new_capacity;
  v->size = v->size < new_capacity ? v->size : new_capacity;
}

void vec_orders_push(Orders *v, Order *value) {
  assert(v);
  if (v->size == v->capacity) {
    vec_expand(v);
  }
  v->data[v->size++] = value;
}

void vec_orders_change_at(Orders *v, size_t i, Order *value) {
  assert(v);
  if (i < 0 || i >= v->size) {
    perror("Out of index!");
    abort();
  }
  v->data[i] = value;
}

void vec_orders_push_at(Orders *v, size_t i, Order *value) {
  assert(v);
  if (i < 0 || i >= v->size) {
    perror("Out of index!");
    abort();
  }
  if (v->size == v->capacity) {
    vec_expand(v);
  }
  for (size_t x = v->size; x > i; x--) {
    v->data[x] = v->data[x - 1];
  }
  v->data[i] = value;
  v->size++;
}

void vec_orders_rm_at(Orders *v, size_t i) {
  assert(v);
  if (i < 0 || i >= v->size) {
    perror("Out of index!");
    abort();
  }
  for (size_t x = i + 1; x < v->size; x++) {
    v->data[x - 1] = v->data[x];
  }
  v->size--;
  if (v->size < v->capacity / 4) {
    vec_halve(v);
  }
}

void vec_orders_clear(Orders *v) {
  assert(v);
  v->size = 0;
  while (v->capacity > V_ALLOC) {
    vec_halve(v);
  }
}