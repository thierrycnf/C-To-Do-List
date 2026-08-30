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
    char *date_string;
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
extern size_t next_task;



bool add_task(const Task task);
bool remove_task(const size_t id);
bool sort_tasks_urgent();
bool sort_tasks_date();
void recalibrate_ids();

char *get_date_string();
size_t get_day();
size_t get_month();
size_t get_year();
void get_date();

void print_task(Task task);
bool get_urgent();
char *get_name();
size_t get_id();
char *get_sort_choice();

bool task_list_cmp(Task_List A1, Task_List A2);
bool task_cmp(Task T1, Task T2);

bool is_sorted_urgent();
bool is_sorted_date();

void merge_sort(size_t p, size_t r);
void merge(size_t p, size_t q, size_t r);

bool free_task_memory(size_t id);
bool free_task_list_memory();


#endif