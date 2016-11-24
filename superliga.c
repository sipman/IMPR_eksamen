#include <stdio.h>
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

int main(void){
  match matches[2] = {
                        {"R1", "Fre", {17,7,2015}, {18,00}, "FCN", "SDR", 0, 2, 3211},
                        {"R1", "Lor", {18,7,2015}, {17,30}, "FCK", "BIF", 7, 0, 43211}
                      };
  printf("%s %s %d/%d/%d %d:%d %s - %s %d-%d %d\n",
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
    );
  return 0;
}
