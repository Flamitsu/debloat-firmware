#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/klog.h>
#include <sys/stat.h>
#include <errno.h>
// This number is based on the Linux Firmware package number of items at date 03/05/2026.
// It's imposible to your computer to have more than this firmware loaded at the same time lmao
#define MAX_ITEMS 6116
int firmware_count = 0;
char *firmware_array[MAX_ITEMS];

// Adds a new item if it is not already in the array
void add_if_new(const char *fw_name){
  for(int i = 0; i < firmware_count; i++){
    if (strcmp(firmware_array[i], fw_name) == 0){
      return;
    }
  }

  if (firmware_count < MAX_ITEMS) {
    firmware_array[firmware_count] = strdup(fw_name);
    firmware_count ++;
  }

}

// Scans the buffer in the kernel and adds it in the firmware_array 
void search_dmesg_string(const char *pattern) {
  // 10 is the command SYSLOG_ACTION_SIZE_BUFFER, asks the kernel how long is the buffer right now
  int buffer_size = klogctl(10, NULL, 0);
  
  if (buffer_size <= 0){
    return;
  }
  
  // +1 here is because C needs the String to end in a '\0' character (NULL)
  char *buffer = malloc(buffer_size + 1);
  // 3 says to the kernel to read the buffer but do not delete it, and storages all the buffer
  int read_ptr = klogctl(3, buffer, buffer_size);
  buffer[read_ptr] = '\0';

  char *line = strtok(buffer, "\n");
  while (line != NULL) {
    char *match = strstr(line, pattern);
    
    if (strstr(line, pattern)){
      char *fw_ptr = match + strlen(pattern);
      add_if_new(fw_ptr);
    }
    
    line = strtok(NULL, "\n");
  }

  free(buffer);
}

void save_to_file(const char *file_name){
  FILE *file = fopen(file_name, "w");
  
  if (file == NULL){
    fprintf(stderr, "Error trying to create the file %s: %m\n",file_name);
    return;
  }
  
  for(int i = 0; i < firmware_count; i++){
    fprintf(file, "%s\n", firmware_array[i]);
  }
  
  fclose(file);
}

void create_directory(const char *path) {
  // If the mkdir command gives an error or it exists
  if (mkdir(path, 0755) == -1 && errno != EEXIST){
    fprintf(stderr, "Error trying to create %s: %m\n",path);
  }
}

int main(){
  search_dmesg_string("Loading firmware: ");
  
  create_directory("/etc/portage/savedconfig");
  create_directory("/etc/portage/savedconfig/sys-kernel");
  
  save_to_file("/etc/portage/savedconfig/sys-kernel/linux-firmware");
  return 0;
}
