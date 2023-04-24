#include <stdlib.h>
#include <stdio.h>
#include "map.h"
#include "edge.h"
#include "input.h"

/** @brief Przywraca zmiany w mapie.
 * Funkcja usuwa krawędzie w liście @p edges i zmienia lata remontów krawędzi
 * w liście @p updated.
 * @param[in] edges     – krawędzie do usunięcia;
 * @param[in] updated   – krawędzie, których rok remontu został zmieniony.
 */
static void reverseChanges(EdgeList edges, EdgeList updated) {
    for(EdgeListElement *e = edges; e != NULL; e = e->next)
        removeEdge(e->edge->nodeA, e->edge->nodeB);

    for(EdgeListElement *e = updated; e != NULL; e = e->next)
        e->edge->year = e->i;
}

Map* newMap(void) {
    Map *newMap = malloc(sizeof(Map));
    RadixTree newRadix = newNode();
    if(newMap == NULL || newRadix == NULL) {
        free(newMap);
        deleteNode(newRadix);
        return NULL;
    }

    newMap->radix = newRadix;
    for(int i = 0; i < 1000; i++) {
        newMap->routeBegin[i] = NULL;
        newMap->routeEnd[i] = NULL;
    }
    newMap->nextId = 1;
    newMap->dijkstraCalls = 0;

    return newMap;
}

void deleteMap(Map *map) {
    deleteTree(map->radix);
    free(map);
}

bool addRoad(Map *map, const char *city1, const char *city2,
             unsigned length, int builtYear) {
    if(length <= 0 || builtYear == 0)
        return false;
    int last1 = 0, last2 = 0;
    bool found1 = false, found2 = false;
    int out = 0;
    Node *added1 = findCity(map->radix, city1);
    if(added1 == NULL) 
        out = addWord(map, map->radix, &last1, city1, &added1);
    else
        found1 = true;

    if(out != 0)
        return false;
        
    Node *added2 = findCity(map->radix, city2);
    if(added2 == NULL)
        out = addWord(map, map->radix, &last2, city2, &added2);
    else
        found2 = true;

    if(out != 0) {
        removeRadixNode(added1);
        return false;
    }

    added1 = findCity(map->radix, city1);
    added2 = findCity(map->radix, city2);

    if(added1 == added2 || last1 != 0 || last2 != 0 || 
            findElement(added1->edges, added1, added2) != NULL) {
        if(!found1) 
            removeRadixNode(added1);
        if(!found2 && added1 != added2)
            removeRadixNode(added2);
        return false;
    }
    if(!addEdge(added1, added2, length, builtYear)) {
        if(!found1) 
            removeRadixNode(added1);
        if(!found2)
            removeRadixNode(added2);
        return false;
    }
    
    return true;
}

bool repairRoad(Map *map, const char *city1, const char *city2, int repairYear) {
    Node *node1 = findCity(map->radix, city1);
    Node *node2 = findCity(map->radix, city2);
    if(node1 == NULL || node2 == NULL || repairYear == 0)
        return false;
    Edge *edge = findEdge(node1->edges, node1, node2);
    if(edge == NULL || edge->year > repairYear)
        return false;
    edge->year = repairYear;
    return true;
}

bool newRoute(Map *map, unsigned routeId, const char *city1, const char *city2) {
    if(routeId < 1 || routeId > 999 || map->routeBegin[routeId] != NULL)
        return false;
    Node *node1 = findCity(map->radix, city1);
    Node *node2 = findCity(map->radix, city2);
    if(node1 == NULL || node2 == NULL || node1 == node2)
        return false;

    if(!Dijkstra(map, node1, routeId))
        return false;
    getNrOfCandidates(map, node2, routeId);

    if(!isVisited(map, node2) || node2->dijkstraData->nrOfCandidates != 1)
        return false;
    
    List list = newListElement(routeId, NULL);
    List list2 = getNodes(node2, true);
    if(list == NULL || list2 == NULL) {
        deleteList(list);
        deleteList(list2);
        return false;
    }
    mergeLists(&list, list2);
    bool memory = UpdateRoutes(list);
    deleteList(list);
    if(!memory)
        return false;
    else {
        map->routeBegin[routeId] = node1;
        map->routeEnd[routeId] = node2;
        return true;
    }
}

bool extendRoute(Map *map, unsigned routeId, const char *city) {
    Node *foundNode = findCity(map->radix, city);
    if(routeId < 1 || routeId > 999 || foundNode == NULL)
        return false;
    if(map->routeBegin[routeId] == NULL || findAvlKey(foundNode->routes, routeId) != NULL
            || map->routeEnd[routeId] == foundNode)
        return false;
    
    if(!Dijkstra(map, foundNode, routeId))
        return false;

    bool reversed = false, ok1 = true, ok2 = true;
    Node *chosenEnd;
    Node *begin = map->routeBegin[routeId], *end = map->routeEnd[routeId];
    getNrOfCandidates(map, begin, routeId);
    getNrOfCandidates(map, end, routeId);
    if(!isVisited(map, begin))
        ok1 = false;
    if(!isVisited(map, end))
        ok2 = false;

    if(!ok1 && !ok2)
        return false;
    else if(!ok1) 
        chosenEnd = end;
    else if(!ok2) {
        reversed = true;
        chosenEnd = begin;
    } 
    else if(begin->dijkstraData->distance < end->dijkstraData->distance) {
        reversed = true;
        chosenEnd = begin;
    }
    else if(begin->dijkstraData->distance > end->dijkstraData->distance)
        chosenEnd = end;
    else if(begin->dijkstraData->year > end->dijkstraData->year) {
        reversed = true;
        chosenEnd = begin;
    }
    else if(begin->dijkstraData->year < end->dijkstraData->year)
        chosenEnd = end;
    else
        return false;
    if(chosenEnd->dijkstraData->nrOfCandidates != 1)
        return false;

    List list = newListElement(routeId, NULL);
    List list2 = getNodes(chosenEnd, reversed);
    if(list == NULL || list2 == NULL) {
        deleteList(list);
        deleteList(list2);
        return false;
    }
    mergeLists(&list, list2);
    int memory = UpdateRoutes(list);
    if(memory) {
        if(reversed)
            map->routeBegin[routeId] = foundNode;
        else
            map->routeEnd[routeId] = foundNode;
    }
    deleteList(list);
    if(!memory)
        return false;
    else
        return true;
}

bool removeRoad(Map *map, const char *city1, const char *city2) {
    Node *node1 = findCity(map->radix, city1);
    Node *node2 = findCity(map->radix, city2);
    if(node1 == NULL || node2 == NULL)
        return false;
    Edge *edge = findEdge(node1->edges, node1, node2);
    ListElement *lastElement = NULL;
    if(edge == NULL)
        return false;
    List routes = edge->routes;
    List update = NULL;
    bool memory = true;
    for(ListElement *element = routes; element != NULL; element = element->next) {
        int route = element->integer;
        List list = newListElement(route, NULL);
        List list2;
        AvlNode *found = findAvlKey(node1->routes, route);
        if(found == NULL || found->node != node2) {
            memory = Dijkstra(map, node2, route);
            getNrOfCandidates(map, node1, route);
            if(list == NULL || !memory || !isVisited(map, node1) || node1->dijkstraData->nrOfCandidates != 1) {
                deleteList(list);
                memory = false;
                break;
            }
            list2 = getNodes(node1, true);
            if(list2 == NULL) {
                deleteList(list2);
                memory = false;
                break;
            }
            lastElement = element;
            element->node = node2;
            removeAvlKey(&(node2->routes), route);
        }
        else {
            memory = Dijkstra(map, node1, route);
            getNrOfCandidates(map, node2, route);
            if(list == NULL || !memory || !isVisited(map, node2) || node2->dijkstraData->nrOfCandidates != 1) {
                deleteList(list);
                memory = false;
                break;
            }

            list2 = getNodes(node2, true);
            if(list2 == NULL) {
                deleteList(list2);
                memory = false;
                break;
            }
            lastElement = element;
            element->node = node1;
            removeAvlKey(&(node1->routes), route);
        }
        mergeLists(&list, list2);
        if(update == NULL)
            update = list;
        else
            mergeLists(&update, list);
    }

    if(!memory || !UpdateRoutes(update)) {
        deleteList(update);
        if(lastElement != NULL)
            for(ListElement *element = routes; element != (lastElement->next); element = element->next) {
                int route = element->integer;
                if((element->node) == node2)
                    addAvlKey(&(node2->routes), route, node1, edge);
                else 
                    addAvlKey(&(node1->routes), route, node2, edge);
            }
        return false;
    }
    else {
        deleteList(update);
        removeEdge(node1, node2);
        return true;
    }
}

char const* getRouteDescription(Map *map, unsigned routeId) {
    Character *result;
    if(routeId >= 1 && routeId <= 999 && map->routeBegin[routeId] != NULL) 
        result = intToChar(routeId);
    else
        result = newChar();
    if(result == NULL)
        return NULL;
    
    if(result->c == -1) {
        const char *str = toStr(result);
        deleteChar(result);
        return str;
    }

    if(addChar(';', result) == 0) {
        deleteChar(result);
        return NULL;
    }
    Node *node = map->routeBegin[routeId];
    AvlNode *foundAvlNode = findAvlKey(node->routes, routeId);
    Character *tmp = getCityName(node);
    if(tmp == NULL) {
        deleteChar(result);
        return NULL;
    }
    appendChar(result, tmp);
    Node *nextNode;
    if(foundAvlNode == NULL) {
        const char *str = toStr(result);
        deleteChar(result);
        return str;
    }
    else
        nextNode = foundAvlNode->node;
    Edge *edge = findEdge(node->edges, node, nextNode);
    while(edge != NULL) {
        node = nextNode;
        foundAvlNode = findAvlKey(node->routes, routeId);
        if(foundAvlNode == NULL) 
            nextNode = NULL;
        else
            nextNode = foundAvlNode->node;
        int memory = addChar(';', result);
        tmp = intToChar(edge->length);
        appendChar(result, tmp);
        memory = memory & addChar(';', result);
        Character *tmp2 = intToChar(edge->year);
        appendChar(result, tmp2);
        memory = memory & addChar(';', result);
        Character *tmp3 = getCityName(node);
        appendChar(result, tmp3);
        if(tmp == NULL || tmp2 == NULL || tmp3 == NULL || memory == 0) {
            deleteChar(result);
            return NULL;
        }
        if(foundAvlNode == NULL)
            edge = NULL;
        else
            edge = findEdge(node->edges, node, nextNode);
    }

    const char *str = toStr(result);
    deleteChar(result);
    return str;
}

bool createRoute(Map *map, int routeId, int *lastChar) {
    if(routeId <= 0 || routeId >= 1000 || map->routeBegin[routeId] != NULL)
        return false;

    int last;
    EdgeList addedEdges = NULL;
    EdgeList updatedEdges = NULL;

    const char *str = readNextArg(&last);
    *lastChar = last;
    if(str == NULL)
        return false;

    if(!isGoodCityName(str) || last != ';') {
        free((void*)str);
        return false;
    }

    Node *foundNode = findCity(map->radix, str);
    if(foundNode == NULL) {
        int tmp;
        if(addWord(map, map->radix, &tmp, str, &foundNode) != 0) {
            free((void*)str);
            return false;
        }
        foundNode = findCity(map->radix, str);
    }

    map->routeBegin[routeId] = foundNode;
    map->routeEnd[routeId] = foundNode;
    bool memory = true;
    while(last != '\n' && last != EOF) {
        int length, year;
        const char *tmpStr, *city;
        bool error;
        tmpStr = readNextArg(&last);
        *lastChar = last;
        length = charToInt(tmpStr, &error);
        free((void*)tmpStr);
        if(length <= 0 || last != ';' || !error) {
            memory = false;
            break;
        }

        tmpStr = readNextArg(&last);
        *lastChar = last;
        year = charToInt(tmpStr, &error);
        free((void*)tmpStr);
        if(year == 0 || last != ';' || !error) {
            memory = false;
            break;
        }

        city = readNextArg(&last);
        *lastChar = last;
        if(city == NULL || !isGoodCityName(city)) {
            memory = false;
            free((void*)city);
            break;
        }

        Node *nextNode = findCity(map->radix, city);
        if(nextNode == NULL) {
            int tmp;
            if(addWord(map, map->radix, &tmp, city, &nextNode) != 0) {
                memory = false;
                free((void*)city);
                break;
            }

            nextNode = findCity(map->radix, city);
            foundNode = findCity(map->radix, str);
        }
        else if(findAvlKey(nextNode->routes, routeId) != NULL) {
            memory = false;
            free((void*)city);
            break;
        }

        if(foundNode == nextNode) {
            memory = false;
            free((void*)city);
            break;
        }

        Edge *edge = findEdge(foundNode->edges, foundNode, nextNode);
        if(edge != NULL && (edge->length != length || edge->year > year)) {
            memory = false;
            free((void*)city);
            break;
        }
        else if(edge == NULL) {
            if(addEdge(foundNode, nextNode, length, year))
                edge = findEdge(foundNode->edges, foundNode, nextNode);
            else {
                memory = false;
                free((void*)city);
                break;
            }

            if(!addEdgeToList(&addedEdges, edge)) {
                memory = false;
                free((void*)city);
                break;
            }
        }
        else if(edge != NULL) {
            if(!addEdgeToList(&updatedEdges, edge)) {
                memory = false;
                free((void*)city);
                break;
            }
            updatedEdges->last->i = edge->year;

            edge->year = year;
        }

        memory = memory & addFrontElement(&(edge->routes), routeId, NULL);
        memory = memory & addAvlKey(&(foundNode->routes), routeId, nextNode, edge);
        map->routeEnd[routeId] = nextNode;

        if(!memory) {
            free((void*)city);
            break;
        }

        foundNode = nextNode;

        free((void*)str);
        str = city;
    }

    free((void*)str);
    
    if(!memory || last == EOF) {
        removeRoute(map, routeId);
        reverseChanges(addedEdges, updatedEdges);
        freeEdgeList(updatedEdges);
        freeEdgeList(addedEdges);
        return false;
    }

    freeEdgeList(updatedEdges);
    freeEdgeList(addedEdges);
    return true;
}

bool removeRoute(Map *map, int routeId) {
    if(routeId <= 0 || routeId >= 1000)
        return false;
    if(map->routeBegin[routeId] == NULL)
        return false;

    Node *node = map->routeBegin[routeId];
    while(node != map->routeEnd[routeId]) {
        Node *nextNode = findAvlKey(node->routes, routeId)->node;
        Edge *edge = findEdge(node->edges, node, nextNode);
        removeAvlKey(&(node->routes), routeId);
        removeListElement(&(edge->routes), findInt(edge->routes, routeId));
        node = nextNode;
    }
    map->routeBegin[routeId] = NULL;
    map->routeEnd[routeId] = NULL;
    return true;    
}