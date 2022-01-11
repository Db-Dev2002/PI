#ifndef __VEC_H__
#define __VEC_H__

#include "tp.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#define V_ALLOC 64
// A contiguos growable array type "vector"
// +------+----------+---------+
// | size | capacity | data[]  |
// +------+----------+---------+
//                   ^
//                   | given pointer

typedef struct vehicle_vec {
  Vehicle *data;
  size_t len, capacity;
} Vehicles;

typedef struct order_vec {
  Order *data;
  size_t len, capacity;
} Orders;

void read_vehicles(Vehicles *);
Orders *vec_orders_new();
size_t vec_orders_len(Orders *);
bool vec_orders_is_empty(Orders *);
void vec_orders_change_at(Orders *, size_t, Order *);
void vec_orders_rm_at(Orders *, size_t);
void vec_orders_push_at(Orders *, size_t, Order *);
void vec_orders_destroy(Orders *);
void vec_orders_halve(Orders *);
void vec_orders_expand(Orders *);

Order *vec_orders_get(Orders *v, size_t idx);
void vec_orders_push(Orders *v, Order *value);

Vehicles *vec_vehicles_new();
Vehicle *vec_vehicles_get(Vehicles *, size_t idx);
size_t vec_vehicles_len(Vehicles *);
bool vec_vehicles_is_empty(Vehicles *);
void vec_vehicles_expand(Vehicles *);
void vec_vehicles_halve(Vehicles *);
void vec_vehicles_push(Vehicles *, Vehicle *);
void vec_vehicles_change_at(Vehicles *, size_t idx, Vehicle *);
void vec_vehicles_rm_at(Vehicles *, size_t idx);
void vec_vehicles_push_at(Vehicles *, size_t idx, Vehicle *);
void vec_vehicles_destroy(Vehicles *);

void menu(Vehicles *, Orders *);
Order *search_order_by_id(Orders *v, const size_t id);
Vehicle *search_vehicle_by_id(Vehicles *v, const char *id);
#endif
