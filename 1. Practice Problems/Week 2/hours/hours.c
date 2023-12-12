//#include <cs50.h>
#include <ctype.h>
#include <stdio.h> //toupper

float calc_hours(int hours[], int weeks, char output);
void flushInput();
int validateInt();

int main(void)
{
    int weeks;
    //loops to ensure that entered value is at least 1 week
    do{
        printf("Number of weeks taking CS50: ");
        //Calls to ensure that an integer will be entered
        weeks = validateInt();
    }
    while(weeks < 1);

    int hours[weeks]; //declares an array with the same size as the number of weeks

    //iterates through each week in the array
    for (int i = 0; i < weeks; i++)
    {
        //loops to ensure that the entered hw hours are not negative values
        do{
            printf("Week %i HW Hours: ", i);
            //Calls to ensure that an integer will be entered
            hours[i] = validateInt();
        }
        while(hours[i] < 0);
    }

    char output;
    //loops to ensure that entered character is one of the desired values
    do
    {
        printf("Enter T for total hours, A for average hours per week: ");
        scanf(" %c", &output); //space for whitespace/special character deletion
        output = toupper(output);
    }
    while (output != 'T' && output != 'A');

    printf("%.1f hours\n", calc_hours(hours, weeks, output));
}

/**
 * Calculates either the total number of hours for the week or the overall average,
 * depending upon the user's choice of output. (Returns -1 for unexpected choices.)
*/
float calc_hours(int hours[], int weeks, char output)
{
    float total = 0;
    float avg = 0;

    //Determines what the user chose, and alerts in case it is not anticipated
    switch(output){
        case 'A':
            for(int i = 0; i < weeks; i++){
                avg += hours[i];
            }
            avg /= weeks;
            return avg;

            break;
        case 'T':
            for(int i = 0; i < weeks; i++){
                total += hours[i];
            }
            return total;

            break;
        default:
            printf("INVALID SELECTION: Error detected!");
            return -1;

            break;
    }
}

/**
 * Clears the current input buffer in the case of an invalid result.
 * (Useful for input validation when using scanf.)
*/
void flushInput(){
    int c;

    //iterates through each character in buffer until its end is reached
    do {
        c = getchar(); //consume character
    } while( c != '\n' && c != EOF );
}

/**
 * Takes in user input and validates that an integer was entered, if not then calls to flush and returns a -1.
*/
int validateInt(){
    int num = -1; //initialize value to -1
    //Takes in the user input and validates that an integer was entered, if not then calls to flush
    if(scanf("%i", &num) != 1){
        flushInput();
    }

    return num;
}

/*
Number of weeks taking CS50: x
Number of weeks taking CS50: 0
Number of weeks taking CS50: 3
Week 0 HW Hours: y
Week 0 HW Hours: -1
Week 0 HW Hours: 0
Week 1 HW Hours: 5
Week 2 HW Hours: 3
Enter T for total hours, A for average hours per week: t
8.0 hours
*/

/*
Number of weeks taking CS50: 5
Week 0 HW Hours: 2
Week 1 HW Hours: 4
Week 2 HW Hours: 6
Week 3 HW Hours: 8
Week 4 HW Hours: 10
Enter T for total hours, A for average hours per week: a
6.0 hours
*/