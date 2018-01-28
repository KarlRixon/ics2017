#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */
  
  char str[32];
  int value;

} WP;

void show_wp();
WP *new_wp(char *str,int value);

#endif
