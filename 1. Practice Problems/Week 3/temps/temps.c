// Practice working with structs
// Practice applying sorting algorithms

//#include <cs50.h>
#include <stdio.h>
#include <string.h>

#define NUM_CITIES 10 //total number of cities
#define MAX 49 //longest city name in the USA is 48 characters long (+1 for null terminator)

typedef struct
{
    char city[MAX];
    int temp;
}
avg_temp; //user defined structure

avg_temp temps[NUM_CITIES]; //global array of avg_temp

void sort_cities(void); //function prototype

int main(void)
{
    strcpy(temps[0].city, "Austin");
    temps[0].temp = 97;

    strcpy(temps[1].city, "Boston");
    temps[1].temp = 82;

    strcpy(temps[2].city, "Chicago");
    temps[2].temp = 85;

    strcpy(temps[3].city, "Denver");
    temps[3].temp = 90;

    strcpy(temps[4].city, "Las Vegas");
    temps[4].temp = 105;

    strcpy(temps[5].city, "Los Angeles");
    temps[5].temp = 82;

    strcpy(temps[6].city, "Miami");
    temps[6].temp = 97;

    strcpy(temps[7].city, "New York");
    temps[7].temp = 85;

    strcpy(temps[8].city, "Phoenix");
    temps[8].temp = 107;

    strcpy(temps[9].city, "San Francisco");
    temps[9].temp = 66;

    sort_cities();

    printf("Average July Temperatures by City\n\n");

    for (int i = 0; i < NUM_CITIES; i++)
    {
        printf("%s: %i\n", temps[i].city, temps[i].temp);
    }
}

/**
 * Sort cities by temperature in descending order using selection sort.
*/
void sort_cities(void)
{
    int swapTemp;
    char swapCity[MAX];

    //loops through every element in the array except the last
    for(int i = 0; i < NUM_CITIES-1; i++)
    {
        int largest = i; //initially sets the largest index to the current index

        //loops through every element after the ith
        for (int j = i+1; j < NUM_CITIES; j++)
        {
            //if the current index's temp is greater than the largest index's
            //then set the largest index to the current index instead
            if(temps[j].temp > temps[largest].temp){
                largest = j;
            }
        }
        
        //if the largest index changed
        //then swap the temperatures & cities between it and those at the ith position
        if(largest != i){
            //store the largest values in variables
            swapTemp = temps[largest].temp;
            strcpy(swapCity, temps[largest].city);

            //moves the ith values to the largest index
            temps[largest].temp = temps[i].temp;
            strcpy(temps[largest].city, temps[i].city);

            //finally, move the largest values to the ith position
            temps[i].temp = swapTemp;
            strcpy(temps[i].city, swapCity);
        }
    }
}

/*
Average July Temperatures by City

Phoenix: 107
Las Vegas: 105
Miami: 97
Austin: 97
Denver: 90
Chicago: 85
New York: 85
Los Angeles: 82
Boston: 82
San Francisco: 66
*/