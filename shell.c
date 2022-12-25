#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>
#include <signal.h>

#define MAXCOM 1000 // max number of letters to be supported
#define MAXLIST 100 // max number of commands to be supported

// Clearing the shell using escape sequences
#define clear() printf("\033[H\033[J")

int printFirstTenLines(char* dir) {
    FILE *myfile;
    char content;
    int max = 0;

    // Open file
    myfile = fopen(dir, "r");
    if (myfile  == NULL){
        printf("File not found \n");
        return 0;
    }
    // Read the first 10 lines from file
    content  = fgetc(myfile);
    while (content != EOF){
        if (content == '\n') max++;
        if (max > 9)
            break;
        printf ("%c", content);
        content = fgetc(myfile);
    }
 
    fclose(myfile);
    return 0;
}

int comentRemover(char* dir) {
    FILE* fp;
    fp = fopen(dir, "r");
    if (fp == NULL) {
        printf("File not found \n");
        return 0;
    }

    int ch;
    while ((ch = getc(fp)) != EOF) {
        if (ch == '#') {
            while (ch != '\n') {
            ch = getc(fp);
            }
        printf("\n");
        } else {
            printf("%c", ch);
        }
    }
}

void sig_handler() {
    char dir[1024];
    getcwd(dir , sizeof(dir));
    printf("\nDir: %s \n", dir);
    printf(">> ");
}

int countLines(char* dir) {
    FILE *fileptr;
    int count_lines = 0;
    char filechar[40], chr;
    fileptr = fopen(dir, "r");
    if (fileptr == NULL) {
        printf("File not found \n");
        return 0;
    }
   //extract character from file and store in chr
    chr = getc(fileptr);
    while (chr != EOF)
    {
        //Count whenever new line is encountered
        if (chr == '\n')
        {
            count_lines = count_lines + 1;
        }
        //take next character from file.
        chr = getc(fileptr);
    }
    fclose(fileptr); //close file.
    printf("There are %d lines in this file\n", count_lines+1);
    return 1;
}

int removeWithespace(char* dir) {
    FILE* fp;
    FILE* fp1;
    fp = fopen(dir, "r+");
    fp1 = fopen("out.txt", "w+");
    if (fp == NULL) {
        printf("File not found \n");
        return 0;
    }
    int ch;
    while ((ch = getc(fp)) != EOF) {
        if ((ch != ' ') && (ch != '\n') && (ch != '\t')) {
            fputc(ch, fp1);
        }
    }
    rewind(fp1);
    int ch1;
    while ((ch1 = getc(fp1)) != EOF) {
        printf("%c", ch1);
    }
    return 1;
}

int mostRepeat(char* dir) {
    //printf("eyjan");
    FILE *file;
    char ch, *line;
    size_t len = 0, read;
    char words[MAXCOM][MAXCOM], word[MAXLIST];
    int i = 0, j, k, maxCount = 0, count; 
    //Opens file in read mode
    file = fopen(dir, "r");
    //If file doesn't exist
    if (file == NULL){
        printf("File not found \n");
        return 0;
    }  
    //Since, C doesn't provide in-built function, 
    //following code will split content of file into words
    while ((read = getline(&line, &len, file)) != -1) {  
        for(k=0; line[k]!='\0'; k++){
            //Here, i represents row and j represents column of two-dimensional array words 
            if(line[k] != ' ' && line[k] != '\n' && line[k] != ',' && line[k] != '.' ){
                words[i][j++] = tolower(line[k]);
            }
            else{
                words[i][j] = '\0';
                //Increment row count to store new word
                i++;
                //Set column count to 0
                j = 0;
            }
        }
    }
    int length = i;
    //Determine the most repeated word in a file
    for(i = 0; i < length; i++){
        count = 1;
        //Count each word in the file and store it in variable count
        for(j = i+1; j < length; j++){
            if(strcmp(words[i], words[j]) == 0 && (strcmp(words[i]," ") != 0)){
                count++;
            } 
        }
        //If maxCount is less than count then store value of count in maxCount 
        //and corresponding word to variable word
        if(count > maxCount){
            maxCount = count;
            strcpy(word, words[i]);
        }
    }
    printf("Most repeated word is: %s \n", word);
    fclose(file);
    return 1;
}



int readFirstWord(char* dir) {
    FILE *fp;
    char word[MAXCOM];
    fp = fopen(dir, "r");
    if (fp == NULL) {
        printf("cant find the file!\n");
        return 0;
    } else {
        int count = 0;
        while (!feof(fp)) {
            count++;
            fscanf(fp,"%s%*[^\n]",word);;
            printf("word read %d is: %s\n", count, word);
            strcpy(word,"");
        }
        fclose(fp);
        return 1;
    }
    
}

int ownComandHandler(char** parsedInput) {
    int numberOfComands = 8;
    int sw = -1;
    char* listOfComands[numberOfComands];
    char* name;

    listOfComands[0] = "cd";
    listOfComands[1] = "rfw";
    listOfComands[2] = "mrw";
    listOfComands[3] = "rw";
    listOfComands[4] = "cl";
    listOfComands[5] = "cmr";
    listOfComands[6] = "ftl";
    listOfComands[7] = "exitt";

    for (int i=0; i < numberOfComands; i++) {
        if (strcmp(parsedInput[0], listOfComands[i]) == 0) {
            sw = i;
            break;
        }
    }

    pid_t pid = fork();
    if (pid == 0) {
        int cd = 0;
        //printf("mamade %d \n", sw);
        switch (sw) {
            case 0:
                //chdir(parsedInput[1]);
                exit(3);
            case 1:
                readFirstWord(parsedInput[1]);
                exit(1);
            case 2:
                //how to code in samurai way
                printf("Most repeated word is: \n");
                printf("\033[A\33[2K\r");
                mostRepeat(parsedInput[1]);
                exit(1);
            case 3:
                removeWithespace(parsedInput[1]);
                exit(1);
            case 4:
                countLines(parsedInput[1]);
                exit(1);
            case 5:
                comentRemover(parsedInput[1]);
                exit(1);
            case 6:
                printFirstTenLines(parsedInput[1]);
                exit(1);
            case 7:
                exit(4);
            default:
                break;
        }
        exit(0);
    } else {
        int state;
        waitpid(pid, &state, 0);
        int rtrn = WEXITSTATUS(state);
        if (rtrn == 3) {
            chdir(parsedInput[1]);
        }
        if  (rtrn == 4) {
            exit(0);
        }
        return rtrn;
    }
}

void implementation(char** parsedInput) {
    pid_t pid = fork();

    if (pid == -1) {
        printf("Failed forking child :( \n");
        return;
    } else if (pid == 0) {
        if (execvp(parsedInput[0], parsedInput) < 0) {
            printf ("Could not run that shit. :( \n");
        }
        exit(0);
    } else {
        wait(NULL);
        return;
    }
}

void parseSpace(char* input, char** parsed) {
    for (int i=0; i<MAXLIST; i++) {
        parsed[i] = strsep(&input, " ");

        if (parsed[i] == NULL) {
            break;
        }

        if (strlen(parsed[i]) == 0) {
            i--;
        }
    }
}

int processComand(char* input, char** out) {
    
    parseSpace(input, out);
    if (ownComandHandler(out)) {
        return 0;
    } else {
        return 1;
    }
}

int takeInput(char* input) {
    char* buffer;

    buffer = readline(">> ");
    if (strlen(buffer) > 0) {
        strcpy(input , buffer);
        return 0;
    } else {
        return 1;
    }
}

void printDir() {
    char dir[1024];
    getcwd(dir , sizeof(dir));
    printf("\nDir: %s \n", dir);
}

void Start() {
    //clear();
    printf("\t*****starting shell*****\n");
    char *userName = getenv("USER");
    printf ("user is: %s \n" , userName);
}

int main() {
    char input[MAXCOM];
    char *parsedInput[MAXCOM];
    int sw = 0;
    Start();
    signal(SIGINT, sig_handler);
    while (1) {

        printDir();

        if (takeInput(input)) {
            continue;
        }

        //printf("%s" , input);

        sw = processComand(input, parsedInput);

        //printf("%d \n", sw);

        if (sw == 1) {
            implementation(parsedInput);
        }
    }
    return 0;
}