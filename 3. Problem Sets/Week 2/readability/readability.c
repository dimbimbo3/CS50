//#include <cs50.h>
#include <stdio.h>
#include <string.h> //strlen
#include <ctype.h> //isalpha, ispunct
#include <math.h> //round

#define BUFFER 1024 //max allowable number of characters (including null terminator)

int count_letters(char[], int);
int count_words(char[], int);
int count_sentences(char[], int);

int main(void) 
{
    char text[BUFFER];
    int letters, words, sentences;
    float L, S;
    int index;

    printf("Text: ");
    //gets user input and if valid then proceeds with calculations
    if(fgets(text, BUFFER, stdin) != NULL){
        int length = strlen(text); //input length
        letters = count_letters(text, length); //letter total
        words = count_words(text, length); //word total
        sentences = count_sentences(text, length); //sentence total

        L = (float) letters / words * 100; //calculates the average number of letters per 100 words
        S = (float) sentences / words * 100; //calculates the average number of sentences per 100 words
        index = round(0.0588 * L - 0.296 * S - 15.8); //calculates the Coleman-Liau index
        
        //prints reading level determined by index
        if(index < 1)
            printf("Before Grade 1\n");
        else if(index >= 16)
            printf("Grade 16+\n");
        else
            printf("Grade %i\n", index);
    }
}

/**
 * Iterates through the given string to determine the number of letters.
*/
int count_letters(char input[], int len){
    int letters = 0;

    for(int i = 0; i < len; i++){
        if(isalpha(input[i])){
            letters++;
        }
    }

    return letters;
}

/**
 * Iterates through the given string to determine the number of words.
*/
int count_words(char input[], int len){
    int words = 0;

    for(int i = 0; i < len; i++){
        //if this is is a space then assume a word must have preceeded it
        if(isspace(input[i])){
            words++;
        }
    }

    return words;
}

/**
 * Iterates through the given string to determine the number of sentences.
*/
int count_sentences(char input[], int len){
    int sentences = 0;

    for(int i = 0; i < len; i++){
        //if this is an ending punctuation mark 
        //then consider it to indicate the end of a sentence
        if((input[i] == '.' || input[i] == '?' || input[i] == '!')){
            sentences++;
        }
    }

    return sentences;
}

/*
Text: One fish. Two fish. Red fish. Blue fish.
Before Grade 1
*/

/*
Text: When he was nearly thirteen, my brother Jem got his arm badly broken at the elbow. When it healed, and Jem's fears of never being able to play football were assuaged, 
he was seldom self-conscious about his injury. His left arm was somewhat shorter than his right; when he stood or walked, the back of his hand was at right angles to his body, 
his thumb parallel to his thigh.
Grade 8
*/

/*
A large class of computational problems involve the determination of properties of graphs, digraphs, integers, arrays of integers, 
finite families of finite sets, boolean formulas and elements of other countable domains.
Grade 16+
*/