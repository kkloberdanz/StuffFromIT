/*
 * Programmer  : Kyle Kloberdanz
 * Date Created: 29 July 2015
 * Instructions: Save the names of the computers you want to ping
 *               to a file called: inputHostNames.txt
 *               Save this file to the same directory as this program
 *
 *               This program will create or overwrite a file called:
 *               validHostNames.txt
 *
 *               validHostNames.txt contains hosts than can be pinged
 *
 *               Accepts other files from the command line. 
 *               Type: "pingable myFile.txt" to run this program with
 *               the input file called "myFile.txt"
 */

#ifdef _WIN32
#include <windows.h>
#include <stdio.h>
#include <tchar.h>

#define DIV 1048576 
#define WIDTH 7
#endif

#ifdef linux
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#endif

#include <stdbool.h>

bool pingable(char*);

int main(int argc, char *argv[]){
    unsigned char IP[30];
	
	const char* defaultInput = "inputHostNames.txt";


    FILE *inputFile, *outputFile, *unpingable;

	if( argc > 1 ){
		printf("Reading from file: %s\n", argv[1]);
		inputFile = fopen(argv[1], "r");
	} else {
		printf("Reading from file: %s\n", defaultInput);
		inputFile = fopen(defaultInput, "r");
	}

    if( inputFile == NULL ){
        fprintf(stderr, "Cannot open input file!\n");
		puts("Press ENTER to exit");
		getchar();
        exit(1);
    }

    outputFile = fopen("validHostNames.txt", "w");
	unpingable = fopen("unpingable.txt", "w");
    
    while( fscanf(inputFile, "%s", &IP) != EOF ){
        if( pingable(IP) ){
            printf("%s Is Reachable!\n", IP);
            fprintf(outputFile, "%s\n", IP);
        } else {
			fprintf(unpingable, "%s\n", IP);
		}
    }

    fclose(outputFile);
    fclose(inputFile);
    puts("Press ENTER to exit");
    getchar();

    return 0;
}


bool pingable(char* IP){
#ifdef _WIN32
    // Works for Windows
    unsigned char toCommandLine[50] = "ping -n 1 ";
#else
    // Works for Linux/BSD/Macintosh/Unix
    unsigned char toCommandLine[50] = "ping -c 1 ";
#endif

    int response;

	strcat(toCommandLine, IP);
	response = system(toCommandLine);
    
    if( response == 0 ){
        return true;
    }
    else{
        return false;
    }
}
