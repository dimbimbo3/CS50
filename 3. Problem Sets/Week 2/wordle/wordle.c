//#include <cs50.h>
#include <stdlib.h> //malloc
#include <stdio.h>
#include <string.h> //strylen
#include <time.h>
#include <stdbool.h> //bool data type

// each of our text files contains 1000 words
#define LISTSIZE 1000

// values for colors and score (EXACT == right letter, right place; CLOSE == right letter, wrong place; WRONG == wrong letter)
#define EXACT 2
#define CLOSE 1
#define WRONG 0

// ANSI color codes for boxed in letters
#define GREEN   "\e[38;2;255;255;255;1m\e[48;2;106;170;100;1m"
#define YELLOW  "\e[38;2;255;255;255;1m\e[48;2;201;180;88;1m"
#define RED     "\e[38;2;255;255;255;1m\e[48;2;220;20;60;1m"
#define RESET   "\e[0;39m"

// user-defined function prototypes
char* get_guess(int wordsize);
int check_word(char* guess, int wordsize, int status[], char choice[]);
void print_word(char* guess, int wordsize, int status[]);

int main(int argc, char* argv[])
{
    // ensures proper usage
    if(argc == 2){
        int wordsize = atoi(argv[1]);
        // ensures argv[1] is either 5, 6, 7, or 8 and stores that value in wordsize
        if(wordsize >= 5 && wordsize <= 8){
            // open correct file, each file has exactly LISTSIZE words
            char wl_filename[6];
            sprintf(wl_filename, "%i.txt", wordsize);
            FILE *wordlist = fopen(wl_filename, "r");
            if (wordlist == NULL)
            {
                printf("Error opening file %s.\n", wl_filename);
                return 1;
            }

            // load word file into an array of size LISTSIZE
            char options[LISTSIZE][wordsize + 1];

            // populate options array with strings from file
            for (int i = 0; i < LISTSIZE; i++)
            {
                fscanf(wordlist, "%s", options[i]);
            }

            // pseudorandomly select a word for this game
            srand(time(NULL)); //sets the starting point for the random generator to the time
            int index = rand() % LISTSIZE; //generates a random index within the bounds of 1000

            char choice[wordsize+1]; //declares chosen string based on word size and a null terminator
            strcpy(choice, options[index]); //copies the string at the randomly selected index into choice

            int guesses = wordsize + 1; // allow one more guess than the length of the word
            bool won = false; //win flag initialized to false

            // print greeting, using ANSI color codes to demonstrate
            printf(GREEN"This is WORDLE50"RESET"\n");
            printf("You have %i tries to guess the %i-letter word I'm thinking of...\n", guesses, wordsize);
            //printf("WORD=%s\n", choice); //debug view

            // main game loop, one iteration for each guess
            for (int i = 0; i < guesses; i++)
            {
                // obtain user's guess
                char* guess = get_guess(wordsize);

                // array to hold guess status
                int status[wordsize];
                // set all elements of status array initially to 0, aka WRONG
                for(int i = 0; i < wordsize; i++)
                {
                    status[i] = 0;
                }

                // Calculate score for the guess
                int score = check_word(guess, wordsize, status, choice);

                printf("Guess %i: ", i + 1);
                
                // Print the guess
                print_word(guess, wordsize, status);

                // if they guessed it exactly right, set terminate loop
                if (score == EXACT * wordsize)
                {
                    won = true;
                    break;
                }

                free(guess); //frees memory used by guess pointer
            }

            // Print the game's result
            if(won){
                printf("Congratulations! You guessed the correct word!\n");
            }
            else{
                printf("Sorry, but you're all out of guesses! The word was %s.\n", choice);
            }

            // that's all folks!
            return 0;
        }
        else{
            printf("Error: wordsize must be either 5, 6, 7, or 8");
            return 1; //error
        }
    }
    else{
        printf("Usage: ./wordle wordsize");
        return 1; //error
    }
}

char* get_guess(int wordsize)
{
    int buffer = wordsize+2; //increase index size by +2 for newline character and null terminator from fgets
    char* guess = malloc(sizeof(char) * buffer); //allocates enough memory
    char* newlinePtr;

    int length, character;
    do{
        printf("Input a %i-letter word: ", wordsize);
        //if there is an error in retrieving input then terminate the program
        if(!fgets(guess, buffer, stdin)){
            printf("Input error has occured! Terminating...");
            exit(1);
        }
        //else if there was enough space there should be a new line character
        else if(newlinePtr = strchr(guess, '\n')){
            *newlinePtr = '\0'; //replace the newline character at the pointer's address with a null terminator
        }
        //else the input exceeded the buffer
        else{
            while(character = getchar() != '\n' && character != EOF); //consume any remaining text for next guess
        }

        length = strlen(guess); //now store the length of the string, not including null terminators
    }
    while(length != wordsize); // ensure users actually provide a guess that is the correct length

    return guess;
}

int check_word(char* guess, int wordsize, int status[], char choice[])
{
    int score = 0;

    for(int i = 0; i < wordsize; i++){
        for(int j = 0; j < wordsize; j++){
            if(guess[i] == choice[j]){
                if(i == j){
                    status[i] = EXACT;
                    score += EXACT;
                    break;
                }
                else{
                    status[i] = CLOSE;
                    score += CLOSE;
                }
            }
        }
    }

    return score;
}

void print_word(char* guess, int wordsize, int status[])
{
    for(int i = 0; i < wordsize; i++){
        switch(status[i]){
            case EXACT:
                printf(GREEN"%c", guess[i]);
                break;
            case CLOSE:
                printf(YELLOW"%c", guess[i]);
                break;
            case WRONG:
                printf(RED"%c", guess[i]);
                break;
        }
    }

    printf(RESET"\n");
    return;
}

/*
You have 6 tries to guess the 5-letter word I'm thinking of
WORD=music
Input a 5-letter word: balls
Guess 1: balls
Input a 5-letter word: sucky
Guess 2: sucky
Input a 5-letter word: mosey
Guess 3: mosey
Input a 5-letter word: music
Guess 4: music
Congratulations! You guessed the correct word!
*/