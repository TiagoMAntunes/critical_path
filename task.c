#include "task.h"
#include <stdio.h>
#include <limits.h>
Task * createTask(unsigned long id, unsigned long duration, char* description, List* dependencies) {
    unsigned long long maxTime = 0;
    Task * task = malloc(sizeof(Task)), * helper;
    task->id = id;
    task->duration = duration;
    task->description = malloc(sizeof(char) * (strlen(description)+1));
    strcpy(task->description, description);
    task->depends = newList();
    task->precedes = newList(); 
    task->lateStart = 0;
    Iterator * it = createIterator(dependencies);
    while (hasNext(it)) {
        helper = (Task*) next(it)->current;
        if (taskTime(helper) > maxTime)
            maxTime = taskTime(helper);
        addDependency(task, helper);
    }
    killIterator(it);
    task->earlyStart = maxTime;
    return task;
}

void addDependency(Task * task, Task * dependency) {
    addEl(task->depends, dependency);
    addPrecedence(dependency, task);
}

void addPrecedence(Task * task, Task * precedent) {
    addEl(task->precedes, precedent);
}

Iterator * iterateDependencies(Task * task) {
    Iterator * t = createIterator(task->depends);
    return t;
}

Iterator * iteratePrecedents(Task * task) {
    Iterator * t = createIterator(task->precedes);
    return t;
}

void deleteTask(Task * task) {
    List * tmpNode;
    if (!isEmpty(task->depends)) {
        Iterator * it = iterateDependencies(task); /* Verificar */
        tmpNode = createNode(task);
        while(hasNext(it))
            removeEl( ((Task*)next(it)->current)->precedes, tmpNode);
        killIterator(it);
        free(tmpNode);
    }
    listFree(task->precedes);
    free(task->description);
    listFree(task->depends);
    free(task);
}

int hasDependencies(Task * task) {
    return !isEmpty(task->precedes);
}

int hasPrecedents(Task * task) {
    return !isEmpty(task->depends);
}

Task * findById(List * head, unsigned long id) {
    Iterator * it = createIterator(head);
    List * curr;
    while (hasNext(it)) {
        curr = next(it);
        if (((Task *) current(curr))->id == id) {
            killIterator(it);
            return current(curr);
        }
    }
    killIterator(it);
    return NULL;
}

void printId(Task * el) {
    printf("%lu", el->id);
}

void printInfoTaskNoTimes(Task * el) {
    Iterator * it;
    printf("%lu \"%s\" %lu", el->id, el->description, el->duration);
    it = iterateDependencies(el);
    while(hasNext(it)) {
        printf(" ");
        print(next(it), printId);
    }
    killIterator(it);
    printf("\n");
}


void printInfoTaskWithTimes(Task * el) {
    Iterator * it;
    printf("%lu \"%s\" %lu [%llu ", el->id, el->description, el->duration, el->earlyStart);
    if (el->earlyStart != el->lateStart)
        printf("%llu]", el->lateStart);
    else
        printf("CRITICAL]");
    it = iterateDependencies(el);
    while(hasNext(it)) {
        printf(" ");
        print(next(it), printId);
    }
    killIterator(it);
    printf("\n");
}

unsigned long getDuration(Task * el) {
    return el->duration;
}

void initializeTasks(Task * el, unsigned long long time) {
    el->lateStart = time;
}

unsigned long long taskTime(Task * t) {
    return t->earlyStart + t->duration;
}

void updateLateStart(Task * el, unsigned long long time) {
    Iterator * it = iterateDependencies(el);
    Task * helper;
    if (el->lateStart > time - el->duration)
        el->lateStart = time - el->duration;

    while(hasNext(it)) {
        helper = (Task *) current(next(it));
        if (helper->lateStart > el->lateStart - helper->duration)
            updateLateStart(helper, el->lateStart);
    }
    killIterator(it);
}

int isCritical(Task * el) {
    return el->earlyStart == el->lateStart;
}

void printCriticalPath(List * head) {
    Task * critical;
    Iterator * it = createIterator(head);
    while(hasNext(it)) {
        critical = (Task *) current(next(it));
        if (isCritical(critical))
            printInfoTaskWithTimes(critical);
    }
    killIterator(it);
}