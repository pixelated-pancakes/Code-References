/*This program is written by: Madelyn */


#include <stdio.h>
#include <string.h>


struct person // holds details about individuals
{
    char name[20];
    int snacks;
    int numRestrictions;
    struct person *restricted[10];
};

int numAttend; // global declartion of # of attendees
struct person attendees[10]; // global declaration of list of people attending
int validPerms = 0; // initializing number of valid permutations 

void createGroup(int size);
void addRestrictions(int size);
struct person* findPerson(char *name, int size);
void permutations(int *perm, int *used, int size);
void process(int *perm, int size);

void createGroup(int size) // recursive functions that reads in information about all people in the group
{
    if (size == 0)
        return;
    createGroup(size-1);
    scanf("%s%d", attendees[size-1].name, &attendees[size-1].snacks);
    attendees[size-1].numRestrictions = 0;
}

void addRestrictions(int size) // recursive function, reads in names for restrictions and adds restricted people to each person structs restricted list
{
    if (size == 0)
        return;
    addRestrictions(size-1);
    char name1[20];
    char name2[20];
    scanf("%s%s", name1, name2);
    struct person *temp1 = findPerson(name1, numAttend);
    struct person *temp2 = findPerson(name2, numAttend);
    temp1->numRestrictions += 1;
    temp2->numRestrictions += 1;
    temp1->restricted[temp1->numRestrictions - 1] = findPerson(name2, numAttend);
    temp2->restricted[temp2->numRestrictions - 1] = findPerson(name1, numAttend);

}

struct person* findPerson(char *name, int size) // finds people based on name
{
    for (int i = 0; i < size; i++)
    {
        if (strcmp(name, attendees[i].name) == 0)
            return &attendees[i];
    }
}

void permutations(int *perm, int *used, int size) // creates all possible permutations of the number of individuals 
{
    if (size == numAttend)
    {
        process(perm, numAttend);
        return;
    }

    for (int i = 0; i < numAttend; i++)
    {
        if (!used[i])
        {
            used[i] = 1;
            perm[size] = i;
            permutations(perm, used, size+1);
            used[i] = 0;
        }
    }
}

void process(int *perm, int size) // takes a permutation and checks for validity
{
    int i = 0;
    while (i < size)
    {

        if (i == 0) // leftmost seat
        {
            if (attendees[perm[i]].snacks != 1 && attendees[perm[i+1]].snacks != 1) // snack check
                return;
            int j = 0;
            while (j < attendees[perm[i]].numRestrictions)
            {
                if ((attendees[perm[i]].restricted[j]) == &(attendees[perm[i+1]])) // restricted people check
                    return;
                j++;
            }

        }
        else if (i > 0 && i < size-1) // middle seats
        {
            if (attendees[perm[i]].snacks != 1 && attendees[perm[i+1]].snacks != 1 && attendees[perm[i-1]].snacks != 1) // snack check
                return;
            int j = 0;
            while (j < attendees[perm[i]].numRestrictions)
            {
                if ((attendees[perm[i]].restricted[j]) == &(attendees[perm[i+1]]) || (attendees[perm[i]].restricted[j]) == &(attendees[perm[i-1]])) // restricted people check
                    return;
                j++;
            }

        }
        else if (i == size-1) // rightmost seat
        {
            if (attendees[perm[i]].snacks != 1 && attendees[perm[i-1]].snacks != 1) // snack check
                return;
            int j = 0;
            while (j < attendees[perm[i]].numRestrictions) // restricted people check
            {
                if ((attendees[perm[i]].restricted[j]) == &(attendees[perm[i-1]]))
                    return;
                j++;
            }
        }

        i++;
    }
    validPerms++; // if the function gets to this point without a return statement, then the permutation is valid 
}



int main()
{
    int numRestrictions;
    scanf("%d%d",&numAttend, &numRestrictions);
    createGroup(numAttend);
    addRestrictions(numRestrictions);
    int perm[numAttend];
    int used[numAttend];
    for (int i = 0; i < numAttend; i++)
    {
        perm[i] = 0;
        used[i] = 0;
    }

    permutations(perm, used, 0);

    printf("%d", validPerms);

}
