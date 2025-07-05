#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAXLENGTH 21


typedef struct Player
{
    char *name; //Player name (dynamically allocated)
    int totalGoals; //Total goals scored by the player(needs to be counted based on the match)
}Player;

typedef struct Team
{
    char *name; // Name of the team (dynamically allocated)
    Player **players; //An array of Player pointers where each pointer is pointing to a player
    int playerCount; // Number of players in the team
}Team;

typedef struct Goal
{
    Player *scorer; // Pointer to the player who scored
    int time; // Time of the goal in minutes
}Goal;

typedef struct Match
{
    Team *team1; // Pointer to Team 1
    Team *team2; // Pointer to Team 2
    Goal *goals; // Array of goals scored in the match
    int goalCount; // Total number of goals in the match
} Match;

typedef struct MatchWinnerTeam
{
    Team *winnerTeam; //pointer to the winner team if any.
    int isDraw; //set to 1 if it was a draw. In that case winnerTeam will be NULL
}MatchWinnerTeam;

Player* createPlayer(char *name)
{
    Player *temp;
    temp = malloc(sizeof(Player)); //allocates space for players
    int reqLen = strlen(name) + 1;
    temp->name = (char*)malloc(sizeof(char)*reqLen); // allocates exact space needed for name
    strcpy(temp->name, name);
    temp->totalGoals = 0; // initializes total goals to 0

    return temp;
}

Team* createTeam(char *name, int playerCount)
{
    Team* temp;
    temp = malloc(sizeof(Team)); // allocates space for a team
    int reqLen = strlen(name) + 1;
    temp->name = malloc(reqLen*sizeof(char)); // allocates exact space needed for team name
    strcpy(temp->name, name);
    temp->players = (Player**)malloc(sizeof(Player*)*playerCount); // allocates the amount of space needed for all players in the team
    temp->playerCount = playerCount;

    return temp;
}

Player* getPlayerByName(char *name, Player **playerList, int numPlayers)
{
    for (int i = 0; i < numPlayers; i++) {
        if (strcmp(playerList[i]->name, name) == 0) // checks if provided name is in the playerList
            return playerList[i]; // returns pointer to player if name is in playerList
    }
    return NULL; // if name is not in playerList, returns NULL
}

Team* getTeamByTeamName(Team **teamList, int numTeams, char *tname)
{
    for (int i = 0; i < numTeams; i++) {
        if (strcmp(teamList[i]->name, tname) == 0)// checks if provided name is in the teamList
            return teamList[i];// returns pointer to team if name is in teamList
    }
    return NULL; // if name is not in teamList, returns NULL
}

void recordGoal(Match *match, Player *player, int time)
{
    match->goalCount++;
    player->totalGoals++;

    if (match->goals == NULL) // Checks to see if pointer is NULL, if NULL then malloc for one goal, if not then realloc
        match->goals = malloc(sizeof(Goal)*match->goalCount);
    else
        match->goals = realloc(match->goals,match->goalCount * sizeof(Goal));

    for (int i = match->goalCount-1; i < match->goalCount; i++) { // fills out goals array with the player pointer of the scorer and the time of the goal
        match->goals[i].scorer = player;
        match->goals[i].time = time;
    }
}

int printHatTrickPlayers(Match *match)
{
    int hattrickFound = 0;
    Player **pp = malloc(sizeof(Player*)*match->goalCount);
    int *gc = calloc(match->goalCount, sizeof(int));
    int ppTracker = 0;

    for (int i = 0; i < match->goalCount; i++)
    {
        int playerTracker = -1; // variable used to see if player is present in the array of player points, pp
        for (int j = 0; j < ppTracker; j++)
        {
            if (pp[j] == match->goals[i].scorer) // if player is already in the player pointer array, just add to their points index
            {
                gc[j]++;
                playerTracker = 1;
                break;
            }
        }
        if (playerTracker == -1) // if player was not already in the player pointer array, add them into the array, increase their goal index, and increase ppTracker
        {
            pp[ppTracker] = match->goals[i].scorer;
            gc[ppTracker]++;
            ppTracker++;
        }

    }
    for (int i = 0; i < ppTracker; i++) // checks to see if a hat trick is in the match
    {
        if (gc[i] >= 3)
        {
            hattrickFound = 1;
        }

    }
    if (hattrickFound == 1) // prints hat trick details if found
    {
        printf("hat trick found in %s vs %s:\n", match->team1->name, match->team2->name);
        for (int i = 0; i < ppTracker; i++)
        {
            if (gc[i]>=3)
            {
                printf("%s %d\n", pp[i]->name, pp[i]->totalGoals);
            }
        }

    }


    free(pp);
    free(gc);
    return hattrickFound; // returns hat trick results
}

MatchWinnerTeam* getWinningTeam(Match *matches, int matchCount, char *team1Name, char *team2Name)
{
    MatchWinnerTeam *temp = malloc(sizeof(MatchWinnerTeam));
    int team1Goals = -100, team2Goals = -100; // initializes variables to unattainable values so the function returns NULL if the if statement within the forloop is not met
    Team *temp1; // stores the correct team pointers when if statement is met
    Team *temp2;
    temp->winnerTeam = NULL; // initializes value incase if statement fails
    temp->isDraw = -1; // initializes value to an unattainable value to ensure the loop runs

    for (int i = 0; i < matchCount; i++)
    {
        if (strcmp(matches[i].team1->name, team1Name) == 0 && strcmp(matches[i].team2->name, team2Name) == 0) // satisfied when provided names are a valid match in the input
        {
            team1Goals = 0;
            team2Goals = 0;
            temp1 = matches[i].team1;
            temp2 = matches[i].team2;
            for (int j = 0; j < matches[i].goalCount; j++)
            {
                for (int k = 0; k < matches[i].team1->playerCount ; k++)
                {
                    if (strcmp(matches[i].goals[j].scorer->name, matches[i].team1->players[k]->name) == 0) // checks if scorer was on team 1
                        team1Goals++;
                }
                for (int l = 0; l < matches[i].team2->playerCount ; l++)
                {
                    if (strcmp(matches[i].goals[j].scorer->name, matches[i].team2->players[l]->name) == 0) // checks if scorer was on team 2
                        team2Goals++;
                }
            }
            break; // breaks so that the for loop doesn't continue
        }


    }
    if(team1Goals > team2Goals)
    {
        temp->winnerTeam = temp1;
        temp->isDraw = 0;
    }
    if(team2Goals > team1Goals)
    {
        temp->winnerTeam = temp2;
        temp->isDraw = 0;
    }
    if(team1Goals == team2Goals && team1Goals >= 0)
    {
        temp->winnerTeam = NULL;
        temp->isDraw = 1;
    }


    return temp;
    }




Team* getChampion(Team **teamList, int teamCount, Match *matchList, int matchCount)
{
    MatchWinnerTeam *temp = NULL;
    Team *tempTeam = NULL;
    int points[teamCount]; // makes an array of points for each team in teamList

    for (int i = 0; i < teamCount; i++)
        points[i] = 0; // initializes the points array to 0

    for (int j = 0; j < matchCount; j++) // get ready for a really big nested loop (not fun)
    {
        temp = getWinningTeam(matchList, matchCount, matchList[j].team1->name, matchList[j].team2->name); // gets match results
        if (temp->isDraw == 0) // adds 3 points to the winning team if the match wasn't a draw
        {
            tempTeam = temp->winnerTeam;
            for (int k = 0; k <teamCount; k++)
            {
                if (tempTeam == teamList[k])
                    points[k]+=3;
            }
        }
        else if(temp->isDraw == 1) // adds 1 point to each team if the match was a draw
        {
            for (int k = 0; k <teamCount; k++)
            {
                if (matchList[j].team1 == teamList[k])
                    points[k]+=1;
                if (matchList[j].team2 == teamList[k])
                    points[k]+=1;
            }
        }
        free(temp); // frees MatchWinnerTeam pointer created in loop
    }
    int maxPoints = -1; // guarantees there is a max points even if all teams somehow have 0 points
    Team *winningTeam = NULL;
    for (int j = 0; j < teamCount; j++) // finds team with max points
    {
        if (points[j] > maxPoints)
        {
            maxPoints = points[j];
            winningTeam = teamList[j];
        }
    }


    return winningTeam;
}


void free_memory(Match *m,int matchCount, Team **t, int teamCount, Player **p, int playerCount) // self explanatory function
{

    for (int i = 0; i < matchCount; i++)
    {
        free(m[i].goals);
    }
    free(m);

    for (int i = 0; i < teamCount; i++)
    {
        for (int j = 0; j < t[i]->playerCount; j++)
        {
            free(t[i]->players[j]->name);
            free(t[i]->players[j]);
        }
        free(t[i]->name);
        free(t[i]->players);
        free(t[i]);
    }
    free(t);
    free(p);
}


int main() {

    int numMatches, numTeams, numCommands, numPlayers;
    Player **playerList = NULL;
    Team **teamList;
    char teamName[MAXLENGTH];
    char playerName[MAXLENGTH];
    int totalPlayers = 0;
    int numGoals = 0;


    scanf("%d%d%d", &numTeams, &numMatches, &numCommands);
    teamList = (Team**)malloc(sizeof(Team*)*numTeams);

    for(int i = 0; i < numTeams; i++)
    {
        scanf("%s%d", teamName, &numPlayers);
        teamList[i] = createTeam(teamName, numPlayers);
        for(int j = 0; j < numPlayers; j++)
        {
            scanf("%s", playerName);
            totalPlayers++;
            if (playerList == NULL)
                playerList = malloc(sizeof(Player*));
            else
                playerList = realloc(playerList,sizeof(Player*)*totalPlayers);

            playerList[totalPlayers-1] = createPlayer(playerName);
            teamList[i]->players[j] = playerList[totalPlayers-1];
        }
    }

    Match *matchList = malloc(sizeof(Match)*numMatches);

    for(int i = 0; i < numMatches; i++)
    {
        matchList[i].goals = NULL;
        matchList[i].goalCount = 0;


        char teamName1[MAXLENGTH];
        char teamName2[MAXLENGTH];

        scanf("%s%s", teamName1, teamName2);

        matchList[i].team1 = getTeamByTeamName(teamList, numTeams, teamName1);
        matchList[i].team2 = getTeamByTeamName(teamList, numTeams, teamName2);

        char traveler[MAXLENGTH] = "not the end";
        while(strcmp(traveler, "end")!= 0)
        {
            int time;
            scanf("%s", traveler);
            if(strcmp(traveler, "end") == 0)
            {
                break;
            }
            scanf("%d", &time);
            Player *tempPlayer = getPlayerByName(traveler, playerList, totalPlayers);
            recordGoal(&matchList[i],tempPlayer, time);
        }
    }

    for(int i = 0; i < numCommands; i++)
    {
        char command[MAXLENGTH];
        scanf("%s", command);
        if(strcmp(command, "max_goal_player") == 0)
        {
            int maxGoals = 0;
            Player *MaxGoalPlayer = NULL;
            for(int j = 0; j < totalPlayers; j++)
            {
                if (playerList[j]->totalGoals > maxGoals)
                {
                    MaxGoalPlayer = playerList[j];
                    maxGoals = playerList[j]->totalGoals;
                }
            }
            if(MaxGoalPlayer != NULL)
                printf("player with max goal %s %d\n", MaxGoalPlayer->name, maxGoals);
        }
        else if (strcmp(command,"winner") == 0)
        {
            char teamName1[MAXLENGTH];
            char teamName2[MAXLENGTH];

            scanf("%s%s", teamName1, teamName2);
            MatchWinnerTeam *temp = getWinningTeam(matchList, numMatches, teamName1, teamName2);
            if (temp->isDraw == 1)
                printf("%s vs %s match drawn\n", teamName1, teamName2);
            else if (temp->isDraw == 0)
            {
                printf("winner of %s vs %s is %s\n", teamName1, teamName2, temp->winnerTeam->name);
            }
            else if (temp->isDraw == -1)
                printf("no match between %s and %s\n", teamName1, teamName2);
            free(temp);

        }

        else if (strcmp(command,"hattrick") == 0)
        {
            int hattrickFound = 0;
            int *hattrick = calloc(numMatches, sizeof(int));
            for (int j = 0; j < numMatches; j++)
            {
                hattrick[j] = printHatTrickPlayers(&matchList[j]);
            }
            for(int j = 0; j < numMatches; j++)
            {
                if(hattrick[j] == 1)
                    hattrickFound = 1;

            }
            if(hattrickFound == 0)
                printf("no hat trick found in the tournament\n");

            free(hattrick);

        }
        else if (strcmp(command, "champion") == 0)
        {
            Team *champion = getChampion(teamList, numTeams, matchList, numMatches);
            printf("champion is %s\n", champion->name);
        }


        else
        {
            printf("unknown command: %s\n", command);
        }

    }

    free_memory(matchList, numMatches, teamList, numTeams, playerList, totalPlayers);

    return 0;
}