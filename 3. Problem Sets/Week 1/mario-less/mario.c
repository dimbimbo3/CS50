//#include <cs50.h>
#include <stdio.h>

void printBlocks(int);

int main(void) 
{
    int height;
    do{
        printf("Height: ");
        scanf("%i", &height);
    }
    while(height < 1 || height > 8);

    printf("Stored: %i\n", height);

    printBlocks(height);
}

/**
 * Prints descending blocks, aligned to the right, for the given value
*/
void printBlocks(int height){
    //iterates through rows
    for(int rows = 0; rows < height; rows++){
        //iterates through columns
        for(int cols = 0 ; cols < height; cols++){
            //if the column index is less than the height minus the row index + 1
            //then print an empty space, else print a hash block
            if(cols < (height - (rows + 1))){
                printf(" ");
            }
            else{
                printf("#");
            }
        }
        //starts a new line beneath the previous one
        printf("\n", rows);
    }
}

/*
Height: 10
Height: -1
Height: 6
Stored: 6
     #
    ##
   ###
  ####
 #####
######
*/