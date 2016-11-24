#include <stdio.h>
#include <stdlib.h>
#define SOURCEFILE "superliga-2015-2016"
typedef struct {
  int day;
  int month;
  int year;
} date;
typedef struct {
  int hours;
  int minutes;
} time;
typedef struct {
  char round[4];
  char weekDay[4];
  date date;
  time time;
  char homeTeam[4];
  char awayTeam[4];
  int homeGoals;
  int awayGoals;
  int attendances;
} match;
void initiateData(int *numberOfRounds, int *numberOfMatches, FILE *inputFile);
match generateMatchFromStr(char *str);
int getNumberOfRounds(char *round, int currentRoundNumber);
int main(void){
  int numberOfRounds=0, numberOfMatches=0;
  FILE *input = fopen(SOURCEFILE, "r");
  if (input == NULL){
        exit(EXIT_FAILURE);
  }
  initiateData(&numberOfRounds, &numberOfMatches, input);

  /*printf("%s %s %02d/%02d/%d %02d:%02d %s - %s %d-%d %d\n",
        matches[0].round,
        matches[0].weekDay,
        matches[0].date.day,
        matches[0].date.month,
        matches[0].date.year,
        matches[0].time.hours,
        matches[0].time.minutes,
        matches[0].homeTeam,
        matches[0].awayTeam,
        matches[0].homeGoals,
        matches[0].awayGoals,
        matches[0].attendances
    );*/
  fclose(input);
  printf("Number of matches: %d\n", numberOfMatches);
  printf("Number of rounds: %d\n", numberOfRounds);
  return 0;
}

void initiateData(int *numberOfRounds, int *numberOfMatches, FILE *inputFile){
  char str[255];
  match currentMatch;
   while (fgets(str, sizeof(str), inputFile)) {
        *numberOfMatches += 1;
        currentMatch = generateMatchFromStr(str);
        *numberOfRounds = getNumberOfRounds(currentMatch.round, *numberOfRounds);
    }
}

match generateMatchFromStr(char *str){
  match currentMatch;
  double attendances = 0;
  sscanf(str, " %[R0123456789] %s %d/%d/%d %d.%d %s - %s %d - %d %lf",
              currentMatch.round,
              currentMatch.weekDay,
              &currentMatch.date.day,
              &currentMatch.date.month,
              &currentMatch.date.year,
              &currentMatch.time.hours,
              &currentMatch.time.minutes,
              currentMatch.homeTeam,
              currentMatch.awayTeam,
              &currentMatch.homeGoals,
              &currentMatch.awayGoals,
              &attendances
        );
  currentMatch.attendances = (attendances*1000);
  return currentMatch;
}

int getNumberOfRounds(char *round, int currentRoundNumber){
    int roundNum;
    sscanf(round, "R%d", &roundNum);
    if(roundNum < currentRoundNumber){
        return currentRoundNumber;
    }
    return roundNum;
}
