//#include <cs50.h>
#include <stdio.h>
#include <string.h> //strcpy
#include <stdbool.h> //bool
#include <stdlib.h> //atoi, malloc
#include <ctype.h> //isdigit, isalpha

// Max number of candidates
#define MAX 9
#define NAME 51 //limit for vote name length (+1 for null terminator)

// Candidates have name and vote count
typedef struct
{
    char name[NAME];
    int votes;
}
candidate; //user defined structure

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(char* name);
void print_winner(void);
char* get_input(char prompt[]);

int main(int argc, char* argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    candidate_count = argc - 1; //stores the total number of candidates

    // Check that the candidate count does not exceed the allowable amount
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }

    // Populates the array of candidates by copying the passed arguments 
    // as the names of each respective element within the global candidates array
    for (int i = 0; i < candidate_count; i++)
    {
        strcpy(candidates[i].name, argv[i + 1]);
        candidates[i].votes = 0;
    }

    char prompt1[] = "Number of voters: "; //prompt used to indicate the retrieval of the number of voters
    char* votersPtr = get_input(prompt1); //retrieves user input and points to its address
    int voter_count = atoi(votersPtr); //stores the converted input as an int
    free(votersPtr); //frees up allocated memory

    char prompt2[] = "Vote: "; //prompt used to indicate the retrieval voters choice of candidate
    // Loop through all voters
    for (int i = 0; i < voter_count; i++)
    {
        printf("[Vote %i] ", i+1);
        char* namePtr = get_input(prompt2);

        // Check for invalid vote
        if (!vote(namePtr))
        {
            printf("Invalid vote.\n");
        }

        free(namePtr); //frees up memory allocated
    }

    // Display winner of election
    print_winner();
}

/**
 * Updates the vote total for the given candidate.
*/
bool vote(char* name)
{
    //loops through every candidate in the global array
    for(int i = 0; i < candidate_count; i++){
        //if a matching candidate is found
        //then add a vote to that candidates vote total and return true
        if(strcmp(strupr(name), strupr(candidates[i].name)) == 0){
            candidates[i].votes++;
            return true;
        }
    }

    return false; //if the entire loop was checked and the candidate was never found, then return false for an invalid vote
}

/**
 * Prints out the winning candidates.
*/
void print_winner(void)
{
    int winTotal = candidates[0].votes; //initially set the winning number of votes to the first candidate's total
    //loops through every element after the 1st to find the highest candidate vote total in in the array
    for(int i = 1; i < candidate_count; i++){
        //if the current candidate has a higher vote total
        //then set them as the current winning total
        if(candidates[i].votes > winTotal)
        {
            winTotal = candidates[i].votes;
        }
    }

    int winCount = 0; //Stores the found number of winners
    //Searches through the array to determine who has won
    for(int i = 0; i < candidate_count; i++){
        //if the current candidate has a vote total equivalent to the winning total
        //then print their name as a winner
        if(candidates[i].votes == winTotal)
        {
            printf("WINNER: %s - %i votes\n", candidates[i].name, winTotal);
            winCount++;
        }
    }

    //if there was more than one winner
    //then declare it to be a tie
    if(winCount > 1){
        printf("It's a %i-way tie!\n", winCount);
    }
}

/**
 * Accepts user input and validates it based on the given prompt.
*/
char* get_input(char prompt[]){
    char* input = malloc(sizeof(char) * NAME); //allocates enough memory

    bool cont; //flag for loop
    do {
        cont = false; //reset each iteration so loop only continues if marked

        //prints out the supplied prompt
        printf("%s", prompt);
        //ensures that there were no input errors
        if(fgets(input, NAME, stdin))
        {
            //removes newline character added by fgets
            char* newlinePtr;
            if(newlinePtr = strchr(input, '\n'))
            {
                *newlinePtr = '\0';
            }

            //Ensures votes will only contain letters
            if(strcmp(prompt, "Vote: ") == 0)
            {
                //loops through each character (up to the null terminator) to ensure they are only letters
                for(int i = 0; i < strlen(input); i++){
                    //if anything other than a letter is found, then mark the flag as true for this iteration
                    //and then break out of this for loop
                    if(!isalpha(input[i])){
                        printf("(ENTER ONLY LETTERS!)\n");
                        cont = true;
                        break;
                    }
                }
            }
            else
            {
                //loops through each character (up to the null terminator) to ensure they are only digits
                for(int i = 0; i < strlen(input); i++){
                    //if a nonnumerical character is found, then mark the flag as true for this iteration
                    //and then break out of this for loop
                    if(!isdigit(input[i])){
                        printf("(ENTER ONLY NUMBERS!)\n");
                        cont = true;
                        break;
                    }
                }
            }
        }
        else
        {
            printf("INPUT ERROR!\n");
            cont = true; //loops to try to retrieve input again
        }
    } while(cont);

    return input;
}

/*
$ ./plurality Mark Samantha Joe Bob
Number of voters: 10
[Vote 1] Vote: Mark
[Vote 2] Vote: Joe
[Vote 3] Vote: Tim
Invalid vote.
[Vote 4] Vote: Barbara 
Invalid vote.
[Vote 5] Vote: Joe
[Vote 6] Vote: Bob
[Vote 7] Vote: Bob
[Vote 8] Vote: Joe
[Vote 9] Vote: Samantha
[Vote 10] Vote: Bob
WINNER: JOE - 3 votes
WINNER: BOB - 3 votes
It's a tie!
*/