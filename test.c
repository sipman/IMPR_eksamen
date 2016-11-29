#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
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
  char teamname[TEAMNAMEBUFFER];
  int attendances;
} spectator;
typedef struct {
  int round;
  char weekDay[WEEKDAYBUFFER];
  date date;
  time time;
  char homeTeam[TEAMNAMEBUFFER], awayTeam[TEAMNAMEBUFFER];
  int homeGoals, awayGoals;
  int attendances;
} match;
void printAMatch(match match);
void printAllMatches(match *matches, int numberOfMatches);
void generateMatchFromStr(char *str, match *destination);
void prepareData(match **matches, FILE *inputFile);
int main(void){
  FILE *input = fopen(SOURCEFILE, "r");
  match *season = calloc(NUMOFTOTALMACHTES, sizeof(match));
  if(input == NULL){
    printf("File not found...\n");
    exit(EXIT_FAILURE);
  }
  if (season == NULL){
    printf("%s", "Not enough ram, sorry..");
    exit(EXIT_FAILURE);
  }
  prepareData(&season, input);
  fclose(input);
  printAllMatches(season, NUMOFTOTALMACHTES);

  free(season);

  return 0;
}

void prepareData(match **matches, FILE *inputFile){
  char *str = (char*) calloc(MAXLINELENGTH, sizeof(char));
  int i=0;
  while (fgets(str, MAXLINELENGTH, inputFile)) {
    generateMatchFromStr(str, &(*matches)[i]);
    i++;
  }
  free(str);
}
void generateMatchFromStr(char *str, match *destination){
  double attendances = 0;
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
}
void printAllMatches(match *matches, int numberOfMatches){
  int i;
  for(i=0; i<numberOfMatches; i++){
    printAMatch(matches[i]);
  }
}
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
