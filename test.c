#include "todo.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


bool populate_task_list() {
        Date test_date_1 = {
        .day = 1,
        .month = 1,
        .year = 2050,
        .total = (2050 * 10000) + (1 * 100) + 1,
    };

    Date test_date_2 = {
        .day = 22,
        .month = 8,
        .year = 2006,
        .total = (2006 * 10000) + (8 * 100) + 22,
    };
    
    Date test_date_3 = {
        .day = 10,
        .month = 9,
        .year = 2025,
        .total = (2025 * 10000) + (9 * 100) + 10,
    };

    char *name_1 = malloc(max_name_length);
    if (name_1 == NULL) {
            return false;
        }

    snprintf(name_1, max_name_length, "%s", "do homework");
    Task test_task_1 = {
        .id = tasks.size + 1,
        .name = name_1,
        .urgent = false,
        .date = test_date_1,
    };
    if (!add_task_from_json(test_task_1)) {
        free_task_memory(test_task_1);
        return false;
    }
    
        
    
    char *name_2 = malloc(max_name_length);
    if (name_2 == NULL) {
        return false;
        }

    snprintf(name_2, max_name_length, "%s", "study for computing exam");
    Task test_task_2 = {
        .date = test_date_2,
        .id = tasks.size + 1,
        .name = name_2,
        .urgent = true,
    };

    if (!add_task_from_json(test_task_2)) {
        free_task_memory(test_task_2);
        return false;
    }

    char *name_3 = malloc(max_name_length);
    if (name_3 == NULL) {
        return __bool_true_false_are_defined;
    }

    snprintf(name_3, max_name_length, "%s", "get ready to go to club");
    Task test_task_3 = {
        .date = test_date_3,
        .id = tasks.size + 1,
        .name = name_3,
        .urgent = false,
    };
    if (!add_task_from_json(test_task_3)) {
        free_task_memory(test_task_3);
        return false;
    }

    return true;

}

bool stock_add_task(const Task task) {
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
            return false;
        }
    }
    tasks.data[tasks.size++] = task;

    return true;
}

bool stock_remove_task(const long id) {
    if (tasks.size == 0) {
        return false;
    }

    else if (id > (long)tasks.size) {
        printf("This task does not exist!\n");
        return false;
    }

    else if (id <= 0) {
        printf("Please enter a valid id\n");
        return false;
    }
    
    size_t index = (size_t)(id - 1);
    Task task = tasks.data[index];
    for (size_t i = index; i + 1< tasks.size; i++) {
        tasks.data[i] = tasks.data[i + 1];
        tasks.data[i].id -= 1;
    }

    tasks.size--;
    

    if (tasks.size <= tasks.capacity / 4) {
        size_t new_capacity = tasks.capacity / 2;
        if (new_capacity == 0) {
            new_capacity = 1;
        }
        Task *temp = realloc(tasks.data, new_capacity * sizeof(Task));

        if (temp != NULL) {
            tasks.data = temp;
            tasks.capacity = new_capacity;
        }
        else {
            printf("%s", failed_malloc);
        }
    }

    free_task_memory(task);

    return true;
}

bool stock_edit_task(const long id, const char *edit) {

    if (tasks.size == 0) {
        printf("You have no tasks!\n");
        return false;
    }

    else if (id > (long)tasks.size) {
        printf("This task does not exist!\n");
        return false;
    }

    else if (id <= 0) {
        printf("Please enter a valid id\n");
        return false;
    }
    
    size_t index = (size_t)(id - 1);
    Task *task = &tasks.data[index];

    bool old_urgent = task -> urgent;
    char *old_name = malloc(max_name_length);
    if (old_name == NULL) {
        printf("%s", failed_malloc);
        return false;
    }
    snprintf(old_name, max_name_length, "%s", task -> name);


    bool new_urgent = true;
    char *new_name = malloc(max_name_length);
    
    

    if (new_name == NULL) {
        free(old_name);
        printf("%s", failed_malloc);
        return false;
    }

    snprintf(new_name, max_name_length, "%s", edit);
    

    if (strcmp(old_name, new_name) == 0 && old_urgent == new_urgent) {
        printf("New task is the same as old task!\n");
        free(new_name);
        free(old_name);
        return false;
    }
    free(task -> name);
    task -> name = new_name;
    task -> urgent = new_urgent;

    free(old_name);

    
    printf("Successfully edited task!\n");
    return true;
}

void stock_remove_all_tasks(void) {
    while (tasks.size > 0) {
        stock_remove_task(1);
    }
}
bool test_add_task(void) {
    const size_t operations = 10000;

    Date date = {
        .day = 1,
        .month = 1,
        .year = 2000,
        .total = (2000 * 10000) + (1 * 100) + 1
    };

    Task task;

    for (size_t i = 0; i < operations; i++) {
        task.id = tasks.size + 1;
        task.name = malloc(max_name_length);
        task.urgent = false;
        task.date = date;
        if (task.name == NULL) {
            return false;
        }
        
        snprintf(task.name, max_name_length, "Test Task");
        
        if (!stock_add_task(task)) {
            return false;
        }
    }

    for (size_t i = 0; i < operations; i++) {
        task = tasks.data[i];
        if (task.id != i + 1 ||
            strcmp(task.name, "Test Task") != 0 ||
            task.urgent != false ||
            task.date.total != date.total
            ) {
                return false;
            }

    }

    if (tasks.size != operations) {
        return false;
    }

    printf("Successfully added %zu tasks.\n", operations);
    stock_remove_all_tasks();
    return true;
}

bool test_remove_task(void) {
    const size_t operations = 10000;
        Date date = {
        .day = 1,
        .month = 1,
        .year = 2000,
        .total = (2000 * 10000) + (1 * 100) + 1
    };

    Task task;

    for (size_t i = 0; i < operations; i++) {
        task.id = tasks.size + 1;
        task.name = malloc(max_name_length);
        task.urgent = false;
        task.date = date;
        if (task.name == NULL) {
            return false;
        }
        
        snprintf(task.name, max_name_length, "Test Task");
        
        if (!stock_add_task(task)) {
            return false;
        }
    }

    for (size_t i = 0; i < operations; i++) {
        task = tasks.data[i];
        if (task.id != i + 1 ||
            strcmp(task.name, "Test Task") != 0 ||
            task.urgent != false ||
            task.date.total != date.total
            ) {
                return false;
            }

    }


    for (size_t i = 0; i < operations - 1; i++) {
        if (!stock_remove_task(1)) {
            return false;
        }
    }

    if (tasks.size != 1) {
        fprintf(stderr, "Removed: %zu tasks\nExpected: %zu tasks\n", operations - tasks.size, operations - tasks.size);
        return false;
    }

    if (tasks.data[0].id != 1) {
        fprintf(stderr, "Failed to recalibrate ids.\n");
        return false;
    }

    if (!stock_remove_task(1) || tasks.size != 0) {
        return false;
    }
    printf("Successfully removed %zu tasks.\n", operations);
    return true;
}

bool test_urgency_sort(void) {
     if (!populate_task_list()) {
        return false;
     }

     if (!sort_tasks_urgent()) {
        return false;
     }

    if (!is_sorted_urgent()) {
        fprintf(stderr, "Tasks are not sorted by urgency.\n");
        return false;
    }

    stock_remove_all_tasks();
    return true;
}

bool test_date_sort(void) {
    if (!populate_task_list()) {
        return false;
    }

    if (!sort_tasks_date()) {   
        return false;
    }

    if (!is_sorted_date()) {
        fprintf(stderr, "Tasks are not sorted by date.\n");
        return false;
    }

    stock_remove_all_tasks();
    return true;
}

bool test_edit(void) {
    char edit[] = "Edited Task Name";
    size_t index = tasks.size;

    Date date = {
        .day = 1,
        .month = 1,
        .year = 2000
    };

    Task task = {
        .date = date,
        .id = index + 1,
        .name = malloc(max_name_length),
        .urgent = false
    };

    if (task.name == NULL) {
        return false;
    }

    snprintf(task.name, max_name_length, "Test Name");

    if (!add_task_from_json(task)) {
        free(task.name);
        fprintf(stderr, "Failed to add task.\n");
        return false;
    }

    if (!stock_edit_task((long)(index + 1), edit)) {
        fprintf(stderr, "Edit task name failed.\n");
        return false;
    }
    
    Task *edited = &tasks.data[index];

    if (strcmp(edited -> name, edit) != 0 || edited -> urgent != true) {
        fprintf(stderr, "Expected task name: %s\nActual task name: %s\nExpected urgency: %d\nActual urgency: %d\n", edit, edited -> name, true, edited -> urgent);
        return false;
    }

    stock_remove_all_tasks();

    return true;


}

bool test_all(void) {
    if (!test_add_task() ||
        !test_remove_task() ||
        !test_edit() ||
        !test_date_sort() ||
        !test_urgency_sort())
        {
            fprintf(stderr, "Did not pass all tests.\n");
            return false;
        }
        printf("All tests passed.\n");
        return true;
}