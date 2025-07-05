// This code was kinda flawed but was good at giving simple projections for which player would win in a set catan board arrangement
// I made it in one hour and if I had more time before submitting it I probably would have came up with a better way to do it
// May update in the future actually...

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void simulation(int trials);

int main()
{
   srand(time(NULL));
   int testTrials = 0;
   printf("Enter desired number (less than 10^8) of test trials: ");
   scanf("%d", &testTrials);
   printf("==================================================\n");
   simulation(testTrials);
   return 0;
}

void simulation(int trials)
{
   int P1 = 0, P2 = 0, P3 = 0, P4 = 0;
   int P1win = 0, P2win = 0, P3win = 0, P4win = 0;
   for (int i = 0; i < trials; i++)
   {
     int j = 0;
     while ( 0 == 0)
     {
       int diceOne = rand() % 6 + 1;
       int diceTwo = rand() % 6 + 1;
       int roll = diceOne + diceTwo;
       
       switch (roll)
       {
         case 2:
           break;
         case 3:
           P4+= 1;
           break;
         case 4:
           P1+= 1;
           P3+= 1;
           break;
         case 5:
           P1 += 1;
           P2 += 1;
           P3 += 1;
           P4 += 1;
           break;
         case 6:
           P1 += 1;
           P4 += 1;
           break;
         case 7:
           break;
         case 8:
           P1 += 1;
           P3 += 1;
           break;
         case 9:
           P2 += 2;
           P4 += 1;
           break;
         case 10:
           P2 += 1;
           P3 += 2;
           break;
         case 11:
           P1 += 1;
           break;
         case 12:
           P2 += 1;
           break;
       }
       j++;
       if (P1 == 9)
       {
         P1win += 1;
         P1 = 0, P2 = 0, P3 = 0, P4 = 0;
         break;
       }
       else if (P2 == 12)
       {
         P2win += 1;
         P1 = 0, P2 = 0, P3 = 0, P4 = 0;
         break;
       }
       else if (P3 == 9)
       {
         P3win += 1;
         P1 = 0, P2 = 0, P3 = 0, P4 = 0;
         break;
       }
       else if (P4 == 12)
      {
         P4win += 1;
         P1 = 0, P2 = 0, P3 = 0, P4 = 0;
         break;
      }
   }
 }
 printf("In %d simulations, \nPlayer 1 won: %d times\nPlayer 2 won: %d times\nPlayer 3 won: %d times\nPlayer 4 won: %d times", trials, P1win, P2win, P3win, P4win);
 printf("\n==================================================\n");
}
