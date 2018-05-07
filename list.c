#include "list.h"


void removeEl(List * head, List * el) {
    List * iterator = head, * tmp;
    while (iterator->next != NULL && iterator->next != el)
        iterator = iterator->next;

    if (iterator->next != NULL) {
        tmp = iterator->next;
        iterator->next = tmp->next;
        free(tmp);
    }
}

List * createNode(void * el) {
    List * tmp = malloc(sizeof(List));
    tmp->next = NULL;
    tmp->current = el;
    return tmp;
}

void addEl(List * head, void * elToAdd) {
    List * tmp = head, * el = createNode(elToAdd);
    while (tmp->next != NULL)
        tmp = tmp->next;
    tmp->next = el;
}


void print(List * el, void (*fn)()) {
    fn(el->current);
}

List * newList() {
    List * head = malloc(sizeof(List));
    head->next = NULL;
    head->current = NULL;
    return head;
}

int isEmpty(List * l) {
    return l->next == NULL;
}

void listFree(List * head) {
    List * tmp;
    while (head->next != NULL) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
    free(head);
}

void * current(List * el) {
    return el->current;
}