#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <assert.h>
#include <malloc.h>

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
    for (int j = size, lastSpace = size,i=0; j>=0; j--)
    {
        //if we've arrived at the first word, it becomes the last.
        if (j == 0)
        {
            //copy the last word over and null terminate
            memcpy(result + i, string + j, lastSpace - j);
            result[size] = '\0'; //append the end string character
        }

        else if (string[j] == ' ')
        {
            arrSpaceIdx[numSpace] = j;
            int bytesToCopy = lastSpace - j - 1;

            //copy from here until last space
            //we add j+i to string memory address because we want the j-th word (and +1 to skip the space)
            //we add i to the string memory address because we want to copy this into the i-th position
            //we subtract 1 from the bytesToCopy because we are not copying the whitespace at the beginning, we are adding it to the end
            memcpy(result+i, string+j+1, bytesToCopy);
            result[i + bytesToCopy] = ' ';
            i += lastSpace - j;

            numSpace++;
            lastSpace = j;
        }
    }

    //numSpace--;
    //string copy arrSpaceIdx backwards to next index
    //for (int j = numSpace-1, endIdx = size; j >= 0; j--)
    //{
        //grab the index of the last 
    //    char* src = string+arrSpaceIdx[j]+1; //address of string + index # of bytes forward
    //    memcpy(result, src, endIdx - arrSpaceIdx[j]);
    //}

    //copy the first word to end
    //if (numSpace != 0)
    //    memcpy(result + arrSpaceIdx[numSpace - 1], string, arrSpaceIdx[0]);
    //else
    //    memcpy(result, string, size+1); //there are no spaces, copy whole word

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
