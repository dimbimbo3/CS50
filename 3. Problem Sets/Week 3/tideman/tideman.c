//#include <cs50.h>
#include <stdio.h>
#include <string.h> //strcpy
#include <stdbool.h> //bool
#include <stdlib.h> //atoi, malloc
#include <ctype.h> //isdigit, isalpha

#define MAX 9 //Max number of candidates
#define NAME 51 //limit for vote name length (+1 for null terminator)

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
char candidates[MAX][NAME];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, char* name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
bool isCycle(int,int);
void print_winner(void);
char* get_input(char[]);

int main(int argc, char* argv[])
{
    //-DEBUG-
    //int argc = 4;
    //char argv[4][NAME] = {"./tideman", "Alice", "Bob", "Charlie"};

    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    candidate_count = argc - 1; //stores the total number of candidates
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }

    // Populate array of candidates
    for (int i = 0; i < candidate_count; i++)
    {
        strcpy(candidates[i], argv[i + 1]);
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    char prompt1[] = "Number of voters: "; //prompt used to indicate the retrieval of the number of voters
    char* votersPtr = get_input(prompt1); //retrieves user input and points to its address
    int voter_count = atoi(votersPtr); //stores the converted input into the global int variable
    free(votersPtr); //frees up allocated memory

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        printf("[Voter %i]\n", i+1); //prints out visual indication of the current voter for readability

        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            char rankNum[2] = {(char) (j + '1'), '\0'}; //stores the counter +1 as a string for current rank number
            char* namePtr = get_input(rankNum); //retrieves vote string and points to its address

            if (!vote(j, namePtr, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    pair_count = 0; //initialize value to 0

    //Constructs pairs to determine condorcet winner
    add_pairs();
    sort_pairs();
    lock_pairs();

    print_winner();

    return 0;
}

/**
 * Updates the vote preference for the given candidate.
*/
bool vote(int rank, char* name, int ranks[])
{
    //loops through every candidate in the global array
    for(int i = 0; i < candidate_count; i++)
    {
        //if a matching candidate is found then store the candidate's index at that rank for the current voter
        if(strcmp(strupr(name), strupr(candidates[i])) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }

    return false; //if the entire loop was checked and the candidate was never found, then return false for an invalid vote
}

/**
 * Updates a candidate's preference over other candidates given one voter's chosen ranks. 
*/
void record_preferences(int ranks[])
{
    //loops through each ranked vote (i) of the given voter
    for(int i = 0; i < (candidate_count - 1); i++)
    {
        int hRankIndex = ranks[i]; //stores the index of the candidate at this current higher rank

        //loops through each candidate (j) ranked lower on the ballot
        for(int j = i+1; j < candidate_count; j++)
        {
            int lRankIndex = ranks[j]; //stores the index of the candidate at this current lower rank
            preferences[hRankIndex][lRankIndex]++; //increments global preference of the higher ranked candidate over the lower ranked one
        }
    }

    return;
}

/**
 * Record pairs of candidates where one is preferred over the other.
*/
void add_pairs(void)
{
    //loops through each candidate
    for(int i = 0; i < candidate_count; i++)
    {
        //loops through this candidate's preference vs other candidates
        for(int j = 0; j < candidate_count; j++)
        {
            //if this candidate is preferred more than the other
            //then pair both and set each of their indices as the winner and loser, respectively
            if(preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++; //increments pair_count for next pair in array
            }
        }
    }

    return;
}

/**
 * Sorts pairs in decreasing order by strength of victory using a reverse bubble sort.
*/
void sort_pairs(void)
{
    //loops through all but the 2nd to last pairs
    for(int i = 0; i < pair_count - 1; i++){
        //initialized to false on each iteration
        bool swap = false; //flag to indicate that a higher margin has been found and should be sorted as such

        //passes through all pairs except those already sorted at the end
        for(int j = 0; j < pair_count - 1 - i; j++)
        {
            //compares the difference in preference between this pair and the next
            //in order to ensure they are placed into descending order
            if(preferences[pairs[j].winner][pairs[j].loser] < preferences[pairs[j+1].winner][pairs[j+1].loser])
            {
                int storedWin = pairs[j].winner; //store the current lower pair winner
                int storedLoss = pairs[j].loser; //store the current lower pair loser

                //move the found higher pair into that lower pair's location in the array
                pairs[j].winner = pairs[j+1].winner;
                pairs[j].loser = pairs[j+1].loser;

                //move the stored lower pair into the higher pair's previous location in the array
                pairs[j+1].winner = storedWin;
                pairs[j+1].loser = storedLoss;

                swap = true; //flag the swap
            }
        }

        //if the pairs were compared and no swaps were made
        //then the array is in sorted order so end the loop early
        if(!swap){
            break;
        }
    }

    return;
}

/**
 * Lock pairs into the candidate graph in order, without creating cycles.
*/
void lock_pairs(void)
{
    //loops through all pairs (at this point they are already sorted)
    for(int i = 0; i < pair_count; i++){
        //checks if locking in the pair would create a cycle
        if(!isCycle(pairs[i].winner, pairs[i].loser)){
            //if no cycle would be made, then lock in the pair
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }

    return;
}

/**
 * Function to determine if locking in a pair will lead to a cycle.
*/
bool isCycle(int winSource, int lossIndex)
{
    //if the lossIndex has reached the win source
    //then return true as this is a cycle
    if(lossIndex == winSource)
    {
        return true;
    }

    //loops through all pairs
    for(int i = 0; i < pair_count; i++)
    {
        //checks if another pair has this current loser winning
        if(pairs[i].winner == lossIndex)
        {
            //if so, then loop through all pairs for other losing candidates
            for(int j = 0; j < pair_count; j++)
            {
                //checks if there are any edges to other candidates they beat
                if(locked[lossIndex][pairs[j].loser])
                {
                    //if so, then recursively call the function with the loser of that edge
                    //in order to see if it eventually cycles back to the source
                    if(isCycle(winSource, pairs[j].loser))
                    {
                        return true; //cycle was found
                    }
                }
            }
        }
    }

    return false; //no cycle was found
}

// Print the winner of the election
void print_winner(void)
{
    //loops through each candidate
    for(int i = 0; i < candidate_count; i++)
    {
        bool edge = false; //flag if an edge is found to this candidate (initialized to false)

        //loops through the other candidates to see if this candidate wins
        for(int j = 0; j < pair_count; j++){
            //if there are any edges pointing to this candidate then they have not won
            if(locked[j][i]){
                edge = true; //marks that an edge was found
                break; //moves on to the next candidate
            }
        }

        //if no edge was found to be pointed to this candidate then they have won
        if(!edge){
            printf("WINNER - %s", candidates[i]);
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
$ ./tideman Alice Bob Charlie Rachel Chandler Betty Smitty Ross Mitchell Madona
Maximum number of candidates is 9
*/

/*
$ ./tideman Alice Bob Charlie
Number of voters: 5
[Voter 1]
Rank 1: test
Invalid vote.
*/

/*
$ ./tideman Alice Bob Charlie
Number of voters: 5
[Voter 1]
Rank 1: Alice
Rank 2: Charlie
Rank 3: Bob

[Voter 2]
Rank 1: Alice
Rank 2: Charlie
Rank 3: Bob

[Voter 3]
Rank 1: Bob
Rank 2: Charlie
Rank 3: Alice

[Voter 4]
Rank 1: Bob
Rank 2: Charlie
Rank 3: Alice

[Voter 5]
Rank 1: Charlie
Rank 2: Alice
Rank 3: Bob

WINNER - CHARLIE
*/

/*
$ ./tideman Alice Bob Charlie
Number of voters: 9
[Voter 1]
Rank 1: alice
Rank 2: bob
Rank 3: charlie

[Voter 2]
Rank 1: alice
Rank 2: bob
Rank 3: charlie

[Voter 3]
Rank 1: alice
Rank 2: bob
Rank 3: charlie

[Voter 4]
Rank 1: bob
Rank 2: charlie
Rank 3: alice

[Voter 5]
Rank 1: bob
Rank 2: charlie
Rank 3: alice

[Voter 6]
Rank 1: charlie
Rank 2: alice
Rank 3: bob

[Voter 7]
Rank 1: charlie
Rank 2: alice
Rank 3: bob

[Voter 8]
Rank 1: charlie
Rank 2: alice
Rank 3: bob

[Voter 9]
Rank 1: charlie
Rank 2: alice
Rank 3: bob

WINNER - CHARLIE
*/