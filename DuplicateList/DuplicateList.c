//|---------------------------------------------------------------------------|
//|    FILE NAME: DuplicateList.c                                             |
//|                                                                           |
//|    AUTHOR   : Alex Redei, SNC Programming Test                            |
//|                                                                           |
//|    PURPOSE  : Deep-copy of a linked list, preserving references           |
//|                                                                           |
//|    NOTES    : ASSUMES: this is not a circularly listed list               |
//|                                                                           |
//|    REVISIONS:                                                             |
//|			   11/22/22 - A. Redei - Initial Implementation                   |
//|			   11/23/22 - A. Redei - Improved Alternate implementation w/ zip |
//|---------------------------------------------------------------------------|
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <assert.h>
#include <malloc.h>

// Linked List Node
typedef struct Node_s
{
    struct Node_s* next;        // points to next node in the list
    struct Node_s* reference;   // points to random node in the list
} Node_t;

// Generate a random linked list
static Node_t* GenerateList(uint32_t numNodes)
{
    Node_t* head = (Node_t*)malloc(sizeof(Node_t));
    assert(head != NULL);
    head->reference = NULL;
    head->next = NULL;

    Node_t* cursor = head;
    for (uint32_t i = 0; i < (numNodes - 1); ++i)
    {
        Node_t* node = (Node_t*)malloc(sizeof(Node_t));
        assert(node != NULL);

        node->next = NULL;
        node->reference = NULL;

        cursor->next = node;
        cursor = node;
    }

    cursor = head;
    while (cursor != NULL)
    {
        // find a random node in the list
        uint32_t r = (uint32_t)(rand() % numNodes);
        assert(r < numNodes);

        Node_t* ref = head;
        for (uint32_t i = 0; i < r; ++i)
        {
            ref = ref->next;
            assert(ref != NULL);
        }

        cursor->reference = ref;
        cursor = cursor->next;
    }

    return head;
}

// Print the list
static void PrintList(Node_t* list)
{
    uint32_t count = 0;
    while (list != NULL)
    {
        printf("Node %i: me(%p), next(%p), ref(%p)\n", count, list, list->next, list->reference);
        list = list->next;

        ++count;
    }
}

//TODO: implement this function
//ASSUMPTION: This is not a circular linked list. The last node MUST have a next pointer of null.
static Node_t* DuplicateList(Node_t* list)
{
    //preserve the head node
    Node_t* head = list;

    //sanity check we don't violate memory
    if (list == NULL)
        return NULL;

    //solve the trivial problem of copying the nodes
    //place the nodes inline since we want to preserve the reference pointer
    while (list != NULL)
    {
        //create a copy node
        Node_t* new_list = (Node_t*)malloc(sizeof(Node_t));
        new_list->next = list->next; //re-route copy node to next node in original list
        
        //this is the tricky part. We are going to come back and traverse to fill these in
        new_list->reference = NULL;

        list->next = new_list; //re-route next node to this
        list = new_list->next; //increment to the next (original) node to repeat copy
    }

    //reset list back to head node so we can begin our search
    list = head;

    //loop through nodes to resolve the reference node
    while (list != NULL)
    {
        Node_t* new_list = list->next;

        //update the reference node to be the copy's reference by using the original reference, and going to next
        if (list->reference != NULL)
            new_list->reference = (list->reference)->next;

        //move to the next original node
        list = list->next->next;
    }

    //reset the list pointer to the beginning of original list
    //As well as set head to beginning of duplicate list
    list = head;
    head = list->next;

    //walk through the list and unzipper the nodes into two independent lists
    while (list != NULL)
    {
        Node_t* new_list;

        //unzipper using the next node
        new_list = list->next;

        //stitch back the original list:
        list->next = (list->next)->next;

        //stitch back the duplicate list:
        if(new_list->next != NULL)
            new_list->next = (new_list->next)->next;

        //increment to the next node and repeat
        list = list->next;
    }

    return head;
}

int main()
{
    /*
    Given a linked list using the following data structure:

    typedef struct Node_s
    {
        struct Node_s* next;
        struct Node_s* reference;
    } Node_t;

    Implement the function DuplicateList() which makes a copy of the linked list.  The input list is a singly linked list with nodes which reference random nodes within the list.
    The input list is generated by calling GenerateList()

    input: a singly linked list (each node contains a pointer to the next node in the list).  Additionaly, each node contains a second pointer that references a random node within the list.
                          /------->-------\
                          |               |
          /------->-------|               |
          |               |               |
         [a]==>==[b]==>==[c]==>==[d]==>==[e]
          |       |       |       |       |
          |---<---/       \---<---/       |
          |                               |
          \---------------<---------------/

    output: a duplicate copy of the list with no dependency on the original
                          /------->-------\
                          |               |
          /------->-------|               |
          |               |               |
         [L]==>==[M]==>==[N]==>==[O]==>==[P]
          |       |       |       |       |
          |---<---/       \---<---/       |
          |                               |
          \---------------<---------------/

    Note: when implementing please consider arbitrary input lists other than the one generated.
    A different random list can be set by changing the seed to srand() and changing number of nodes in the list.
    */
    srand(3);

    Node_t* list = GenerateList(1);

    printf("Original List:\n");
    PrintList(list);


    Node_t* new_list = DuplicateList(list);


    printf("\nNew List:\n");
    PrintList(new_list);

    //test 3 nodes
    list = GenerateList(3);

    printf("\n3 Node Original List:\n");
    PrintList(list);

    new_list = DuplicateList(list);
    printf("\n3 Node New List:\n");
    PrintList(new_list);


    //test 5 nodes
    list = GenerateList(5);

    printf("\n5 Node Original List:\n");
    PrintList(list);

    new_list = DuplicateList(list);

    printf("\n5 Node New List:\n");
    PrintList(new_list);

    //test 10 nodes
    list = GenerateList(10);

    printf("\n10 Node Original List:\n");
    PrintList(list);

    new_list = DuplicateList(list);

    printf("\n10 Node New List:\n");
    PrintList(new_list);




    printf("\nPress any key to continue...");

    char c;
    scanf("%c", &c);
}
