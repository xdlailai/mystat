#include "tsar.h"

int is_digit(const char *str)
{
  while(*str) {
    if(!isdigit(*str++)) {
      return 0;
    }
  }
  return 1;
}

int convert_record_to_array(U_64 *array, int l_array, const char *record)
{
  int i = 0;
  char *token;
  char n_str[LEN_4096] = {0};
  if(!record || !strlen(record)){
    return 0;
  }
  memcpy(n_str, record, strlen(record));
  token = strtok(n_str, DATA_SPLIT);
  while(token){
    if(!is_digit(token)){
      return 0;
    }
    if(i < l_array){
      *(array + i) = strtoull(token, NULL, 10);
    }
    token = strtok(NULL, DATA_SPLIT);
    i++;
  }
  if(i != l_array){
      return 0;
  }
  return i;
}

int get_strtok_num(const char *str, const char *split)
{
  int num = 0;
  char *token, n_str[LEN_4096] = {0};
  if(!str || !strlen(str)){
    return 0;
  }
  memcpy(n_str, str, strlen(str));

  token = strtok(n_str, split);
  while(token){
    num++;
    token = strtok(NULL, split);
  }
  return num;
}

int strtok_next_item(char item[], char *record, int *start)
{
  char *s_token, *e_token, *n_record;
  if(!record || !strlen(record) || strlen(record) <= *start){
    return 0;
  }
  n_record = record + *start;
  e_token = strstr(n_record, ITEM_SPLIT);
  if(!e_token){
    return 0;
  }
  s_token = strstr(n_record, ITEM_SPSTART);
  if(!e_token){
    return 0;
  }
  memcpy(item, s_token + sizeof(ITEM_SPSTART) - 1, e_token - s_token - 1);
  *start = e_token -record + sizeof(ITEM_SPLIT);
  return 1;
}
