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
  char round[3];
  char weekDay[3];
  date date;
  time time;
  char homeTeam[3];
  char awayTeam[3];
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
        matches[1].round,
        matches[1].weekDay,
        matches[1].date.day,
        matches[1].date.month,
        matches[1].date.year,
        matches[1].time.hours,
        matches[1].time.minutes,
        matches[1].homeTeam,
        matches[1].awayTeam,
        matches[1].homeGoals,
        matches[1].awayGoals,
        matches[1].attendances
    );
  return 0;
}
