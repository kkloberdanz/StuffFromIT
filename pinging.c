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
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool pinging(char*);

int main(int argc, char *argv[]){
    unsigned char IP[30];


    FILE *inputFile, *outputFile;
    /*
     * TODO:
     * implement so if one provides a file through command line,
     * it overrides the standard input file
     */

    /*inputFile = fopen(argv[1], "r");*/
    inputFile = fopen("inputHostNames.txt", "r");

    if( inputFile == NULL ){
        fprintf(stderr, "Cannot open input file!\n");
        exit(1);
    }

    outputFile = fopen("validHostNames.txt", "w");
    
    while( fscanf(inputFile, "%s", &IP) != EOF ){
        if( pinging(IP) ){
            printf("%s Is Reachable!\n", IP);
            fprintf(outputFile, "%s\n", IP);
        }
    }

    fclose(outputFile);
    fclose(inputFile);
    puts("Press ENTER to exit");
    getchar();

    return 0;
}


bool pinging(char* IP){
    unsigned char toCommandLine[50] = "ping -n 1 ";
    int response;

    int i;
    for(i = 0; IP[i] != '\0'; i++){
        toCommandLine[i + 10] = IP[i];
    }

    response = system(toCommandLine);
    

    if( response == 0 ){
        return true;
    }
    else{
        return false;
    }
}
