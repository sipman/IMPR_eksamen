#include <stdio.h>
#include <stdlib.h>

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

int main (void){
  FILE *input = fopen(SOURCEFILE, "r");
  match *season = malloc(NUMOFTOTALMACHTES*sizeof(match));
  if(input == NULL){
    printf("File not found...\n");
    exit(EXIT_FAILURE);
  }
  if (season == NULL){
    printf("%s", "Not enough ram, sorry..");
    exit(EXIT_FAILURE);
  }
  prepareData(input, season);
  fclose(input);

  free(season);
  return 0;
}
