A simple terminal based to do list software that allows users to:
  1. Create and delete tasks
  2. Sort tasks based on user-determined urgency or the date of the task
  3. View all the tasks that have been created
  4. Edit pre-existing tasks
  5. Tasks are saved in a JSON file

How to compile:
  1. Extract the ZIP file into a folder
  2. Open your desired IDE and open the new folder as a directory.  Run the program from the project directory because tasks.txt is stored there.
  3. Copy and paste the following command into the terminal:
    	1. (Windows PowerShell): gcc -std=c17 -Wall -Wextra -Wpedantic -Wshadow -g -O0 main.c todo.c test.c cJSON-1.7.19/cJSON.c -o main.exe; if ($LASTEXITCODE -eq 0) { .\main.exe }
		2. (Linux/macOS): gcc -std=c17 -Wall -Wextra -Wpedantic -Wshadow -g -O0 main.c todo.c test.c cJSON-1.7.19/cJSON.c -o main && ./main
  4. Press enter, causing the program to compile and run 
  5. This also creates an executable so you do not have to run the command every time
