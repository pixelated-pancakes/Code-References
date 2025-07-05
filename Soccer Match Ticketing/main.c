/* COP 3502C Assignment 2
 * This program is written by: Madelyn Noth*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 12
#define NAMESIZE 16
#define FULL 1000
#define EMPTY -999

struct customer
{
    int enTime; // Time the customer steps into line
    int liNum; // line that the customer steps in
    char *name; // dynamically allocated string
    int numTix; // # of tickets that the customer plans on buying

};

struct node
{
    struct customer *customer; // holds a pointer to a customer
    struct node *next; // holds the next customer in the queue
};

struct queue
{
    struct node *front; // holds front of line/queue (first in)
    struct node *back; // holds back of line/queue (last in)
};

void init(struct queue *q);// declaring so my functions don't get mad at me for calling other functions
void enqueue(struct queue *q, struct customer *c);
struct customer* dequeue(struct queue *q);
struct customer* peek(struct queue *q);
int empty(struct queue *q);
struct customer* createCustomer(int enTime, int liNum, char *name, int numTix);
void checkout(struct queue *q, int *curTime);
int search(struct queue *q, int customers, int *curTime);


void init(struct queue *q) // initializes queues
{
    q->front = NULL;
    q->back = NULL;
}

void enqueue(struct queue *q, struct customer *c)
{
    struct node *temp = malloc(sizeof(struct node)); // creates new node for the passed queue
    temp->customer = c;
    temp->next = NULL;

    if(empty(q) == 1) // if passed queue is empty, just make it the front and back (since queue with only 1 element is the first and last in)
    {
        q->front = temp;
        q->back = temp;
        return;
    }
    q->back->next = temp; // if queue isn't empty, adds new node at the back of the line/queue
    q->back = temp;
    return;
}

struct customer* dequeue(struct queue *q)
{
    if (empty(q) == 1) // stops function from seg faulting if no one is in the passed queue
        return EMPTY;

    struct customer *save = q->front->customer; // holds customer pointer so DMA and data isn't lost
    struct node *temp = q->front; // holds the node that is being dequeued
    q->front = q->front->next; // makes new front the next person in line
    free(temp); // frees the old front

    if (empty(q) == 1) // if queue was only one person, this makes it so that if the back isn't called, it isn't trying to access the memory we just freed
        q->back = NULL;

    return save; // returns the customer that is being served
}

struct customer* peek(struct queue *q)
{
    if (q->front != NULL) // gets front of queue
        return q->front->customer;
    else
        return NULL;
}

int empty(struct queue *q) // checks if queue is empty
{
    if (q->front == NULL)
        return 1;
    return 0;
}

struct customer* createCustomer(int enTime, int liNum, char *name, int numTix) // creates a customer node from the given information, DMAs for name
{
    struct customer *temp = malloc(sizeof(struct customer));
    temp->enTime = enTime;
    temp->liNum = liNum;
    temp->name = malloc ((strlen(name) + 1)*sizeof(char));
    strcpy(temp->name, name);
    temp->numTix = numTix;

    return temp;
}


void checkout(struct queue *q, int *curTime) // checks out customer
{
    int checkoutTime = (20 + 10*q->front->customer->numTix); // calculates how long the interaction will take
    *curTime += checkoutTime; // adds checkout time to the current  simulation time
    struct customer *temp = dequeue(q); // gets the customer who needs to be checked out
    printf("%s left the counter at time %d from line %d.\n", temp->name, *curTime, temp->liNum); // prints checkout details
    free(temp->name); // frees name
    free(temp); // frees up the rest of the customer space
}
int search(struct queue *q, int customers, int *curTime)
{

    long bestTime = 1000000001; // 1 value above the maximum arrival time
    int leastTickets = 999; // value above max tickets so that it can guaranteed to be checked
    int queueLeastTickets = -1; // value that will hold the index of the queue with least tickets
    int queueBestTime = -1; // value that will hold the index of the queue with the best/earliest arrival time

    for (int i = 0; i < SIZE; i++) // finds the lowest arrival time in all queues
    {
        struct customer *temp = peek(&q[i]);
        if (temp != NULL && temp->enTime < bestTime)
        {
            bestTime = temp->enTime;
            queueBestTime = i;
        }

    }
    if (bestTime >= *curTime) // if the lowest available arrival time is greater or equal to cur time, then make curtime the best time and return the queue index with bestime
    {
        *curTime = bestTime;
        return queueBestTime;
    }
    else if (bestTime < *curTime) // if the lowest arrival time is greater than curtime, find the first queue with that has a customer arriving before or on curtime and the least number of tickets
    {
        for (int i = 0; i < SIZE; i++)
        {
            struct customer *temp = peek(&q[i]);
            if (temp != NULL && temp->enTime <= *curTime && temp->numTix < leastTickets)
            {
                leastTickets = temp->numTix;
                queueLeastTickets = i;
            }
        }
        return queueLeastTickets;// returns the index value of the queue with the lowest ticket number in front
    }
}

int main()
{
    int testCases, customers, i, j; // loop variables
    int numTix, liNum, enTime; // customer data values
    char name[NAMESIZE]; // placeholder string of specified max size for a customer's name


    struct queue lines[SIZE];
    for (i = 0; i < SIZE; i++) // initializes all queues
        init(&lines[i]);

    scanf("%d", &testCases);
    for (i = 0; i < testCases; i++)
    {
        int curTime = 0; // resets time for each case scenario

        scanf("%d", &customers);
        for (j = 0; j < customers; j++) // enqueues all customers for simulation
        {
            scanf("%d %d %s %d", &enTime, &liNum, name, &numTix);
            struct customer *temp = createCustomer(enTime, liNum, name, numTix);
            enqueue(&lines[liNum-1], temp);
        }
        for (j = 0; j < customers; j++) // checks out (and dequeues) all customers in desired order
        {
            int nextServe = search(lines, customers, &curTime);
            checkout(&lines[nextServe], &curTime);
        }
        printf("======\n");

    }


    return 0;
}