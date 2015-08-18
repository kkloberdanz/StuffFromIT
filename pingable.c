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
 *               Type: "pingable.exe myFile.txt" to run this program with
 *               the input file called "myFile.txt"
 *
 *
 *               For the input File:
 *                   Comments can be added like this: 
 *                       # This is a comment #
 *                   The comment must be between two # symbols
 *                   The comments will appear in the output file
 *
 *                   Comments can be used to specify which group
 *                   the host belongs to
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

bool pingable(unsigned char*);

int main(int argc, char *argv[]){
    unsigned char IP[30];
    unsigned char junk[30];
	
	const char* defaultInput = "inputHostNames.txt";


    FILE *firstInputFile, *inputFile, *outputFile, *unpingable;

    /*
     * If another file is specified from the command line,
     * then the first specified file will be used, and any
     * other inputs will be ignored
     */
	if( argc > 1 ){
		printf("Reading from file: %s\n", argv[1]);
		inputFile = fopen(argv[1], "r");
		firstInputFile = fopen(argv[1], "r");
	} else {
		printf("Reading from file: %s\n", defaultInput);
		inputFile = fopen(defaultInput, "r");
		firstInputFile = fopen(defaultInput, "r");
	}

    if( inputFile == NULL ){
        fprintf(stderr, "Cannot open input file!\n");
		puts("Press ENTER to exit");
		getchar();
        exit(1);
    }

    outputFile = fopen("validHostNames.txt", "w");
	unpingable = fopen("unpingable.txt", "w");
    
    puts("Callculating file length ...");
    double i = 0.0;
    while( fscanf(firstInputFile, "%s", &junk) != EOF ){
            i++;
    }

    puts("Pinging ...");
    double j = 0.0;

    /* Head for the output text file */
    if( argc > 1 ){
        fprintf(outputFile, "From file: %s\n", argv[1]);
    } else {
        fprintf(outputFile, "From file: %s\n", defaultInput);
    }
    while( fscanf(inputFile, "%s", &IP) != EOF ){
        printf("Progress: %d%%\n", (int) (100 * (j / i)) );

        /* if line is not a comment */
        if( (IP[0] != '#') && (IP[0] != '\n') ){
            if( pingable(IP) ){
                printf("%s Is Reachable!\n", IP);
                fprintf(outputFile, "%s\n", IP);
            } else {
                /* if host cannot be reached */
                fprintf(unpingable, "\n%s\n", IP);
            }
        } else if( IP[0] == '#' ){
            /* write comment to file */
            fprintf(outputFile, "\n%s ", IP);
            fscanf(inputFile, "%s", &IP);
            j++;
            while( (IP[0] != '\n') && (IP[0] != '#') ){
                fprintf(outputFile, "%s ", IP);
                fscanf(inputFile, "%s", &IP);
                j++;
            }
            fprintf(outputFile, "# \n");
        }
        j++;
    }

    printf("Progress: %d%%\n", 100 );

    fclose(outputFile);
    fclose(inputFile);
    fclose(outputFile);
    fclose(firstInputFile);
    puts("Press ENTER to exit");
    getchar();

    return 0;
}


bool pingable(unsigned char* IP){
#ifdef _WIN32
    // Works for Windows
    unsigned char toCommandLine[50] = "ping -n 1 ";
#else
    // Works for Linux
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
