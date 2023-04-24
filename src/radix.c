#include <stdio.h>
#include <stdlib.h>
#include "radix.h"
#include "node.h"
#include "character.h"
#include "list.h"
#include "map.h"

/** Sprawdza, czy @p c nie jest numerem z przedziału [0, 31], ani średnikiem.
 * @param[in] c – sprawdzana litera;
 * @return Wartość @p true, jeżeli @p c spełnia powyższe warunki lub wartość @p false
 * w przeciwnym przypadku.
 */
static bool isGoodChar(int c) {
    if ((c >= 0 && c <= 31) || c == ';') 
        return 0;
    else 
        return 1;
}

/** @brief Sprawdza, czy @p c nie jest numerem z przedziału [1, 31], ani średnikiem.
 * @param[in] c – sprawdzana litera;
 * @return Wartość @p true, jeżeli @p c spełnia powyższe warunki lub wartość @p false
 * w przeciwnym przypadku.
 */
static bool isOk(int c) {
    if (isGoodChar(c) || c == 0)
        return 1;
    else
        return 0;
}

Node *findWord(Node *node, const char **c, int *found, Character **character, const char *cityName) {
    int firstLetter = 1;
    *character = node->word;
    *found = 0;
    
    while (1) {
        if(firstLetter == 0)
            cityName++;
        int input = *cityName;
        if(input == -1)
            input = 255;
        *c = cityName;

        if (firstLetter && !isGoodChar(input))
            return NULL;
        else
            firstLetter = 0;

        if (!isOk(input))
            return NULL;
        
        if (isGoodChar(input) && (*character)->c != -1) {
            if (input == (*character)->c)
                *character = (*character)->next;
            else {
                *found = 0;
                return node;
            }
        }
        else if (isGoodChar(input) && (*character)->c == -1) {
            AvlNode *foundNode = findAvlKey(node->children, input);
            if (foundNode != NULL) {
                node = foundNode->node;
                *character = node->word->next;
            }
            else {
                *found = 0;
                return node;
            }
        }
        else if (!isGoodChar(input)) {
            *found = 1;
            return node;
        }
    }
}

int addWord(Map *map, Node *node, int *c, const char *cityName, Node **addedNode) {
    const char *lastChar;
    Character *firstDifference;
    int found;

    Node *lastNode = findWord(node, &lastChar, &found, &firstDifference, cityName);

    *c = *lastChar;
    *addedNode = NULL;

    if (!isOk(*lastChar) || lastNode == NULL)
        return 2;
    if (found && firstDifference->c == -1) {
        *addedNode = lastNode;
        if(lastNode->id == 0) {
            DijkstraData *tmp = newDijkstraData();
            if(tmp == NULL)
                return 1;

            lastNode->dijkstraData = tmp;
            lastNode->id = map->nextId;
            map->nextId++;
        }
        return 0;
    }

    Character *word = newChar();
    if (word == NULL)
        return 1;
    
    while (*lastChar != 0) {
        if (!isOk(*lastChar)) {
            deleteChar(word);
            return 2;
        }
        
        int memory;
        if(*lastChar != -1)
            memory = addChar(*lastChar, word);
        else 
            memory = addChar(255, word);

        if (!memory) {
            deleteChar(word);
            return 1;
        }
        
        lastChar++;
        *c = *lastChar;
    }
    
    if(*lastChar != 0) {
        deleteChar(word);
        return 2;
    }

    if (firstDifference->c == -1) {
        if (lastNode->numberOfChildren == 0 && lastNode->id == 0 && lastNode->father != NULL) {
            DijkstraData *tmp = newDijkstraData();
            if(tmp == NULL)
                return 1;

            word->prev = firstDifference->prev;
            *firstDifference = *word;
            word->next->prev = firstDifference;
            lastNode->word->last = word->last;
            lastNode->dijkstraData = tmp;
            lastNode->id = map->nextId;
            map->nextId++;
            *addedNode = lastNode;

            free(word);
            return 0;
        }
        else {
            Node *new = newNode();
            DijkstraData *tmp = newDijkstraData();
            bool memory = addAvlKey(&(lastNode->children), word->c, new, NULL);
            if (new == NULL || tmp == NULL || !memory) {
                deleteChar(word);
                deleteNode(new);
                free(tmp);
                if(memory)
                    removeAvlKey(&(lastNode->children), word->c);
                return 1;
            }

            deleteChar(new->word);
            new->father = lastNode;
            new->word = word;
            new->dijkstraData = tmp;
            new->id = map->nextId;
            map->nextId++;
            *addedNode = new;

            lastNode->numberOfChildren++;

            return 0;
        }
    }
    else {
        if(word->c != -1) {
            int result = splitNode(map, lastNode, firstDifference);

            Node *newNodeA = newNode();
            DijkstraData *tmp = newDijkstraData();
            bool memory = addAvlKey(&(lastNode->children), word->c, newNodeA, NULL);

            if (newNodeA == NULL || result == 1 || tmp == NULL || !memory) {
                deleteChar(word);
                deleteNode(newNodeA);
                free(tmp);
                if(memory)
                    removeAvlKey(&(lastNode->children), word->c);
                return 1;
            }
            
            deleteChar(newNodeA->word);
            newNodeA->word = word;
            newNodeA->father = lastNode;
            newNodeA->dijkstraData = tmp;
            newNodeA->id = map->nextId;
            map->nextId++;
            *addedNode = newNodeA;
            
            lastNode->numberOfChildren = 2;
            
            return 0;
        }
        else {
            int result = splitNode(map, lastNode, firstDifference);
            if(result == 1)
                return 1;
            DijkstraData *data = newDijkstraData();
            if(data == NULL)
                return 1;
            deleteChar(word);
            lastNode->id = map->nextId;
            map->nextId++;
            lastNode->numberOfChildren = 1;
            lastNode->dijkstraData = data;
            return 0;
        }
    }
}

Node *findCity(Node *node, const char *city) {
    const char *last;
    int found;
    Character *difference;
    Node *foundNode = findWord(node, &last, &found, &difference, city);

    if(found == 0 || foundNode == NULL || difference->c != -1 || *last != 0
            || foundNode->id == 0)
        return NULL;
    else
        return foundNode;
}

int splitNode(Map *map, Node *node, Character *word) {
    Node *new = newNode();
    Character *character = newChar();
    AvlTree avlTree = newAvlNode(word->c, new, NULL);
    if (new == NULL || character == NULL) {
        deleteNode(new);
        deleteChar(character);
        deleteAvlTree(avlTree);
        return 1;
    }


    deleteChar(new->word);
    *new = *node;
    new->father = node;
    new->id = node->id;
    new->dijkstraData = node->dijkstraData;
    new->edges = node->edges;
    for(EdgeListElement *e = new->edges; e != NULL; e = e->next) {
        Node *endOfEdge;
        if(e->edge->nodeA == node) {
            e->edge->nodeA = new;
            endOfEdge = e->edge->nodeB;
        }
        else {
            e->edge->nodeB = new;
            endOfEdge = e->edge->nodeA;
        }

        for(ListElement *l = e->edge->routes; l != NULL; l = l->next) {
            AvlNode *avlNode = findAvlKey(endOfEdge->routes, l->integer);
            if(avlNode != NULL && avlNode->node == node)
                avlNode->node = new;
            if(map->routeBegin[l->integer] == node)
                map->routeBegin[l->integer] = new;
            if(map->routeEnd[l->integer] == node)
                map->routeEnd[l->integer] = new;
        }
    }
    new->routes = node->routes;
    new->word = word;
    word->last = node->word->last;
    new->children = node->children;
    changeNodes(new->children, new);

    for(int i = 1; i <= 999; i++) {
        if(map->routeBegin[i] == node)
            map->routeBegin[i] = new;
        if(map->routeEnd[i] == node)
            map->routeEnd[i] = new;
    }
    
    node->children = avlTree;
    node->dijkstraData = NULL;
    node->edges = NULL;
    node->id = 0;
    node->routes = NULL;
    node->numberOfChildren = 1;

    word->prev->next = character;
    character->prev = word->prev;
    word->prev = NULL;
    node->word->last = character;

    return 0;
}

void mergeNode(Node *nodeA) {
    if (nodeA->father == NULL)
        return;

    if (nodeA->numberOfChildren == 1 && nodeA->id == 0) {
        Node *child = findMinimumKey(nodeA->children)->node;
    
        Character *tmp = nodeA->word;
        appendChar(tmp, child->word);
        child->word = tmp;
        child->father = nodeA->father;
        if(nodeA->father != NULL) {
            removeAvlKey(&(nodeA->father->children), nodeA->word->c);
            addAvlKey(&(nodeA->father->children), nodeA->word->c, child, NULL);
        }

        deleteAvlTree(nodeA->children);
        free(nodeA);
    }
}

Character *getCityName(Node *node) {
    Character *new = getCopy(node->word);
    if(new == NULL)
        return NULL;
    node = node->father;
    while(node != NULL) {
        Character *tmp = newChar();
        if(tmp == NULL) {
            deleteChar(new);
            return NULL;
        }
        *tmp = *new;
        free(new);
        new = getCopy(node->word);
        if(new == NULL) {
            deleteChar(tmp);
            return NULL;
        }
        appendChar(new, tmp);
        node = node->father;
        
    }
    return new;
}

void removeRadixNode(Node* node) {
    free(node->dijkstraData);
    deleteEdgeList(node->edges);
    deleteAvlTree(node->routes);
    node->dijkstraData = NULL;
    node->id = 0;

    if(node->numberOfChildren == 1)
        mergeNode(node);
    else if(node->numberOfChildren == 0)
        deleteTree(node);
}

void deleteTree(Node *node) {
    if (node == NULL)
        return;

    while(node->children != NULL) {
        AvlNode *avlNode = findMinimumKey(node->children);
        deleteTree(avlNode->node);
    }

    if(node->father != NULL) {
        removeAvlKey(&(node->father->children), node->word->c);
        node->father->numberOfChildren--;
    }
    deleteNode(node);
}
