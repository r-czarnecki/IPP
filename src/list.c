#include <stdlib.h>
#include "list.h"

ListElement* newListElement(int intArg, Node *nodeArg) {
    ListElement *newList = malloc(sizeof(ListElement));
    if(newList == NULL)
        return NULL;

    newList->integer = intArg;
    newList->node = nodeArg;
    newList->next = NULL;
    newList->prev = NULL;
    newList->last = newList;
    return newList;
}

void removeListElement(List *list, ListElement *element) {
    if(element == NULL)
        return;

    if(element->prev != NULL)
        element->prev->next = element->next;
    else {
        *list = element->next;
        if((*list) != NULL)
            (*list)->last = element->last;
    }

    if(element->next != NULL)
        element->next->prev = element->prev;
    else {
        if((*list) != NULL)
            (*list)->last = element->prev;
    }

    free(element);
}

void deleteList(List list) {
    while(list != NULL) {
        ListElement *next = list->next;
        free(list);
        list = next;
    }
}

bool addFrontElement(List* list, int intArg, Node *nodeArg) {
    ListElement *newElement = newListElement(intArg, nodeArg);
    if(newElement == NULL)
        return false;
    
    if(*list == NULL) {
        (*list) = newElement;
        return true;
    }

    newElement->next = (*list);
    newElement->last = (*list)->last;
    (*list)->prev = newElement;
    (*list) = newElement;
    return true;
}

bool addBackElement(List* list, int intArg, Node *nodeArg) {
    ListElement *newElement = newListElement(intArg, nodeArg);
    if(newElement == NULL)
        return false;
    
    if(*list == NULL) {
        *list = newElement;
        return true;
    }

    newElement->prev = (*list)->last;
    (*list)->last->next = newElement;
    (*list)->last = newElement;
    return true;
}

void mergeLists(List* listA, List listB) {
    if(*listA == NULL) {
        *listA = listB;
        return;
    }
    else if(listB == NULL)
        return;

    (*listA)->last->next = listB;
    listB->prev = (*listA)->last;
    (*listA)->last = listB->last;
}

ListElement* findInt(List list, int find) {
    ListElement *element = list;
    while(element != NULL) {
        if(element->integer == find)
            return element;
        element = element->next;
    }
    
    return NULL;
}