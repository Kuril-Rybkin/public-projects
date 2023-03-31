#include <stdio.h>
#include <stdlib.h>

/*analyseInput validates input and returns an easy to manage output value: 1 to continue with the while loop and 0 to stop with the while loop.
It also prints out the error messages to keep them separate from the main function code and logic. It also detects EOF.*/
int analyseInput(char * command, int * n1, int * n2, int * counter) {

    scanf(" %c", command);

    if (feof(stdin)) {  //Check if input is empty. If it is, program should cleanly exit.
        return 0;   //Returning 0 skips the while loop and exits with code 0.
    }

    switch (*command) {
        case '+':
            (*counter)++; //Counter keeps track of how many user IDs have been added so far.
            if (scanf(" %d", n1) != 1 || *n1 < 0 || *n1 > 99999 || (*counter > 1000000)) { //Input validation according to task requirements
                printf("Invalid input.\n"); //print error message
                return 0;   //skip while loop
            }
            else {
                return 1;   //proceed with while loop
            }
            break;
        
        case '?':
            if (scanf(" %d %d", n1, n2) != 2 || *n1 < 0 || *n2 > 1000000 || (*n1 > *n2) || (*n2 >= *counter)) { //Input validation according to task requirements
                printf("Invalid input.\n");
                return 0;   //skip while loop
            }
            else {
                return 1;   //proceed with while loop
            }
            break;
        
        default:
            printf("Invalid input.\n"); //if different character was entered, or no character at all
            return 0;   //skip while loop
    }
}

int main(void) {
    static int histogram[100000][3] = {0};
    //histogram stores arrays indexed by user ID which contain total number of visits and a number which states uniqueness.

    static int log[1000001] = {0};
    //log stores user IDs by order of access.

    int n1 = 0, n2 = 0, j = 0, counter = 0, unique = 0, total = 0;
    char command;

    printf("Requests:\n");

    int flag = analyseInput(&command, &n1, &n2, &counter);  //flag is return value of analyseInput. If flag is true (1), then proceed with while loop.
    while (flag) {
        switch (command) {
            case '+':
                histogram[n1][0]++; //Increment total number of visits by user ID n1
                if (histogram[n1][0] == 1) {    //check if first visit
                    printf("> first visit\n");
               }
               else {
                    printf("> visit #%d\n", histogram[n1][0]);
                }
                log[counter - 1] = n1;  //Add user ID to the log of user logons. Counter is used, but since counter starts at 1, it needs to be decreased to match array indexing.
                break;

            case '?':
                for (j = n1; j <= n2; j++) {    //variable j loops in the range from n1 to n2 (from and to) inclusively
                    if (histogram[(log[j])][1] == 0) {
                        //Histogram[x][1] stores a value which determines the uniqueness of an ID denoted by x. if this value is 0, the ID is considered to be unique in the specified range
                        unique++;
                        total++;
                        histogram[(log[j])][1] = 1; //Changes the uniqueness value to make sure that the ID isnt considered unique again.
                    }
                    else {  //If ID Is not unique
                        total++;
                    }
                }
                for (j = n1; j <= n2; j++) {    //sets the uniqueness values of all IDs searched to 0 again ready for next search.
                    histogram[(log[j])][1] = 0;
                }
                printf("> %d / %d\n", unique, total);
                unique = 0; total = 0;  //Resets these values ready for next use again.
                break;
        }
        flag = analyseInput(&command, &n1, &n2, &counter);  //Sets flag value which determines whether to continue with the while loop or not.
    }
    return 0;
}
