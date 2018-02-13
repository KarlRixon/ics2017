#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

 WP wp_pool[NR_WP];
static WP *head, *free_;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */


void show_wp(){
	WP *p=head;
	printf(c_yellow c_bold "NO		str		value\n" c_normal);
	while(p){
		printf(c_bold "wp[%d]		%s		%d\n" c_normal, p->NO,p->str,p->value);
		p = p->next;
	}
}
