#ifndef TO_DO_H
#define TO_DO_H
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef struct {
    size_t day;
    size_t month;
    size_t year;
    size_t total;
} Date;

typedef struct {
    size_t id;
    char *name;
    bool urgent;
    Date date;
} Task;

typedef struct {
    Task *data;
    size_t size;
    size_t capacity;
} Task_List;




extern Task_List tasks;


bool initialise_task_list(void);
bool add_task(const Task task);
void add_test_tasks(void);
bool remove_task(const long id);
bool sort_tasks_urgent(void);
bool sort_tasks_date(void);
void recalibrate_ids(void);


void set_date(Task *task);

void print_task(Task task);
bool get_urgent(void);
char *get_name(void);
long get_id(void);
char *get_sort_choice(void);
void clear_input_line(void);

bool task_list_cmp(Task_List A1, Task_List A2);
bool task_cmp(Task T1, Task T2);

bool is_sorted_urgent(void);
bool is_sorted_date(void);

bool merge_sort(size_t p, size_t r);
bool merge(size_t p, size_t q, size_t r);

bool free_task_memory(Task task);
bool free_task_list_memory(void);


#endif