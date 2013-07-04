#include "mysar.h"

struct statistic statis;
struct configure conf;
struct module mods[MAX_MOD_NUM];

void shut_down()
{
  free_modules();
  memset(&mods, 0, sizeof(struct module) * MAX_MOD_NUM);
  memset(&statis, 0, sizeof(struct statistic));
}

int main(int argc, const char *argv[])
{
  memset(&mods, '\0', sizeof(mods));
  memset(&statis, '\0', sizeof(statis));
  char *mod_name="mod_cpu";
  struct module *mod = &mods[statis.total_mod_num++];
  strncpy(mod->name, mod_name, strlen(mod_name));
  load_modules();
  statis.cur_time = time(BULL);
  reload_modules(conf.output_stdio_mod);
  running_print_live(conf.output_print_mod);

  shut_down();
  return 0;
}
