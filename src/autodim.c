#include <autodim.h>

#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

int autodim() {
  int powerFlag = open("/sys/class/power_supply/ADP1/online", O_RDONLY);
  char pastInput;
  char input;
  FILE *ptr;
  char previousBrightness[9];
  char commandBase[] = "xbacklight -set ";
  char restoreCommand[25];

  read(powerFlag, &input, 1);
  ptr = popen("xbacklight -get", "r");
  fgets(previousBrightness, 9, ptr);
  strcpy(restoreCommand, commandBase);
  strcat(restoreCommand, previousBrightness);
  pclose(ptr);

  while (1) {
    pastInput = input;
    if(read(powerFlag, &input, 1) != 1) return 1;
    if(lseek(powerFlag, 0, SEEK_SET) != 0) return 1;
    if (input != pastInput) {
      if (input == '0') {
        ptr = popen("xbacklight -get", "r");
        fgets(previousBrightness, 9, ptr);
        strcpy(restoreCommand, commandBase);
        strcat(restoreCommand, previousBrightness);
        pclose(ptr);
        system("xbacklight -set 30");
      }
      else
        system(restoreCommand);
    }
    fflush(stdout);
    sleep(1);
  }
  return 0;
}
