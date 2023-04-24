#include <stdlib.h>
#include "node.h"
#include "character.h"
#include "avl.h"

Node *newNode() {
    Node *node = malloc(sizeof(Node));
    Character *newCharacter = newChar();
    if(node == NULL || newCharacter == NULL) {
        free(node);
        deleteChar(newCharacter);
        return NULL;
    }

    node->word = newCharacter;
    node->numberOfChildren = 0;
    node->father = NULL;
    node->children = NULL;
    node->routes = NULL;
    node->id = 0;
    node->edges = NULL;
    node->dijkstraData = NULL;

    return node;
}

void deleteNode(Node *node) {
    if(node == NULL)
        return;
        
    deleteChar(node->word);
    deleteAvlTree(node->children);
    deleteAvlTree(node->routes);
    deleteEdgeList(node->edges);
    free(node->dijkstraData);
    free(node);
}

bool addChild(Node *nodeA, Node *nodeB, int c) {
    return addAvlKey(&(nodeA->children), c, nodeB, NULL);
}

void removeChild(Node *node, int c) {
    removeAvlKey(&(node->children), c);
}