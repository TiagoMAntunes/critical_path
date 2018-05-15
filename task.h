#include "hash.h"
#include <string.h>
#include <stdio.h>
#include <limits.h>

typedef struct
{
    unsigned long id, duration;
    char *description;
    unsigned long int earlyStart, lateStart;
    List *depends;
    List *precedes;
    int verified;
} Task;

/**
 * Creates a task and returns its pointer
 * Receives a unsigned long id and duration, string description and list pointer for dependencies
 * Returns the created task pointer
 */
Task *createTask(unsigned long id, unsigned long duration, char *description, List *dependencies);

/**
 * Adds the given dependency do the given task and also updates the dependency's precedents
 * Receives a task pointer of the main task and a task pointer to its dependencies
 */
void addDependency(Task *task, Task *dependency);

/**
 * Add the precedents to the task's list of precendents
 * Receives a task pointer for the main task and a task pointer to the new precedence
 */
void addPrecedence(Task *task, Task *precedent);

/**
 * Creates an iterator for the task's dependencies list
 * Receives a task pointer
 * Returns an iterator pointer
 */
Iterator *iterateDependencies(Task *task);

/**
 * Creates an iterator for the task's precents list
 * Receives a task pointer
 * Returns an iterator pointer
 */
Iterator *iteratePrecedents(Task *task);

/**
 * Frees a given task's memory
 * Receives a task pointer
 */
void deleteTask(Task *task);

/**
 * Checks if a given task has any dependencies
 * Receives a task pointer
 * Returns true if task has dependencies
 */
int hasDependencies(Task *task);

/**
 * Checks if a given task has any precedencies
 * Receives a task pointer
 * Returns true if task has precedencies
 */
int hasPrecedents(Task *task);

/**
 * Finds a task in a given list given its id
 * Receives a list pointer and an unsigned long id
 * Returns the given task if found (or NULL otherwise)
 */
Task *findById(List *head, unsigned long id);

/**
 * Receives a task pointer
 * Returns the task duration parameter
 */
unsigned long getDuration(Task *el);

/**
 * Prints a task id
 * Receives a task pointer
 */
void printId(Task *el);

/**
 * Prints the task information without late and early starts
 * Receives a task pointer
 */
void printInfoTaskNoTimes(Task *el);

/**
 * Prints the task information with late and early starts
 * Receives a task pointer
 */
void printInfoTaskWithTimes(Task *el);

/**
 * Sets the task lateStart to the given value
 * Receives a task pointer and an unsigned long int time
 */
void initializeTask(Task *el, unsigned long int time);

/**
 * Receives a task pointer
 * Returns the tasks duration + early start
 */
unsigned long int taskTime(Task *t);

/**
 * Updates the late start of a given task to time and of all its dependencies, recursively
 * Receives a task pointer and an unsigned long int time
 */
void updateLateStart(Task *el, unsigned long int time);

/**
 * Receives a task pointer
 * Returns true if task is critical (earlyStart == lateStart)
 */
int isCritical(Task *el);

/**
 * Receives a list pointer 
 * Prints all elements from that el that are critical
 */
void printCriticalPath(List *head);

/**
 * Receives a list pointer and a void print function
 * Prints all the task elements of the list using the print function
 */
void printRecursive(List *head, void (*printFn)());

/**
 * Deletes all the tasks of the given list (does not free list)
 * Receives a list pointer
 */
void deleteListOfTasks(List *head);