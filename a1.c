#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/types.h>

#define MAX_LINE 80 /* The maximum length of command */
#define MAX_LENGTH 20 /* The maximum length of history */

int main(void) {
    char *args[MAX_LINE / 2 + 1]; /* command line arguments */
    int should_run = 1;  /* flag to determine when to exit program */
    int run_concur = 0;
    char input[MAX_LENGTH]; /* user input */
    int num;  /* numbers of token in args */
    char *history[20]; /* array to store history commands */
    int count; /* numbers of command that user entered */

    while (should_run) {
        printf("CSCI3120>");
        fflush(stdout); /* display the user input */

        /* After reading user input, the steps are
         * (1) fork a child process using fork()
         * (2) the child process will invoke execvp()
         * (3) if command included &, parent will not invoke wait()
         */

        scanf("%[^\n]%*c", input);

        //if enter 'exit'
        if(strcmp(input, "exit") == 0){
            should_run = 0;
            printf("Exit Program.\n");
            exit(1);
        }

        /* In this part, we need to do some error checking
        * if the user input is not !!, !# and history, we
        * still need to add to the history buffer.     */
        if(input[0] != '!' && strcmp(input,"history") != 0 ){
            history[count] = strdup(input);
            count++;
        }

        //if user need to check the history table
        if(strcmp(input,"history") == 0) {
            if (count == 0) { //table is empty
                printf("History Table is empty.\n");
            } else {
                for (int k = 0; k<count && k <10 ; k++) {
                    printf("ID: %d\t  PID: %d\t Command: %s\n", k + 1, getpid(), history[count-k-1]);
                }
            }
        }

        //if input is '!!'
        if(strcmp(input, "!!") ==0){
            if(count == 0){
                printf("No commands in history.\n");
                //terminate the program
            }
            else{ //call the most recent command (which is located in the last of history)
                strcpy(input, history[count-1]); /* load the last index of history as a new input */
                history[count] = strdup(input);
                count++;
            }
        }

        //if input is '!#'
        if(input[0] == '!' && isdigit(input[1]) != 0 ){
            int value = input[1] -'0';
            if(value > 10 || value == 0 || count < value ){
                printf("No such command in history.\n");
            } else{
                strcpy(input, history[count-value]); /* call the command related to the index given*/
                history[count] = strdup(input);
                count++;
            }

        }

        /* parsing the command line and store into array */
        //referening from: https://stackoverflow.com/questions/15472299/split-string-into-tokens-and-save-them-in-an-array
        num = 0;
        args[num] = strtok(input," ");
        while (args[num] != NULL) {
            num++;
            args[num] = strtok(NULL, " ");
        }

        //Run concurrently (end with '&' )
        if (strcmp(args[num - 1], "&") == 0) {
            run_concur = 1;
            args[num-1] = NULL;
            num--;
        }

        if ( num>0 ){
            pid_t pid = fork();
            // child process
            if(pid == 0){
                execvp(args[0],args);
                //printf("Execution Failed.\n");
                exit(1);
            }
            //parent process
            else{
                if(run_concur == 0){
                    while(wait(NULL) != pid);
                } else{
                    run_concur = 0 ;
                }
            }
        }
        }
        return 0 ;
}
