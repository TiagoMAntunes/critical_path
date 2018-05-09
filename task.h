#include "hash.h"
#include <string.h>

typedef struct {
    unsigned long id, duration;
    char * description;
    unsigned long long earlyStart, lateStart;
    List * depends;
    List * precedes;
    List ** dependsLast;
    List ** precedesLast;
} Task;

/* Task methods */
Task * createTask(unsigned long id, unsigned long duration, char* description, List * dependencies);
void addDependency(Task *task, Task *dependency);
void addPrecedence(Task *task, Task *precedent);
Iterator * iterateDependencies(Task *task);
Iterator * iteratePrecedents(Task *task);
void deleteTask(Task * task);
int hasDependencies(Task * task);
Task * findById(List * head, unsigned long id);
unsigned long getDuration(Task * el);
void printId(Task * el);
void printInfoTaskNoTimes(Task * el);
void printInfoTaskWithTimes(Task * el);