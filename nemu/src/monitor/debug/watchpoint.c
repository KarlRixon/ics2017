#include "monitor/watchpoint.h"
#include "monitor/expr.h"

//#include <string.h>

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
	printf(c_yellow c_bold "NO\t\t" c_yellow "type\t\t" c_blue "str\t\t" c_purple "oldvalue\t\t\t" c_cyan "value\n" c_normal);
	while(p){
		printf(c_bold "wp[%d]\t\t" c_yellow "%s\t\t" c_blue "%s\t\t" c_purple "%d | 0x%x\t\t" c_cyan "%d | 0x%x\n" c_normal, p->NO, p->breakpoint ? "b":"w", p->str, p->oldvalue, p->oldvalue, p->value, p->value);
		p = p->next;
	}
}

void new_wp(char *str, int value, bool b){
	if(free_ == NULL){
		printf(c_red c_bold "too many watchpoints\ntry to change NR_WP\n" c_normal);
		assert(0);
	}
	WP *n = NULL;
	n = free_;
	free_ = free_->next;

	n->oldvalue = n->value = value;
	strcpy(n->str, str);
	n->next = NULL;
	if(b == true)
		n->breakpoint = true;
	else if(b == false)
		n->breakpoint = false;

	if(head == NULL){
		head = n;
	}
	else{
		n->next = head;
		head = n;
	}
	return;
}

void free_wp(int no){
	WP *p = head;
	if(p == NULL){
		printf(c_red c_bold "no watchpoints\n" c_normal);
		return;
	}
	else if(p->NO == no){
		printf(c_blue c_bold "deleted a watchpoint: %s = %d | %x\n" c_normal, p->str, p->value, p->value);
		memset(p->str, 0, sizeof(p->str)-1);
		p->oldvalue = p->value = 0;
		p->breakpoint = false;
		
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
				printf(c_blue c_bold "deleted a watchpoint: %s = %d | %x\n" c_normal, p->str, p->value, p->value);
				memset(p->str, 0, sizeof(p->str)-1);
				p->oldvalue = p->value = 0;

				q->next = p->next;
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

WP* check_wp(WP *w){
	if(w == NULL){
		w = head;
	}
	while(w != NULL){
		bool success = false;
		int result = expr(w->str, &success);
		if(!success){
			printf(c_red c_bold "error in calculating EXPR\n" c_normal);
			assert(0);
		}
		else if(result != w->value){
			w->oldvalue = w->value;
			w->value = result;
			return w;
		}
		w = w->next;
	}
	return NULL;
}
