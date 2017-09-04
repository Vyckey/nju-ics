#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
	int NO;
	struct watchpoint *next;
	/* TODO: Add more members if necessary */
	uint32_t last_value;
	char *expr_str;
} WP;

void init_wp_pool();
WP *new_wp(char *str);
void free_wp(unsigned int no);
void clear_wp();
void list_wp();
void update_wp();
#endif
