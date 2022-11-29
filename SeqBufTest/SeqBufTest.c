#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <assert.h>
#include <malloc.h>
#include <pthread.h>
#include <semaphore.h>
#include <Windows.h>

#define SEQ_BUFFER_STRING_MAX   (32)
#define SEQ_SEARCH_RADIUS_MAX   (50)

// Set to 0 to disable console output
// Set to 1 for logging mutex activity on console
// Set to 2 for enhanced debugging / printing the linked list before and after each operation
#define DEBUG                   (0) 

static uint32_t g_done = 0;

// Doubly Linked List Node
//ASSUMPTION: sequences are ints > 0
//ASSUMPTION: sequnece always starts at the lowest number (1)
typedef struct Node_s
{
    struct Node_s* next;        // points to next node in the list
    struct Node_s* prev;        // points to previous node in the list
    uint32_t seq;
    char seqText[SEQ_BUFFER_STRING_MAX];
} Node_t;

// Sequence Buffer definition
//ASSUMPTION: Sequences are not more than 5 out of order (it seems they are)
typedef struct SequenceBuffer_s
{
    //double-linked list to store sequence items
    Node_t* head;
    Node_t* last;
    Node_t* current;

    uint32_t maxSeq;
    pthread_mutex_t mutex;

} SequenceBuffer_t;


// Print the list
static void PrintList(Node_t* list)
{
    uint32_t count = 0;
    while (list != NULL)
    {
        printf("Node %i [#%s]: me(%p), next(%p), prev(%p)\n", count, list->seqText, list, list->next, list->prev);
        list = list->next;

        ++count;
    }
    #if DEBUG > 2
    printf("Printed %d nodes\n", count);
    #endif
}


// TODO: implement this function
static void SequenceBuffer_Init(SequenceBuffer_t* seqBuf)
{
    seqBuf->maxSeq = 0;
    seqBuf->mutex = PTHREAD_MUTEX_INITIALIZER;

    int t;
    t = pthread_mutex_init(&seqBuf->mutex, NULL);
    printf("Initialized the mutex returned %d \n", t);
    seqBuf->last = seqBuf->head = seqBuf->current = NULL;
}

// TODO: implement this function
//non-blocking thread safe
static void SequenceBuffer_Push(SequenceBuffer_t* seqBuf, const char* string, uint32_t seq)
{
    //Sanity check that seqBuf is not null
    if (seqBuf == NULL)
    {
        printf("ERROR: SeqBuf in SequenceBuffer_Push() was NULL!");
        return;
    }

    int t = pthread_mutex_lock(&seqBuf->mutex);
    while (t == EBUSY)
    {
        #if DEBUG > 0
            printf("Thread is busy at sequence %d \n", seq);
        #endif // DEBUG
        Sleep(50);
    }

    #if DEBUG > 1 
    printf("BEGIN PUSH() FUNCTION \n");
    printf("Head %p | Last %p | Current %p | MaxSeq %d\n", seqBuf->head, seqBuf->last, seqBuf->current, seqBuf->maxSeq);
    PrintList(seqBuf->head);
    #endif // DEBUG LINKED LIST



    #if DEBUG > 0
    printf("Sequence %d locked mutex with code %d \n", seq, t);
    #endif // DEBUG

    //if this is the first item in our list, set head and set list to 1st element
    if (seqBuf->last == NULL)
    {
        seqBuf->last = seqBuf->head = (Node_t*)malloc(sizeof(Node_t));
        seqBuf->last->prev = NULL;                  //the head node has no previous by definition
    }
    else
    {
        //add the next item onto the list and increment list to next node
        seqBuf->last->next = (Node_t*)malloc(sizeof(Node_t));
        seqBuf->last->next->prev = seqBuf->last;    //set the previous of the next node to the current node
        seqBuf->last = seqBuf->last->next;          //increment the current node
    }

    //finally, assign the new last sequence to the text
    seqBuf->last->seq = seq;
    strcpy(seqBuf->last->seqText, string);

    //set the next item to NULL
    seqBuf->last->next = NULL;
    
    //demonstration of ternary operator
    //update maxSeq if current sequence is greater than max
    seqBuf->maxSeq = (seq > seqBuf->maxSeq) ? seq : seqBuf->maxSeq;

    #if DEBUG > 0       //mutex troubleshooting
    printf("Sequence %d unlocked mutex with code %d \n", seq, t);
    printf("Added sequence %d with text %s \n", seq, string);
    #endif //DEBUG MUTEX

    #if DEBUG > 1 
    printf("AFTER PUSH() FUNCTION \n");
    printf("Head %p | Last %p | Current %p | MaxSeq %d\n", seqBuf->head, seqBuf->last, seqBuf->current, seqBuf->maxSeq);
    PrintList(seqBuf->head);
    #endif // DEBUG LINKED LIST

    t = pthread_mutex_unlock(&seqBuf->mutex);
    if (t != 0)
    {
        printf("ERROR: Unlock mutex failed at SequenceBuffer_Push() returned code %d", t);
        return;
    }
}

//a helper function 
//RETRUNS: a pointer to the next item in the sequence (or null if not found)
static Node_t* SearchNextNode(Node_t* list, const int expectedResult, const int maxSearchRadius)
{
    Node_t* forward  = list->next;
    Node_t* backward = list->prev;

    //search forwards and backwards simultaniously
    for (int i = 0; i < maxSearchRadius; i++)
    {
        //check backwards
        if (backward != NULL)
        {
            if (backward->seq == expectedResult)
                return backward;

            //go back a node and repeat
            backward = backward->prev;
        }

        //check forwards
        if (forward != NULL)
        {
            if (forward->seq == expectedResult)
                return (forward);

            //go forwards a node and repeat
            forward = forward->next;
        }

        if (backward == NULL && forward == NULL)
            break; //end the search, this node does not exist
    }

    //if we made it here it means we exhausted our search, but didn't find the next item in the sequence
    return NULL;
}

// TODO: implement this function
// blocking thread-safe
static void SequenceBuffer_Pop(SequenceBuffer_t* seqBuf, char outputString[SEQ_BUFFER_STRING_MAX])
{
    //Sanity check that seqBuf is not null
    if (seqBuf == NULL)
    {
        printf("ERROR: SeqBuf in SequenceBuffer_Pop() was NULL!");
        return;
    }

    //sanity check that current is not null
    //if it is, try to retieve the very first item
    if (seqBuf->current == NULL)
    {
        //and we check we have at least 1 item
        if (seqBuf->head != NULL)
        {
            //retrieve the first item
            pthread_mutex_lock(&seqBuf->mutex);
            seqBuf->current = seqBuf->head;
            strcpy(outputString, seqBuf->current->seqText);
            pthread_mutex_unlock(&seqBuf->mutex);
        }

        //if current is null, and head is null, this means our list is empty.
        //wait for next sequence to be added
        return;
    }

    strcpy(outputString, "");
    int t = pthread_mutex_lock(&seqBuf->mutex);
    //printf("Locked pop with mutex returned code %d \n", t);
    if (t != 0)
        printf("ERROR: Unable to lock mutex in SequenceBuffer_Pop() with error code %d \n", t);


    if (seqBuf->current != NULL)
    {
        //if there aren't any more nodes since the last node reported, just yield to the OS.
        while (seqBuf->current->seq == seqBuf->maxSeq)
        {
            t = pthread_mutex_unlock(&seqBuf->mutex);
            if (t != 0)
                printf("ERROR: Unable to lock mutex in SequenceBuffer_Pop() with error code %d \n", t);

            t = sched_yield(); //try yielding to see if that changes the situation
            Sleep(50);

            #if DEBUG > 1
            printf("Scheduled yield returned code %d \n", t);
            #endif // DEBUG

            //now relock the mutex and proceed:

            int t = pthread_mutex_lock(&seqBuf->mutex);
            if (t != 0)
                printf("ERROR: Unable to unlock mutex in SequenceBuffer_Pop() after yield with error code %d \n", t);

        }// END WHILE


        //at this point we know theres a node greater than current, but we don't know if its valid
        //use the search helper function to try to find the location of the next node
        int expectedNode = seqBuf->current->seq + 1;
        Node_t* nextSeqNode = SearchNextNode(seqBuf->current, expectedNode, SEQ_SEARCH_RADIUS_MAX);

        //at this point, current either is the location of the next sequence
        //or the next sequence simply does not exist yet...
        if (nextSeqNode != NULL)
        {
            //set the current node to the node we found
            seqBuf->current = nextSeqNode;

            //copy the result to the string
            strcpy(outputString, seqBuf->current->seqText);
        }
        else
        {
            #if DEBUG > 1
            printf("We searched the array and were not able to find the next sequence \n");
            printf("Going to yield until the next node shows up. Below is a dump of the linked list \n");
            printf("Head %p | Last %p | Current %p | MaxSeq %d\n", seqBuf->head, seqBuf->last, seqBuf->current, seqBuf->maxSeq);
            PrintList(seqBuf->head);
            #endif // DEBUG
        }

    } //END IF


    //unlock the mutex and report errors to console
    t= pthread_mutex_unlock(&seqBuf->mutex);
    if (t != 0)
    {
        printf("ERROR: Problem unlocking mutex in SequenceBuffer_Pop() returned with code %d \n", t);
    }

    return;
}

// return a random number in the specified range
static uint32_t RandomInRange(uint32_t lower, uint32_t upper)
{
    return rand() % (upper - lower + 1) + lower;
}

// Push items into the sequence buffer
void* PushThread(void* arg)
{
    SequenceBuffer_t* seq_buf = (SequenceBuffer_t*)arg;

    int32_t out_of_order_retry = 0;
    int32_t out_of_order_seq = 0;
    uint32_t count = 1;
    uint32_t prev_seq = 1;
    while (!g_done)
    {
        uint32_t seq = count;

        if (out_of_order_retry == 0)
        {
            if (out_of_order_seq != 0)
            {
                seq = out_of_order_seq;
                out_of_order_seq = 0;
                --count;
            }
            else
            {
                //20% chance out of order
                uint32_t percent = RandomInRange(0, 100);
                if (percent > 80)
                {
                    out_of_order_seq = seq;
                    out_of_order_retry = rand() % 5;

                    ++seq;
                    ++count;
                }
                else
                {
                    // 20% duplicate
                    uint32_t percent = RandomInRange(0, 100);
                    if (percent > 80)
                    {
                        seq = prev_seq;
                        --count;
                    }
                }
            }
        }

        char item_string[32];
        sprintf(item_string, "item%i", seq);

        SequenceBuffer_Push(seq_buf, item_string, seq);

        prev_seq = seq;
        if (out_of_order_retry > 0)
        {
            --out_of_order_retry;
        }
        else
        {
            ++count;
        }

        Sleep(50);
    }

    return 0;
}

// Pop items from the sequence buffer
void* PopThread(void* arg)
{
    SequenceBuffer_t* seq_buf = (SequenceBuffer_t*)arg;

    char string[SEQ_BUFFER_STRING_MAX] = { 0 };
    while (!g_done)
    {
        SequenceBuffer_Pop(seq_buf, string);
        printf("%s\n", string);
        Sleep(1);
    }

    return 0;
}

int main()
{
    /*
    * Given a shared sequence buffer between two threads, Thread 1 will be inserting items into the sequence buffer (starting with sequence 1), and Thread 2 will be 
    * receiving items from the sequence buffer.
    * Thread1 will insert items which might be out of order or duplicated.  An item will never be more than 5 elements out of order.  No sequences will be dropped.
    * Thread 2 will only receive items in exact sequence order
    *
    * Both Thread1 and Thread2 will run indefinitly until the user presses the ESC key.
    *
    * Implement the functions SequenceBuffer_Init(), SequenceBuffer_Push() and SequenceBuffer_Pop().
    *
    * SequenceBuffer_Init() will initialize a SequenceBuffer_t object.
    *
    * SequenceBuffer_Push() is a non-blocking thread-safe function which shall accept a message (w/ max string length of SEQ_BUFFER_STRING_MAX) and the associated sequence number.
    *
    * SequenceBuffer_Pop() shall be a blocking thread-safe function and shall populate the output string w/ the message of the next sequence in order.
    *
    * You may create additional structures, functions, etc. as needed.  I've included the pthreads library for you to use as needed.
    *
    * If you encounter any corner cases which aren't covered by the requirements above, you may make an assumption and implement the desired behavior.  Please document these assumptions.
    *
    * Nominal example:
    * Thread1:
    *   SequenceBuffer seqBuf;
    *   SequenceBuffer_Push(&seqBuf, "item1", 1);
    *   SequenceBuffer_Push(&seqBuf, "item2", 2);
    *   SequenceBuffer_Push(&seqBuf, "item3", 3);
    *   SequenceBuffer_Push(&seqBuf, "item4", 4);
    *   SequenceBuffer_Push(&seqBuf, "item4", 4);   // duplicate
    *   SequenceBuffer_Push(&seqBuf, "item6", 6);   // out of order
    *   SequenceBuffer_Push(&seqBuf, "item5", 5);   // out of order
    *   SequenceBuffer_Push(&seqBuf, "item7", 7);
    *
    * Thread2:
    *
    *   Expected Output:
    *       "item1"
    *       "item2"
    *       "item3"
    *       "item4"
    *       "item5"
    *       "item6"
    *       "item7"
    */

    pthread_t thread1;
    pthread_t thread2;
    pthread_t thread3;
    pthread_t thread4;
    char* output[SEQ_BUFFER_STRING_MAX] = { 0 };

    SequenceBuffer_t seq_buf;
    SequenceBuffer_Init(&seq_buf);

    int32_t ret1 = pthread_create(&thread1, NULL, PushThread, &seq_buf);
    assert(ret1 == 0);

    int32_t ret2 = pthread_create(&thread2, NULL, PopThread, &seq_buf);
    assert(ret2 == 0);

    //int32_t ret3 = pthread_create(&thread3, NULL, PushThread, &seq_buf);
    //assert(ret3 == 0);

    //int32_t ret4 = pthread_create(&thread4, NULL, PopThread, &seq_buf);
    //assert(ret4 == 0);

    // done
    printf("\nPress ESC to finish...\n");

    while (1)
    {
        if (GetAsyncKeyState(VK_ESCAPE) & 0x1)
        {
            g_done = 1;
            break;
        }
    }

    printf("\nPress any key to continue...");

    char c;
    scanf("%c", &c);
}
