#include <stdio.h>
#include <stdlib.h>
#define SOURCEFILE "superliga-2015-2016"

typedef struct {
  int day, month, year;
} date;

typedef struct {
  int hours, minutes;
} time;

typedef struct {
  int round;
  char weekDay[4];
  date date;
  time time;
  char homeTeam[4], awayTeam[4];
  int homeGoals, awayGoals, attendances;
} match;

match *findDrawsSearch(int goalDelimeter, match *matches, int matches_length);
int countLines(int *outputLineLength, FILE *inputFile);
void prepareData(match *matches, int str_buffer, FILE *inputFile);
match generateMatchFromStr(char *str);
int getNumberOfRounds(int roundNum, int currentRoundNumber);
void printAllMatches(match *matches, int numberOfMatches);

int main(void){
  FILE *input = fopen(SOURCEFILE, "r");
  int maxLineLength, numberOfMatches = countLines(&maxLineLength,input);
  match *draws;
  int i;
  match *season = malloc(numberOfMatches*sizeof(match));
  if (input == NULL){
        exit(EXIT_FAILURE);
  }
  prepareData(season, maxLineLength, input);
  fclose(input);

  draws = findDrawsSearch(4, season, numberOfMatches);
  for (i = 0; i < numberOfMatches; i++ ) {
    if((draws+i)->round == 0){
      break;
    }
      printf("%02d %s %02d/%02d/%d %02d:%02d %s - %s %d-%d %d\n",
        (draws+i)->round,
        (draws+i)->weekDay,
        (draws+i)->date.day,
        (draws+i)->date.month,
        (draws+i)->date.year,
        (draws+i)->time.hours,
        (draws+i)->time.minutes,
        (draws+i)->homeTeam,
        (draws+i)->awayTeam,
        (draws+i)->homeGoals,
        (draws+i)->awayGoals,
        (draws+i)->attendances);
   }
  /*printAllMatches(season, numberOfMatches);*/

  return 0;
}

match *findDrawsSearch(int goalDelimeter, match *matches, int matches_length){
  match *hits = (match*) malloc(matches_length*sizeof(match));
  int i, numOfHits=0;
    for(i=0; i<matches_length; i++){
        if(matches[i].homeGoals == matches[i].awayGoals && matches[i].homeGoals+matches[i].awayGoals==goalDelimeter){
            hits[numOfHits] = matches[i];
            numOfHits++;
        }
    }
  return hits;
}

/**
 * Counts the amount of newlines in a text file, it start by counting 1 line
 * in order to return correct number total lines in the file.
 * @FILE*   inputFile [The text file]
 * @return  int       [The total amount of lines in the file]
 */
int countLines(int *outputLineLength, FILE *inputFile){
    int numberOfMatches=1, charCount=0, maxCharCount=0;
    char ch;
    while(!feof(inputFile))
    {
      ch = fgetc(inputFile);
      charCount++;
      if(ch == '\n'){
        if(charCount>maxCharCount){
            maxCharCount = charCount;
        }
        charCount = 0;
        numberOfMatches += 1;
      }
    }
    rewind(inputFile);
    *outputLineLength = maxCharCount;
    return numberOfMatches;
}

/**
 * Parses data from file to the array of match struct
 * @match*  season     [The array of match structs]
 * @int     str_buffer [The buffer size of the string array]
 * @FILE*   inputFile  [The text file]
 */
void prepareData(match *matches, int str_buffer, FILE *inputFile){
    char *str = (char*) malloc(str_buffer+1);
    int i=0;
    /* DEVNOTE: why is +1 neede ? no one knows....
                why dosn't sizeof(str) work ? what a mistery !!*/
    while (fgets(str, str_buffer+1, inputFile)) {
        matches[i] = generateMatchFromStr(str);
        i++;
    }
    free(str);
}

/**
 * [HELPER FUNCTION TO iniateData]
 * Takes a string and parse it to a match struct
 * @char*   str    [The string array from the text line]
 * @return  match  [The match struct containing all the data]
 */
match generateMatchFromStr(char *str){
  match currentMatch;
  double attendances = 0;
  sscanf(str, " R%02d %s %d/%d/%d %d.%d %s - %s %d - %d %lf",
              &currentMatch.round,
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

/**
 * [DEVELOPER FUNCTION]
 * Simply prints out an array of matches in the same format as the textfile.
 * @match* matches          [An array containing match structs]
 * @int    numberOfMatches  [The number of matches that are in the array] (because C89 sucks)
 */
void printAllMatches(match *matches, int numberOfMatches){
  int i;
  for(i=0; i<numberOfMatches; i++){
      printf("%d %s %02d/%02d/%d %02d:%02d %s - %s %d-%d %d\n",
        matches[i].round,
        matches[i].weekDay,
        matches[i].date.day,
        matches[i].date.month,
        matches[i].date.year,
        matches[i].time.hours,
        matches[i].time.minutes,
        matches[i].homeTeam,
        matches[i].awayTeam,
        matches[i].homeGoals,
        matches[i].awayGoals,
        matches[i].attendances
    );
  }
}
