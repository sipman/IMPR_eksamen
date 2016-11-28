#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NUMOFROUNDS 33
#define NUMOFMATCHESPRROUND 6
#define NUMOFTOTALMACHTES (NUMOFROUNDS*NUMOFMATCHESPRROUND)
#define NUMOFTEAMS 12
#define TEAMNAMEBUFFER 4
#define WEEKDAYBUFFER 4
#define MAXLINELENGTH 80
#define SOURCEFILE "superliga-2015-2016"

typedef struct {
  int day, month, year;
} date;

typedef struct {
  int hours, minutes;
} time;

typedef struct {
  int round, goals;
} round;

typedef struct {
  char name[TEAMNAMEBUFFER];
  int awayMatches, homeMatches,
      awayWins, homeWins,
      awayLoses, homeLoses,
      awayDraws, homeDraws,
      totalGoalsScored, totalGoalsConceded,
      awayGoalsScored, homeGoalsScored,
      awayGoalsConceded, homeGoalsConceded,
      points;
} team;

typedef struct {
  round *round;
  char weekDay[WEEKDAYBUFFER];
  date date;
  time time;
  team *homeTeam, *awayTeam;
  int homeGoals, awayGoals, attendances;
} match;

void prepareData(match *matches, round *rounds, team *teams, FILE *inputFile);
void showTeamsDominatingAway(team *teams);
void showDrawMatches(int goalDelimiter, match *matches);
void showARoundWithLesserGoals(int goalDelimiter, round *rounds);
void printAMatch(match match);
void printAllMatches(match *matches, int numberOfMatches);
match generateMatchFromStr(char *str, round *rounds, team *teams);
int generateNumberOfRounds(int roundNumber, int currentRoundNumber);
team *findTeamsDominatingAway(team *teams, int *numOfTeamsDominatingAway);
team *findTeam(char *teamName, team *teams);
match *findDrawsSearch(int goalDelimiter, match *matches);
void findFirstRoundWithLesserGoals(int *resultRound, int *resultGoals, int goalDelimiter, round *rounds);

int main(void){
  FILE *input = fopen(SOURCEFILE, "r");
  match *season = calloc(NUMOFTOTALMACHTES, sizeof(match));
  round *rounds = calloc(NUMOFROUNDS, sizeof(rounds));
  team  *teams  = calloc(NUMOFTEAMS, sizeof(team));
  if (input == NULL || season == NULL || rounds == NULL || teams == NULL){
    printf("%s", "Not enough ram, sorry..");
    exit(EXIT_FAILURE);
  }
  prepareData(season, rounds, teams, input);
  fclose(input);
  showTeamsDominatingAway(teams);
  /*showARoundWithLesserGoals(10, rounds);*/
  /*showDrawMatches(4, season);*/
  /*printAllMatches(season, NUMOFTOTALMACHTES);*/

  return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * DATA FUNCTIONS  * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
 * @brief      Parses data from file to the array of match struct
 *
 * @param      matches     The array of matches
 * @param      rounds      The array of rounds
 * @param      inputFile   The input file
 */
void prepareData(match *matches, round *rounds, team *teams,  FILE *inputFile){
  char *str = (char*) malloc(MAXLINELENGTH);
  int i=0;
  while (fgets(str, MAXLINELENGTH, inputFile)) {
    matches[i] = generateMatchFromStr(str, rounds, teams);
     i++;
  }
  free(str);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * OUTPUT FUNCTIONS  * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
 * @brief      Shows the teams dominating away.
 *
 * @param      teams  The teams
 */
void showTeamsDominatingAway(team *teams){
  int numOfTeamsDominatingAway, i;
  team *teamsDominatingAway = findTeamsDominatingAway(teams, &numOfTeamsDominatingAway);
  for(i=0; i < numOfTeamsDominatingAway; i++){
    printf("%s with %d away wins against %d home wins\n", teamsDominatingAway[i].name, teamsDominatingAway[i].awayWins, teamsDominatingAway[i].homeWins);
  }
}

/**
 * @brief      Shows the rounds with lesser total goals than the delimiter.
 *
 * @param[in]  goalDelimiter   The goal delimiter.
 * @param[in]  *rounds         The array of rounds.
 */
void showARoundWithLesserGoals(int goalDelimiter, round *rounds){
  int resultRound, resultGoals;
  findFirstRoundWithLesserGoals(&resultRound, &resultGoals, goalDelimiter, rounds);
  printf("Round: %2d\t Goals: %d\n", resultRound, resultGoals);
}
/**
 * @brief      Shows the draw matches with lesser total goal than the delimiter.
 *
 * @param[in]  goalDelimiter    The goal delimiter.
 * @param[in]  *matches         The array of matches.
 */
void showDrawMatches(int goalDelimiter, match *matches){
  match *draws = findDrawsSearch(4, matches);
  int i=0;
  while(draws[i].round != 0){
    printAMatch(draws[i]);
    i++;
  }
  free(draws);
}
/**
 * @brief      Prints a single match.
 *
 * @param[in]  match  The match that is desired printed
 */
void printAMatch(match match){
  printf("R%-2d\t%s\t%02d/%02d/%d\t%02d:%02d\t%-3s - %-3s\t%d-%d\t%d\n",
        match.round->round,
        match.weekDay,
        match.date.day,
        match.date.month,
        match.date.year,
        match.time.hours,
        match.time.minutes,
        match.homeTeam->name,
        match.awayTeam->name,
        match.homeGoals,
        match.awayGoals,
        match.attendances
    );
}
/**
 * @brief      Prints an array of matches.
 *
 * @param[in]  *matches         The array of matches that is desired printed.
 * @param[in]  numberOfMatches  The number of matches in the array.
 */
void printAllMatches(match *matches, int numberOfMatches){
  int i;
  for(i=0; i<numberOfMatches; i++){
    printAMatch(matches[i]);
  }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * HELPER FUNCTIONS  * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
 * @brief      Takes a string and parse it to a match struct
 *               - also it generates the accordently rounds array with total goals
 *
 * @param[in]  str        The string.
 * @param      *rounds    The array of rounds.
 *
 * @return     The match struct.
 */
match generateMatchFromStr(char *str, round *rounds, team *teams){
  match currentMatch;
  double attendances = 0;
  int round;
  char homeTeam[TEAMNAMEBUFFER], awayTeam[TEAMNAMEBUFFER];
  sscanf(str, " R%02d %s %d/%d/%d %d.%d %s - %s %d - %d %lf",
        &round,
        currentMatch.weekDay,
        &currentMatch.date.day,
        &currentMatch.date.month,
        &currentMatch.date.year,
        &currentMatch.time.hours,
        &currentMatch.time.minutes,
        homeTeam,
        awayTeam,
        &currentMatch.homeGoals,
        &currentMatch.awayGoals,
        &attendances
  );
  currentMatch.attendances = (attendances*1000);
  /* Home team stat generate */
  currentMatch.homeTeam = findTeam(homeTeam, teams);
  strcpy(currentMatch.homeTeam->name, homeTeam);
  currentMatch.homeTeam->homeMatches += 1;
  currentMatch.homeTeam->homeGoalsScored += currentMatch.homeGoals;
  currentMatch.homeTeam->homeGoalsConceded += currentMatch.awayGoals;
  currentMatch.homeTeam->totalGoalsScored += currentMatch.homeGoals;
  currentMatch.homeTeam->totalGoalsConceded += currentMatch.awayGoals;
  currentMatch.homeTeam->homeWins += (currentMatch.homeGoals>currentMatch.awayGoals);
  currentMatch.homeTeam->homeLoses += (currentMatch.homeGoals<currentMatch.awayGoals);
  currentMatch.homeTeam->homeDraws += (currentMatch.homeGoals==currentMatch.awayGoals);
  currentMatch.homeTeam->points += (currentMatch.homeGoals>currentMatch.awayGoals) ? 3 : (currentMatch.homeGoals==currentMatch.awayGoals) ? 1 : 0;
  /* Away team stat generate */
  currentMatch.awayTeam = findTeam(awayTeam, teams);
  strcpy(currentMatch.awayTeam->name, awayTeam);
  currentMatch.awayTeam->awayMatches += 1;
  currentMatch.awayTeam->awayGoalsScored += currentMatch.awayGoals;
  currentMatch.awayTeam->awayGoalsConceded += currentMatch.homeGoals;
  currentMatch.awayTeam->totalGoalsScored += currentMatch.awayGoals;
  currentMatch.awayTeam->totalGoalsConceded += currentMatch.homeGoals;
  currentMatch.awayTeam->awayWins += (currentMatch.awayGoals>currentMatch.homeGoals);
  currentMatch.awayTeam->awayLoses += (currentMatch.awayGoals<currentMatch.homeGoals);
  currentMatch.awayTeam->awayDraws += (currentMatch.awayGoals==currentMatch.homeGoals);
  currentMatch.awayTeam->points += (currentMatch.awayGoals>currentMatch.homeGoals) ? 3 : (currentMatch.awayGoals==currentMatch.homeGoals) ? 1 : 0;
  /* Round stat generate */
  currentMatch.round = &rounds[(round-1)];
  currentMatch.round->round = round;
  currentMatch.round->goals += currentMatch.homeGoals+currentMatch.awayGoals;
  return currentMatch;
}
/**
 * @brief      Finds an array of the teams who dominated away games
 *
 * @param[in]      teams                     The teams
 * @param          numOfTeamsDominatingAway  The number of teams dominating away
 *
 * @return     A pointer to the array of the teams who wins more away than home games
 */
team *findTeamsDominatingAway(team *teams, int *numOfTeamsDominatingAway){
  int i, numOfHits=0;
  int *hits = calloc(NUMOFTEAMS, sizeof(int));
  team *resultArray;
  for(i=0; i < NUMOFTEAMS; i++){
    if(teams[i].awayWins > teams[i].homeWins){
        hits[numOfHits] = i;
        numOfHits++;
    }
  }
  resultArray = calloc(numOfHits, sizeof(team));
  for(i=0; i < numOfHits; i++){
    resultArray[i] = teams[hits[i]];
  }
  free(hits);
  *numOfTeamsDominatingAway = numOfHits;
  return resultArray;
}
/**
 * @brief      Find the corosponding pointer in the team array,
 *             if not present returns the pointer to the next space in the array
 *
 * @param      teamName  The team name
 * @param      teams     The array of teams
 *
 * @return     A pointer to the correct placement of the teamName
 */
team *findTeam(char *teamName, team *teams){
  int i=0, found=0, nextTick=-1;
  while(!found && i < NUMOFTEAMS){
    if(!strcmp(teams[i].name, teamName)){
        found = 1;
        return &teams[i];
    }else if(nextTick == -1 && strlen(teams[i].name) == 0){
        nextTick = i;
    }
    i++;
  }
  return &teams[nextTick];
}

/**
 * @brief      Takes an array of matches, searches for draws with the delimiter of total goals and returns an array pointer
 *
 * @param[in]  goalDelimiter   The goal delimiter
 * @param[in]  *matches        The array matches
 *
 * @return     Returns the pointer to the resultArray containing all the hits.
 */
match *findDrawsSearch(int goalDelimiter, match *matches){
  int *hits = (int*) malloc(NUMOFTOTALMACHTES*sizeof(int));
  match *returnArray;
  int i, numOfHits=0;
  if (hits == NULL){
    printf("%s", "Not enough ram, sorry..");
    exit(EXIT_FAILURE);
  }
  for(i=0; i<NUMOFTOTALMACHTES; i++){
    if(matches[i].homeGoals == matches[i].awayGoals && matches[i].homeGoals+matches[i].awayGoals==goalDelimiter){
        hits[numOfHits] = i;
        numOfHits++;
    }
  }
  returnArray = (match*) malloc(numOfHits*sizeof(match));
  if (returnArray == NULL){
    printf("%s", "Not enough ram, sorry..");
    exit(EXIT_FAILURE);
  }
  for(i=0; i<numOfHits; i++){
    returnArray[i] = matches[hits[i]];
  }
  free(hits);
  return returnArray;
}

/**
 * @brief      Finds the first round with lesser total goals than the delimiter.
 *
 * @param      resultRound     The output result round
 * @param      resultGoals     the output result goals
 * @param[in]  goalDelimiter   The goal delimiter
 * @param[in]  rounds          The array of rounds
 */
void findFirstRoundWithLesserGoals(int *resultRound, int *resultGoals, int goalDelimiter, round *rounds){
  int i;
  for(i=0; i<NUMOFROUNDS; i++){
      if(rounds[i].goals < goalDelimiter){
        *resultRound = rounds[i].round;
        *resultGoals = rounds[i].goals;
      }
    }
}
