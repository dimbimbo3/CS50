// Implements a spell-checker

#include <ctype.h>
#include <stdio.h>
#include <sys/resource.h>
#include <sys/time.h>

#include "dictionary.h" //.h->/c for debugging

// Undefine any definitions
#undef calculate
#undef getrusage

// Default dictionary
#define DICTIONARY "dictionaries/large" //large->small for debugging

// Prototype
double calculate(const struct rusage *b, const struct rusage *a);

int main(int argc, char *argv[]) //(int argc, char *argv[]) -> () for debugging
{
    /*
    //DEBUGGING
    int argc = 2;
    char *argv[2] = {"./speller", "texts/holmes.txt"};
    */

    // Check for correct number of args
    if (argc != 2 && argc != 3)
    {
        printf("Usage: ./speller [DICTIONARY] text\n");
        return 1;
    }

    // Structures for timing data
    struct rusage before, after;

    // Benchmarks
    double time_load = 0.0, time_check = 0.0, time_size = 0.0, time_unload = 0.0;

    // Determine dictionary to use
    char *dictionary = (argc == 3) ? argv[1] : DICTIONARY; //if argc == 3 then dictionary = argv[1] else dictionary = DICTIONARY

    // Load dictionary
    getrusage(RUSAGE_SELF, &before);
    bool loaded = load(dictionary);
    getrusage(RUSAGE_SELF, &after);

    // Exit if dictionary not loaded
    if (!loaded)
    {
        printf("Could not load %s.\n", dictionary);
        return 1;
    }

    // Calculate time to load dictionary
    time_load = calculate(&before, &after);

    // Try to open text
    char *text = (argc == 3) ? argv[2] : argv[1]; //if argc == 3 then *text = argv[2] else *text = argv[1]
    FILE *file = fopen(text, "r");
    if (file == NULL)
    {
        printf("Could not open %s.\n", text);
        unload();
        return 1;
    }

    // Prepare to report misspellings
    printf("\nMISSPELLED WORDS\n\n");

    // Prepare to spell-check
    int index = 0, misspellings = 0, words = 0;
    char word[LENGTH + 1];

    // Spell-check each word in text (loops through each character)
    char c;
    while (fread(&c, sizeof(char), 1, file))
    {
        // Allow only alphabetical characters and apostrophes
        if (isalpha(c) || (c == '\'' && index > 0))
        {
            // Append character to word
            word[index] = c;
            index++;

            // Ignore alphabetical strings too long to be words
            if (index > LENGTH)
            {
                // Consume remainder of alphabetical string
                while (fread(&c, sizeof(char), 1, file) && isalpha(c));

                // Prepare for new word
                index = 0;
            }
        }
        // Ignore words with numbers (like MS Word can)
        else if (isdigit(c))
        {
            // Consume remainder of alphanumeric string
            while (fread(&c, sizeof(char), 1, file) && isalnum(c));

            // Prepare for new word
            index = 0;
        }
        // We must have found a whole word
        else if (index > 0)
        {
            // Terminate current word
            word[index] = '\0';

            // Update counter
            words++;

            // Check word's spelling
            getrusage(RUSAGE_SELF, &before);
            bool misspelled = !check(word);
            getrusage(RUSAGE_SELF, &after);

            // Update benchmark
            time_check += calculate(&before, &after);

            // Print word if misspelled
            if (misspelled)
            {
                printf("%s\n", word);
                misspellings++;
            }

            // Prepare for next word
            index = 0;
        }
    }

    // Check whether there was an error
    if (ferror(file))
    {
        fclose(file);
        printf("Error reading %s.\n", text);
        unload();
        return 1;
    }

    // Close text
    fclose(file);

    // Determine dictionary's size
    getrusage(RUSAGE_SELF, &before);
    unsigned int n = size();
    getrusage(RUSAGE_SELF, &after);

    // Calculate time to determine dictionary's size
    time_size = calculate(&before, &after);

    // Unload dictionary
    getrusage(RUSAGE_SELF, &before);
    bool unloaded = unload();
    getrusage(RUSAGE_SELF, &after);

    // Abort if dictionary not unloaded
    if (!unloaded)
    {
        printf("Could not unload %s.\n", dictionary);
        return 1;
    }

    // Calculate time to unload dictionary
    time_unload = calculate(&before, &after);

    // Report benchmarks
    printf("\nWORDS MISSPELLED:     %d\n", misspellings);
    printf("WORDS IN DICTIONARY:  %d\n", n);
    printf("WORDS IN TEXT:        %d\n", words);
    printf("TIME IN load:         %.2f\n", time_load);
    printf("TIME IN check:        %.2f\n", time_check);
    printf("TIME IN size:         %.2f\n", time_size);
    printf("TIME IN unload:       %.2f\n", time_unload);
    printf("TIME IN TOTAL:        %.2f\n\n",
           time_load + time_check + time_size + time_unload);

    // Success
    return 0;
}

// Returns number of seconds between b and a
double calculate(const struct rusage *b, const struct rusage *a)
{
    if (b == NULL || a == NULL)
    {
        return 0.0;
    }
    else
    {
        return ((((a->ru_utime.tv_sec * 1000000 + a->ru_utime.tv_usec) -
                  (b->ru_utime.tv_sec * 1000000 + b->ru_utime.tv_usec)) +
                 ((a->ru_stime.tv_sec * 1000000 + a->ru_stime.tv_usec) -
                  (b->ru_stime.tv_sec * 1000000 + b->ru_stime.tv_usec)))
                / 1000000.0);
    }
}

/*
dan@Dan-PC:/mnt/c/Users/Dan/xCS50/3. Problem Sets/Week 5/speller$ valgrind ./speller texts/lalaland.txt
==21579== Memcheck, a memory error detector
==21579== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==21579== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==21579== Command: ./speller texts/lalaland.txt
==21579== 

MISSPELLED WORDS

Chazelle
L
TECHNO
L
Thelonious
Prius
MIA
L
MIA
Mia
MIA
Mia
AHHHHHHHHHHHHHHHT
Mia
Mia
INT
Mia
MIA
Mia
Mia
Mia
Mia
Mia
Mia's
Mia
MIA
MIA
MIA
INT
Mia's
MIA
MIA
Turner's
Mia
Yoohoo
MIA
Mia
scurries
Mia
Mia
Mia
INT
Mia
INT
MIA'S
Mia
Ingrid
Uta
Mia
Mia
Mia
MIA
Cheetos
Mia
How'd
MIA
Jen
MIA
Jen
MIA
Mia
CAITLIN
Mia
Mia
MIA
O
S
Mia
Mia's
MIA
inbox
beelines
Mia's
MIA
MIA
Mia's
Mia
clich
Mia
clich
s
Caitlin
Mia
Mia
Mia
Caitlin
Mia
Mia
Caitlin
Mia
INT
Mia
D
J
clich
Mia
Mia
Mia
carousers
clich
s
Mia
MIA
O
S
Mia
MIA
Mia
AHHHHHHHHHHHHHHHHHHHHHHT
Mia's
RAYO'S
s
Deco
BEEK
TAPAS
INT
SEBASTIAN'S
Steinway
Sebastian's
tapas
Seb
tapas
Uh
long's
Shanghai'ed
Shanghai'ed
INT
SEBASTIAN'S
Thelonious
LP
INT
beelining
Seb
INT
Mia
Mia
Sebastian's
Mia
MIA
SEBASTIAN'S
Mia
Mia
Mia
Mia
MIA
Mia's
Mia
INT
Mia
MIA
GT
achromatopsia
MIA
D
O
perp
P
D
MIA
O
S
trippin'
MIA
Jamal
trippin'
Mia
s
Mia
Mia
Carlo's
Mia
MIA
y'all
MIA
Mia
Mia
INT
Mia
Mia
MIA
MIA
MIA
MIA
MIA
MIA
Uh
barista
Mia
smirks
MIA
MIA
Mia
Mia
lookin'
Mia
Warners
Mia
Charlize
MIA
MIA
MIA
MIA
Prius
Prius
MIA
Mia
Mia
MIA
MIA
n't
MIA
MIA
MIA
Mia
MIA
MIA
Uh
MIA
MIA
MIA
Mia
Mia
Mia
Mia
Mia
blossoming
Mia
Mia
MIA
K
Prius
MIA
MIA
MIA
Mia
INT
Mia's
MIA
Mia
MIA
MIA
Mia
MIA
MIA
Uh
How'd
Mia
Mia
MIA
Mia
fa
ade
MIA
Ingrid
Bogart's
Mia
MIA
MIA
MIA
MIA
Beek
tapas
MIA
tapas
joke's
Mia
L
P
P
MIA
C
Mia
D
O
S
Mia
How'd
MIA
O
S
Mia
MIA
MIA
soundstage
MIA
MIA
O
C
Should've
soundstages
MIA
MIA
MIA
MIA
mic
MIA
MIA
MIA
MIA
MIA
Mia
INT
Mia
MIA
G
Mia
MIA
facials
MIA
Mia
Mia
Pantages
Mia
MIA
Beek
music's
sizzles
Mia
Mia
INT
MIA
INT
Mia
Sebastian's
Mia
MIA
MIA
O
C
MIA
bulletsssss
Mia
MIA
MIA
me's
MIA
MIA
Mia
Mia
Mia
Mia
MIA
MIA'S
O
S
MIA
MIA'S
O
S
MIA
MIA'S
O
S
MIA
MIA'S
O
S
MIA
MIA'S
O
S
MIA
MIA'S
O
S
MIA
MIA'S
O
S
ABC
HBO
MIA
MIA'S
O
S
MIA
INT
Mia
Pfeiffer's
Mia
Mia
O
S
Mia
INT
Mia
Mia's
Mia
Mia
MIA
Kapish
Mia
MIA
Mia
INT
MIA'S
Mia
MIA'S
O
S
Mia
INT
MIA'S
Mia
Mia
Fritos
Mia
Mia
MIA
Mia
O
S
Mia
INT
Sebastian's
INT
Mia
Josh's
Fianc
e
Mia
Mia
Mia
INT
Mia
eco
Mia
Sebastian's
morphed
Mia
MIA
Mia's
INT
Mia
Sebastian's
Mia
INT
Mia
Sebastian's
Mia
Mia
Mia
MIA
Sebastian's
INT
Mia
Mia's
unspools
Mia
Mia
INT
MIA'S
Mia's
O
S
Mia
MIA
O
S
MIA
AHHHHHHHHHHHHHHHHHHHHHHHHHHHT
MIA
MIA'S
Mia
Sebastian's
MIA
O
S
Mia
Mia
s
Mia
BEEK
TAPAS
Mia
Mia
s
Deco
INT
Sebastian's
Mia
Mia
O
S
Mia
Mia
Sebastian's
Mia
Mia
Mia
MIA
Mia
how've
Mia
MIA
Mia
INT
SEBASTIAN'S
MIA
MIA
Mia's
MIA
MIA
MIA
MIA
Mia
MIA
MIA
Seb's
MIA
MIA
MIA
Beek
MIA
play's
Mia
MIA
MIA
MIA
MIA
MIA
MIA
MIA
Mia
INT
SEBASTIAN'S
Mia's
MIA
O
S
MIA
O
S
INT
Keith's
Sebastian's
Sebastian's
olds
INT
SEBASTIAN'S
Mia
Mia
INT
INT
Mia
INT
INT
Mia
INT
INT
Mia
INT
Sebastian's
INT
Mia
INT
YOUTUBE
Mia
INT
SEBASTIAN'S
Mia
INT
SEBASTIAN'S
Mia
INT
SEBASTIAN'S
INT
SEBASTIAN'S
Mia
INT
SEBASTIAN'S
Mia
INT
SEBASTIAN'S
INT
SEBASTIAN'S
Mia
MIA
INT
Mia
Mia
Mia
Mia
synth
Mia
bandmates
Mia
Mia
Mia
Mia's
INT
Mia
Mia
s
Mia
Mia
MIA
Mia
Seb
MIA
Mia
MIA
MIA
Seb
MIA
Mia
Seb
Mia
MIA
MIA
Mia
overthink
Mia
INT
Mia
e
MIA
INT
SEBASTIAN'S
Mia
sashaying
Mia
Mia's
Sebastian's
INT
SEBASTIAN'S
Mia
MIA
MIA
MIA
MIA
MIA
MIA
MIA
MIA
MIA
Mia
MIA
MIA
MIA
MIA
MIA
show's
MIA
MIA
Mia
Mia
MIA
MIA
MIA
MIA
MIA
MIA
MIA
MIA
MIA
MIA
MIA
MIA
MIA
MIA
Shangri
MIA
Mia
MIA
MIA
MIA
LP
Mia
Mia
MIA
Mia
MIA
Mia
INT
Mia
INT
ya
Sebastian's
Mojo
INT
INT
Mia
Mia
INT
band's
pantomiming
Photographer's
Mia's
INT
Mia
wallpapered
Mia
Mia
Mia
INT
Mia
O
S
O
S
O
S
O
S
Mia
Mia's
Mia
Mia
MIA
Unmoored
MIA
MIA
MIA
MIA
MIA
Mia
MIA'S
Mia
INT
MIA'S
Mia
INT
MIA'S
Mia
fianc
McKenzies
Sebastian's
INT
SEBASTIAN'S
O
S
Mia
O
S
Mia's
O
S
O
S
INT
MIA'S
Mia's
MIA'S
MIA
Mia
MIA'S
MIA
MIA'S
MIA
Mia's
MIA'S
Mia
MIA
MIA'S
MIA'S
MIA
MIA'S
Mia's
Mia
Mia's
MIA'S
Mia's
SEBASTIAN'S
Mia
MIA'S
SEBASTIAN'S
Mia
MIA
MIA
MIA
Mia
MIA
MIA
MIA
MIA
MIA
MIA
lookalikes
MIA
MIA
MIA
MIA
MIA
Mia
MIA
Mia
MIA
Mia
MIA'S
MIA'S
Sebastian's
Mia
Mia
Mia
Deco
soundstages
INT
Mia
Mia
Mia
INT
INT
MIA
Mia
Mia
MIA
MIA
Mia
Mia
Mia's
MIA
Mia's
Mia
Mia
Mia
Mia
MIA
MIA
MIA
MIA
MIA
MIA
MIA
Mia
Mia
MIA
Mia's
MIA
MIA
MIA
Mia
L
fa
ades
INT
MIA
MIA
BARISTA
Mia's
Mia
Mia
MIA
Mia
Barista
Mia
INT
s
Carson's
How'd
MARMONT
Mia
INT
MARMONT
Mia
Mia's
INT
SEBASTIAN'S
INT
SEBASTIAN'S
MIA
INT
Seb
Seb
Edgar's
INT
MARMONT
Mia
Mia
MIA
Mia
INT
Mia
MIA
INT
Mia
Mia
MIA
Mia
s
Deco
music's
Mia
Mia
SEB'S
Mia
Mia
INT
Mia's
Sebastian's
Sebastian's
Carmichael's
Mia
Mia
Manny
Halloran
Mia
Seb's
Mia
Mia
Mia
Sebastian's
Mia
Mia
Mia
Mia
MIA
Mia
Mia
INT
Mia
INT
INT
Mia
Mia's
INT
SOUNDSTAGE
Mia
L
L
L
Mia
figments
s
s
Mia
Mia
Mia
Mia
Mia
Sacr
Triomphe
CAVEAU
HUCHETTE
Caveau
Mia
Caveau
Mia
Mia
Mia's
Mia
L
Sebastian's
combo's
Mia
Sebastian's
Mia
Mia
Mia
MIA
Mia
Mia
Mia
Sebastian's
L

WORDS MISSPELLED:     955
WORDS IN DICTIONARY:  143091
WORDS IN TEXT:        17756
TIME IN load:         1.36
TIME IN check:        2.51
TIME IN size:         0.00
TIME IN unload:       0.04
TIME IN TOTAL:        3.92

==21579== 
==21579== HEAP SUMMARY:
==21579==     in use at exit: 0 bytes in 0 blocks
==21579==   total heap usage: 143,096 allocs, 143,096 frees, 8,023,256 bytes allocated
==21579== 
==21579== All heap blocks were freed -- no leaks are possible
==21579== 
==21579== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
*/