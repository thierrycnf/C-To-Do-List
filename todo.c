#include "todo.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

Task_List tasks = {
    .data = NULL,
    .size = 0,
    .capacity = 1
};

bool add_task(const Task task) {
    if (tasks.size + 1 > tasks.capacity) {
        size_t new_capacity = tasks.capacity * 2;
        Task *temp = realloc(
            tasks.data,
            new_capacity * sizeof(Task)
        );

        if (temp != NULL) {
            tasks.data = temp;
            tasks.capacity = new_capacity;
        }
        else {
            printf("Failed to reallocate memory.\n");
            return false;
        }
    }

    tasks.data[tasks.size++] = task;
    printf("Task created successfully!\n");
    return true;
}

char *get_date_string(Task task){
    char *date = malloc((20) * sizeof *date);
    if (date == NULL) {
        printf("Failed to allocate memory for date string\n");
        return NULL;
    }


    snprintf(date, 20, "%zu/%zu/%zu", task.date.day, task.date.month, task.date.year);

    return date;
}

size_t get_day(void) {
    time_t now = time(NULL);
    struct tm *local = localtime(&now);

    size_t day   = (size_t)local->tm_mday;

    return day;
}

size_t get_month(void) {
    time_t now = time(NULL);
    struct tm *local = localtime(&now);

    size_t month = (size_t)(local->tm_mon + 1);

    return month;
}

size_t get_year(void) {
    time_t now = time(NULL);
    struct tm *local = localtime(&now);

    size_t year  = (size_t)(local->tm_year + 1900);

    return year;
}

void get_date(Task *task) {
    task -> date.day = get_day();
    task -> date.month = get_month();
    task -> date.year = get_year();
    task -> date.total = (task -> date.year * 10000) + (task -> date.month * 100) + task -> date.day;
    task -> date.date_string = get_date_string(*task);
}


void print_task(Task task) {
    printf("%zu. ", task.id);
    if (task.urgent) {
        printf("URGENT | ");
    }

    printf("%s | %s\n", task.name, task.date.date_string);
}

void my_to_lower(char *string) {
    for (size_t i = 0; string[i] != '\0'; i++) {
        string[i] = tolower(string[i]);
    }
}
bool get_urgent(void) {
    char buffer[8];
    bool valid_input = false;
    do {
        printf("Is this task urgent? [yes/no]\n");
        if  (fgets(buffer, sizeof buffer, stdin) != NULL) {
            buffer[strcspn(buffer, "\n")] = '\0';
            my_to_lower(buffer);
        }
        else {
            printf("Unable to read input\n");
            continue;
        }

        if (strcmp(buffer, "yes") != 0 && strcmp(buffer, "no") != 0) {
                printf("Please enter yes or no\n");
            }
        else {
            valid_input = true;
        }
    }
    while (valid_input == false);

    if (strcmp(buffer, "yes") == 0) {
        return true;
    }
    else if (strcmp(buffer, "no") == 0) {
        return false;
    }
    return false;
    
}

char *get_name(void) {
    char *buffer = malloc(100 * sizeof(*buffer));
     if (buffer == NULL) {
        printf("Failed to allocate memory for name string\n");
        return NULL;
    }

    printf("Enter task name\n");

    if (fgets(buffer, 100, stdin) == NULL) {
        printf("Unable to read input\n");
        free(buffer);
        return NULL;
    }

    buffer[strcspn(buffer, "\n")] = '\0';

    return buffer;

    
}

char *get_sort_choice(void) {
    static size_t allocated_space = 10;
    char *buffer = malloc(allocated_space * sizeof(*buffer));
    if (buffer == NULL) {
        printf("Failed to allocated memory for buffer\n");
        return NULL;
    }
    bool valid_input = false;
    do {
        printf("How would you like to sort your tasks? [date/urgency]\n");
        if  (fgets(buffer, allocated_space, stdin) != NULL) {
            buffer[strcspn(buffer, "\n")] = '\0';
            my_to_lower(buffer);
        }
        else {
            printf("Unable to read input\n");
            continue;
        }

        if (strcmp(buffer, "date") != 0 && strcmp(buffer, "urgency") != 0) {
                printf("Please enter date or urgency\n");
            }
        else {
            valid_input = true;
        }
    }
    while (valid_input == false);

    return buffer;


}

bool remove_task(const size_t id) {
    if (tasks.size == 0) {
        printf("You have no tasks!\n");
        return false;
    }

    else if (id > tasks.size) {
        printf("This task does not exist!\n");
        return false;
    }

    else if (id <= 0) {
        printf("Please enter a valid id\n");
        return false;
    }
    
    size_t index = id - 1;
    Task task = tasks.data[index];
    free_task_memory(task);
    for (size_t i = index; i + 1< tasks.size; i++) {
        tasks.data[i] = tasks.data[i + 1];
        tasks.data[i].id -= 1;
    }


    if (--tasks.size <= tasks.capacity / 4) {
        size_t new_capacity = tasks.capacity / 2;
        if (new_capacity == 0) {
            new_capacity = 1;
        }
        Task *temp = realloc(
        tasks.data,
        new_capacity * sizeof(Task)
            
    );

        if (temp != NULL) {
            tasks.data = temp;
            tasks.capacity = new_capacity;
        }
        else {
            printf("Failed to reallocate memory.\n");
            return false;
        }
    }
    printf("Successfully removed task!\n");
    return true;
    }
        

size_t get_id(void) {
    char buffer[32];
    char *end;
    size_t id;
    bool valid_input = false;
    
    printf("Enter the task ID\n");
    do {
        if (fgets(buffer, sizeof buffer, stdin) != NULL) {
            id = strtol(buffer, &end, 10);

            if (end == buffer) {
                printf("Please enter a number\n");
            }
            else if (*end != '\n' && *end != '\0') {
                printf("Invalid characters after the number\n");
            }
            else {
                valid_input = true;
            }
            
        }
    }
    while (valid_input == false);
    return id;
}

bool task_cmp(Task T1, Task T2) {


    if (T1.date.total != T2.date.total) {
        return false;
    }

    if (strcmp(T1.name, T2.name) != 0) {
        return false;
    }


    if (T1.id != T2.id) {
        return false;
    }

    if (T1.urgent != T2.urgent) {
        return false;
    }

    return true;
}

bool task_list_cmp(Task_List A1, Task_List A2) {
    if (A1.size != A2.size) {
        return false;
    }

    for (size_t i = 0; i < A1.size; i++) {
        if (task_cmp(A1.data[i], A2.data[i]) == false) {
            return false;
        }
    }
    return true;
}

bool is_sorted_urgent(void) {
    bool encountered_non_urgent = false;
    for (size_t i = 0; i < tasks.size; i++) {
        if (tasks.data[i].urgent == false) {
            encountered_non_urgent = true;
        }

        if (tasks.data[i].urgent == true && encountered_non_urgent == true) {
            return false;
        }
    }

    return true;
}

bool is_sorted_date(void) {
    for (size_t i = 1; i < tasks.size; i++) {
        if (tasks.data[i].date.total < tasks.data[i - 1].date.total) {
            return false;
        }
    }
    return true;
}
bool sort_tasks_urgent(void) {
    if (is_sorted_urgent()) {
        printf("Task list is already sorted!\n");
        return false;
    }


    Task urgent_tasks[tasks.size];
    Task non_urgent_tasks[tasks.size];
    
    size_t urgent_tasks_n = 0;
    size_t non_urgent_tasks_n = 0;

    size_t j = 0;
    size_t k = 0;


    for (size_t i = 0; i < tasks.size; i++) {
        if (tasks.data[i].urgent) {
            urgent_tasks[urgent_tasks_n++] = tasks.data[i];
        }
        else {
            non_urgent_tasks[non_urgent_tasks_n++] = tasks.data[i];
        }
    }
    
    for (size_t i = 0; i < urgent_tasks_n; i++) {
        tasks.data[i] = urgent_tasks[j++];
    }

    for (size_t i = urgent_tasks_n; i < tasks.size; i++) {
        tasks.data[i] = non_urgent_tasks[k++];
    }

    recalibrate_ids();

    printf("Successfully sorted tasks!\n");

    return true;
}

bool sort_tasks_date(void) {
    if (is_sorted_date() == false) {
        if ( merge_sort(0, tasks.size - 1)) {
            recalibrate_ids();
            printf("Successfully sorted tasks by date!\n");
            return true;
        }
        else {
            printf("Sorting by date failed!\n");
            return false;
        }
    }
    printf("Tasks already sorted by date!\n");
    return false;    

} 

void recalibrate_ids(void) {
    for (size_t i = 0; i < tasks.size; i++) {
        tasks.data[i].id = i + 1;;
    }
}

bool free_task_memory(Task task) {
    if (task.test == false) {
        free(task.name);
        free(task.date.date_string);
    }
    return true;
}

bool free_task_list_memory(void) {
    for (size_t i = 0; i < tasks.size; i++) {
        free_task_memory(tasks.data[i]);
    }
    free(tasks.data);
    return true;
}

bool merge_sort(size_t p, size_t r) {
    if (r > p) {
        size_t q = (p + r) / 2;
        merge_sort(p, q);
        merge_sort(q + 1, r);
        if (!merge(p, q, r)) {
            return false;
        }
    } 
    return true;
}


bool merge(size_t p, size_t q, size_t r) {
    size_t n1 = q - p + 1; //size of left subarray
    size_t n2 = r - q; //size of right subarray

    Task_List L = {
        .data = malloc(n1 * sizeof *L.data),
        .capacity = n1,
        .size = 0
    };
    
    Task_List R = {
        .data = malloc(n2 * sizeof *R.data),
        .capacity = n2,
        .size = 0
    };

   if (L.data == NULL || R.data == NULL) {
        free(L.data);
        free(R.data);
        printf("Failed to allocate memory to a subarray(s\n)");
        return false;
}


    for (size_t i = 0; i < n1; i++) { //fill left sub array 
        L.data[i] = tasks.data[p + i];
        L.size++;
    }


    for (size_t j = 0; j < n2; j++) { //fill right sub array 
        R.data[j] = tasks.data[q + 1 + j];
        R.size++;
    }


    size_t i = 0;
    size_t j = 0;
    size_t k = p;
    
    while (i < n1 && j < n2) {
        if (L.data[i].date.total <= R.data[j].date.total) {
            tasks.data[k] = L.data[i];
            i++;
        }
        else {
            tasks.data[k] = R.data[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        tasks.data[k] = L.data[i];
        i++;
        k++;
    }

    while (j < n2) {
        tasks.data[k] = R.data[j];
        j++;
        k++;
    }
    free(L.data);
    free(R.data);
    return true;
}   
