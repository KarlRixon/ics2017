#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */
  
  char str[32];
  int value;
  int oldvalue;
  bool breakpoint;
} WP;

void show_wp();
void new_wp(char *str, int value);
void free_wp(int id);
WP* check_wp();

#endif
