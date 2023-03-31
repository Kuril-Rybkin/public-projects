#include <stdio.h>
#include <math.h>

void decToBase(double n, double b) {
    double power = 0, divisor, counter;

    if (b == 10.0) {
        printf("%0.lf", n);
    }
    else if (n == 0.0) {
        printf("%0.lf", n);
    }
    else {
        /*find highest power of base b that can fit in the number*/
        while (n - (b * pow(b, power)) >= 0) {
            power++;
        }
        counter = power + 1;

        while (n > 0) {
            divisor = floor(n / pow(b, power));
            if (divisor < 10) {
                printf("%.0lf", divisor);
            }
            else {
                printf("%c", (int)(divisor + 87));
            }
            n -= divisor * pow(b, power);
            power--;
            counter--;
        }
        if (counter > power) {
            for (; counter > 0; counter--) {
                printf("0");
            }
        }
    }
    printf("\n");
}

int main (void) {

    double position, base, i;
    double count;
    double digitNum = 1;

    printf("Position and radix:\n");
    int userInput = scanf("%lf %lf", &position, &base);
    while (userInput == 2 && position >= 0 && base >= 2 && base <= 36) {/*Loops through inputs until incorrect input is entered*/
        count = 1; digitNum = 1;/*resets count and digitNum to starting values each loop*/

        if (position < base) {
            decToBase(position, base);
            printf("^\n");
        }
        else {
            count = base;
            while (count <= position) {/*loop finds out how many digits are in the number at the position entered*/
                digitNum ++;
                count += (digitNum * ((pow(base, digitNum)) - (pow(base, digitNum - 1.0))));
                /*b^n - b^(-n) gives the number of n digit numbers in base b. I then multiply by the number of digits to get the total number of
                side-by-side numbers*/
            }

            for (i = digitNum - 1.0; i > 1; i--) {
                position -= i * (pow(base, i) - pow (base, i - 1.0));
                /*Removes the unnecessary preceding n-digit numbers, divide and conquer*/
            }

            if (position - (base) >= 0) {
                position -= (base);/*Removes the 1 digit-numbers trivially, without using the expensive for-loop*/
            }


            double number = floor(position / digitNum);/*Checks which number in n-digit numbers it is*/
            double digitOrder = fmod(position, digitNum);/*Checks the position within the number of the position in question*/


            decToBase((pow(base, digitNum - 1) + number), base);
            for (i = 0; i < digitOrder; i++)/*Interpretation of this for loop is trivial and left as an exercise for the reader :^)*/
                printf(" ");
            printf("^\n");
        }
        userInput = scanf("%lf %lf", &position, &base);
    }
    if (userInput != EOF)
        printf("Invalid input.\n");
    return 0;
}
