#include <autodim.h>

#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

// How often daemon checks status of power supply in seconds
#define INTERVAL 1
// Brightness on battery
#define DIM_PERCENT 0.25
// Default string length for reading max brightness
#define LENGTH 4

int autodim() {
  /* TODO: Current paths are hardcoded, need to develop alternative methods
            to dynamically detect paths
     TODO: Currently require changing file permissions to change brightness
  */
  int powerReg = open("/sys/class/power_supply/ADP1/online", O_RDONLY);
  int brightnessReg = open("/sys/class/backlight/intel_backlight/brightness", O_RDWR);
  int maxBrightnessReg = open("/sys/class/backlight/intel_backlight/max_brightness", O_RDONLY);
  if (powerReg < 0 || brightnessReg < 0 || maxBrightnessReg < 0) return 1;
  char pastInput;
  char input;

  unsigned int length = LENGTH;
  char* maxBrightness = malloc(LENGTH);

  unsigned int i = 0;
  if (read(maxBrightnessReg, &input, 1) < 0) return 1;
  // TODO: terminating character hardcoded to 10, change to be more flexible
  while (input != 10) {
    maxBrightness[i++] = input;
    if (i == length) {
      length = i+1;
      maxBrightness = realloc(maxBrightness, length);
    }
    read(maxBrightnessReg, &input, 1);
  }
  // Handle for string shorter than default size
  length = strlen(maxBrightness);
  char* brightness = malloc(length);
  char* dimBrightness = malloc(length);
  if (read(brightnessReg, brightness, length) < 0) return 1;
  sprintf(dimBrightness, "%d\n", (int)(atoi(maxBrightness) * DIM_PERCENT));
  close(maxBrightnessReg);

  if (read(powerReg, &input, 1) < 0) return 1;

  while (1) {
    pastInput = input;
    if(read(powerReg, &input, 1) != 1) return 1;
    if(lseek(powerReg, 0, SEEK_SET) != 0) return 1;
    if (input != pastInput) {
      if (input == '0') {
        if (read(brightnessReg, brightness, length) < 0) return 1;
        if (write(brightnessReg, dimBrightness, length) < 0) return 1;
        if (lseek(brightnessReg, 0, SEEK_SET) != 0) return 1;
      }
      else {
        // FIXME: Write does not work when ran as daemon
        write(brightnessReg, brightness, length);
        if(lseek(brightnessReg, 0, SEEK_SET) != 0) return 1;
      }
    }
    // fflush(stdout);
    sleep(INTERVAL);
  }
  return 0;
}
