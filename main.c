#include <stdio.h>
#include <string.h>
int main(){
    // Open all the needed files
    // This will read the ring kernel buffer but the problem is that it gets blocked.
    // Working on fixing it soon
    FILE *openLog = fopen("/dev/kmsg", "r+");
    FILE *openWriteFile = fopen("linux-firmware","w+");
   if (!openWriteFile | !openLog){
       return 1;
   }
    // Define what dmesg string needs to search and the char per line addmited

    const char *searchingString = "Loading firmware: ";
    char charLines[256];

    // this while reads and process each line of the log
    while (fgets(charLines, sizeof(charLines), openLog)){
        // Saving the name into a function that read the lines of the log and save it into a pointer
        char *firmwareName = strstr(charLines, searchingString);
        // if the string matches:
        if (firmwareName){
            // Uses the variable to look for the next to "Loading firmware: "
            firmwareName += strlen(searchingString);
            fprintf(openWriteFile, "%s", firmwareName);
            printf("%s", firmwareName);
        }
    }
    fclose(openLog);
    fclose(openWriteFile);
    return 0;
}
