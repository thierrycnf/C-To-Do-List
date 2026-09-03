#include "todo.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int main(void) {

    tasks.data = malloc(1 * sizeof(Task));
    if (tasks.data == NULL) {
        return EXIT_FAILURE;
    }
    Date test_date = {
        .day = 1,
        .month = 1,
        .year = 2050,
        .total = (2050 * 10000) + (1 * 100) + 1,
    };
    Task test_task = {
        .date = test_date,
        .id = tasks.size + 1,
        .name = "do homework",
        .urgent = false,
        .test = true
    };

    add_task(test_task);
    char buffer[3];
    Task task;

    // Task task1 = {
    //     .id = ++next_task,
    //     .name = "Take out the bin",
    //     .urgent = true,
    //     .date = get_date()
    // };

    do {
        start:

            printf("What would you like to do\n");
            printf("A. Create a task\n");
            printf("B. View your tasks\n");
            printf("C. Delete a task\n");
            printf("D. Sort tasks\n");
            printf("E. End program\n");
            
        if (fgets(buffer, sizeof buffer, stdin) != NULL) {
            buffer[strcspn(buffer, "\n")] = '\0';
            if (strcmp(buffer, "A") == 0) {
                task.urgent = get_urgent();
                task.name = get_name();
                task.test = false;
                if (task.name == NULL) {
                    goto start;
                }
                
                get_date(&task);
                task.id =  tasks.size + 1;
                
                if (!add_task(task)) {
                    free_task_memory(task);
                }
            }
            else if (strcmp(buffer, "B") == 0) {
                if (tasks.size == 0) {
                    printf("You have no tasks!\n");
                }
                else {
                    for (size_t i = 0; i < tasks.size; i++) {
                        print_task(tasks.data[i]);
                    }
            }
            }
            else if (strcmp(buffer, "C") == 0) {
                if (tasks.size == 0) {
                    printf("You have no tasks!\n");
                    continue;
                }
                size_t delete_id = get_id();
                remove_task(delete_id);
            }
            else if (strcmp(buffer, "D") == 0) {
                if (tasks.size == 0) {
                    printf("You have no tasks to sort!\n");
                    continue;
                }
                char *sort_choice = get_sort_choice();

                if (sort_choice == NULL) {
                    continue;
                }
                if (strcmp(sort_choice, "urgency") == 0) {
                    sort_tasks_urgent();
                }
                else if (strcmp(sort_choice, "date") == 0) {
                    sort_tasks_date();
                }
                free(sort_choice);
                
            }
            else if (strcmp(buffer, "E") == 0) {
                break;
        }   
            else{
                printf("Enter a valid letter!\n");
            }


        
    }
    printf("\n");
}
    while (true); 
    free_task_list_memory();

    return 0;   
}
    

  


//gcc -std=c17 -Wall -Wextra -Wpedantic -Wshadow -g -O0 main.c todo.c -o main.exe; if ($LASTEXITCODE -eq 0) { .\main.exe }
//compile and run
