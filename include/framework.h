#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#include "define.h"

struct mod_info {
  char hdr[LEN_128];
  int summary_bit;
  int merge_mode;
  int stats_opt;
};

struct module {
  char name[LEN_32];
  char opt_line[LEN_32];
  char record[LEN_4096];
  char usage[LEN_256];
  char parameter[LEN_256];
  char print_item[LEN_32];

  struct mod_info *info;
  void *lib;
  int enable;
  int spec;
  int p_item;

  /*private data used by framework*/
  int n_item;
  int n_col;
  long n_record;

  int pre_flay:4;
  int st_flay:4;

  U_64 *pre_array;
  U_64 *cur_array;
  double *st_array;
  double *max_array;
  double *mean_array;
  double *min_array;

  /*callback function of module*/
  void (*data_collect) (struct module *, char *);
  void (*set_st_record) (struct module *, double *, U_64 *, U_64 *, int);
  /*mod manage*/
  void (*mod_register) (struct module *);
};

void register_mod_fileds(struct module *mod, const char *opt, const char *usage, struct mod_info *info, int n_col, void *data_collect, void *set_st_record);
void set_mod_record(struct module *mod, const char *record);
int init_module_fields();
int reload_modules(const char *s_mod);
void load_modules();
void free_modules();
void collect_record();
void read_line_to_module_record(char *line);
int collect_record_stat();
void disable_col_zero();

#endif
