//#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // TODO: Prompt for start size
    int popStart;
    do{
        printf("Start size: ");
        scanf("%i", &popStart);
    }
    while(popStart < 9);

    // TODO: Prompt for end size
    int popEnd;
    do{
        printf("End size: ");
        scanf("%i", &popEnd);
    }
    while(popEnd < popStart);

    // TODO: Calculate number of years until we reach threshold
    int years;
    int popCurrent = popStart;
    for(years = 0 ; popCurrent < popEnd; years++){
        int born = popCurrent / 3; // n/3 llamas are born
        int died = popCurrent / 4; // n/4 llamas pass away
        popCurrent += born - died; // update current population
    }

    // TODO: Print number of years
    printf("Years: %i", years);
}


/*
Start size: 100
End size: 1000000
Years: 115
*/