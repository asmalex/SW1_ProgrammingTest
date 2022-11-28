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

static uint32_t g_done = 0;

// Doubly Linked List Node
typedef struct Node_s
{
    struct Node_s* next;        // points to next node in the list
    struct Node_s* prev;        // points to previous node in the list
    uint32_t seq;
    char seqText[SEQ_BUFFER_STRING_MAX];
} Node_t;

// Sequence Buffer definition
//ASSUMPTION: Sequences are more-or-less in order. (not more than 5 out of order)
typedef struct SequenceBuffer_s
{
    // TODO: add members as needed here
    // TODO: add a mutex

    //double-linked list to store sequence items
    Node_t* head;
    Node_t* list;

    uint32_t maxSeq;
    char tmp;
    pthread_mutex_t mutex;

} SequenceBuffer_t;



// TODO: implement this function
static void SequenceBuffer_Init(SequenceBuffer_t* seqBuf)
{
    seqBuf->maxSeq = 0;
    seqBuf->mutex = PTHREAD_MUTEX_INITIALIZER;

    int t;
    t = pthread_mutex_init(&seqBuf->mutex, NULL);
    printf("Initialized the mutex returned %d \n", t);
    seqBuf->list = seqBuf->head = NULL;
}

// TODO: implement this function
//non-blocking thread safe
static void SequenceBuffer_Push(SequenceBuffer_t* seqBuf, const char* string, uint32_t seq)
{

    int t = pthread_mutex_trylock(&seqBuf->mutex);
    while (t == EBUSY)
    {
        //do something here
        printf("Thread is busy at sequence %d \n", seq);
    }

    printf("Sequence %d locked mutex with code %d \n", seq, t);

    //if this is the first item in our list, set head and set list to 1st element
    if (seqBuf->list == NULL)
        seqBuf->list = seqBuf->head = (Node_t*)malloc(sizeof(Node_t));
    else
    {
        //push the next item onto the list and increment list to next node
        seqBuf->list->next = (Node_t*)malloc(sizeof(Node_t));
        seqBuf->list = seqBuf->list->next;
    }

    //finally, assign the sequence at the text
    seqBuf->list->seq = seq;
    strcpy(seqBuf->list->seqText, string);

    //set the next item to NULL
    seqBuf->list->next = NULL;
    //FIX THIS
    seqBuf->list->prev = NULL;
    
    //demonstration of ternary operator
    //update maxSeq if current sequence is greater than max
    seqBuf->maxSeq = (seq > seqBuf->maxSeq) ? seq : seqBuf->maxSeq;

    t = pthread_mutex_unlock(&seqBuf->mutex);
    printf("Sequence %d unlocked mutex with code %d \n", seq, t);
}

// TODO: implement this function
// blocking thread-safe
static void SequenceBuffer_Pop(SequenceBuffer_t* seqBuf, char outputString[SEQ_BUFFER_STRING_MAX])
{
    //lock the buffer
    //sort the sequences

    //to start: just pop off the linked list in order.


    int t = pthread_mutex_lock(&seqBuf->mutex);
    printf("Locked pop with mutex returned code %d \n", t);

    if (seqBuf->head == NULL)
    {
        t = pthread_mutex_unlock(&seqBuf->mutex);
        printf("Unlocked pop with code %d \n", t);
        return;
    }

    strcpy(outputString, seqBuf->head->seqText);
    seqBuf->head->next;

    t= pthread_mutex_unlock(&seqBuf->mutex);
    printf("Unlocked pop with code %d \n", t);

    //perhaps to start we can begin a loop - but how do we know what the largest number is?
    //

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

    SequenceBuffer_t seq_buf;
    SequenceBuffer_Init(&seq_buf);

    int32_t ret1 = pthread_create(&thread1, NULL, PushThread, &seq_buf);
    assert(ret1 == 0);

    int32_t ret2 = pthread_create(&thread2, NULL, PopThread, &seq_buf);
    assert(ret2 == 0);

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
