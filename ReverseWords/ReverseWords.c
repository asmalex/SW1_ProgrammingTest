#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <assert.h>
#include <malloc.h>
#include "ccan/lstack/lstack.h" //standard C library of a linked stack

struct word
{
    char *substring;
    struct lstack_link sl;
};

//TODO: implement this function '
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

    LSTACK(struct word, sl) stack;

    //finds all the spaces in the string and stores their index in the arrSpaceIdx array
    for (int i = 0, j = 0, lastIdx=0; i < size; i++)
    {
        if (string[i] == ' ')
        {
            arrSpaceIdx[j] = i;     //store the whitespace index in the array
            numSpace = ++j;         //increment j and assign to numSpace

            struct word* nextWord;
            nextWord = (struct word*)malloc(sizeof(*nextWord));
            nextWord->substring = (char*)malloc(sizeof(char) * (i - lastIdx));
            strncpy(nextWord->substring, string + lastIdx, i - lastIdx);

            //add the node to the stack
            lstack_push(&stack, static_cast<word*>nextWord);
        }
    }

    //loop through the string and pop the items onto a string





}



/*





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
