/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: KnightDanila
 *
 * Created on 22 апреля 2019 г., 17:40
 */
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <string.h>

#include <queue>

using namespace std;

/*
 * 
 */
template < typename T>
void println(T i) {
    cout << i << endl;
}

void argsEcho(int argc, char** argv) {
    println("____ARGS____");
    if (argc == 0) {
        println("NO ARGS");
    } else {
        for (int i = 0; i < argc; i++) {
            println(argv[i]);
        }
    }
    println("____________");
}
// Load Binary data from File

char *loadBinDatFile(char *file, long &size) {
    FILE *fp;
    long lSize;
    char *buffer;

    fp = fopen(file, "rb");
    if (!fp) perror(file), exit(1);

    fseek(fp, 0L, SEEK_END);
    lSize = (unsigned long) ftell(fp);
    rewind(fp);

    /* allocate memory for entire content */
    buffer = (char*) calloc(1, lSize + 1);
    if (!buffer) fclose(fp), fputs("memory alloc fails", stderr), exit(1);

    /* copy the file into the buffer */
    if (1 != fread(buffer, lSize, 1, fp))
        fclose(fp), free(buffer), fputs("entire read fails", stderr), exit(1);


    /* do your work here, buffer is a string contains the whole text */
    size = lSize;
    fclose(fp);
    return buffer;
}

int main(int argc, char** argv) {
    argsEcho(argc, argv);

    char inFileName[50] = "test.txt";
    char outFileName[50] = "test_new.txt";

    if (argc == 3) {
        strcpy(inFileName, argv[1]);
        strcpy(outFileName, argv[2]);
    }

    long int fileInSize = 0;
    char *fileInBytes = loadBinDatFile(inFileName, fileInSize);

    FILE* fileOut;
    fileOut = fopen(outFileName, "wb");


    if (fileOut == NULL && fileInSize <= 0) {
        if (fileOut == NULL) {
            perror("Error - cannot open output file");
        }
        if (fileInSize <= 0) {
            perror("Error - input file is empty");
        }
    } else {


        /*
        long int fileSize = 0;
        fseek(file, 0, SEEK_END);
        fileSize = (unsigned long) ftell(file);
        printf("%ld\n", fileSize);
        fseek(file, 0, SEEK_SET);
         */

        char searchBytes[] = {0x0a};
        //char newBytes [] = {0x11, 0x13};
        char replaceBytes[] = {0x0d, 0x0a};
        printf("Size of searchBytes %d\n", sizeof (searchBytes));
        printf("Size of replaceBytes %d\n", sizeof (replaceBytes));
        printf("Size of file %d\n", fileInSize);
        printf("______FileData______\n");
        println(fileInBytes); //println(&fileInBytes[0]);
        printf("____________________\n");

        queue<long int> nQueue;

        // For CRLF
        char d = {0x0d};
        for (long int i = 0; i < fileInSize; i++) {
            printf("Progress:%d --- byte:%d-memAdr:%d\n", ((i + 1)*100 / fileInSize), fileInBytes[i], &fileInBytes[i]);

            //char test[] = {0x00};
            //fwrite(test, sizeof (char), sizeof (char), fileOut);

            //For CRLF {0x0d, 0x0a} - fileInBytes[i-1] != d 
            if (memcmp(&fileInBytes[i], searchBytes, sizeof (searchBytes)) == 0 && fileInBytes[i - 1] != d) {
                nQueue.push(i);
                printf("FOUND --- numberOfBite:%ld-byte:%d\n", i, fileInBytes[i]);
                fwrite(replaceBytes, sizeof (char), sizeof (replaceBytes), fileOut);
                i += sizeof (searchBytes) - 1;
            } else {
                fwrite(&fileInBytes[i], sizeof (char), sizeof (char), fileOut);
            }

        }
        fclose(fileOut);
    }

    system("pause");
    return 0;
}

