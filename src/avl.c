#include <stdlib.h>
#include <stdio.h>
#include "avl.h"
#include "node.h"

#define MAX(a,b) ((a > b) ? a : b)
#define ABS(a) ((a < 0) ? -(a) : a)

AvlNode* newAvlNode(int key, Node *nodeArg, Edge *edgeArg) {
    AvlNode *newNode = malloc(sizeof(AvlNode));

    if(newNode == NULL)
        return NULL;

    newNode->key = key;
    newNode->node = nodeArg;
    newNode->edge = edgeArg;
    newNode->height = 0;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->father = NULL;
    return newNode;
}

void deleteAvlNode(AvlNode *avlNode) {
    free(avlNode);
}

void deleteAvlTree(AvlTree avl) {
    if(avl == NULL)
        return;

    deleteAvlTree(avl->left);
    deleteAvlTree(avl->right);
    deleteAvlNode(avl);
}

AvlNode* findAvlKey(AvlTree avl, int key) {
    while(avl != NULL && avl->key != key) {
        if(avl->key > key)
            avl = avl->left;
        else if(avl->key < key)
            avl = avl->right;
    }

    return avl;
}

AvlNode* findAvlNode(AvlTree avl, int key, Node *node) {
    while(avl != NULL && (avl->key != key || avl->node != node)) {
        if(avl->key > key)
            avl = avl->left;
        else if(avl->key < key)
            avl = avl->right;
        else if(avl->node->id > node->id)
            avl = avl->left;
        else
            avl = avl->right;
    }

    return avl;
}

AvlNode* findMinimumKey(AvlTree avl) {
    if(avl == NULL)
        return NULL;
    
    while(avl->left != NULL)
        avl = avl->left;
    return avl;
}

bool addAvlKey(AvlTree* avl, int key, Node *node, Edge *edge) {
    AvlNode *newNode = newAvlNode(key, node, edge);
    if(newNode == NULL)
        return false;

    if(*avl == NULL) {
        *avl = newNode;
        return true;
    }

    AvlNode *tmp = *avl;
    bool left = false;
    while(true) {
        AvlNode *tmp2 = tmp;
        if(tmp->key > key) {
            tmp2 = tmp->left;
            left = true;
        }
        else if(tmp->key < key) {
            tmp2 = tmp->right;
            left = false;
        }
        else if(tmp->node->id > node->id) {
            tmp2 = tmp->left;
            left = true;
        }
        else {
            tmp2 = tmp->right;
            left = false;
        }
        
        if(tmp2 == NULL)
            break;
        else
            tmp = tmp2;
    }

    newNode->father = tmp;
    if(left)
        tmp->left = newNode;
    else
        tmp->right = newNode;

    tmp = newNode;
    while(tmp != NULL) {
        balance(avl, tmp);
        tmp = tmp->father;
    }
    return true;
}

void removeAvlNode(AvlTree* avl, AvlNode *foundNode) {
    bool left = isLeft(foundNode);

    if(foundNode->left != NULL) {
        AvlNode *tmp = foundNode;
        foundNode = foundNode->left;
        left = true;

        while(foundNode->right != NULL) {
            left = false;
            foundNode = foundNode->right;
        }

        tmp->key = foundNode->key;
        tmp->node = foundNode->node;
        tmp->edge = foundNode->edge;

        if(foundNode->left != NULL) {
            if(left) 
                foundNode->father->left = foundNode->left;
            else
                foundNode->father->right = foundNode->left;
            
            foundNode->left->father = foundNode->father;
        }
        else {
            if(left)
                foundNode->father->left = NULL;
            else
                foundNode->father->right = NULL;
        }

        tmp = foundNode->father;
        deleteAvlNode(foundNode);

        while(tmp != NULL) {
            balance(avl, tmp);
            tmp = tmp->father;
        }
    }
    else {
        if(*avl == foundNode) {
            *avl = foundNode->right;
            if(*avl != NULL)
                (*avl)->father = NULL;
            deleteAvlNode(foundNode);
            return;
        }

        AvlNode *tmp = foundNode->right;
        if(tmp != NULL) {
            tmp->father = foundNode->father;
            *foundNode = *(foundNode->right);
            if(foundNode->left != NULL)
                foundNode->left->father = foundNode;
            if(foundNode->right != NULL)
                foundNode->right->father = foundNode;
            deleteAvlNode(tmp);
        }
        else {
            tmp = foundNode->father;
            if(tmp != NULL) {
                if(isLeft(foundNode))
                    tmp->left = NULL;
                else
                    tmp->right = NULL;
            }
            deleteAvlNode(foundNode);
            foundNode = tmp;
        }

        while(foundNode != NULL) {
            balance(avl, foundNode);
            foundNode = foundNode->father;
        }
    }
}

void removeAvlKey(AvlTree* avl, int key) {
    AvlNode *foundNode = findAvlKey(*avl, key);
    if(foundNode == NULL)
        return;

    removeAvlNode(avl, foundNode);
}

void changeNodes(AvlTree avl, Node *nodeArg) {
    if(avl == NULL)
        return;
    changeNodes(avl->left, nodeArg);
    changeNodes(avl->right, nodeArg);
    avl->node->father = nodeArg;
}

void leftRotation(AvlTree* avl, AvlNode *avlNode) {
    AvlNode *father = avlNode->father;
    bool left = isLeft(father);

    if(avlNode->left != NULL)
        avlNode->left->father = father;
    father->right = avlNode->left;
    avlNode->left = father;
    avlNode->father = father->father;
    father->father = avlNode;
    if(avlNode->father == NULL)
        *avl = avlNode;
    else if(left)
        avlNode->father->left = avlNode;
    else
        avlNode->father->right = avlNode;
    updateHeight(father);
    updateHeight(avlNode);
}

void rightRotation(AvlTree *avl, AvlNode *avlNode) {
    AvlNode *father = avlNode->father;
    bool left = isLeft(father);

    if(avlNode->right != NULL)
        avlNode->right->father = father;
    father->left = avlNode->right;
    avlNode->right = father;
    avlNode->father = father->father;
    father->father = avlNode;
    if(avlNode->father == NULL)
        *avl = avlNode;
    else if(left)
        avlNode->father->left = avlNode;
    else
        avlNode->father->right = avlNode;
    updateHeight(father);
    updateHeight(avlNode);
}

void balance(AvlTree *avl, AvlNode *avlNode) {
    int hLeft = getHeight(avlNode->left), hRight = getHeight(avlNode->right);

    if(ABS(hLeft - hRight) <= 1) {
        updateHeight(avlNode);
        return;
    }

    bool left1, left2;
    AvlNode *tmp1, *tmp2;
    if(hLeft > hRight) {
        left1 = true;
        tmp1 = avlNode->left;
    }
    else {
        left1 = false;
        tmp1 = avlNode->right;
    }

    if(getHeight(tmp1->left) > getHeight(tmp1->right)) {
        left2 = true;
        tmp2 = tmp1->left;
    }
    else {
        left2 = false;
        tmp2 = tmp1->right;
    }

    if(left1 && left2) 
        rightRotation(avl, tmp1);
    else if(left1 && !left2) {
        leftRotation(avl, tmp2);
        rightRotation(avl, tmp2);
    }
    else if(!left1 && !left2)
        leftRotation(avl, tmp1);
    else {
        rightRotation(avl, tmp2);
        leftRotation(avl, tmp2);
    }
}

int getHeight(AvlNode *avlNode) {
    if(avlNode == NULL)
        return -1;
    else
        return avlNode->height; 
}

void updateHeight(AvlNode *avlNode) {
    int hLeft = getHeight(avlNode->left), hRight = getHeight(avlNode->right);
    avlNode->height = MAX(hLeft, hRight) + 1;
}

bool isLeft(AvlNode *avlNode) {
    if(avlNode->father != NULL) {
        if(avlNode->father->left == avlNode)
            return true;
        else
            return false;
    }
    else
        return false;
}