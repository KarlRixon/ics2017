#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#include <string.h>

#define NR_WP 32

static WP wp_pool[NR_WP];
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
	printf(c_yellow c_bold "NO\t\t" c_blue "str\t\t" c_cyan "value\n" c_normal);
	while(p){
		printf(c_bold "wp[%d]\t\t" c_blue "%s\t\t" c_cyan "%d\n" c_normal, p->NO,p->str,p->value);
		p = p->next;
	}
}

WP* new_wp(char *str, int value){
	if(free_ == NULL){
		printf(c_red c_bold "too many watchpoints\ntry to change NR_WP\n" c_normal);
		assert(0);
	}
	WP *n = NULL;
	n = free_;
	free_ = free_->next;

	n->value = value;
	strcpy(n->str, str);
	n->next = NULL;

	if(head == NULL){
		head = n;
	}
	else{
		n->next = head;
		head = n;
	}
	return n;
}

void free_wp(int no){
	WP *p = head;
	if(p == NULL){
		printf(c_red c_bold "no watchpoints\n" c_normal);
		return;
	}
	else if(p->NO == no){
		memset(p->str, 0, sizeof(p->str)-1);
		p->value = 0;
		
		head = p->next;
		p->next = free_;
		free_ = p;
		return;
	}
	else{
		WP *q = head;
		p = q->next;
		while(p != NULL){
			if(p->NO == no){
				memset(p->str, 0, sizeof(p->str)-1);
				p->value = 0;

				head = p->next;
				p->next = free_;
				free_ = p;
				return;
			}
			else{
				q = q->next;
				p = p->next;
			}
		}
	}
	printf(c_red c_bold "no such watchpoint\n" c_normal);
	return;
}
