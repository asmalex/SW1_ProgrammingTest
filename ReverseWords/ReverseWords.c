#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <assert.h>
#include <malloc.h>
#include "ccan/lstack/lstack.h" //standard C library of a linked stack

struct nextChar
{
    char *myChar;
    struct lstack_link sl;
};

// TODO: implement this function '
//ASSUMPTION: the sentence punctuation at the end needs to stay at the end
//ASSUMPTION: no double spaces allowed. Use trim() to eliminate
//ASSUMPTION: only special character is . ? or ! for end of sentence.
static void ReverseWords(char* string)
{
    //split the words by whitespace
    //give them a number
    //put back the number in reverse order. 

    int size = strlen(string);
    int numSpace = 0;

    int* arrSpaceIdx = (int*)malloc(size * sizeof(int));
    char* result = (char*)malloc(size);

    //finds all the spaces in the string and stores their index in the arrSpaceIdx array
    for (int j = size-1, lastSpace = size-1,i=0; j>=0; j--)
    {
        if (string[j] == ' ')
        {
            arrSpaceIdx[numSpace] = j;

            //copy from now until lastspace
            memcpy(result+i, string+j, lastSpace - j);
            i += lastSpace - j;

            numSpace++;
            lastSpace = j;
        }
    }

    //numSpace--;
    //string copy arrSpaceIdx backwards to next index
    for (int j = numSpace-1, endIdx = size; j >= 0; j--)
    {
        //grab the index of the last 
        char* src = string+arrSpaceIdx[j]+1; //address of string + index # of bytes forward
        memcpy(result, src, endIdx - arrSpaceIdx[j]);
    }

    //copy the first word to end
    memcpy(result+arrSpaceIdx[numSpace-1], string, arrSpaceIdx[0]);

    //reassign
    strcpy(string, result);


    //FIX mem leak
    //free(result);

}



/*

   struct nextChar *nextCharInStack;
    LSTACK(struct myStack, sl) stack;

    //O(n) push items all items character-by-character onto the stack
    for (int i=0; i < mySize; i++)
    {
        lstack_push(&stack, string[i]);

        //ASSUMPTION: punctuation stays with the word its paired with.
        //  otherwise, implement exceptions above
    }

    char* result = malloc(sizeof(*string));

    for (int i = 0; i < mySize; i++)
    {
        result[i] = lstack_pop(&stack);
    }



*/



int main()
{
    /*
    Implement the function ReverseWords() which reverses the words in a sentence.

    input: a character string consisting of words separated by whitespace
    output: the same string, with the words reversed

    Example:
    (input) "The quick brown fox jumps over the lazy dog"
    (output) "dog lazy the over jumps fox brown quick The"


    Note: when implementing, please consider other inputs other than the example string provided.
    */
    char string[] = "The quick brown fox jumps over the lazy dog";
    printf("Original String: %s\n", string);

    ReverseWords(string);
    printf("Reversed String: %s\n\n\n", string);

    printf("\nPress any key to continue...");

    char c;
    scanf("%c", &c);
}
