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
    int i = 0, it = 0, counter = 0, helpCount;
    unsigned long id, duration, helper;
    char * description, *c, ch;
    List * precedents, * idFind;
    Task * task, * idFindTask;

    c = malloc(sizeof(char)*12);
    while ((ch = buffer[it]) != ' ' && it < 11)
        c[it++] = ch;
    c[it] = '\0';
    if (ch != ' ')
        return 1;
    i += sscanf(c, "%lu", &id);

    if (i != 1 || id <= 0) {
        free(c);
        return 1;
    }
        

    if ((idFind = findInTable(table, id)) && (idFindTask = findById(idFind, id))) {
        printf("id already exists\n");
        free(c);
        return 0;
    }

    it++;
    description = malloc(sizeof(char) * MAXSTRING);
    helpCount = 0;
    while (counter != 2 && helpCount < MAXSTRING-1) {
        ch = buffer[it++];
        if (ch == '\"')
            counter++;
        description[helpCount++] = ch;
    }
    description[helpCount] = '\0';
    description = realloc(description, sizeof(char) * (strlen(description)+1));
    
    if (ch != '\"') {
        free(c);
        free(description);
        return 1;
    }
    
    it++;
    helpCount = it;
    while ((ch = buffer[it]) != ' ' && ch != '\0' && it - helpCount < 11) {
        c[it-helpCount] = ch;
        it++;
    }

    c[it-helpCount] = '\0';
    if (ch != ' ' && ch != '\0') {
        free(c);
        free(description);
        return 1;
    }
    i += sscanf(c, "%lu", &duration);

    if (i != 2 || duration <= 0) {
        free(description);
        free(c);
        return 1;
    }
    
    precedents = newList();
    free(c);
    c = strtok(buffer + it, " ");
    if (c != NULL) {
        do {
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
        } while ((c = strtok(NULL, " \n")));
    }
    task = createTask(id, duration, description, precedents);
    free(description);
    listFree(precedents);
    addEl(ordered, task);
    insertInTable(table, task, id);
    return 0;
}

int duration(int validPath, List * ordered) {
    char * c, ch;
    int i = 0;
    unsigned long duration;
    void  (*printFn)();
    if (getchar() == '\n')
        duration = 0;
    else {
        c = malloc(sizeof(char) * 13);
        while ((ch = getchar()) != '\n' && i < 12 && ch >= '0' && ch <= '9' && ch != '-')
            c[i++] = ch;
        c[i] = '\0';
        if ((ch != '\n' && i != 0) || ch == '-') {
            clearbuffer();
            return 1;
        }
        if (i == 0)
            duration = 0;
        else {
            sscanf(c, "%lu", &duration);
            if (duration == 0)
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
    if (!i || id == 0) {
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
    if (!i || id == 0) {
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
    helper = ordered;
    it = createIterator(ordered);
    while (hasNext(it)) {
        tmp = helper;
        helper = next(it);
        if (current(helper) == task) {
            removeEl(tmp, helper);
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