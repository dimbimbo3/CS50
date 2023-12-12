//#include <cs50.h>
#include <stdio.h>

void printBlocks(int);

int main(void) 
{
    int height;

    //loop prevents input of any value greater than 8 or less than 1
    do{
        printf("Height: ");
        scanf("%i", &height);
    }
    while(height < 1 || height > 8);

    //confirms stored user input
    printf("Stored: %i\n", height);

    //calls void function to print
    printBlocks(height);
}

/**
 * Prints descending blocks, aligned to the right and then the left, for the given value
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

        //Prints two spaces between the last block of the first pyramid and this one
        printf("  ");

        //iterates through the columns again, but this time creating a reverse pyramid
        for(int colsRev = 0 ; colsRev < height; colsRev++){
            //if the column index is greater than the row index
            //then print an empty space, else print a hash block
            if(colsRev > rows){
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
Height: 8
Stored: 8
       #  #       
      ##  ##      
     ###  ###     
    ####  ####    
   #####  #####   
  ######  ######  
 #######  ####### 
########  ########
*/