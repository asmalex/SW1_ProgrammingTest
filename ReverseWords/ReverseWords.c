#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <assert.h>
#include <malloc.h>

//TODO: implement this function '
//ASSUMPTION: the sentence punctuation at the end needs to stay at the end
//ASSUMPTION: no double spaces allowed. Use trim() to eliminate
//ASSUMPTION: only special character is . ? or ! for end of sentence.
static void ReverseWords(char* string)
{
    //split the words by whitespace
    //give them a number
    //put back the number in reverse order. 

    int size            = strlen(string);
    int numSpace        = 0;

    //result string - add one to size because the strlen function does not count the null terminator
    int* arrSpaceIdx    = (int*)malloc(size * sizeof(int));
    char* result        = (char*)malloc(size + 1); 

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

        //when a space is found, modify the pointer sliding window
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

            //increment number of spaces found, and set the last space to the current index
            numSpace++;
            lastSpace = j;
        }
    }

    //reassign
    memcpy(string, result, size);


    //release memory
    free(arrSpaceIdx);
    free(result);

    return;
}

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
