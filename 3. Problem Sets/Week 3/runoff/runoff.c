//#include <cs50.h>
#include <stdio.h>
#include <string.h> //strcpy
#include <stdbool.h> //bool
#include <stdlib.h> //atoi, malloc
#include <ctype.h> //isdigit, isalpha

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9
#define NAME 51 //limit for vote name length (+1 for null terminator)

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    char name[NAME];
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, char* name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);
char* get_input(char prompt[]);

int main(int argc, char* argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    candidate_count = argc - 1; //stores the total number of candidates

    // Check that the candidate count does not exceed the allowable amount
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }

    // Populates the array of candidates by copying the passed arguments 
    // as the names of each respective element within the global candidates array
    for (int i = 0; i < candidate_count; i++)
    {
        strcpy(candidates[i].name, argv[i + 1]);
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    char prompt1[] = "Number of voters: "; //prompt used to indicate the retrieval of the number of voters
    char* votersPtr = get_input(prompt1); //retrieves user input and points to its address
    voter_count = atoi(votersPtr); //stores the converted input into the global int variable
    free(votersPtr); //frees up allocated memory

    //Checks whether the given vote counter exceeds the maximum allowable amount
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            char rankNum[2] = {(char) (j + '1'), '\0'}; //stores the counter +1 as a string for rank number
            char* namePtr = get_input(rankNum); //retrieves vote string and points to its address

            // Record vote, unless it's invalid
            if (!vote(i, j, namePtr))
            {
                printf("Invalid vote.\n");
                return 4;
            }

            free(namePtr); //frees up memory allocated
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes of the remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            int count = 0;
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("WINNER: %s - %i votes\n", candidates[i].name, min);
                    count++;
                }
            }
            printf("It's a %i-way tie!", count);
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

/**
 * Updates the vote preference for the given candidate.
*/
bool vote(int voter, int rank, char* name)
{
    //loops through every candidate in the global array
    for(int i = 0; i < candidate_count; i++)
    {
        //if a matching candidate is found then store the candidate's index at that rank for the voter
        if(strcmp(strupr(name), strupr(candidates[i].name)) == 0)
        {
            preferences[voter][rank] = i;
            return true;
        }
    }

    return false; //if the entire loop was checked and the candidate was never found, then return false for an invalid vote
}

/**
 * Tabulates the votes for non-eliminated candidates. 
*/
void tabulate(void)
{
    //loops through each voter
    for(int i = 0; i < voter_count; i++)
    {
        //loops through each ranked vote by the given voter
        for(int j = 0; j < candidate_count; j++){
            //finds the first candidate on the voter's ballot who has not already been eliminated
            //then adds a vote to their tally before moving to the next voter
            if(!candidates[preferences[i][j]].eliminated){
                candidates[preferences[i][j]].votes++;
                break;
            }
        }
    }

    return;
}

/**
 * Prints out the winning candidate or returns false to indicate a run off.
*/
bool print_winner(void)
{
    int highestIndex = 0; //intially set to the index of the first candidate
    int highestTotal = candidates[0].votes; //initially set to the number of votes for the first candidate
    //loops through every element after the 1st to find the highest candidate vote total in in the array
    for(int i = 1; i < candidate_count; i++)
    {
        //if the current candidate has a higher vote total
        //then set them as the current highest total
        if(candidates[i].votes > highestTotal)
        {
            highestTotal = candidates[i].votes;
            highestIndex = i; //only used if there isn't a runoff
        }
    }

    int required = voter_count / 2.0; //required total of votes to beat in order to win

    //if the highest vote tally is greater than 1/2 of the total number of votes
    //then declare that candidate as the winner
    if(highestTotal > required)
    {
        printf("WINNER: %s - %i votes\n", candidates[highestIndex].name, highestTotal);
        return true;
    }

    //in the case that no candidate was found to have won 
    //then return false for a potential run off
    printf("Runoff...\n");
    return false;
}

/**
 * Return the minimum number of votes any remaining candidate has.
*/
int find_min(void)
{
    int lowest = MAX_VOTERS; //initialize lowest number of votes to the maximum possible number
    //loops through every element to find the lowest candidate vote total in in the array
    for(int i = 0; i < candidate_count; i++)
    {
        //if the current candidate has already been eliminated 
        //then do not factor them in
        if(!candidates[i].eliminated)
        {
            //if the current candidate has a lower vote total
            //then set them as the current lowest total
            if(candidates[i].votes < lowest)
            {
                lowest = candidates[i].votes;
            }
        }
    }

    return lowest;
}

/**
 * Return true if the election is tied between all candidates, false otherwise.
*/
bool is_tie(int min)
{
    int eliminated = 0; //counter to determine how many candidates are no longer in the running
    //loops through all candidates
    for(int i = 0; i < candidate_count; i++){
        //if the candidate has been eliminated then increment counter
        if(candidates[i].eliminated)
        {
            eliminated++;
        }
    }

    int available = (candidate_count - eliminated); //indicates how many candidates are still in the race
    int tieCount = 0; //counter to indicate how many candidates are tied with the lowest score
    //loops through all candidates who haven't been eliminated
    for(int i = 0; i < available; i++){
        //if the candidate's vote total is equal to the minimum tally then increment the tie counter
        if(candidates[i].votes == min)
        {
            tieCount++;
        }
    }

    //if all of the candidates who are still in the running have the same score
    //then it is declared to be a tie
    if(tieCount == available)
    {   
        return true;
    }

    return false;
}

/**
 * Eliminate the candidate (or candidates) in last place
*/
void eliminate(int min)
{
    //loops through all candidates to determine who will be eliminiated
    for(int i = 0; i < candidate_count; i++)
    {
        if(candidates[i].votes == min)
        {
            candidates[i].eliminated = true;
        }
    }

    return;
}

/**
 * Accepts user input and validates it based on the given prompt.
*/
char* get_input(char prompt[]){
    char* input = malloc(sizeof(char) * NAME); //allocates enough memory

    bool cont; //flag for loop
    int promptLength = strlen(prompt); // stores fixed value to prevent function from having to run every iteration of the loop
    do {
        cont = false; //reset each iteration so loop only continues if marked

        //ensures that ranked prompt logic will not be applied
        if(promptLength > 1)
        {
            //prints out only the supplied prompt
            printf("%s", prompt);
        }
        //else this prompt actually is the one for ranked votes
        else
        {
            //prints out the rank specific prompt
            printf("Rank %s: ", prompt);
        }

        //ensures that there are no errors in getting user input
        if(fgets(input, NAME, stdin))
        {
            //removes newline character added by fgets
            char* newlinePtr;
            if(newlinePtr = strchr(input, '\n'))
            {
                *newlinePtr = '\0';
            }

            int inputLength = strlen(input); //get the length of the input now that the newline char has been removed

            //ensures that ranked prompt logic will not be applied
            if(promptLength > 1)
            {
                //loops through each character (up to the null terminator) to ensure they are only digits
                for(int i = 0; i < inputLength; i++)
                {
                    //if a nonnumerical character is found, then mark the continue flag as true for this iteration
                    //and then break out of this for loop
                    if(!isdigit(input[i])){
                        printf("(ENTER ONLY NUMBERS!)\n");
                        cont = true;
                        break;
                    }
                }
            }
            //else this input is for ranked votes and allows only letters
            else
            {
                //loops through each character (up to the null terminator) to ensure they are only letters
                for(int i = 0; i < inputLength; i++)
                {
                    //if anything other than a letter is found, then mark the flag as true for this iteration
                    //and then break out of this for loop
                    if(!isalpha(input[i])){
                        printf("(ENTER ONLY LETTERS!)\n");
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
Number of voters: 6
Rank 1: alice
Rank 2: bob
Rank 3: charlie

Rank 1: alice
Rank 2: bob
Rank 3: charlie

Rank 1: charlie
Rank 2: bob
Rank 3: alice

Rank 1: charlie
Rank 2: bob
Rank 3: alice

Rank 1: bob
Rank 2: charlie
Rank 3: alice

Rank 1: bob
Rank 2: charlie
Rank 3: alice

Runoff...
WINNER: ALICE - 2 votes
WINNER: BOB - 2 votes
WINNER: CHARLIE - 2 votes
It's a 3-way tie!
*/

/*
Number of voters: 4
Rank 1: alice
Rank 2: bob
Rank 3: charlie

Rank 1: alice
Rank 2: bob
Rank 3: charlie

Rank 1: charlie
Rank 2: alice
Rank 3: bob

Rank 1: bob
Rank 2: charlie
Rank 3: alice

Runoff...
WINNER: ALICE - 2 votes
*/

/*
Number of voters: 5
Rank 1: bob
Rank 2: charlie
Rank 3: alice

Rank 1: bob
Rank 2: charlie
Rank 3: alice

Rank 1: charlie
Rank 2: bob
Rank 3: alice

Rank 1: charlie
Rank 2: bob
Rank 3: alice

Rank 1: alice
Rank 2: charlie
Rank 3: bob

Runoff...
WINNER: CHARLIE - 3 votes
*/