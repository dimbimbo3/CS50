//#include <cs50.h>
#include <stdio.h>
#include <ctype.h> //isalpha
#include <string.h> //strlen
#include <stdbool.h> //bool data type

#define MAX 15+1 //scrabble only allows words up to 15 characters (+1 for null terminator)
#define PLAYERS 2 //total of 2 players
#define A 65 //ascii value for 'A'

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(char[]);

int main(void)
{
    //declare 2d array for words of each player
    char words[PLAYERS][MAX];

    //get words from each player
    for(int i = 0; i < PLAYERS; i++){
        printf("[Player %i]\n", i+1);
        //prevents empty user input and stores result in array
        do{
            printf("Enter word: ");
        }
        while(fgets(words[i], MAX, stdin) == NULL || words[i][0] == '\n'); //gets input up to the MAX - 1 (fgets places \n at the last index)
    }

    //declare array to store score of each player
    int scores[PLAYERS];

    //gets scores of each player's word
    for(int i = 0; i < PLAYERS; i++){
        scores[i] = compute_score(words[i]);
    }

    bool tie = false; //checks for tied score
    int highest = -1; //initialize highest score to -1
    int playerIndex; //stores index for highest score
    //determines the winner based on scores
    for(int i = 0; i < PLAYERS; i++){
        if(scores[i] > highest){
            highest = scores[i];
            playerIndex = i;
        }
        else if(scores[i] == highest){
            tie = true;
        }
    }

    //prints out the winner and their score
    if(!tie)
        printf("Winner: Player %i (Score = %i)\n", playerIndex+1, highest);
    else
        printf("It's a tie! (Tied Score = %i)\n", highest);
}

/**
 * Calculates the total point value of a given word.
*/
int compute_score(char word[])
{
    int letterIndex;
    int total = 0;

    //iterates through each character of the word
    for(int i = 0; i < strlen(word); i++){
        //if the character is a letter then get its value and add to player's score
        if(isalpha(word[i])){
            letterIndex = (int) toupper(word[i]) - A; //determines the index of the letter's points based on ascii value and position in alphabet (0-25)
            total += POINTS[letterIndex]; //adds the value of the letter to the player's running total
        }
    }

    return total;
}

/*
[Player 1]
Enter word: test
[Player 2]
Enter word: zesty
Winner: Player 2 (Score = 17)
*/

/*
[Player 1]
Enter word: zesty
[Player 2]
Enter word: test123
Winner: Player 1 (Score = 17)
*/

/*
[Player 1]
Enter word: 123
[Player 2]
Enter word: @#$
It's a tie! (Tied Score = 0)
*/