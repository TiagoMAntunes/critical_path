#include "hash.h"
#include <string.h>

typedef struct {
    unsigned long id, duration;
    char * description;
    unsigned long int earlyStart, lateStart;
    List * depends;
    List * precedes;
    int verified;
} Task;

/* Task methods */
Task * createTask(unsigned long id, unsigned long duration, char* description, List * dependencies);
void addDependency(Task *task, Task *dependency);
void addPrecedence(Task *task, Task *precedent);
Iterator * iterateDependencies(Task *task);
Iterator * iteratePrecedents(Task *task);
void deleteTask(Task * task);
int hasDependencies(Task * task);
int hasPrecedents(Task * task);
Task * findById(List * head, unsigned long id);
unsigned long getDuration(Task * el);
void printId(Task * el);
void printInfoTaskNoTimes(Task * el);
void printInfoTaskWithTimes(Task * el);
void initializeTasks(Task * el, unsigned long int time);
unsigned long int taskTime(Task * t);
void updateLateStart(Task * el, unsigned long int time);
int isCritical(Task * el);
void printCriticalPath(List * head);
void printRecursiveDuration(List * head, void (*printFn)(), unsigned long duration);
void printRecursive(List * head, void (*printFn)());
void deleteListOfTasks(List * head);