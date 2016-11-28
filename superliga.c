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
  int round, goals;
} round;

typedef struct {
  round *round;
  char weekDay[4];
  date date;
  time time;
  char homeTeam[4], awayTeam[4];
  int homeGoals, awayGoals, attendances;
} match;

void initateData(int *maxLineLength, int *numberOfMatches, int *numberOfRounds, FILE *inputFile);
void prepareData(match *matches, round *rounds, int str_buffer, FILE *inputFile);
void showDrawMatches(int goalDelimiter, match *matches, int numberOfMatches);
void showARoundWithLesserGoals(int goalDelimiter, round *rounds, int numberOfRounds);
void printAMatch(match match);
void printAllMatches(match *matches, int numberOfMatches);
match generateMatchFromStr(char *str, round *rounds);
int generateNumberOfRounds(int roundNumber, int currentRoundNumber);
match *findDrawsSearch(int goalDelimiter, match *matches, int matches_length);
void findFirstRoundWithLesserGoals(int *resultRound, int *resultGoals, int goalDelimiter, round *rounds, int numberOfRounds);

int main(void){
  FILE *input = fopen(SOURCEFILE, "r");
  int maxLineLength, numberOfMatches, numberOfRounds;
  match *season;
  round *rounds;
  if (input == NULL){
    printf("%s", "Not enough ram, sorry..");
    exit(EXIT_FAILURE);
  }
  initateData(&maxLineLength, &numberOfMatches, &numberOfRounds, input);
  season = malloc(numberOfMatches*sizeof(match));
  rounds = malloc(numberOfRounds*sizeof(rounds));
  if (season == NULL || rounds == NULL){
    printf("%s", "Not enough ram, sorry..");
    exit(EXIT_FAILURE);
  }
  prepareData(season, rounds, maxLineLength, input);
  fclose(input);
  showARoundWithLesserGoals(10, rounds, numberOfRounds);
  /*showDrawMatches(4, season, numberOfMatches);*/
  /*printAllMatches(season, numberOfMatches);*/

  return 0;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * DATA FUNCTIONS  * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
 * @brief      Counts the amount of newlines in a text file, it start by counting 1 line
 *             in order to return correct number total lines in the file.
 *             Also counts max line length.
 *
 * @param      maxLineLength    The maximum line length
 * @param      numberOfMatches  The number of matches
 * @param      numberOfRounds   The number of rounds
 * @param      inputFile        The input file
 */
void initateData(int *maxLineLength, int *numberOfMatches, int *numberOfRounds,  FILE *inputFile){
  int charCount=0, maxCharCount=0;
  char ch;
  fpos_t position;
  while(!feof(inputFile)){
    ch = fgetc(inputFile);
    charCount++;

    if(ch == '\n'){
      if(charCount>maxCharCount){
        maxCharCount = charCount;
      }
      fgetpos(inputFile, &position);
      charCount = 0;
      *numberOfMatches += 1;
    }
  }
  fsetpos(inputFile, &position);
  fscanf(inputFile, " R%d %*[^\n]",
        numberOfRounds
  );
  rewind(inputFile);
  *maxLineLength = maxCharCount;
}
/**
 * @brief      Parses data from file to the array of match struct
 *
 * @param      matches     The array of matches
 * @param      rounds      The array of rounds
 * @param[in]  str_buffer  The string buffer
 * @param      inputFile   The input file
 */
void prepareData(match *matches, round *rounds, int str_buffer, FILE *inputFile){
  char *str = (char*) malloc(str_buffer+1);
  int i=0;
  /* DEVNOTE: why is +1 neede ? no one knows....
              why dosn't sizeof(str) work ? what a mistery !!*/
  while (fgets(str, str_buffer+1, inputFile)) {
    matches[i] = generateMatchFromStr(str, rounds);
     i++;
  }
  free(str);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * OUTPUT FUNCTIONS  * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
 * @brief      Shows the rounds with lesser total goals than the delimiter.
 *
 * @param[in]  goalDelimiter   The goal delimiter.
 * @param[in]  *rounds         The array of rounds.
 * @param[in]  numberOfRounds  The number of rounds in the array.
 */
void showARoundWithLesserGoals(int goalDelimiter, round *rounds, int numberOfRounds){
  int resultRound, resultGoals;
  findFirstRoundWithLesserGoals(&resultRound, &resultGoals, goalDelimiter, rounds, numberOfRounds);
  printf("Round: %2d\t Goals: %d\n", resultRound, resultGoals);
}
/**
 * @brief      Shows the draw matches with lesser total goal than the delimiter.
 *
 * @param[in]  goalDelimiter    The goal delimiter.
 * @param[in]  *matches         The array of matches.
 * @param[in]  numberOfMatches  The number of matches in the array.
 */
void showDrawMatches(int goalDelimiter, match *matches, int numberOfMatches){
  match *draws = findDrawsSearch(4, matches, numberOfMatches);
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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * HELPER FUNCTIONS  * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
 * @brief      Takes a string and parse it to a match struct
 *               - also it generates the accordently rounds array with total goals
 *
 * @param[in]  str     The string.
 * @param      *rounds The array of rounds.
 *
 * @return     The match struct.
 */
match generateMatchFromStr(char *str, round *rounds){
  match currentMatch;
  double attendances = 0;
  int round;
  sscanf(str, " R%02d %s %d/%d/%d %d.%d %s - %s %d - %d %lf",
        &round,
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
  currentMatch.round = &rounds[(round-1)];
  currentMatch.round->round = round;
  currentMatch.round->goals += currentMatch.homeGoals+currentMatch.awayGoals;
  return currentMatch;
}
/**
 * @brief      Takes an array of matches, searches for draws with the delimiter of total goals and returns an array pointer
 *
 * @param[in]  goalDelimiter   The goal delimiter
 * @param[in]  *matches        The array matches
 * @param[in]  matches_length  The matches array length
 *
 * @return     Returns the pointer to the resultArray containing all the hits.
 */
match *findDrawsSearch(int goalDelimiter, match *matches, int matches_length){
  int *hits = (int*) malloc(matches_length*sizeof(int));
  match *returnArray;
  int i, numOfHits=0;
  if (hits == NULL){
    printf("%s", "Not enough ram, sorry..");
    exit(EXIT_FAILURE);
  }
  for(i=0; i<matches_length; i++){
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
 * @param[in]  numberOfRounds  The number of rounds in array
 */
void findFirstRoundWithLesserGoals(int *resultRound, int *resultGoals, int goalDelimiter, round *rounds, int numberOfRounds){
  int i;
  for(i=0; i<numberOfRounds; i++){
      if(rounds[i].goals < goalDelimiter){
        *resultRound = rounds[i].round;
        *resultGoals = rounds[i].goals;
      }
    }
}
