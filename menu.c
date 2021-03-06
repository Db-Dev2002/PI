#include "validation.h"
#include "vec.h"
#include <string.h>

static void menu_print() {
  puts("_____________________________________________________________");
  puts("1=> inserir um novo meio de de mobilidade eletrica.");
  puts("2=> remover um transporte urbano");
  puts("3=> requerir a utilizacao de um meio de transporte.");
  puts("4=> cancelar o pedido de utilizacao.");
  puts("5=> listagem de todos os meios de transporte");
  puts("6=> listagem de todos os pedidos efetuados");
  puts("7=> calcular o custo de utilizacao da ordem requisitada");
  puts("8=> listar o plano de utilizacao de um determinado veiculo");
  puts("c=> clear");
  puts("e=> sair");
  puts("q=> sair sem guardar");
}

static inline void prompt_vid(char *input) {
  printf("insira o id de veiculo: ");
  while (scanf("%s", input) != 1) {
    clean_stdin(); // fflush(stdin) is bad practice;
    LOG_ERR("Valor introduzido nao validado, reintroduza!");
    printf("insira o id de veiculo: ");
  }
}

static inline void prompt_type(char *input) {
  printf("insira o tipo de veiculo: ");
  while (scanf("%s", input) != 1) {
    clean_stdin();
    LOG_ERR("Valor introduzido nao validado, reintroduza!");
    printf("insira o tipo de veiculo: ");
  }
}

static inline void prompt_price(float *input) {
  printf("insira o preco/min: ");
  while (scanf("%f", input) != 1) {
    clean_stdin();
    LOG_ERR("Valor introduzido nao validado, reintroduza!");
    printf("insira o preco/min: ");
  }
}

static inline void prompt_autonomy(uint32_t *input) {
  printf("insira a autonomia do veiculo: ");
  while (scanf("%u", input) != 1) {
    clean_stdin();
    LOG_ERR("Valor introduzido nao validado, reintroduza!");
    printf("insira a autonomia do veiculo: ");
  }
}

static inline void prompt_oid(size_t *input) {
  printf("insira o id: ");
  while (scanf("%lu", input) != 1) {
    clean_stdin();
    LOG_ERR("Valor introduzido nao validado, reintroduza!");
    printf("insira o id: ");
  }
}

static inline void prompt_nif(size_t *input) {
  printf("insira o nif: ");
  while (scanf("%lu", input) != 1) {
    clean_stdin();
    LOG_ERR("Valor introduzido nao validado, reintroduza!");
    printf("insira o nif: ");
  }
}

static inline void prompt_time(uint32_t *input) {
  printf("insira o tempo de uso: ");
  while (scanf("%u", input) != 1) {
    clean_stdin();
    LOG_ERR("Valor introduzido nao validado, reintroduza!");
    printf("insira o tempo de uso: ");
  }
}

static inline void prompt_distance(uint32_t *input) {
  printf("insira a distancia: ");
  while (scanf("%u", input) != 1) {
    clean_stdin();
    LOG_ERR("Valor introduzido nao validado, reintroduza!");
    printf("insira a distancia: ");
  }
}

static inline void list_order(Orders *v, size_t i) {
  printf("%lu\t%lu\t%s\t%u\t%u\n", (&v->data[i])->id, (&v->data[i])->nif,
         (&v->data[i])->v_id, (&v->data[i])->time, (&v->data[i])->distance);
}

static inline void list_order_by_ptr(Order *oid) {
  printf("%lu\t%lu\t%s\t%u\t%u\n", oid->id, oid->nif, oid->v_id, oid->time,
         oid->distance);
}

static inline void log_order(Orders *v, size_t i) {
  char *str = malloc(sizeof(char) * (VEHICLE_ID_MAX_CHARS * 4));
  sprintf(str, L_INFO "Registered Vehicle ->%lu|%lu|%s|%u|%u|",
          (&v->data[i])->id, (&v->data[i])->nif, (&v->data[i])->v_id,
          (&v->data[i])->time, (&v->data[i])->distance);
  log_to_file(str);
  free(str);
}

static inline void log_order_by_ptr(Order *oid) {
  char *str = malloc(sizeof(char) * (VEHICLE_ID_MAX_CHARS * 4));
  sprintf(str, L_INFO "Registered Vehicle ->%lu|%lu|%s|%u|%u|", oid->id,
          oid->nif, oid->v_id, oid->time, oid->distance);
  log_to_file(str);
  free(str);
}

static inline void list_vehicle(Vehicles *v, size_t i) {
  printf("%s\t%s\t%f\t%u\n", (&v->data[i])->id, (&v->data[i])->type,
         (&v->data[i])->price, (&v->data[i])->autonomy);
}

static inline void list_vehicle_by_ptr(Vehicle *v_id) {
  printf("%s\t%s\t%f\t%u\n", v_id->id, v_id->type, v_id->price, v_id->autonomy);
}

static inline void log_vehicle(Vehicles *v, size_t i) {
  char *str = malloc(sizeof(char) * (VEHICLE_ID_MAX_CHARS * 4));
  sprintf(str, L_INFO "Registered Vehicle ->%s|%s|%f|%u|", (&v->data[i])->id,
          (&v->data[i])->type, (&v->data[i])->price, (&v->data[i])->autonomy);
  log_to_file(str);
  free(str);
}

static inline void log_vehicle_by_ptr(Vehicle *v_id) {
  char *str = malloc(sizeof(char) * (VEHICLE_ID_MAX_CHARS * 4));
  sprintf(str, L_INFO "Registered Vehicle ->%s|%s|%f|%u|", v_id->id, v_id->type,
          v_id->price, v_id->autonomy);
  log_to_file(str);
  free(str);
}

static inline uint8_t vehicle_build_prompt(Vehicles *v) {
  // malloc is more suitable than calloc, string be copied latter with
  // initialized chars only, and has less cpu iterations.
  char *id = malloc(VEHICLE_ID_MAX_CHARS);
  char *type = malloc(VEHICLE_TYPE_MAX_CHARS);
  char *log;
  float price = 0.0;
  uint32_t autonomy = 0;

  prompt_vid(id);
  if (vehicle_id_exists(v, id)) {
    free(id);
    free(type);
    puts("Id de veiculo ja existe!");
    return 0;
  }
  prompt_type(type); // nothing to verify

  prompt_price(&price);
  while (price == 0 || price >= 1000000) {
    puts("Inserido valor igual a 0 ou maior que E6!");
    prompt_price(&price);
  }
  prompt_autonomy(&autonomy);
  while (autonomy == 0 || autonomy >= 1000000) {
    puts("Inserido valor igual a 0 ou maior que E6!");
    prompt_autonomy(&autonomy);
  }

  // all inputs are verified at this point
  vec_vehicles_push(v, vehicle_build(id, type, price, autonomy));
  LOG_INFO_NW("Registered vehicle");
  log_vehicle(v, (v->len) - 1);
  free(id);
  free(type);
  return 0;
}

static inline void cancel_vehicle_plan(Vehicle *v_id, Orders *v) {
  for (size_t i = 0; i < v->len; i++) {
    if ((strcmp(v_id->id, (&v->data[i])->v_id)) == 0) {
      vec_orders_rm_at(v, i);
      i -= 1; // now data[i] has other order, so we have to check this
    }         // index again, v->len was updated
  }
}

static inline uint8_t rm_vehicle_by_id_prompt(Vehicles *v, Orders *o) {
  char *input = malloc(VEHICLE_ID_MAX_CHARS);
  char verify;
  prompt_vid(input);
  for (size_t i = 0; i < v->len; i++) {
    if ((strcmp(input, (&v->data[i])->id)) == 0) {
      if ((&v->data[i])->active == true) {
        LOG_WARN("existem ordens dependentes do veiculo que pretende remover!, "
                 "cancelar Ordens? (y/n) ");
        clean_stdin();
        scanf("%c", &verify);
        if (verify == 'y') {
          // search for orders that need to be canceled
          cancel_vehicle_plan(&v->data[i], o);
        } else {
          puts("aborted!");
          return 1;
        }
      }
      vec_vehicles_rm_at(v, i);
      LOG_INFO("Vehicles removed");
      free(input);
      return 0;
    }
  }
  if (input) {
    free(input);
  }
  puts("Veiculo nao encontrado");
  return 1; // no data found
}

static inline uint8_t order_build_prompt(Orders *o, Vehicles *v) {
  size_t nif = 0;
  char *v_id_str = malloc(VEHICLE_ID_MAX_CHARS);
  Vehicle *v_id = NULL;
  uint32_t time = 0;
  uint32_t distance = 0;

  if (v->len == 0) {
    LOGF_ERR("Nao exitem veiculos!");
    free(v_id_str);
    return 1;
  }

  prompt_nif(&nif);
  while (nif <= 10000000) {
    puts("inserido um nif invalido!");
    prompt_nif(&nif);
  }
  prompt_vid(v_id_str);
  while ((v_id = search_vehicle_by_id(v, v_id_str)) == NULL) {
    puts("inserido um Id de veiculo invalido!");
    prompt_vid(v_id_str);
  }
  prompt_time(&time);
  while (time == 0 || time >= 1000000) {
    puts("Inserido valor igual a 0 ou maior que E6!");
    prompt_time(&time);
  }
  prompt_distance(&distance);
  while (distance == 0 || distance >= 1000000) {
    puts("Inserido valor igual a 0 ou maior que E6!");
    prompt_distance(&distance);
  }

  // at this point all inputs are verified!
  Vehicle *vid = assign_vid(v, o, v_id_str, distance);
  if (vid == NULL) {
    puts("Nao foi encontrado nenhum veiculo apto para a ordem pretendida.");
    return 1;
  }
  if (vid == v_id) {
    puts("Veiculo pretendido disponivel!");
  } else {
    puts("Veiculo pretendido nao disponivel!");
    printf("Esta disponivel o veiculo %s do tipo %s\n", vid->id, vid->type);
  }
  if (vid->active == false)
    vid->active = true;
  vec_orders_push(o, order_build(assign_oid(o), nif, vid->id, time, distance));
  LOG_INFO_NW("Registered Order");
  log_order(o, (o->len) - 1);
  return 0;
}

static inline uint8_t rm_order_by_id_prompt(Orders *v) {
  size_t input;
  prompt_oid(&input);
  for (size_t i = 0; i < v->len; i++) {
    if (input == (&v->data[i])->id) {
      vec_orders_rm_at(v, i);
      LOG_INFO("Order removed");
      return 0;
    }
  }
  puts("nada encontrado");
  return 1;
}

// 06
static inline void list_vehicles(Vehicles *v) {
  for (size_t i = 0; i < v->len; i++) {
    printf("%s\t%s\t%f\t%u\n", (&v->data[i])->id, (&v->data[i])->type,
           (&v->data[i])->price, (&v->data[i])->autonomy);
  }
}

// 07
static inline void list_orders(Orders *v) {
  for (size_t i = 0; i < v->len; i++) {
    printf("%lu\t%lu\t%s\t%u\t%u\n", (&v->data[i])->id, (&v->data[i])->nif,
           (&v->data[i])->v_id, (&v->data[i])->time, (&v->data[i])->distance);
  }
}

// 07
static inline uint8_t print_calculated_cost_prompt_id(Vehicles *v, Orders *o) {
  size_t input;
  prompt_oid(&input);
  Order *oid = search_order_by_id(o, input);
  if (oid == NULL) {
    puts("Id nao encontrado");
    return 1;
  }
  Vehicle *v_id = search_vehicle_by_id(v, oid->v_id);
  printf("preco de uso calculado: %f eur/min\n", (v_id->price) * (oid->time));
  return 0;
}

static inline void vehicle_plan_prompt_id(Vehicles *v, Orders *o) {
  char *v_id_str = malloc(VEHICLE_ID_MAX_CHARS);
  prompt_vid(v_id_str);
  Vehicle *v_id = search_vehicle_by_id(v, v_id_str);
  if (v_id == NULL) {
    puts("Id nao encontrado");
    free(v_id_str);
    return;
  }
  size_t t = 0;
  uint32_t a = v_id->autonomy;
  for (size_t i = 0; i < o->len; i++) {
    if ((strcmp(v_id->id, (&o->data[i])->v_id)) == 0) {
      printf("%lu\t%lu\t%lu\t%lu\t%u\t%s\n", (&o->data[i])->id,
             (&o->data[i])->nif, t, t + (&o->data[i])->time, a, v_id->id);
      a -= (&o->data[i])->distance;
      t += (&o->data[i])->time;
    }
  }
  free(v_id_str);
}

void input_switch(Vehicles *v, Orders *o) {
  // 10 bytes allocated just in case the user writes more than one char and
  // tries to crash the program, if more than 10 chars are typed undefined
  // behavior wont be present, all the user gets is an Adress boundary error
  char *control = calloc(10, sizeof(char)); // all chars initialized
  char input = '\0';
  uint8_t status = 0; // return values 'errno'
  printf("\n\t\033[94m Opcao: ");
  if (scanf("%s", control) != 1) { // just in case a \n leaked from
    if (control) {
      free(control); // free gargabe
    }
    input_switch(v, o);
  }
  if (control[1] == '\0') { // this garantees the user only typed one char
    input = control[0];
  } else { // recall funtion
    LOG_ERR("varios caracteres inseridos!\n");
    free(control); // free garbage
    input_switch(v, o);
  }
  if (control) {
    free(control); // free garbage
  }
  printf("\33[0m"); // reset blue color

  switch (input) {
  case '1':
    vehicle_build_prompt(v);
    break;
  case '2':
    rm_vehicle_by_id_prompt(v, o);
    break;
  case '3':
    order_build_prompt(o, v);
    break;
  case '4':
    rm_order_by_id_prompt(o);
    break;
  case '5':
    list_vehicles(v);
    break;
  case '6':
    list_orders(o);
    break;
  case '7':
    print_calculated_cost_prompt_id(v, o);
    break;
  case '8':
    vehicle_plan_prompt_id(v, o);
    break;
  case 'c':
    system("clear");
    menu_print();
    input_switch(v, o);
    break;
  case 'e':
    if (write_data_err(v, o)) {
      _exit(0, v, o);
    }
    return;
  case 'q':
    _exit(0, v, o);
  default:
    puts("caracter nao reconhecido!, reinsira");
    input_switch(v, o);
    break;
  }
  input_switch(v, o);
}

void menu(Vehicles *v, Orders *o) {
  system("clear");
  if (read_data_err(v, o)) {
    LOG_WARN("continuing without data!");
  }
  menu_print();
  input_switch(v, o);
}
