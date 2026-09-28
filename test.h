#ifndef TEST
#define TEST
#include <stdlib.h>
#include <stdbool.h>

bool populate_task_list(void);
void stock_remove_all_tasks(void);
bool test_add_task(void);
bool test_remove_task(void);
bool test_urgent_sort(void);
bool test_date_sort(void);
bool test_edit(void);
bool test_all(void);

#endif