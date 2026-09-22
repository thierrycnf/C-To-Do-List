#include "todo.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include "cJSON-1.7.19/cJSON.h"
#include <errno.h>
#ifdef _WIN32
#include <windows.h> //only includes this if compiling on windows
#endif

const size_t max_name_length = 100;
const char failed_malloc[] = "Failed to allocate memory\n";
const char failed_save[] = "Failed to save task list as JSON\n";

Task_List tasks = {
    .data = NULL,
    .size = 0,
    .capacity = 1
};

bool initialise_task_list(void) {
    if (tasks.data != NULL) {
        return false; //this function has already been called
    }

    tasks.data = malloc(tasks.capacity * sizeof(Task));
    if (tasks.data == NULL) {
        return false;
    }

    return true;
}
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
            printf("%s", failed_malloc);
            return false;
        }
    }
    tasks.data[tasks.size++] = task;
    
    if (!save_task_list()) {
        printf("%s", failed_save);
        remove_task_from_json(tasks.size);
        return false;
    }
    printf("Task created successfully!\n");

    return true;
}

bool add_task_from_json(const Task task) {
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
            printf("%s", failed_malloc);
            return false;
        }
    }
    tasks.data[tasks.size++] = task;
    return true;
}

bool remove_task_from_json(const long id) {
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
        Task *temp = realloc(tasks.data, new_capacity * sizeof(Task)
            
    );

        if (temp != NULL) {
            tasks.data = temp;
            tasks.capacity = new_capacity;
        }
        else {
            printf("%s", failed_malloc);
        }
    }
    return true;
    }

void remove_all_tasks(void) {
    while (tasks.size > 0) {
        remove_task_from_json(1);
    }
}

void add_test_tasks(void) {
    const size_t name_1_n = 20;
    const size_t name_2_n = 30;
    const size_t name_3_n = 30;
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

    char *name_1 = malloc(name_1_n);
    if (name_1 == NULL) {
            return;
        }

    snprintf(name_1, name_1_n, "%s", "do homework");
    Task test_task_1 = {
        .id = tasks.size + 1,
        .name = name_1,
        .urgent = false,
        .date = test_date_1,
    };
    if (!add_task(test_task_1)) {
        free_task_memory(test_task_1);
        return;
    }
    
        
    
    char *name_2 = malloc(name_2_n);
    if (name_2 == NULL) {
        return;
        }

    snprintf(name_2, name_2_n, "%s", "study for computing exam");
    Task test_task_2 = {
        .date = test_date_2,
        .id = tasks.size + 1,
        .name = name_2,
        .urgent = true,
    };

    if (!add_task(test_task_2)) {
        free_task_memory(test_task_2);
        return;
    }

    char *name_3 = malloc(name_3_n);
    if (name_3 == NULL) {
        return;
    }

    snprintf(name_3, name_3_n, "%s", "get ready to go to club");
    Task test_task_3 = {
        .date = test_date_3,
        .id = tasks.size + 1,
        .name = name_3,
        .urgent = false,
    };
    if (!add_task(test_task_3)) {
        free_task_memory(test_task_3);
        return;
    }

}


void set_date(Task *task) {
    time_t now = time(NULL);
    struct tm *local = localtime(&now);

    if (local == NULL) {
        task -> date.day = 0;
        task -> date.month = 0;
        task -> date.year = 0;
        task -> date.total = 0;
        return;
    }
    
    task -> date.day = (size_t)local->tm_mday;
    task -> date.month = (size_t)(local->tm_mon + 1);
    task -> date.year = (size_t)(local->tm_year + 1900);
    task -> date.total = (task -> date.year * 10000) + (task -> date.month * 100) + task -> date.day;
}


void print_task(Task task) {
    printf("%zu. ", task.id);
    if (task.urgent) {
        printf("URGENT | ");
    }

    char date_string[16];
    snprintf(date_string, sizeof date_string, "%zu/%zu/%zu", task.date.day, task.date.month, task.date.year);

    printf("%s | %s\n", task.name, date_string);
}

void print_all_tasks(void) {
    for (size_t i = 0; i < tasks.size; i++) {
        print_task(tasks.data[i]);
    }
}

void my_to_lower(char *string) {
    for (size_t i = 0; string[i] != '\0'; i++) {
        string[i] = (char)tolower((unsigned char)string[i]);
    }
}
bool get_urgent(void) {
    char buffer[8];
    bool valid_input = false;
    do {
        printf("Is this task urgent? [yes/no]\n");
        if  (fgets(buffer, sizeof buffer, stdin) != NULL) {
            if (strchr(buffer, '\n') == NULL) {
                clear_input_line();
             }
            buffer[strcspn(buffer, "\n")] = '\0';
            my_to_lower(buffer);
        }
        else {
            printf("Unable to read input\n");
            free_task_list_memory();
            exit(EXIT_FAILURE);
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
    char *buffer = malloc(max_name_length * sizeof(*buffer));
     if (buffer == NULL) {
        printf("%s", failed_malloc);
        return NULL;
    }

    printf("Enter task name\n");

    if (fgets(buffer, max_name_length, stdin) == NULL) {
        printf("Unable to read input\n");
        free(buffer);
        return NULL;
    }

    if (strchr(buffer, '\n') == NULL) {
            clear_input_line();
        }
    buffer[strcspn(buffer, "\n")] = '\0';

    return buffer;

    
}

char *get_sort_choice(void) {
    static size_t allocated_space = 10;
    char *buffer = malloc(allocated_space * sizeof(*buffer));
    if (buffer == NULL) {
        printf("%s", failed_malloc);
        return NULL;
    }
    bool valid_input = false;
    do {
        printf("How would you like to sort your tasks? [date/urgency]\n");
        if  (fgets(buffer, allocated_space, stdin) != NULL) {
            if (strchr(buffer, '\n') == NULL) {
                clear_input_line();
             }
            buffer[strcspn(buffer, "\n")] = '\0';
            my_to_lower(buffer);
        }
        else {
            printf("Unable to read input\n");
            free(buffer);
            free_task_list_memory();
            exit(EXIT_FAILURE);
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

bool remove_task(const long id) {
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
    Task task = tasks.data[index];
    for (size_t i = index; i + 1< tasks.size; i++) {
        tasks.data[i] = tasks.data[i + 1];
        tasks.data[i].id -= 1;
    }

    tasks.size--;
    
    if (!save_task_list()) {
        undo_remove_task(task, index);
        printf("%s", failed_save);
        return false;
    }

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
    printf("Successfully removed task!\n");

    return true;
    }

void undo_remove_task(const Task task, const size_t index) {
   for (size_t i = tasks.size; i > index; i--) {
        tasks.data[i] = tasks.data[i - 1];
   }

   tasks.data[index] = task;
   tasks.size++;
   recalibrate_ids(0);
   return;
}

long get_id(void) {
    char buffer[32];
    char *end;
    long id;
    bool valid_input = false;
    
    printf("Enter the task ID\n");
    do {
        if (fgets(buffer, sizeof buffer, stdin) != NULL) {
            if (strchr(buffer, '\n') == NULL) {
                clear_input_line();
             }
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
        else {
            printf("Unable to read input\n");
            free_task_list_memory();
            exit(EXIT_FAILURE);
    }
    }
    while (valid_input == false);
    return id;
}

void clear_input_line(void) {
    int c;

    while ((c = getchar()) != '\n' && c != EOF) {
    }
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
        return true;
    }

    Task *backup_tasks = malloc(tasks.size * sizeof(*backup_tasks));

    if (backup_tasks == NULL) {
        printf("%s", failed_malloc);
        return false;
    }

    memcpy(backup_tasks, tasks.data, tasks.size * sizeof(*backup_tasks));

    Task *sorted_tasks = malloc(tasks.size * sizeof(*sorted_tasks));
    if (sorted_tasks == NULL) {
        printf("%s", failed_malloc);
        free(backup_tasks);
        return false;
    }
    size_t n = 0;

    for (size_t i = 0; i < tasks.size; i++) {
        if (tasks.data[i].urgent) {
            sorted_tasks[n++] = tasks.data[i];
        }
    }

    for (size_t i = 0; i < tasks.size; i++) {
        if (!(tasks.data[i].urgent)) {
            sorted_tasks[n++] = tasks.data[i];
        }
    }
    
   for (size_t i = 0; i < tasks.size; i++) {
        tasks.data[i] = sorted_tasks[i];
   }

    recalibrate_ids(0);
    free(sorted_tasks);
    
    if (is_sorted_urgent()) {
        if (save_task_list()) {
            printf("Successfully sorted tasks by urgency!\n");
            free(backup_tasks);
            return true;
        }
        printf("Failed to save task list as JSON\n");
        memcpy(tasks.data, backup_tasks, tasks.size * sizeof(*backup_tasks));
        free(backup_tasks);
        return false;
    }
    
    free(backup_tasks);
    printf("Unsuccessfully sorted tasks by urgency!\n");
    return false;
}

bool sort_tasks_date(void) {
    if (!is_sorted_date()) {
        Task *backup_tasks = malloc(tasks.size * sizeof(*backup_tasks));
        if (backup_tasks == NULL) {
            printf("%s", failed_malloc);
            return false;
        }

        memcpy(backup_tasks, tasks.data, tasks.size * sizeof(*backup_tasks));
        if ( merge_sort(0, tasks.size - 1)) {
            recalibrate_ids(0);
            if (!save_task_list()) {
                memcpy(tasks.data, backup_tasks, tasks.size * sizeof(*backup_tasks));
                free(backup_tasks);
                printf("%s", failed_save);
                return false;
            }
            printf("Successfully sorted tasks by date!\n");
            free(backup_tasks);
            return true;
        }
        else {
            memcpy(tasks.data, backup_tasks, tasks.size * sizeof(*backup_tasks));
            free(backup_tasks);
            printf("Sorting by date failed!\n");
            return false;
        }
    }
    printf("Tasks already sorted by date!\n");
    return true;    

} 

void recalibrate_ids(size_t index) {
    for (size_t i = index; i < tasks.size; i++) {
        tasks.data[i].id = i + 1;
    }
}

bool edit_task(const long id) {

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


    bool new_urgent = get_urgent();
    char *new_name = get_name();
    
    

    if (new_name == NULL) {
        free(old_name);
        printf("%s", failed_malloc);
        return false;
    }

    

    if (strcmp(old_name, new_name) == 0 && old_urgent == new_urgent) {
        printf("New task is the same as old task!\n");
        free(new_name);
        free(old_name);
        return false;
    }
    free(task -> name);
    task -> name = new_name;
    task -> urgent = new_urgent;
    if (!save_task_list()) {
        task -> urgent = old_urgent;
        task -> name = old_name;

        free(new_name);

        printf("%s", failed_save);
        return false;
    }

    free(old_name);

    
    printf("Successfully edited task!\n");
    return true;
}

bool free_task_memory(Task task) {
    free(task.name);
    return true;
}

bool free_task_list_memory(void) {
    for (size_t i = 0; i < tasks.size; i++) {
        free_task_memory(tasks.data[i]);
    }
    free(tasks.data);
    tasks.data = NULL;
    tasks.size = 0;
    tasks.capacity = 1;
    return true;
}

bool merge_sort(size_t p, size_t r) {
    if (r > p) {
        size_t q = (p + r) / 2;
       if (!merge_sort(p, q)) {
        return false;
       }

       if (!merge_sort(q + 1, r)) {
        return false;
       }
       
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
        printf("%s", failed_malloc);
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

cJSON *task_to_json(const Task *task) {
    cJSON *json_task = cJSON_CreateObject();

    if (json_task == NULL) {
        return NULL;
    }

    if (cJSON_AddNumberToObject(json_task, "id", task->id) == NULL ||
        cJSON_AddStringToObject(json_task, "name", task->name) == NULL ||
        cJSON_AddBoolToObject(json_task, "urgent", task->urgent) == NULL ||
        cJSON_AddNumberToObject(json_task, "day", task->date.day) == NULL ||
        cJSON_AddNumberToObject(json_task, "month", task->date.month) == NULL ||
        cJSON_AddNumberToObject(json_task, "year", task->date.year) == NULL) {

        cJSON_Delete(json_task);
        return NULL;
    }

    return json_task;
}

cJSON *task_list_to_json(void) {
    cJSON *root = cJSON_CreateObject();

    if (root == NULL) {
        return NULL;
    }

    cJSON *json_tasks = cJSON_AddArrayToObject(root, "tasks");

    if (json_tasks == NULL) {
        cJSON_Delete(root);
        return NULL;
    }

    for (size_t i = 0; i < tasks.size; i++) {

        cJSON *json_task = task_to_json(&tasks.data[i]);

        if (json_task == NULL) {
            cJSON_Delete(root);
            return NULL;
        }

        cJSON_AddItemToArray(json_tasks, json_task);
    }


    return root;
}
bool save_task_list(void) {
    cJSON *json = task_list_to_json();

    if (json == NULL) {
        printf("Failed to create JSON.\n");
        return false;
    }

     char *json_string = cJSON_Print(json);

    if (json_string == NULL) {
        cJSON_Delete(json);
        printf("Failed to convert JSON to string.\n");
        return false;
    }

    
    char temp_name[] = "tasks.tmp";
    char file_name[] = "tasks.txt";
    FILE *f = fopen(temp_name, "w");

    if (f == NULL) {
        free(json_string);
        cJSON_Delete(json);
        printf("Failed to open %s\n", temp_name);
        return false;
    }

    if (fputs(json_string, f) == EOF) {
        fclose(f);
        remove(temp_name);
        free(json_string);
        cJSON_Delete(json);
        printf("Failed to write to file.\n");
        
        return false;
    }   
   
    if (fclose(f) == EOF) {
        remove(temp_name);
        free(json_string);
        cJSON_Delete(json);
        printf("Failed to close file.\n");
        return false;
    }       
    
    
    if (!replace_file(temp_name, file_name)) {
        free(json_string);
        cJSON_Delete(json);
        remove(temp_name);

        printf("Failed to replace %s\n", file_name);

        return false;
    }

    free(json_string);
    cJSON_Delete(json);

    return true;
} 

bool json_to_task() {
    char file_name[] = "tasks.txt";
    char corrupt_name[] = "tasks.txt.corrupt";
    char *text = read_file(file_name);

    if (text == NULL) {
        if (!create_tasks_txt()) {
            printf("Could not create %s\n", file_name);
            return false;
        }

        text = read_file(file_name);

        if (text == NULL) {
            printf("Could not read newly created %s\n", file_name);
            return false;
        }

    }
    cJSON *json = cJSON_Parse(text);

    if (json == NULL) {
        free(text);
        if (remove(corrupt_name) != 0 && errno != ENOENT) {
            return false;
        }
        
        if (rename(file_name, corrupt_name) != 0) {
            return false;
        }
        if (!create_tasks_txt()) {
            return false;
        }
        
        printf("Failed to parse tasks file, creating new tasks file\n");
        return json_to_task();
    }

    cJSON *task_list_json = cJSON_GetObjectItemCaseSensitive(json, "tasks");

    if (!cJSON_IsArray(task_list_json)) {
        cJSON_Delete(json);
        free(text);   

        if (remove(corrupt_name) != 0 && errno != ENOENT) {
            return false;
        }
        
        if (rename(file_name, corrupt_name) != 0) {
            return false;
        }
        if (!create_tasks_txt()) {
            return false;
        }

         printf("JSON root is not a task list, creating new tasks file\n");
        return json_to_task();
    }


    int task_count = cJSON_GetArraySize(task_list_json);
    for (int i = 0; i < task_count; i++) {
        cJSON *task_json = cJSON_GetArrayItem(task_list_json, i);

        if (!cJSON_IsObject(task_json)) {
            remove_all_tasks();
            cJSON_Delete(json);
            free(text);

            if (remove(corrupt_name) != 0 && errno != ENOENT) {
                return false;
            }
        
            if (rename(file_name, corrupt_name) != 0) {
                return false;
            }
            if (!create_tasks_txt()) {
                return false;
            }

            printf("Invalid task in JSON\n");

            return json_to_task();
        }

        cJSON *id = cJSON_GetObjectItemCaseSensitive(task_json, "id");
        cJSON *name = cJSON_GetObjectItemCaseSensitive(task_json, "name");
        cJSON *urgent = cJSON_GetObjectItemCaseSensitive(task_json, "urgent");
        cJSON *day = cJSON_GetObjectItemCaseSensitive(task_json, "day");
        cJSON *month = cJSON_GetObjectItemCaseSensitive(task_json, "month");
        cJSON *year = cJSON_GetObjectItemCaseSensitive(task_json, "year");



        if ((!cJSON_IsNumber(id)) || (!cJSON_IsString(name) || name->valuestring == NULL) || (!cJSON_IsBool(urgent) ||
             !cJSON_IsNumber(day) || !cJSON_IsNumber(month) || !cJSON_IsNumber(year)))  {
            
            remove_all_tasks();
            cJSON_Delete(json);
            free(text);

            if (remove(corrupt_name) != 0 && errno != ENOENT) {
                return false;
            }
        
            if (rename(file_name, corrupt_name) != 0) {
                return false;
            }
            if (!create_tasks_txt()) {
                return false;
            }

            printf("Failed to read JSON\n");
            return json_to_task();
        }


        Date task_date = {
            .day = (size_t)day -> valueint,
            .month = (size_t)month -> valueint,
            .year = (size_t)year -> valueint
        };

        task_date.total = (task_date.year * 10000) + (task_date.month * 100) + task_date.day;

        Task task = {
            .name = malloc(max_name_length),
            .id = (size_t)id -> valueint,
            .urgent = cJSON_IsTrue(urgent),
            .date = task_date
        };

        if (task.name == NULL) {
            printf("%s", failed_malloc);
            remove_all_tasks();
            cJSON_Delete(json);
            free(text);
            return false;
        }
        snprintf(task.name, max_name_length, "%s", name -> valuestring);
        if (!add_task_from_json(task)) {
            free(task.name);
            cJSON_Delete(json);
            free(text);
            remove_all_tasks();
            printf("Failed to add tasks\n");
            return false;
        }
    }
    cJSON_Delete(json);
    free(text);
    return true;
}

    


char *read_file(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
        return NULL;

    if (fseek(file, 0, SEEK_END) != 0) {
        fclose(file);
        return NULL;
    }   
    long size = ftell(file);

    if (size < 0) {
        fclose(file);
        return NULL;
    }

    if (fseek(file, 0, SEEK_SET) != 0) {
        fclose(file);
        return NULL;
    }

    char *text = malloc((size_t)size + 1);
    if (text == NULL) {
        fclose(file);
        return NULL;
    }

    size_t bytes_read = fread(text, 1, (size_t)size, file);

    if (bytes_read != (size_t)size) {
        free(text);
        fclose(file);
        return NULL;
    }


    text[size] = '\0';

    fclose(file);
    return text;
}

long get_file_size(const char filename[]) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        return -1;
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        fclose(file);
        return -1;
    }

    long size = ftell(file);
    fclose(file);

    return size;
}

bool replace_file(const char *temp_name, const char *file_name) {
    #ifdef _WIN32 //this code only runs when compiling on windows
        return MoveFileExA(
            temp_name, 
            file_name, 
            MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH
        ) != 0;
    #else   //this code only runs when compiling on non-windows
        return rename(temp_name, file_name) == 0; 
    #endif
    }

bool create_tasks_txt(void) {
    const char file_name[] = "tasks.txt";
    FILE *file = fopen(file_name, "w");
    if (file == NULL) {
        return false;
    }


    if (fputs("{\"tasks\":[]}", file) == EOF) {
        fclose(file);
        printf("Failed to write to %s\n", file_name);
        return false;
    }

    if (fclose(file) == EOF) {
        printf("Failed to close %s\n", file_name);
        return false;
    }

    return true;



}   