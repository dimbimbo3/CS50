// Simulate genetic inheritance of blood type

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Each person has two parents and two alleles
typedef struct person
{
    struct person *parents[2];
    char alleles[2];
} person;

const int GENERATIONS = 3;
const int INDENT_LENGTH = 4;

person *create_family(int generations);
void print_family(person *p, int generation);
void free_family(person *p);
char random_allele();
char parent_allele(person*);

int main(void)
{
    // Seed random number generator
    srand(time(0));

    // Create a new family with three generations (starts from the newest generation child)
    person *p = create_family(GENERATIONS);

    // Print family tree of blood types
    print_family(p, 0);

    // Free memory
    free_family(p);
}

/**
 * Create a new individual with `generations`
*/
person *create_family(int generations)
{
    // Allocate memory for new person
    person* newPerson = malloc(sizeof(person));

    // If there are still generations left to create
    if (generations > 1)
    {
        // Create two new parents for current person by recursively calling create_family
        person *parent0 = create_family(generations - 1);
        person *parent1 = create_family(generations - 1);

        // Set parent pointers for current person
        newPerson->parents[0] = parent0;
        newPerson->parents[1] = parent1;

        // Randomly assign current person's alleles based on the alleles of their parents
        newPerson->alleles[0] = parent_allele(newPerson->parents[0]);
        newPerson->alleles[1] = parent_allele(newPerson->parents[1]);
    }

    // If there are no generations left to create (this is the oldest generation)
    else
    {
        // Set parent pointers to NULL
        newPerson->parents[0] = NULL;
        newPerson->parents[1] = NULL;

        // Randomly assign alleles
        newPerson->alleles[0] = random_allele();
        newPerson->alleles[1] = random_allele();
    }

    // Return newly created person
    return newPerson;
}

/**
 * Print each family member and their alleles.
*/
void print_family(person *p, int generation)
{
    // Handle base case
    if (p == NULL)
    {
        return;
    }

    // Print indentation
    for (int i = 0; i < generation * INDENT_LENGTH; i++)
    {
        printf(" ");
    }

    // Print person
    if (generation == 0)
    {
        printf("Child (Generation %i): blood type %c%c\n", generation, p->alleles[0], p->alleles[1]);
    }
    else if (generation == 1)
    {
        printf("Parent (Generation %i): blood type %c%c\n", generation, p->alleles[0], p->alleles[1]);
    }
    else
    {
        for (int i = 0; i < generation - 2; i++)
        {
            printf("Great-");
        }
        printf("Grandparent (Generation %i): blood type %c%c\n", generation, p->alleles[0], p->alleles[1]);
    }

    // Print parents of current generation
    print_family(p->parents[0], generation + 1);
    print_family(p->parents[1], generation + 1);
}

/**
 * Free `p` and all ancestors of `p`.
*/
void free_family(person *p)
{
    // Handle base case (pointer to the oldest generation's parent has been reached)
    if(p == NULL)
    {
        return;
    }

    // Free parents recursively
    free_family(p->parents[0]);
    free_family(p->parents[1]);

    // Free the given child
    free(p);
}

/**
 * Randomly chooses a blood type allele.
*/
char random_allele()
{
    int r = rand() % 3;
    if (r == 0)
    {
        return 'A';
    }
    else if (r == 1)
    {
        return 'B';
    }
    else
    {
        return 'O';
    }
}

/**
 * Randomly selects one of a parent's blood type alleles.
*/
char parent_allele(person* parent)
{
    int r = rand() % 2; //randomly select 0 or 1
    if (r == 0)
    {
        return parent->alleles[0];
    }
    else
    {
        return parent->alleles[1];
    }
}

/*
Child (Generation 0): blood type AB
    Parent (Generation 1): blood type OA
        Grandparent (Generation 2): blood type OA
        Grandparent (Generation 2): blood type AA
    Parent (Generation 1): blood type BO
        Grandparent (Generation 2): blood type AB
        Grandparent (Generation 2): blood type AO
*/

/*
==11210== Memcheck, a memory error detector
==11210== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==11210== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==11210== Command: ./inheritance
==11210== 
Child (Generation 0): blood type AA
    Parent (Generation 1): blood type AB
        Grandparent (Generation 2): blood type OA
        Grandparent (Generation 2): blood type OB
    Parent (Generation 1): blood type BA
        Grandparent (Generation 2): blood type OB
        Grandparent (Generation 2): blood type AB
==11210== 
==11210== HEAP SUMMARY:
==11210==     in use at exit: 0 bytes in 0 blocks
==11210==   total heap usage: 8 allocs, 8 frees, 1,192 bytes allocated
==11210== 
==11210== All heap blocks were freed -- no leaks are possible
==11210== 
==11210== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
*/