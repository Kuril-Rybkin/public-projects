#include <stdio.h>
#include <stdlib.h> //used for quicksort function
#include <string.h> //used for strcomp() and strcpy()

typedef struct {
    int sold;
    char name[100]; //Max item name is 99 characters as specified in progtest
} TPRODUCT;

int analyseInput(char name[100]) {  //Validates input. if valid returns a number 1,2,3 based on input or 0 if input is invalid.
    char cmd;
    char newLine;

    scanf(" %c", &cmd);

    if (feof(stdin)) {
        return 0;
    }

    switch(cmd) {
        case '+':
            if(scanf(" %99s%c", name, &newLine) != 2 || newLine != '\n') {
                printf("Invalid input.\n");
                return 0;
            }
            else {
                return 1;
            }
            break;
        case '#':
            return 2;
        case '?':
            return 3;
            break;
        default:
            printf("Invalid input.\n");
            return 0;
            break;
    }
}

int nameSearch(TPRODUCT arr[], char name[100], int n) { //searches for a matching string in an array of TPRODUCT. Returns position if found, -1 if no element found.
    for (int i = 0; i <= n; i++) {
        if (strncmp(arr[i].name, name, 100) == 0) {
            return i;
        }
    }
    return -1;
}

int cmpInt(const void * a, const void * b) {    //This function is used by the qsort() function from stdlib
    const TPRODUCT * arg1 = (const TPRODUCT*)a;
    const TPRODUCT * arg2 = (const TPRODUCT*)b;

    if (arg1->sold < arg2->sold) return 1;
    if (arg1->sold > arg2->sold) return -1;
    
    return 0;
}

void enumerate (TPRODUCT arr[], int top, int size, int mode) {
    int i, j, k, count = 0;
    /*indexes [k..j] inclusive have the same number of sold items.
    This function has an input parameter "mode". This is used so that this function can be reused.
    If mode is 1, then the rankings are printed out. if mode is 0, then only the total number is printed out.*/
    for (i = 0; i < top; i++) {

        if (i > size) {    //Check if going outside the range of the array to avoid segmentation fault.
            break;
        }

        if (i + 1 <= size && arr[i].sold == arr[i + 1].sold) {   //Check if next item has the same amount of sales as the current item
            j = i;
            while ((j + 1 <= size) && (arr[j].sold == arr[j + 1].sold)) {
                j++;    //find the index of the last entry in the array which has a matching number of sales
            }

            for (k = i; k <= j; k++) {
                /*k is set to i. k is now the lower bound of the range of identical sales amounts, and j is the upper bound.
                A third variable is needed here because we need to conserve the starting index in order to print it*/
                if (mode == 1) printf("%d.-%d. %s, %dx\n", i + 1, j + 1, arr[k].name, arr[k].sold);
                count += arr[k].sold;
            }
            i = k - 1;  //Set i to k to skip the rest of the range of identical sales amounts
        }
        else {
            if (mode == 1) printf("%d. %s, %dx\n", i + 1, arr[i].name, arr[i].sold);
            count += arr[i].sold;
        }
    }
    printf("Top sellers: sold %d items\n", count);
}

int main(void) {
    int size = 10;
    int n = 0;  //Counter of how many TPRODUCT structures are in the array
    int top;
    int userInput;
    char name[100]; //Stores the name of the product, later to be used in nameSearch() and to be written into the TPRODUCT structure.
    int position;
    int sorted = 0;
    TPRODUCT * sales = (TPRODUCT *)malloc(size * sizeof(TPRODUCT)); //allocates enough memory for an array of length 10

    printf("Top N:\n"); //validates input of top
    if (scanf(" %d", &top) != 1 || top < 1) {
        if (feof(stdin)) {
            free(sales);
            return 0;
        }

        printf("Invalid input.\n");
        free(sales);
        return 0;
    }

    printf("Requests:\n");
    userInput = analyseInput(name); //Validates user inputs and returns a number, 0 if invalid, 1 if its a + command, 2 if its a # command, 3 if its a ? command

    while (userInput) { //since analyseInput returns non-zero values if the input is valid, they are evaluated to true.
        switch (userInput) {
            case 1: //analyseInput returns 1 if the command is a +
                position = nameSearch(sales, name, n - 1);

                if (position < 0) { //no matching element was found so a new entry must be made
                    if (n == size) {    //since a new entry is being made in the array, it needs to be checked if the array is at max capacity and reallocate memory accordingly..
                        size *= 2;
                        sales = (TPRODUCT *)realloc(sales, size * sizeof(TPRODUCT));
                    }
                    strncpy(sales[n].name, name, 100);    //copies the string stored in variable name into new structure in the sales array
                    sales[n].sold = 1;   //Increments the amount sold for this item, since the default is 0
                    n++;    //Increments the number of elements stored in the array.
                            //Since this new product is added at the end, the array is still sorted so there is no need
                            //to set sorted = 0
                }
                else {
                    sales[position].sold ++;    //position was found, so nameSearch returned a position. this position has it's sold variable incremented.
                    sorted = 0; //The amount of products sold was changed, so the array may be out of order.
                }
                break;

            case 2:
                if (sorted == 0) {  //if array is unsorted, sort it.
                    qsort(sales, n, sizeof(TPRODUCT), cmpInt);
                    sorted = 1;
                }

                enumerate(sales, top, n - 1, 1);
                //Call enumerate function in mode 1, where rankings are also printed
                break;
            
            case 3:
                if (sorted == 0) {
                    qsort(sales, n, sizeof(TPRODUCT), cmpInt);
                    sorted = 1;
                }

                enumerate(sales, top, n - 1, 0);
                //Call enumerate() function in mode 0, where the rankings arent printed.
                break;
        }
        userInput = analyseInput(name);
    }

    free(sales);

    return 0;
}
