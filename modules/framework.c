#include "mysar.h"

void register_mod_fileds(struct module *mod, const char *opt, const char *usage,
    struct mod_info *info, int n_col, void *data_collect, void *set_st_record)
{
  sprintf(mod->opt_line, "%s", opt);
  sprintf(mod->usage, "%s", usage);
  mod->info = info;
  mod->n_col = n_col;
  mod->data_collect = data_collect;
  mod->set_st_record = set_st_record;

}

void load_modules()
{
  int i;
  char buff[LEN_128] = {0};
  char mod_path[LEN_128] = {0};
  struct module *mod = NULL;
  int (*mod_register) (struct module *);
  sprintf(buff, "/home/fly100/workplace/mystat/modules");

  for(i = 0; i < statis.total_mod_num; i++) {
    mod = &mods[i];
    if(!mod->lib) {
      memset(mod_path, '\0', LEN_128);
      snprintf(mod_path, LEN_128, "%s/%s.so", buff, mod->name);
      if(!(mod->lib = dlopen(mod_path, RTLD_NOW|RTLD_GLOBAL))) {
        do_debug(LOG_ERR, "load_modules: dlopen module %s err %s\n", mod_name, dlerror());
      }else{
        mod_register = dlsym(mod->lib, "mod_register");
        if(dlerror()) {
          do_debug(LOG_ERR, "Load_modules:dlsym module %s err %s\n", mod_name, dlerror());
          break;
        }else {
          mod_register(mod);
          mod->enable = 1;
          mod->spec = 0;
          do_debug(LOG_INFO, "load_modules: load new module '%s' to mods\n", mod_path);
        }
      }
    }
  }
}


int reload_modules(const char *s_mod)
{
  int i;
  int reload = 0;
  struct module *mod;
  if(!s_mod || !strlen(s_mod)) {
    return reload;
  }

  for(i=0;i<statis.total_mod_num;i++) {
    mod = &mods[i];
    if(is_include_string(s_mod, mod->name) || is_include_string(s_mod, mod->opt_line)){
      mod->enable = 1;
      reload = 1;
    }else {
      mod->enable = 0;
    }
  }
  return reload;
}

void collect_record()
{
  int i;
  struct module *mod = NULL;
  for(i = 0; i < statis.total_mod_num; i++) {
    mod = &mods[i];
    if(!mod->enable) {
      continue;
    }
    memset(mod->record, 0, sizeof(mod->record));
    if(mod->data_collect){
      mod->data_collect(mod, mod->parameter);
    }
  }
}

void set_mod_record(struct module *mod, const char *record)
{
  if(record){
    sprintf(mod->record, "%s", record);
  }
}

void init_module_fields()
{
  int i;
  struct module *mod = NULL;

  for(i = 0; i < statis.total_mod_num; i++) {
    mod = &mods[i];
    if(!mod->enable){
      continue;
    }
    mod->n_item = get_strtok_num(mod->record, ITEM_SPLIT);
    if(mod->n_item) {
      mod->pre_array = (U_64)calloc(mod->n_item * mod->n_col, sizeof(U_64));
      mod->cur_array = (U_64)calloc(mod->n_item * mod->n_col, sizeof(U_64));
      mod->st_array = (double *)calloc(mod->n_item * mod->n_col, sizeof(double));
     /*print_tail*/
      mod->max_array = (double *)calloc(mod->n_item * mod->n_col, sizeof(double));
      mod->mean_array = (double *)calloc(mod->n_item * mod->n_col, sizeof(double));
      mod->min_array = (double *)calloc(mod->n_item * mod->n_col, sizeof(double));
    }
  }
}

void set_st_record(struct module *mod)
{
  int i, j, k = 0;
  struct mod_info *info = mod->info;
  mod->st_flag = 1;

  for(i = 0; i < mod->n_item; i++){
    if(mod->set_st_record){
      mod->set_st_record(mod, &mod->st_array[i * mod->n_col],
          &mod->pre_array[i * mod->n_col],
          &mod->cur_array[i * mod->n_col],
          2
          );
    }
  }
}

int collect_record_stat()
{
  int i, n_item, ret, no_p_hdr = 1;
  U_64 *tmp, array[MAX_COL_NUM] = {0};
  struct module *mod = NULL;
  for(i = 0; i < statis.total_mod_num; i++){
    mod = &mods[i];
    if(!mod->enable) {
      continue;
    }

    memset(array, 0, sizeof(array));
    mod->st_flag = 0;
    ret =0;

    if((n_item = get_strtok_num(mod_record, ITEM_SPLIT))){
      mod->item = n_item;
      if(strstr(mod->record, ITEM_SPLIT)){
        char item[LEN_128] = {0};
        int num = 0;
        int pos = 0;
        while(strtok_next_item(item, mod->record, &pos)){
          if(!(ret = convert_record_to_array(&mod->cur_array[num * mod->n_col], mod->ncol, item))){
            break;
          }
          memset(item, 0, sizeof(item));
          num ++;
        }
      }else {
        ret = convert_record_to_array(mod->cur_array, mod->n_col, mod->record);
      }
      if(no_p_hdr && mod->pre_flag && ret) {
        set_st_record(mod);
      }
      if(!ret){
        mod->pre_flag = 0;
      }else{
        mod->pre_flag = 1;
      }

    }else {
      mod->pre_flag = 0;
    }
    tmp = mod->pre_array;
    mod->pre_array = mod->cur_array;
    mod->cur_array = tmp;
  }
  return no_p_hdr;
}
