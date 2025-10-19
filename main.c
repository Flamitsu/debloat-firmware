#include <stdio.h>
#include <string.h>
int main(){
    // Open all the needed files
    // Opening the /dev/kmsg file should be the last option
    // if i dont find any log file to rely on and the linux-firmware is empty
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
