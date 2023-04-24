#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "edge.h"
#include "node.h"

Edge* newEdge(Node* nodeA, Node* nodeB, int year, int length) {
    Edge *new = malloc(sizeof(Edge));
    if(new == NULL)
        return NULL;

    if(nodeA->id > nodeB->id) {
        Node *tmp = nodeA;
        nodeA = nodeB;
        nodeB = tmp;
    }

    new->routes = NULL;
    new->year = year;
    new->length = length;
    new->nodeA = nodeA;
    new->nodeB = nodeB;
    return new;
}

void deleteEdge(Edge* edge) {
    deleteList(edge->routes);
    free(edge);
}

EdgeListElement* newEdgeListElement(Edge* edge) {
    EdgeListElement *new = malloc(sizeof(EdgeListElement));
    if(new == NULL)
        return NULL;

    new->edge = edge;
    new->i = 0;
    new->prev = NULL;
    new->next = NULL;
    new->last = NULL;
    return new;
}

void deleteEdgeList(EdgeList list) {
    while(list != NULL) {
        EdgeList next = list->next;
        removeEdge(list->edge->nodeA, list->edge->nodeB);
        list = next;
    }
}

bool addEdge(Node *node1, Node *node2, int length, int year) {
    Edge *edge = newEdge(node1, node2, year, length);
    if(edge == NULL)
        return false;
    bool memory = addEdgeToList(&(node1->edges), edge);
    memory = memory & addEdgeToList(&(node2->edges), edge);
    if(!memory) {
        removeEdgeFromList(&(node1->edges), node1, node2);
        removeEdgeFromList(&(node2->edges), node1, node2);
        deleteEdge(edge);
        return false;
    }

    return true;
}

bool addEdgeToList(EdgeList* list, Edge *edge) {
    EdgeListElement *element = newEdgeListElement(edge);
    if(element == NULL) {
        deleteEdgeList(element);
        return false;
    }

    if(*list == NULL) {
        *list = element;
        element->last = element;
    }
    else {
        (*list)->last->next = element;
        element->prev = (*list)->last;
        (*list)->last = element;
    }

    return true;
}

EdgeListElement* findElement(EdgeList list, Node* nodeA, Node* nodeB) {
    if(nodeA->id > nodeB->id) {
        Node *tmp = nodeA;
        nodeA = nodeB;
        nodeB = tmp;
    }
    EdgeListElement *element = list;
    while(element != NULL) {
        if(element->edge->nodeA == nodeA && element->edge->nodeB == nodeB)
            return element;
        element = element->next;
    }
    return NULL;
}

Edge *findEdge(EdgeList list, Node* nodeA, Node* nodeB) {
    EdgeListElement *element = findElement(list, nodeA, nodeB);
    if(element == NULL)
        return NULL;
    else
        return element->edge;
}

void removeEdge(Node *node1, Node *node2) {
    Edge *edge = findEdge(node1->edges, node1, node2);
    if(edge == NULL)
        return;
    
    removeEdgeFromList(&(node1->edges), node1, node2);
    removeEdgeFromList(&(node2->edges), node1, node2);
    deleteEdge(edge);
}

void freeEdgeList(EdgeList list) {
    EdgeListElement *e = list;

    while(e != NULL) {
        EdgeListElement *next = e->next;
        free(e);
        e = next;
    }
}

void removeEdgeFromList(EdgeList* list, Node* nodeA, Node* nodeB) {
    if(nodeA->id > nodeB->id) {
        Node *tmp = nodeA;
        nodeA = nodeB;
        nodeB = tmp;
    }
    EdgeListElement *element = findElement(*list, nodeA, nodeB);
    if(element == NULL)
        return;
    
    if(element->prev != NULL)
        element->prev->next = element->next;
    else {
        *list = element->next;
        if(*list != NULL)
            (*list)->last = element->last;
    }

    if(element->next != NULL)
        element->next->prev = element->prev;
    else {
        if(*list != NULL)
            (*list)->last = element->prev;
    }

    free(element);
}

bool addRoute(Edge* edge, int route) {
    if(findInt(edge->routes, route) != NULL)
        return true;
    return addBackElement(&(edge->routes), route, NULL);
}

void removeRouteFromEdge(Edge* edge, int route) {
    removeListElement(&(edge->routes), findInt(edge->routes, route));
}