#include "hash.h"
#include <string.h>

typedef struct {
    unsigned long id, duration;
    char * description;
    unsigned long long earlyStart, lateStart;
    List * dependencies;
    List * precedents;
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