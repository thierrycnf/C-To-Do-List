#include "todo.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int main(void) {
    if (!initialise_task_list()) {
        return EXIT_FAILURE;
    }

    add_test_tasks();
    

    char buffer[3];
    Task task;


    do {
        start:

            printf("What would you like to do\n");
            printf("A. Create a task\n");
            printf("B. View your tasks\n");
            printf("C. Delete a task\n");
            printf("D. Sort tasks\n");
            printf("E. End program\n");
            
        if (fgets(buffer, sizeof buffer, stdin) != NULL) {
            if (strchr(buffer, '\n') == NULL) {
                clear_input_line();
             }
            buffer[strcspn(buffer, "\n")] = '\0';
            my_to_lower(buffer);
            if (strcmp(buffer, "a") == 0) {
                task.urgent = get_urgent();
                task.name = get_name();
                if (task.name == NULL) {
                    goto start;
                }
                
                set_date(&task);
                task.id =  tasks.size + 1;
                
                if (!add_task(task)) {
                    free_task_memory(task);
                }
            }
            else if (strcmp(buffer, "b") == 0) {
                if (tasks.size == 0) {
                    printf("You have no tasks!\n");
                }
                else {
                    for (size_t i = 0; i < tasks.size; i++) {
                        print_task(tasks.data[i]);
                    }
            }
            }
            else if (strcmp(buffer, "c") == 0) {
                if (tasks.size == 0) {
                    printf("You have no tasks!\n");
                    continue;
                }
                long delete_id = get_id();
                remove_task(delete_id);
            }
            else if (strcmp(buffer, "d") == 0) {
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
            else if (strcmp(buffer, "e") == 0) {
                break;
        }   
            else{
                printf("Enter a valid letter!\n");
            }


        
    }
    else {
        break;
    }
    printf("\n");
}
    while (true); 
    free_task_list_memory();

    return EXIT_SUCCESS;   
}
    

  


//gcc -std=c17 -Wall -Wextra -Wpedantic -Wshadow -g -O0 main.c todo.c -o main.exe; if ($LASTEXITCODE -eq 0) { .\main.exe }
//compile and run