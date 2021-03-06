/**
Author:  Morten Hartvigsen
Email:   mhartv16@student.aau.dk
Group:   B2-28A
Study:   Software
Development time: 28 hours

Notes:
    * The task #2 only searches for one instance, this is due to the wording in the task
      seemed very specific and i interpreted it that way. If it should be able to find
      one or more instances i would solve it like i solved task #1.

Known issues:
    * The program only runs when compiled on GCC 6.10
    * There is known issues when running the program on 32-bit windows
    * The program dosen't run if the sourcefile has a .txt extension - it has to be without extension
    * The program dosen't return correct feedback if a function cant find what it is looking for.

Success cases:
    * Compiling with gcc 6.10 on MacOS Sierra 10.12
    * compiling with gcc 6.10 on Windows 10 64-bit
*/
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
#define WINPOINTS 3
#define DRAWPOINTS 1
#define LOOSEPOINTS 0
#define SOURCEFILE "superliga-2015-2016"
typedef struct date{
  int day, month, year;
} date;
typedef struct time{
  int hours, minutes;
} time;
typedef struct round{
  int round, goals;
} round;
typedef struct team{
  char name[TEAMNAMEBUFFER];
  int totalMatches, totalWins, totalDraws, totalLoses, totalGoalsScored, totalGoalsConceded, awayWins, homeWins, points;
} team;
typedef struct spectator{
  char teamname[TEAMNAMEBUFFER];
  int attendances;
} spectator;
typedef struct match{
  int round;
  char weekDay[WEEKDAYBUFFER];
  date date;
  time time;
  char homeTeam[TEAMNAMEBUFFER], awayTeam[TEAMNAMEBUFFER];
  int homeGoals, awayGoals;
  int attendances;
} match;

void prepareData(match **matches, round **rounds, team *teams, int *numOfGeneratedTeams, FILE *inputFile);
void welcomeMessage();
void helpMessage();
int scanOption();
int runCommand(int option, FILE *input, match *season, round *rounds, team *teams);
void clearBuffer();
void showLeagueTable(team *teams);
void showMatchesFromAWeekDay(char *from, char *to, char *weekday, match *matches);
void showTeamWithLowestAttendances(char *from, char *to, match *matches);
void showTeamsDominatingAway(team *teams);
void showDrawMatches(int goalDelimiter, match *matches);
void showARoundWithLesserGoals(int goalDelimiter, round *rounds);
void printAMatch(match match);
void printAllMatches(match *matches, int numberOfMatches);
void generateMatchFromStr(char *str, round *rounds, team *teams, int *numOfGeneratedTeams, match *match);
int generateNumberOfRounds(int roundNumber, int currentRoundNumber);
void copyTeamArray(team *dest, team *path);
int findMatchesFromAWeekDay(time from, time to, char *weekday, match *matches, match **filteredMatches);
int sortForLeagueTable(const void * a, const void * b);
void findTeamWithLowestAttendances(char *teamname, int *attendances, date from, date to, match *matches);
int findSpectators(char *teamName, spectator *attendances);
int filterMatchesByDate(date from, date to, match *matches, match **filteredMatches);
int findTeamsDominatingAway(team *teams, team **teamsDominatingAway);
int findTeam(char *teamName, team *teams, int *numOfGeneratedTeams);
int findDrawsSearch(int goalDelimiter, match *matches, match **draws);
void findFirstRoundWithLesserGoals(int *resultRound, int *resultGoals, int goalDelimiter, round *rounds);

int main(int argc, char const *argv[]){
  FILE *input = fopen(SOURCEFILE, "r");
  match *season = malloc(NUMOFTOTALMACHTES*sizeof(match));
  round *rounds = calloc(NUMOFROUNDS,sizeof(rounds));
  team  *teams = calloc(NUMOFTEAMS,sizeof(team));
  int option, numOfGeneratedTeams=0;
  if(input == NULL){
    printf("File not found...\n");
    exit(EXIT_FAILURE);
  }
  if (season == NULL || rounds == NULL || teams == NULL){
    printf("%s", "Not enough ram, sorry..");
    exit(EXIT_FAILURE);
  }
  prepareData(&season, &rounds, teams, &numOfGeneratedTeams, input);
  fclose(input);
  if(argc > 1 && strcmp(argv[1], "--print")==0){
      runCommand(-1, input, season, rounds, teams);
  }else{
    welcomeMessage();
    helpMessage();
    option = scanOption();
    runCommand(option, input, season, rounds, teams);
  }
  free(season);
  free(rounds);
  free(teams);
  return 0;
}

/**
 * @brief      Parses data from file to the array of match struct
 *
 * @param      matches    The array of matches
 * @param      rounds     The array of rounds
 * @param      teams      The array of teams
 * @param      inputFile  The input file
 */
void prepareData(match **matches, round **rounds, team *teams, int *numOfGeneratedTeams,  FILE *inputFile){
  char str[MAXLINELENGTH];
  int i=0;
  while (fgets(str, MAXLINELENGTH, inputFile)) {
    generateMatchFromStr(str, *rounds, teams, numOfGeneratedTeams, &(*matches)[i]);
    i++;
  }
}

/**
 * @brief      welcomes the user and shows help message
 */
void welcomeMessage(){
  printf("Welcome to the Danish Superliga Stats analyser\n");
}

/**
 * @brief      Shows the help message
 */
void helpMessage(){
  printf("[HELP] Program Commands:\n");
  printf("  [0] => %-40s  [%-s]\n","Help", "Shows this messages");
  printf("  [1] => %-40s  [%-s]\n", "Show Draws", "Shows a list of drawed macthes with at least 4 goals total");
  printf("  [2] => %-40s  [%-s]\n", "Show round with goals < 10", "Shows the first round where the number of goals is <10");
  printf("  [3] => %-40s  [%-s]\n", "Show teams dominating away games", "Shows a list of teams who is winning more games away than home");
  printf("  [4] => %-40s  [%-s]\n", "Show team with fewest spectators in 2015", "Shows the team which had the fewest total spectators in 2015");
  printf("  [5] => %-40s  [%-s]\n", "Show matches on Saturdays", "Shows a list of matches played on saturdays between 08:00 and 22:30");
  printf("  [6] => %-40s  [%-s]\n", "Show league table", "Shows the league table");
  printf("  [7] => %-40s  [%-s]\n", "Exit", "Closes the program");
}
/**
 * @brief      Prompt user for an command option
 *
 * @return     the options
 */
int scanOption(){
  int option, res;
  printf("Please select an command (type 0 for help): ");

  res = scanf(" %d", &option);
  if(res != 1 || option < -1 || option > 8){
    printf("Couldn't reconize the command, please try again...\n");
    clearBuffer();
    return scanOption();
  }
  return option;
}
/**
 * @brief      Actually runs the commands
 *
 * @param[in]  option  The option
 * @param      input   The input
 * @param      season  The season
 * @param      rounds  The rounds
 * @param      teams   The teams
 *
 * @return     returns 0 to keep it form endning in a inf-loop
 */
int runCommand(int option, FILE *input, match *season, round *rounds, team *teams){
  int newOption, done=0;
  switch(option){
    case -1:
      showDrawMatches(4, season);
      showARoundWithLesserGoals(10, rounds);
      showTeamsDominatingAway(teams);
      showTeamWithLowestAttendances("1/1/2015", "31/12/2015", season);
      showMatchesFromAWeekDay("13:15", "14:15", "Son", season);
      showLeagueTable(teams);
      done = 1;
    break;
    case 0:
      helpMessage();
    break;
    case 1:
       showDrawMatches(4, season);
    break;
    case 2:
        showARoundWithLesserGoals(10, rounds);
    break;
    case 3:
        showTeamsDominatingAway(teams);
    break;
    case 4:
        showTeamWithLowestAttendances("1/1/2015", "31/12/2015", season);
    break;
    case 5:
        showMatchesFromAWeekDay("08:00", "22:30", "Lor", season);
    break;
    case 6:
        showLeagueTable(teams);
    break;
    case 7:
        done = 1;
    break;
  }
  if(!done){
    newOption = scanOption();
    return runCommand(newOption, input, season, rounds, teams);
  }
  return 0;
}

/**
 * @brief      Clears the buffer so that the recursive scan isnt a inf-loop
 */
void clearBuffer(){
  int c;
  while((c = getchar()) != EOF)
    if (c == '\n')
        break;
}
/**
 * @brief      Shows the league table.
 *
 * @param      teams  The teams
 */
void showLeagueTable(team *teams){
  int i;
  team *leagueTable = malloc(NUMOFTEAMS*sizeof(team));
  copyTeamArray(leagueTable, teams);
  qsort(leagueTable, NUMOFTEAMS, sizeof(team), sortForLeagueTable);
  printf("\n------------------- 6: League Table -------------------\n");
  printf("-------------------------------------------------------\n");
  printf("| Pos | Hold | K  | V  | U  | T  | M+ | M- | MF  | P  |\n");
  for(i=0; i<NUMOFTEAMS; i++){
    printf("| %3d | %-4s | %-2d | %-2d | %-2d | %-2d | %-2d | %-2d | %+3d | %-2d |\n",
        (i+1),
        leagueTable[i].name,
        leagueTable[i].totalMatches,
        leagueTable[i].totalWins,
        leagueTable[i].totalDraws,
        leagueTable[i].totalLoses,
        leagueTable[i].totalGoalsScored,
        leagueTable[i].totalGoalsConceded,
        leagueTable[i].totalGoalsScored-leagueTable[i].totalGoalsConceded,
        leagueTable[i].points
      );
  }
  printf("-------------------------------------------------------\n");
  free(leagueTable);
}

/**
 * @brief      Shows the matches from a week day and given time periode.
 *
 * @param[in]      from     The from string ex. HH:MM.
 * @param[in]      to       The to string ex. HH:MM.
 * @param[in]      weekday  The weekday string.
 * @param          matches  The array of matches.
 */
void showMatchesFromAWeekDay(char *from, char *to, char *weekday, match *matches){
  time fromTimestamp;
  time toTimestamp;
  int numOfFilteredMatches;
  match *filteredMatches;
  sscanf(from, "%d:%d", &fromTimestamp.hours, &fromTimestamp.minutes);
  sscanf(to, "%d:%d", &toTimestamp.hours, &toTimestamp.minutes);
  numOfFilteredMatches = findMatchesFromAWeekDay(fromTimestamp, toTimestamp, weekday, matches, &filteredMatches);

  printf("\n-------------------- 5: Show matches on Sundays  --------------------\n");
  printAllMatches(filteredMatches, numOfFilteredMatches);
  printf("----------------------------------------------------------------------\n\n");
  free(filteredMatches);
}

/**
 * @brief      Shows the team with lowest attendances in a given year.
 *
 * @param      fromStr  The from date in string format ex. dd/mm/yyyy
 * @param      toStr    The to date in string format ex. dd/mm/yyyy
 * @param      matches  The matches
 */
void showTeamWithLowestAttendances(char *fromStr, char *toStr, match *matches){
  char teamname[TEAMNAMEBUFFER];
  int attendances=0;
  date from;
  date to;
  sscanf(fromStr, "%d/%d/%d",
          &from.day,
          &from.month,
          &from.year
        );
  sscanf(toStr, "%d/%d/%d",
          &to.day,
          &to.month,
          &to.year
        );
  findTeamWithLowestAttendances(teamname, &attendances, from, to, matches);
  printf("\n------------- 4: Show team with fewest spectators in 2015  ------------\n");
  printf("Between %02d/%02d/%02d and %02d/%02d/%02d the team with the lowest attendances is %-3s with %d\n", from.day, from.month, from.year, to.day, to.month, to.year, teamname, attendances);
  printf("----------------------------------------------------------------------\n\n");
}

/**
 * @brief      Shows the teams dominating away.
 *
 * @param      teams  The teams
 */
void showTeamsDominatingAway(team *teams){
  int numOfTeamsDominatingAway, i;
  team *teamsDominatingAway;
  numOfTeamsDominatingAway = findTeamsDominatingAway(teams, &teamsDominatingAway);
  printf("\n---------------- 3: Show teams dominating away games  ----------------\n");
  for(i=0; i < numOfTeamsDominatingAway; i++){
    printf("%s with %d away wins against %d home wins\n", teamsDominatingAway[i].name, teamsDominatingAway[i].awayWins, teamsDominatingAway[i].homeWins);
  }
  printf("----------------------------------------------------------------------\n\n");
  free(teamsDominatingAway);
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
  printf("\n------------------- 2: Show round with goals < 10 -------------------\n");
  printf("Round: %2d\t Goals: %d\n", resultRound, resultGoals);
  printf("----------------------------------------------------------------------\n\n");
}

/**
 * @brief      Shows the draw matches with lesser total goal than the delimiter.
 *
 * @param[in]  goalDelimiter    The goal delimiter.
 * @param[in]  *matches         The array of matches.
 */
void showDrawMatches(int goalDelimiter, match *matches){
  int i;
  match *draws;
  int numOfDraws = findDrawsSearch(4, matches, &draws);
  printf("\n------------------------- 1: Show Draws ----------------------------\n");
  for(i=0; i < numOfDraws; i++){
    printAMatch(draws[i]);
  }
  printf("----------------------------------------------------------------------\n\n");
  free(draws);
}

/**
 * @brief      Prints a single match.
 *
 * @param[in]  match  The match that is desired printed
 */
void printAMatch(match match){
  printf("R%-2d\t%s\t%02d/%02d/%d\t%02d:%02d\t%-3s - %-3s\t%d-%d\t%d\n",
        match.round,
        match.weekDay,
        match.date.day,
        match.date.month,
        match.date.year,
        match.time.hours,
        match.time.minutes,
        match.homeTeam,
        match.awayTeam,
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

/**
 * @brief      copies one team struct array over in another
 *
 * @param      dest  The destination
 * @param      path  The path
 */
void copyTeamArray(team *dest, team *path){
  int i;
  for(i=0; i<NUMOFTEAMS; i++){
    dest[i] = path[i];
  }
}

/**
 * @brief      Takes a string and parse it to a match struct
 *               - also it generates the accordently rounds array with total goals
 *
 * @param      str                  The string
 * @param      rounds               The rounds
 * @param      teams                The teams
 * @param      numOfGeneratedTeams  The number of generated teams
 * @param      destination          The destination
 */
void generateMatchFromStr(char *str, round *rounds, team *teams, int *numOfGeneratedTeams, match *destination){
  double attendances = 0;
  int homeTeamKey, awayTeamKey, roundsKey;
  sscanf(str, " R%02d %s %d/%d/%d %d.%d %s - %s %d - %d %lf",
        &destination->round,
        destination->weekDay,
        &destination->date.day,
        &destination->date.month,
        &destination->date.year,
        &destination->time.hours,
        &destination->time.minutes,
        destination->homeTeam,
        destination->awayTeam,
        &destination->homeGoals,
        &destination->awayGoals,
        &attendances
  );

  destination->attendances = (attendances*1000);
  /* Home team stat generate */
  homeTeamKey = findTeam(destination->homeTeam, teams, numOfGeneratedTeams);
  teams[homeTeamKey].totalGoalsScored += destination->homeGoals;
  teams[homeTeamKey].totalMatches += 1;
  teams[homeTeamKey].totalGoalsConceded += destination->awayGoals;
  teams[homeTeamKey].totalWins += (destination->homeGoals>destination->awayGoals);
  teams[homeTeamKey].totalDraws += (destination->homeGoals==destination->awayGoals);
  teams[homeTeamKey].totalLoses += (destination->homeGoals<destination->awayGoals);
  teams[homeTeamKey].homeWins += (destination->homeGoals>destination->awayGoals);
  teams[homeTeamKey].points += (destination->homeGoals>destination->awayGoals) ? WINPOINTS : (destination->homeGoals==destination->awayGoals) ? DRAWPOINTS : LOOSEPOINTS;
  /* Away team stat generate */
  awayTeamKey = findTeam(destination->awayTeam, teams, numOfGeneratedTeams);
  teams[awayTeamKey].totalGoalsScored += destination->awayGoals;
  teams[awayTeamKey].totalMatches += 1;
  teams[awayTeamKey].totalGoalsConceded += destination->homeGoals;
  teams[awayTeamKey].totalWins += (destination->awayGoals>destination->homeGoals);
  teams[awayTeamKey].totalDraws += (destination->awayGoals==destination->homeGoals);
  teams[awayTeamKey].totalLoses += (destination->awayGoals<destination->homeGoals);
  teams[awayTeamKey].awayWins += (destination->awayGoals>destination->homeGoals);
  teams[awayTeamKey].points += (destination->awayGoals>destination->homeGoals) ? WINPOINTS : (destination->awayGoals==destination->homeGoals) ? DRAWPOINTS : LOOSEPOINTS;
  /* Round stat generate */
  roundsKey = (destination->round)-1;
  rounds[roundsKey].round = destination->round;
  rounds[roundsKey].goals += destination->homeGoals+destination->awayGoals;
}

/**
 * @brief      QSort compare function for showLeagueTable-function
 */
int sortForLeagueTable(const void * a, const void * b){
  team *A = (team*) a;
  team *B = (team*) b;
  if(A->points > B->points){
    return -1;
  }else if(A->points < B->points){
    return 1;
  }else{
    if((A->totalGoalsScored-A->totalGoalsConceded) > (B->totalGoalsScored-B->totalGoalsConceded)){
      return -1;
    }else if((A->totalGoalsScored-A->totalGoalsConceded) < (B->totalGoalsScored-B->totalGoalsConceded)){
      return 1;
    }else{
      if(A->totalGoalsScored > B->totalGoalsScored){
        return -1;
      }else if(A->totalGoalsScored < B->totalGoalsScored){
        return 1;
      }else{
        return strcmp(A->name, B->name);
      }
    }
  }
}

/**
 * @brief      Finds and returns the array of matches that is played
 *             on the given weekday and from & to timestamp
 *
 * @param[in]  from             The from timestamp
 * @param[in]  to               The to timestamp
 * @param      weekday          The weekday string
 * @param      matches          The matches array
 * @param      filteredMatches  The filtered matches
 *
 * @return     The size of the array
 */
int findMatchesFromAWeekDay(time from, time to, char *weekday, match *matches, match **filteredMatches){
  int *resultArray = malloc(NUMOFTOTALMACHTES*sizeof(int));
  int i, numOfHits=0;
  if (resultArray == NULL){
    printf("%s", "Not enough ram, sorry..");
    exit(EXIT_FAILURE);
  }
  for(i=0; i < NUMOFTOTALMACHTES; i++){
    if(strcmp(matches[i].weekDay,weekday)==0 && matches[i].time.hours >= from.hours && matches[i].time.hours <= to.hours && ((matches[i].time.hours*60)+matches[i].time.minutes) >= ((from.hours*60)+from.minutes) && ((matches[i].time.hours*60)+matches[i].time.minutes) <= ((to.hours*60)+to.minutes)){
      resultArray[numOfHits] = i;
      numOfHits++;
    }
  }
  *filteredMatches = malloc(numOfHits*sizeof(match));
  for(i=0; i<numOfHits; i++){
    (*filteredMatches)[i] = matches[resultArray[i]];
  }
  free(resultArray);
  return numOfHits;
}

/**
 * @brief      Finds the team with the lowest attendances in a given time span
 *
 * @param      teamname     The teamname
 * @param      attendances  The attendances
 * @param[in]  from         The from date
 * @param[in]  to           The to date
 * @param      matches      The matches
 */
void findTeamWithLowestAttendances(char *teamname, int *attendances, date from, date to, match *matches){
  int i, currentKey;
  match *filteredMatches;
  int numOfHits = filterMatchesByDate(from, to, matches, &filteredMatches);
  spectator *filteredAttendences = calloc(NUMOFTEAMS, sizeof(spectator));
  if (filteredAttendences == NULL){
    printf("%s", "Not enough ram, sorry..");
    exit(EXIT_FAILURE);
  }
  for(i=0; i<numOfHits; i++){
      currentKey = findSpectators(filteredMatches[i].homeTeam, filteredAttendences);
      strcpy(filteredAttendences[currentKey].teamname, filteredMatches[i].homeTeam);
      filteredAttendences[currentKey].attendances += filteredMatches[i].attendances;
  }

  for(i=0; i<NUMOFTEAMS; i++){
    if(filteredAttendences[i].attendances < *attendances || *attendances == 0){
          *attendances = filteredAttendences[i].attendances;
          strcpy(teamname, filteredAttendences[i].teamname);
      }
  }
  free(filteredAttendences);
  free(filteredMatches);
}

/**
 * @brief      Find the corosponding index key to the teamname in the spectator array
 *
 * @param      teamName     The team name
 * @param      attendances  The attendances
 *
 * @return     The index key
 */
int findSpectators(char *teamName, spectator *attendances){
  int i=0, found=0, nextTick=-1;
  while(!found && i < NUMOFTEAMS){
    if(!strcmp(attendances[i].teamname, teamName)){
        found = 1;
        return i;
    }else if(nextTick == -1 && strlen(attendances[i].teamname) == 0){
        nextTick = i;
    }
    i++;
  }
  return nextTick;
}

/**
 * @brief      filter an array of mathces by date.
 *
 * @param[in]  from             The from
 * @param[in]  to               The to date
 * @param      matches          The matches
 * @param      filteredMatches  The filtered matches
 *
 * @return     The size of the array
 */
int filterMatchesByDate(date from, date to, match *matches, match **filteredMatches){
  int i, numOfHits=0;
  int *returnArray = (int*) calloc(NUMOFTOTALMACHTES,sizeof(int));
  if(returnArray == NULL){
    exit(EXIT_FAILURE);
  }
  for(i=0; i<NUMOFTOTALMACHTES; i++){
      if(matches[i].date.day >= from.day && matches[i].date.month >= from.month && matches[i].date.year >= from.year && matches[i].date.day <= to.day && matches[i].date.month <= to.month && matches[i].date.year <= to.year){
          returnArray[numOfHits] = i;
          numOfHits++;
      }
  }
  *filteredMatches = calloc(numOfHits, sizeof(match));
  for(i=0; i<numOfHits; i++){
    (*filteredMatches)[i] = matches[returnArray[i]];
  }
  free(returnArray);
  return numOfHits;
}

/**
 * @brief      Finds an array of the teams who dominated away games
 *
 * @param      teams                The teams
 * @param      teamsDominatingAway  The teams dominating away
 *
 * @return     The size of the array
 */
int findTeamsDominatingAway(team *teams, team **teamsDominatingAway){
  int i, numOfHits=0;
  int *resultArray = calloc(NUMOFTEAMS, sizeof(int));;
  if (resultArray == NULL){
    printf("%s", "Not enough ram, sorry..");
    exit(EXIT_FAILURE);
  }
  for(i=0; i < NUMOFTEAMS; i++){
    if(teams[i].awayWins > teams[i].homeWins){
        resultArray[numOfHits] = i;
        numOfHits++;
    }
  }
  *teamsDominatingAway = calloc(numOfHits, sizeof(team));
  for(i=0; i<numOfHits; i++){
    (*teamsDominatingAway)[i] = teams[resultArray[i]];
  }
  free(resultArray);
  return numOfHits;
}

/**
 * @brief      Find the corosponding index key for a teamName in the team array.
 *
 * @param      teamName             The team name
 * @param      teams                The teams
 * @param      numOfGeneratedTeams  The number of generated teams
 *
 * @return     The index key
 */
int findTeam(char *teamName, team *teams, int *numOfGeneratedTeams){
  int i, nextTick=*numOfGeneratedTeams;
  for(i=0; i<NUMOFTEAMS; i++){
    if(strcmp(teams[i].name, teamName)==0)
      return i;
  }
    strcpy(teams[nextTick].name, teamName);
    *numOfGeneratedTeams +=1;
    return nextTick;
}

/**
 * @brief      Takes an array of matches, searches for draws with the delimiter of total goals.
 *
 * @param[in]  goalDelimiter  The goal delimiter
 * @param      matches        The matches
 * @param      draws          The draws
 *
 * @return     The arry size of draws
 */
int findDrawsSearch(int goalDelimiter, match *matches, match **draws){
  int *returnArray = (int*) calloc(NUMOFTOTALMACHTES, sizeof(int));
  int i, numOfHits=0;
  if (returnArray == NULL){
    printf("%s", "Not enough ram, sorry..");
    exit(EXIT_FAILURE);
  }
  for(i=0; i<NUMOFTOTALMACHTES; i++){
    if(matches[i].homeGoals == matches[i].awayGoals && matches[i].homeGoals+matches[i].awayGoals>=goalDelimiter){
        returnArray[numOfHits] = i;
        numOfHits++;
    }
  }
  *draws = (match*) calloc(numOfHits, sizeof(match));
  for(i=0; i<numOfHits; i++){
    (*draws)[i] = matches[returnArray[i]];
  }
  free(returnArray);
  return numOfHits;
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
