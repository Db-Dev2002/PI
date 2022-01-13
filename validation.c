#include "validation.h"

// static definitions hide the validation proccess from other libs, the program
// is insecure since pointers are given with memory that's not read
// only, so "full" control is granted, thats why these definitions are extern
// "public", data validation definitions should be visible only to this file
// in a more secure environment and data modification would be statically called
// here, given pointers to other libs should point only to allocated temporary
// structs, not all the data the program has, if thats nedded then the const
// modifier would have to be considered in some places to avoid security issues.
// clones and read only memory managed at some degree of control, as the project
// grows some areas are not meant to be accessable from others.

// validation by struct field for readability, allocating all this funtions
// comes with some runtime cost.

size_t assign_oid(Orders *v) { return (v->len)++; }

bool vehicle_id_exists(Vehicles *v, const char *id) {
  for (size_t i = 0; i < v->len; i++) {
    if ((strcmp(id, (&v->data[i])->id)) == 0)
      return true;
  }
  return false;
}

bool order_id_exists(Orders *v, const size_t id) {
  for (size_t i = 0; i < v->len; i++) {
    if (id == (&v->data[i])->id)
      return true;
  }
  return false;
}

uint8_t validate_v_price(float *price) {
  return (*price > 0) && (*price < 1000000) ? 1 : 0;
}

uint8_t validate_v_autonomy(uint32_t *autonomy) {
  return (*autonomy != 0 && *autonomy <= 10000) ? 1 : 0;
}

uint8_t validate_o_nif(size_t *nif) { return (*nif >= 10000000) ? 1 : 0; }

uint8_t validate_o_time(uint32_t *time) {
  return (*time <= 10000 && *time != 0) ? 1 : 0;
}

uint8_t validate_o_distance(uint32_t *distance) {
  return (*distance <= 10000 && *distance != 0) ? 1 : 0;
}

static inline size_t calculate_dst_v(Vehicle *v_id, Orders *o) {
  size_t distance_needed = 0;
  for (size_t i = 0; i < o->len; i++) {
    if (v_id == (&o->data[i])->v_id)
      distance_needed += (&o->data[i])->distance;
  }
  return distance_needed;
}

static inline Vehicle *search_vehicle_by_type(Vehicles *v, const char *type,
                                              int previous_match) {
  for (size_t i = previous_match++; i < v->len; i++) {
    if ((strcmp(type, (&v->data[i])->type)) == 0)
      return &v->data[i];
  }
  return NULL;
}

Vehicle *assign_vid(Vehicles *v, Orders *o, Vehicle *v_id,
                    const uint32_t distance) {
  uint32_t distance_needed = distance;
  if (distance_needed += calculate_dst_v(v_id, o) <= v_id->autonomy)
    return v_id;
  // try find another vehicle with the same type
  Vehicle *temp = v_id;
  for (int i = -1; v_id == NULL;
       v_id = search_vehicle_by_type(v, v_id->type, i)) {
    if (temp == v_id) // dont check the same vehicle
      continue;
    distance_needed = distance; // reset variable
    if (distance_needed += calculate_dst_v(v_id, o) <= v_id->autonomy)
      return v_id;
  }
  return NULL;
}