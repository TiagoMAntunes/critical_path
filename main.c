#include "task.h"
#include <stdio.h>
#define HASH 100
#define MAXSTRING 8001

char * getInput();
void clearbuffer();
int addTask(char * buffer, List * ordered, Hash * table);
int duration(int validPath, List * ordered);
int depend(Hash * hashtable);

int main() {
    List * ordered = newList();
    Hash * hashtable = createTable(HASH, sizeof(List));
    int inputFlag = 0, validPath = 0;
    char command[10];
    scanf("%s", command);
    while (strcmp(command, "exit")) {
        if (!strcmp(command, "add")) {
            char * buffer, c;
            if ((c=getchar()) != ' ') {
                inputFlag = 1;
                if (c != '\n')
                    clearbuffer();
            } else {
                buffer = getInput();
                inputFlag = addTask(buffer, ordered, hashtable);
                free(buffer);
            }
            inputFlag = 0;
        } else if (!strcmp(command, "duration")) {
            inputFlag = duration(validPath, ordered);
        } else if (!strcmp(command, "depend")) {
            inputFlag = depend(hashtable);
        } else if (!strcmp(command, "remove")) {

        } else if (!strcmp(command, "path")) {

        } else {
            inputFlag = 1;
            clearbuffer();
        }
        if (inputFlag) {
            printf("illegal arguments.\n");
            inputFlag = 0;
        }
        scanf("%s", command);
    }
    return 0;
}

char * getInput() {
    char * inputLine = malloc(sizeof(char) * 10000);
    fgets(inputLine, 10000, stdin);
    inputLine = realloc(inputLine, strlen(inputLine)+1);
    return inputLine;
}

void clearbuffer() {
    char c;
    while ((c = getchar()) != '\n');
}

int addTask(char * buffer, List * ordered, Hash * table) {
    char * c, i = 0;
    unsigned long id, duration, helper;
    char * description;
    List * precedents, * idFind;
    Task * task, * idFindTask;
    c = strtok(buffer, " ");
    i += sscanf(c, "%lu", &id);

    if ((idFind = findInTable(table, id)) && (idFindTask = findById(idFind, id))) {
        printf("id already exists\n");
        return 0;
    }

    description = strtok(NULL, "\"");
    
    if (strlen(description) > MAXSTRING)
        return 1;

    c = strtok(NULL, " ");
    i += sscanf(c, "%lu", &duration);

    if (i != 2)
        return 1;
    
    precedents = newList();
    /*Fix idFind and idFindTask*/
    while((c = strtok(NULL, " \n"))) {
        i = sscanf(c, "%lu", &helper);
        if (((idFind = findInTable(table, helper)) && (idFindTask = findById(idFind, helper))) && i == 1)
            addEl(precedents,idFindTask);
        else if (i == 1) {
            printf("no such task\n");
            return 0;
        } else {
            return 1;
        }
    }
    task = createTask(id, duration, description, precedents);
    free(precedents);
    addEl(ordered, task);
    insertInTable(table, task, id);
    return 0;
}

int duration(int validPath, List * ordered) {
    char c;
    int i = 1;
    unsigned long duration;
    void  (*printFn)();
    if ((c = getchar()) == '\n')
        duration = 0;
    else {
        i = scanf("%lu", &duration);
        if (!i || getchar() != '\n') {
            clearbuffer();
            return 1;
        }
    }
    if (validPath)
        printFn = printInfoTaskWithTimes;
    else
        printFn = printInfoTaskNoTimes;
    Iterator * it = createIterator(ordered);
    while (hasNext(it)) {
    List * listEl = next(it);
    if (getDuration((Task*) current(listEl)) >= duration) 
        print(listEl, printFn);
    }
    killIterator(it);
    return 0;
}

int depend(Hash * hashtable) {
    int i, changed = 0;
    unsigned long id;
    Task * task;
    i = scanf(" %lu", &id);
    if (!i) {
        clearbuffer();
        return 1;
    }
    task = findById(findInTable(hashtable, id), id);
    if (!task) {
        printf("no such task\n");
        return 0;
    }
    Iterator * it = iteratePrecedents(task);
    changed = hasNext(it);
    printf("%lu:", id);
    while (hasNext(it)) {
        printf(" ");
        print(next(it), printId);
    }
    if (!changed)
        printf(" no dependencies");
    printf("\n");
    return 0;
}