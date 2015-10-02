/*
 * Programmer  : Kyle Kloberdanz
 * Date Created: 29 July 2015
 *
 *
 * Instructions: Save the names of the computers you want to ping
 *               to a file called: inputHostNames.txt
 *               Save this file to the same directory as this program
 *
 *               OR
 *
 *               To read from a file of a different name, type: pingable.exe aDifferentFile.txt
 *
 *               OR
 *
 *               To read from a file of a different name AND specify the output file, 
 *               type: pingable.exe aDifferentFile.txt -o aDifferentOutputFile.txt
 *
 *               This program will otherwise create or overwrite a file called:
 *               validHostNames.txt
 *
 *               validHostNames.txt will then contain hosts than can be pinged
 *
 *
 *               For the input File:
 *                   Comments can be added like this: 
 *                   # This is a comment #
 *                   The comment must be on its own line, with space between the words and # symbols
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
    char inputFileName[30] = "";


    FILE *firstInputFile, *inputFile, *outputFile, *unpingable;

    /*
     * If another file is specified from the command line,
     * then the first specified file will be used as inputFile and firstInputFile
     * 
     * if the next 2 arguments are specified as: -o someOutputFile.txt
     * then someOutpurFile.txt will be erased and written to as the output
     */
    if( argc == 1 ){
        /* no argument was specified from the command line */
        strcat( inputFileName, defaultInput );
    } else if( (argc == 2) || (argc == 4) ){
        strcat( inputFileName, argv[1] );
	} else {
        puts("ERROR: and invalid number of arguments was specified");
        exit(1);
	}

    if( argc == 4 ){
        /* strcmp returns 0 if strings are the same */
        if( !(strcmp(argv[2], "-o")) ){
            outputFile = fopen( argv[3], "w" );
        } else {
            puts("ERROR: When using multiple arguments from command line, use -o to denote the output file");
            exit(1);
        }
    } else {
        outputFile = fopen( "validHostNames.txt", "w" );
    }

    printf("Reading from file: %s\n", inputFileName);
    firstInputFile = fopen( inputFileName, "r" );

	unpingable = fopen("unpingable.txt", "w");
    
    /*
     * Determines how large the file is by counting each word
     */
    puts("Callculating file length ...");
    double i = 0.0;
    while( fscanf(firstInputFile, "%s", &junk) != EOF ){
        i++;
    }
    fclose( firstInputFile );

    inputFile = fopen( inputFileName, "r" );

    puts("Pinging ...");
    double j = 0.0;

    /* Header for the output text file */
    fprintf( outputFile, "From file: %s\n", inputFileName );

    while( fscanf(inputFile, "%s", &IP) != EOF ){
        printf("Progress: %d%%\n", (int) (100 * (j / i)) );

        /* if line is a comment, just print it to the output */
        if( IP[0] == '#' ){
            fprintf( outputFile, "\n%s ", IP );
            if( fscanf( inputFile, "%s", &IP ) != EOF ) {
                fprintf( outputFile, "%s ", IP );
                j++;
                while( (IP[0] != '#')  && fscanf(inputFile, "%s", &IP) != EOF ){
                    fprintf( outputFile, "%s ", IP );
                    j++;
                }
                fprintf( outputFile, "%c", '\n' );
            }


        /* ping host, if reachable, print to file */
        }  else if( pingable(IP) ){
            fprintf( outputFile, "%s\n", IP );
            printf( "%s IS UP!\n", IP );

        /* host is not reachable */
        } else {
            printf("%s %s\n", IP, "IS DOWN!");
        }
        j++;
    }

    printf("Progress: %d%%\n", 100 );

    fclose(inputFile);
    fclose(outputFile);
    fclose(unpingable);
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
    } else {
        return false;
    }
}
