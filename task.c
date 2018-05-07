#include "task.h"

Task * createTask(unsigned long id, unsigned long duration, char* description, List* dependencies) {
    Task * task = malloc(sizeof(Task));
    task->id = id;
    task->duration = duration;
    task->description = malloc(sizeof(char) * (strlen(description)+1));
    strcpy(task->description, description);
    task->dependencies = newList();
    task->precedents = newList(); 
    task->earlyStart = task->lateStart = 0;
    Iterator * it = createIterator(dependencies);
    while (hasNext(it))
        addDependency(task, (Task*) next(it)->current);
    killIterator(it);
    return task;
}

void addDependency(Task * task, Task * dependency) {
    addEl(task->dependencies, dependency);
    addPrecedence(dependency, task);
}

void addPrecedence(Task * task, Task * precedent) {
    addEl(task->precedents, precedent);
}

Iterator * iterateDependencies(Task * task) {
    Iterator * t = createIterator(task->dependencies);
    return t;
}

Iterator * iteratePrecedents(Task * task) {
    Iterator * t = createIterator(task->precedents);
    return t;
}

void deleteTask(Task * task) {
    List * tmpNode;
    if (!isEmpty(task->precedents)) {
        Iterator * it = iteratePrecedents(task);
        tmpNode = createNode(task);
        while(hasNext(it))
            removeEl( ((Task*)next(it)->current)->dependencies, tmpNode);
        killIterator(it);
        free(tmpNode);
    }
    listFree(task->precedents);
    free(task->description);
    listFree(task->dependencies);
    free(task);
}

int hasDependencies(Task * task) {
    return !isEmpty(task->dependencies);
}

Task * findById(List * head, unsigned long id) {
    Iterator * it = createIterator(head);
    List * curr;
    while (hasNext(it)) {
        curr = next(it);
        if (((Task *) current(curr))->id == id)
            return current(curr);
    }
    return NULL;
}