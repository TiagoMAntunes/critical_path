#include "task.h"
#include <stdio.h>
#include <limits.h>

#define HASH 128
#define MAXSTRING 8001

char * getInput();
void clearbuffer();
int addTask(char * buffer, List * ordered, Hash * table);
int duration(int validPath, List * ordered);
int depend(Hash * hashtable);
int removeTask(Hash * hashtable, List * ordered, int * path);
int path(Hash * table, List * ordered);

int main() {
    List * ordered = newList();
    Hash * hashtable = createTable(HASH, sizeof(List));
    int inputFlag = 0, validPath = 0;
    char command[10];
    scanf("%s", command);
    while (1) {
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
            validPath = 0;
        } else if (!strcmp(command, "duration")) {
            inputFlag = duration(validPath, ordered);
        } else if (!strcmp(command, "depend")) {
            inputFlag = depend(hashtable);
        } else if (!strcmp(command, "remove")) {
            inputFlag = removeTask(hashtable, ordered, &validPath);
        } else if (!strcmp(command, "path")) {
            inputFlag = path(hashtable, ordered);
            if (!inputFlag)
                validPath = 1;
        } else if (!strcmp(command, "exit")) {
            if (getchar() != '\n'){
                clearbuffer();
                inputFlag = 1;
            } else
                break;

        } else {
            inputFlag = 1;
            if (getchar() != '\n')
                clearbuffer();
        }
        if (inputFlag) {
            printf("illegal arguments\n");
            inputFlag = 0;
        }
        scanf("%s", command);
    }
    deleteListOfTasks(ordered);
    listFree(ordered);
    deleteTable(hashtable);
    return 0;
}

char * getInput() {
    unsigned int i = 0, limit = 1000;
    char * inputLine = malloc(sizeof(char) * limit);
    char c;
    while((c = getchar()) != '\n') {
        if (i >= limit) {
            limit += 1000;
            inputLine = (char *) realloc(inputLine, sizeof(char)*limit);
        }
        inputLine[i++] = c;
    }
    if (i >= limit)
        inputLine = (char *) realloc(inputLine, sizeof(char)*(limit+1));
    inputLine[i] = '\0';
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

    if (i != 1 || id <= 0)
        return 1;

    if ((idFind = findInTable(table, id)) && (idFindTask = findById(idFind, id))) {
        printf("id already exists\n");
        return 0;
    }

    description = strtok(NULL, "\"");
    
    if (strlen(description) > MAXSTRING)
        return 1;

    c = strtok(NULL, " ");
    i += sscanf(c, "%lu", &duration);

    if (i != 2 || duration <= 0)
        return 1;
    
    precedents = newList();
    
    while((c = strtok(NULL, " \n"))) {
        i = sscanf(c, "%lu", &helper);
        if (((idFind = findInTable(table, helper)) && (idFindTask = findById(idFind, helper))) && i == 1)
            addEl(precedents,idFindTask);
        else if (i == 1) {
            printf("no such task\n");
            listFree(precedents);
            return 0;
        } else {
            return 1;
        }
    }
    task = createTask(id, duration, description, precedents);
    listFree(precedents);
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
    printRecursiveDuration(ordered, printFn, duration);
    return 0;
}

int depend(Hash * hashtable) {
    int i, changed = 0;
    unsigned long id;
    Task * task;
    Iterator * it;
    if (getchar() == '\n')
        return 1;
    i = scanf("%lu", &id);
    if (!i) {
        clearbuffer();
        return 1;
    }
    task = findById(findInTable(hashtable, id), id);
    if (!task) {
        printf("no such task\n");
        return 0;
    }
    it = iteratePrecedents(task);
    changed = hasNext(it);
    printf("%lu:", id);
    if(hasNext(it))
        printRecursive(next(it), printId);
    if (!changed)
        printf(" no dependencies");
    printf("\n");
    killIterator(it);
    return 0;
}

int removeTask(Hash * hashtable, List * ordered, int * path) {
    int i;
    unsigned long id;
    Task * task;
    Iterator * it;
    List * tmp, *helper;
    if (getchar() == '\n')
        return 1;
    i = scanf("%lu", &id);
    if (!i) {
        clearbuffer();
        return 1;
    }

    task = findById(findInTable(hashtable, id), id);
    if (!task) {
        printf("no such task\n");
        return 0;
    } else if (hasDependencies(task)) {
        printf("task with dependencies\n");
        return 0;
    }

    /* Remove from ordered linked list */
    it = createIterator(ordered);
    while (hasNext(it)) {
        helper = next(it);
        if (current(helper) == task) {
            removeEl(ordered, helper);
            break;
        }
    }
    killIterator(it);
    
    /* Remove from hashtable */
    tmp = findInTable(hashtable, id);
    it = createIterator(tmp);
    while(hasNext(it)) {
        helper = next(it);
        if (current(helper) == task) {
            removeEl(tmp, helper);
            break;
        }
    }
    killIterator(it);

    deleteTask(task);
    *path = 0;
    return 0;
}

int path(Hash * table, List * ordered) {
    unsigned long int maxDuration = 0, tmp;
    Iterator * it = createIterator(ordered);
    Task * helper;
    List * endingTasks = newList();
    while(hasNext(it)) {
        helper = (Task *) current(next(it));
        initializeTasks(helper, ULONG_MAX);
        if (!hasDependencies(helper))
            addEl(endingTasks, helper);
        tmp = helper->earlyStart + helper->duration;
        if (maxDuration < tmp)
            maxDuration = tmp;
    }

    killIterator(it);
    it = createIterator(endingTasks);
    while(hasNext(it))
        updateLateStart((Task*)current(next(it)), maxDuration);
    killIterator(it);
    printCriticalPath(ordered);
    printf("project duration = %lu\n", maxDuration);
    listFree(endingTasks);
    return 0;
}