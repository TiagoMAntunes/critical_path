#include "task.h"
#include <stdio.h>
#include <limits.h>

#define HASH 1023
#define MAXSTRING 8001

char *getInput();
void clearbuffer();
int addTask(char *buffer, List *ordered, Hash *table);
int duration(int validPath, List *ordered, char last);
int depend(Hash *hashtable, char last);
int removeTask(Hash *hashtable, List *ordered, int *path, char last);
int path(Hash *table, List *ordered, char last);
char getCommand(char *command);

int main()
{
    List *ordered = newList();
    Hash *hashtable = createTable(HASH, sizeof(List));
    int inputFlag = 0, validPath = 0;
    char command[12], last;
    do
    {
        last = getCommand(command);
        inputFlag = 1;
        if (!strcmp(command, "add"))
        {
            char *buffer;
            if (last != ' ')
            {
                inputFlag = 1;
                if (last != '\n')
                    clearbuffer();
            }
            else
            {
                buffer = getInput();
                inputFlag = addTask(buffer, ordered, hashtable);
                free(buffer);
            }
            validPath = 0;
        }
        else if (!strcmp(command, "duration"))
        {
            inputFlag = duration(validPath, ordered, last);
        }
        else if (!strcmp(command, "depend"))
        {
            inputFlag = depend(hashtable, last);
        }
        else if (!strcmp(command, "remove"))
        {
            inputFlag = removeTask(hashtable, ordered, &validPath, last);
        }
        else if (!strcmp(command, "path"))
        {
            inputFlag = path(hashtable, ordered, last);
            if (!inputFlag)
                validPath = 1;
        }
        else if (!strcmp(command, "exit"))
        {
            if (last != '\n')
            {
                clearbuffer();
                inputFlag = 1;
            }
            else
                break;
        }
        else
        {
            inputFlag = 1;
            if (last != '\n')
                clearbuffer();
        }
        if (inputFlag)
        {
            printf("illegal arguments\n");
            inputFlag = 0;
        }
    } while (strcmp(command, "exit"));
    deleteListOfTasks(ordered);
    listFree(ordered);
    deleteTable(hashtable);
    return 0;
}

/**
 * Receives a string pointer
 * Returns the last read character
 * Fills the array with given input
 */
char getCommand(char *command)
{
    char c;
    int i;
    command[0] = '\0';
    if ((c = getchar()) == ' ' || c == '\n')
        return c;
        
    command[0] = c;
    for (i = 1; i < 11 && (c = getchar()) != ' ' && c != '\n'; i++)
        command[i] = c;
    command[i] = '\0';
    return c;
}

/**
 * Returns a pointer to a user input line
 */
char *getInput()
{
    unsigned int i = 0, limit = 1000;
    char *inputLine = malloc(sizeof(char) * limit);
    char c;
    while ((c = getchar()) != '\n')
    {
        if (i >= limit)
        {
            limit += 1000;
            /* Resizes according to input size in blocks of 1000 */
            inputLine = (char *)realloc(inputLine, sizeof(char) * limit);
        }
        inputLine[i++] = c;
    }
    if (i >= limit)
        inputLine = (char *)realloc(inputLine, sizeof(char) * (limit + 1));
    inputLine[i] = '\0';
    return inputLine;
}

/**
 * Clears the buffer input until next \n
 */
void clearbuffer()
{
    char c;
    while ((c = getchar()) != '\n')
        ;
}

/**
 * Receives a char pointer, a List pointer and a Hash pointer
 * Adds a task, given its info in the buffer, to the ordered list and to the hashtable 
 * Returns true if input was incorrect
 */
int addTask(char *buffer, List *ordered, Hash *table)
{
    int i = 0, it = 0, counter, helpCount;
    unsigned long id, duration, helper;
    char *description, *c, ch;
    List *precedents, *idFind;
    Task *task, *idFindTask;

    /* Gets ID */
    c = malloc(sizeof(char) * 12);
    while ((ch = buffer[it]) != ' ' && ch != '-' && it < 11)
        c[it++] = ch;
    c[it] = '\0';
    if (ch != ' ' || (strlen(c) == 10 && strcmp(c, "4294967295") > 0))
        return 1;
    i += sscanf(c, "%lu", &id);

    if (i != 1 || id <= 0)
    {
        free(c);
        return 1;
    }

    /* Checks if ID is already used */
    if ((idFind = findInTable(table, id)) && (idFindTask = findById(idFind, id)))
    {
        printf("id already exists\n");
        free(c);
        return 0;
    }

    /* Gets description */
    it++;
    description = malloc(sizeof(char) * MAXSTRING);
    helpCount = 0;
    counter = 0;
    while (counter != 2 && helpCount < MAXSTRING - 1)
    {
        ch = buffer[it++];
        if (ch == '\"')
            counter++;
        description[helpCount++] = ch;
    }
    description[helpCount] = '\0';
    description = realloc(description, sizeof(char) * (strlen(description) + 1));

    if (ch != '\"')
    {
        free(c);
        free(description);
        return 1;
    }

    it++;
    helpCount = it;
    while ((ch = buffer[it]) != ' ' && ch != '\n'  && ch != '-' && ch != '\0' && it - helpCount < 11)
    {
        c[it - helpCount] = ch;
        it++;
    }

    c[it - helpCount] = '\0';
    if ((ch != ' ' && ch != '\n' && ch != '\0') || (strlen(c) == 10 && strcmp(c, "4294967295") > 0))
    {
        free(c);
        free(description);
        return 1;
    }

    /* Gets duration*/

    i += sscanf(c, "%lu", &duration);
    if (i != 2 || duration <= 0)
    {
        free(description);
        free(c);
        return 1;
    }

    /* Gets task precedents and adds to list */
    precedents = newList();
    free(c);
    c = strtok(buffer + it, " ");
    if (c != NULL)
    {
        do
        {
            i = sscanf(c, "%lu", &helper);
            if (((idFind = findInTable(table, helper)) && (idFindTask = findById(idFind, helper))) && i == 1)
                addEl(precedents, idFindTask);
            else if (i == 1)
            {
                printf("no such task\n");
                free(description);
                listFree(precedents);
                return 0;
            }
            else
            {
                free(description);
                listFree(precedents);
                return 1;
            }
        } while ((c = strtok(NULL, " \n")));
    }

    /* Creates task and frees memory */
    task = createTask(id, duration, description, precedents);
    free(description);
    listFree(precedents);
    addEl(ordered, task);
    insertInTable(table, task, id);
    return 0;
}

/**
 * Receives a int, a List pointer and a char
 * Prints all the elements with equal or bigger duration than the one given in input
 * Returns true if input was incorrect
 */
int duration(int validPath, List *ordered, char last)
{
    char c[12], ch;
    int i = 0, limit = 100;
    unsigned long duration;
    void (*printFn)();
    List ** vec;
    Iterator * it;
    if (last == '\n')
        duration = 0;
    else
    {
        /* Gets duration value */
        while ((ch = getchar()) != '\n' && i < 11 && ch >= '0' && ch <= '9' && ch != '-')
            c[i++] = ch;
        c[i] = '\0';

        if (ch != '\n')
        { /* Verifies if it was read a non negative number*/
            clearbuffer();
            return 1;
        }

        if (i == 0)
            duration = 0;
        else
        {
            sscanf(c, "%lu", &duration);
            if (duration == 0) /* Wrong input */
                return 1;
        }
    }


    if (validPath)
        printFn = printInfoTaskWithTimes;
    else
        printFn = printInfoTaskNoTimes;

    /* Create array of all tasks */
    vec = malloc(sizeof(Task*) * limit);
    it = createIterator(ordered);
    for (i = 0; hasNext(it); i++) {
        if (i >= limit)
            vec = realloc(vec, sizeof(Task*) * (i+100));
        vec[i] = next(it);
    }
    killIterator(it);

    /* Print elements */
    i--;
    for (; i >= 0; i--) {
        if (((Task *)current(vec[i]))->duration >= duration)
            print(vec[i], printFn);
    }

    free(vec);
    return 0;
}

/**
 * Receives a Hash pointer and a char
 * Prints all the dependencies of task given by input
 * Returns true if input was incorrect
 */
int depend(Hash *hashtable, char last)
{
    int i = 0;
    unsigned long id;
    Task *task;
    Iterator *it;
    char c[12], ch;

    /* Verify and get input */
    if (last == '\n')
        return 1;
    while ((ch = getchar()) != '\n' && i < 11 && ch >= '0' && ch <= '9' && ch != '-')
        c[i++] = ch;
    c[i] = '\0';
    if (ch != '\n')
    {
        clearbuffer();
        return 1;
    }
    sscanf(c, "%lu", &id);
    if (id == 0)
        return 1;

    /* Find task */
    task = findById(findInTable(hashtable, id), id);
    if (!task)
    {
        printf("no such task\n");
        return 0;
    }

    /* Print precedents */
    it = iteratePrecedents(task);
    printf("%lu:", id);
    if (hasNext(it))
        printRecursive(next(it), printId);
    else
        printf(" no dependencies");
    printf("\n");
    killIterator(it);
    return 0;
}

/**
 * Receives a Hash pointer, a List pointer, a int pointer and a char
 * Removes the task given specified via input from the memory (including from hash and list)
 * Returns true if input was incorrect
 */
int removeTask(Hash *hashtable, List *ordered, int *path, char last)
{
    int i = 0;
    unsigned long id;
    Task *task;
    Iterator *it;
    List *tmp, *helper;
    char ch, c[12];

    if (last == '\n')
        return 1;

    while ((ch = getchar()) != '\n' && i < 11 && ch >= '0' && ch <= '9' && ch != '-')
        c[i++] = ch;
    c[i] = '\0';
    if (ch != '\n')
    {
        clearbuffer();
        return 1;
    }
    sscanf(c, "%lu", &id);
    if (id == 0)
        return 1;

    task = findById(findInTable(hashtable, id), id);
    if (!task)
    {
        printf("no such task\n");
        return 0;
    }
    else if (hasDependencies(task))
    {
        printf("task with dependencies\n");
        return 0;
    }

    /* Remove from ordered linked list */
    helper = ordered;
    it = createIterator(ordered);
    while (hasNext(it))
    {
        tmp = helper;
        helper = next(it);
        if (current(helper) == task)
        {
            removeEl(tmp, helper);
            break;
        }
    }
    killIterator(it);

    /* Remove from hashtable */
    tmp = findInTable(hashtable, id);
    it = createIterator(tmp);
    while (hasNext(it))
    {
        helper = next(it);
        if (current(helper) == task)
        {
            removeEl(tmp, helper);
            break;
        }
    }
    killIterator(it);

    deleteTask(task);
    *path = 0;
    return 0;
}

/**
 * Receives a hash pointer, a list pointer and a char
 * Calculates and prints the critical path of the program
 * Returns true if input was incorrect
 */
int path(Hash *table, List *ordered, char last)
{
    unsigned long int maxDuration = 0, tmp;
    Iterator *it = createIterator(ordered);
    Task *helper;
    List *endingTasks;

    if (last != '\n')
    {
        clearbuffer();
        return 1;
    }

    endingTasks = newList();
    while (hasNext(it))
    {
        helper = (Task *)current(next(it));
        initializeTask(helper, ULONG_MAX);
        if (!hasDependencies(helper))
            addEl(endingTasks, helper);
        tmp = helper->earlyStart + helper->duration;
        if (maxDuration < tmp)
            maxDuration = tmp;
    }

    killIterator(it);
    it = createIterator(endingTasks);
    while (hasNext(it))
        updateLateStart((Task *)current(next(it)), maxDuration);
    killIterator(it);
    printCriticalPath(ordered);
    printf("project duration = %lu\n", maxDuration);
    listFree(endingTasks);
    return 0;
}