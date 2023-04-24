#include <stdlib.h>
#include "dijkstra.h"
#include "avl.h"
#include "node.h"
#include "edge.h"
#include "map.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define LLU unsigned long long int

DijkstraData* newDijkstraData() {
    DijkstraData *new = malloc(sizeof(DijkstraData));
    if(new == NULL)
        return NULL;

    new->distance = 0;
    new->year = 0;
    new->visited = 0;
    new->nrOfCandidates = 0;
    new->bestNode = NULL;
    return new;
}

bool Dijkstra(Map *map, Node *node, int route) {
    AvlTree queue = newAvlNode(0, node, NULL);
    if(queue == NULL)
        return false;
    map->dijkstraCalls++;
    node->dijkstraData->distance = 0;
    node->dijkstraData->year = 0;
    node->dijkstraData->visited = map->dijkstraCalls;
    node->dijkstraData->nrOfCandidates = 1;
    node->dijkstraData->bestNode = NULL;

    while(queue != NULL) {
        AvlNode *avlNode = findMinimumKey(queue);
        Node *currentNode = avlNode->node;
  
        EdgeListElement *element = currentNode->edges;
        removeAvlNode(&queue, avlNode);
        if(findAvlKey(currentNode->routes, route) != NULL || currentNode == map->routeBegin[route] || 
                currentNode == map->routeEnd[route]) {
            if(currentNode != node) {
                currentNode->dijkstraData->nrOfCandidates = -1;
                continue;
            }
        }

        while(element != NULL) {
            Node *endOfEdge;
            if(currentNode == element->edge->nodeA) 
                endOfEdge = element->edge->nodeB;
            else 
                endOfEdge = element->edge->nodeA;

            if(findInt(element->edge->routes, route) != NULL) {
                element = element->next;
                continue;
            }

            int roadLength = element->edge->length;
            int roadYear = element->edge->year;
            int oldDistance = endOfEdge->dijkstraData->distance;
            int newDistance = currentNode->dijkstraData->distance + roadLength;
            int newYear = currentNode->dijkstraData->year;
            bool memory = true;
            if(newYear == 0 || roadYear < newYear)
                newYear = roadYear;
            
            if(endOfEdge->dijkstraData->visited != map->dijkstraCalls) {
                endOfEdge->dijkstraData->visited = map->dijkstraCalls;
                endOfEdge->dijkstraData->nrOfCandidates = currentNode->dijkstraData->nrOfCandidates;
                endOfEdge->dijkstraData->bestNode = currentNode;
                endOfEdge->dijkstraData->distance = newDistance;
                endOfEdge->dijkstraData->year = newYear;
                memory = addAvlKey(&queue, newDistance, endOfEdge, NULL);
            }
            else if(newDistance < oldDistance) {
                AvlNode *avlf = findAvlNode(queue, oldDistance, endOfEdge);
                removeAvlNode(&queue, avlf);
                endOfEdge->dijkstraData->nrOfCandidates = currentNode->dijkstraData->nrOfCandidates;
                endOfEdge->dijkstraData->bestNode = currentNode;
                endOfEdge->dijkstraData->distance = newDistance;
                endOfEdge->dijkstraData->year = newYear;
                memory = addAvlKey(&queue, newDistance, endOfEdge, NULL);
            }
            else if(newDistance == oldDistance) {
                if(endOfEdge->dijkstraData->year < newYear) {
                    endOfEdge->dijkstraData->nrOfCandidates = currentNode->dijkstraData->nrOfCandidates;
                    endOfEdge->dijkstraData->bestNode = currentNode;
                    endOfEdge->dijkstraData->year = newYear;
                }
                else if(endOfEdge->dijkstraData->year == newYear)
                    endOfEdge->dijkstraData->nrOfCandidates += currentNode->dijkstraData->nrOfCandidates;
            }

            if(!memory) {
                deleteAvlTree(queue);
                return false;
            }
            element = element->next;
        }
    }

    deleteAvlTree(queue);
    return true;
}

void getNrOfCandidates(Map *map, Node *node, int route) {
    if(!isVisited(map, node))
        return;
    int year = node->dijkstraData->year;
    Node *tmp = node;
    while(node->dijkstraData->distance != 0) {
        int candidates = 0;
        for(EdgeListElement *e = node->edges; e != NULL; e = e->next) {
            Node *endOfEdge;
            if(node == e->edge->nodeA) 
                endOfEdge = e->edge->nodeB;
            else 
                endOfEdge = e->edge->nodeA;
            int edgeYear = 0;
            if(findInt(e->edge->routes, route) != NULL)
                continue;
            if(isVisited(map, endOfEdge) && endOfEdge->dijkstraData->year == 0) 
                edgeYear = e->edge->year;
            else if(isVisited(map, endOfEdge))
                edgeYear = MIN(endOfEdge->dijkstraData->year, e->edge->year);
            if(isVisited(map, endOfEdge) && endOfEdge->dijkstraData->distance + e->edge->length == node->dijkstraData->distance &&
                    edgeYear >= year && endOfEdge->dijkstraData->nrOfCandidates != -1)
                candidates++;
        }
        if(candidates != 1) {
            tmp->dijkstraData->nrOfCandidates = candidates;
            return;
        }
        else 
            node = node->dijkstraData->bestNode;
    }
    tmp->dijkstraData->nrOfCandidates = 1;
}

List getNodes(Node *node, bool reversed) {
    List list = newListElement(0, node);
    if(list == NULL)
        return NULL;

    do {
        bool memory = true;
        node = node->dijkstraData->bestNode;
        if(!reversed)
            memory = addBackElement(&list, 0, node);
        else
            memory = addFrontElement(&list, 0, node);
        
        if(!memory) {
            deleteList(list);
            return NULL;
        }
    } while(node->dijkstraData->bestNode != NULL);

    return list;
}

bool UpdateRoutes(List list) {
    int currentRoute = 0;
    bool memory = true;
    List tmp = list;

    while(list != NULL) {
        if(list->node == NULL && list->integer != 0)
            currentRoute = list->integer;
        else if(list->next != NULL && list->next->node != NULL) {
            memory = true;
            Node *nodeA = list->node;
            Node *nodeB = list->next->node;
            Edge *edge = findEdge(nodeA->edges, nodeA, nodeB);
            memory = addAvlKey(&(nodeA->routes), currentRoute, nodeB, edge);
            memory = memory & addRoute(edge, currentRoute);

            if(!memory)
                break;
        }

        list = list->next;
    }

    if(memory)
        return true;

    do {
        if(tmp->node == NULL && tmp->integer != 0)
            currentRoute = list->integer;
        else if(tmp->next != NULL && tmp->next->node != NULL) {
            Node *nodeA = tmp->node;
            Node *nodeB = tmp->next->node;
            removeAvlKey(&(nodeA->routes), currentRoute);
            Edge *edge = findEdge(nodeA->edges, nodeA, nodeB);
            removeRouteFromEdge(edge, currentRoute);
        }
        tmp = tmp->next;
    } while(tmp != list->next);

    return false;
}

bool isVisited(Map *map, Node *node) {
    if(node->dijkstraData->visited == map->dijkstraCalls)
        return true;
    else
        return false;
}