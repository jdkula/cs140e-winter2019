#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <boot-messages.h>

#include <demand.h>
#include "support.h"

// read entire file into buffer.  return it, write totat bytes to <size>
unsigned char* read_file(int *size, const char *name) {
  struct stat fileStat;
  if(stat(name, &fileStat) < 0) {
    panic("Stat failed");
  }
  off_t fileSize = fileStat.st_size;


  unsigned char* buf = malloc(fileSize);

  int fileFd = open(name, O_RDONLY);
  if(fileFd < 0) {
    panic("Could not open file...");
  }

  int thisRead = read(fileFd, buf, fileSize);
  assert(thisRead == fileSize);

  *size = fileSize;
	return buf;
}

#define _SVID_SOURCE
#include <dirent.h>
const char *ttyusb_prefixes[] = {
	"ttyUSB",	// linux
	"tty.SLAB_USB", // mac os
	0
};

int filter(const struct dirent* info) {
  int i = 0;
  while(ttyusb_prefixes[i] != 0) {
    if(strncmp(ttyusb_prefixes[i], info->d_name, strlen(ttyusb_prefixes[i])) == 0) {
      return 1;
    }
    i++;
  }

  return 0;
}

void clear_namelist(struct dirent** namelist, int numEntries) {
  for(int i = 0; i < numEntries; i++) {
    free(namelist[i]);
  }
  free(namelist);
}

// open the TTY-usb device:
//	- use <scandir> to find a device with a prefix given by ttyusb_prefixes
//	- returns an open fd to it
// 	- write the absolute path into <pathname> if it wasn't already
//	  given.
int open_tty(const char **portname) {
  struct dirent** namelist;
  int numEntries = scandir("/dev", &namelist, filter, alphasort);
  if(numEntries == 0) return -1;
  
  char* out = malloc(PATH_MAX);
  strcpy(out, "/dev/");
  strcat(out, namelist[0]->d_name);
  clear_namelist(namelist, numEntries);

  *portname = out;

  return open(*portname, O_RDWR | O_NOCTTY | O_SYNC);
}
