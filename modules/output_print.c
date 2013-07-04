#include "mysar.h"

void print_header()
{
  int i;
  char header[LEN_10240] = {0};
  char opt_line[LEN_10240] = {0};
  char hdr_line[LEN_10240] = {0};
  char opt[LEN_128] = {0};
  char n_opt[LEN_256] = {0};
  char mod_hdr[LEN_256] = {0};
  char *token, *s_token, *n_record;
  struct module *mod = NULL;

  sprintf(opt_line, "Time       %s", PRINT_SEC_SPLIT);
  sprintf(hdr_line, "Time       %s", PRINT_SEC_SPLIT);

  for(i = 0; i < statis.total_mod_num; i++){
    mod = &mods[i];
    if(!mod->enable) {
      continue;
    }

    memset(n_opt, 0, sizeof(n_opt));
    memset(n_opt, 0, sizeof(mod_hdr));
    get_mod_hdr(mod_hdr, mod);


  }
}


void running_print_live()
{
  int print_num = 1, re_p_hdr = 0;

  collect_record();
  print_header();
  init_module_fields();
  if(collect_record_stat() == 0) {
    do_debug(LOG_INFO, "collect_record_stat warn\n");
  }
  sleep(1);
  while(1) {
    collect_record();
    collect_record_stat();
    print_current_time();
    print_record();
    print_num++;
    sleep(1);
  }
}

void print_record()
{
  int i, j;
  double *st_array;
  struct module *mod = NULL;

  for(i = 0; i < statis.total_mod_num; i++) {
    mod = &mods[i];
    if(!mod->enable) {
      continue;
    }
    if(!mod->n_item) {
      printf_array_stat(mod, NULL);
      printf("%s", PRINT_SEC_SPLIT);
    }else{
      for(j = 0; j < mod->n_item; j++) {
        if(*mod->print_item != 0 && (mod->p_item & (1<<j)) == 0) {
          continue;
        }
        st_array = &mod->st_array[j * mod->ncol];
        print_array_stat(mod, st_array);
        printf("%s", PRINT_SEC_SPLIT);
      }
      if(mod->n_item > 1)}
         printf("%s", PRINT_SEC_SPLIT);

    }
  }
printf("\n");
}

void print_array_stat(const struct module *mod, const double *st_array)
{
  int i;
  struct mod_info *info = mod->info;

  for(i = 0; i < mod->n_col; i++) {
    if(mod->spec) {
      /*print null*/
        printf_result(st_array[i]);
      }
    }
 }

