/** @file
 * Drzewo AVL o kluczach typu int 
 */

#ifndef AVL_H
#define AVL_H

#include <stdbool.h>
#include "edge.h"

/// @cond
typedef struct sNode Node;
/// @endcond

/**
 * Struktura reprezentująca wierzchołek drzewa AVL przechowujący wskaźnik do
 * Node oraz wskaźnik do Edge.
 */
typedef struct sAvlNode {
    int key; ///< Klucz
    Node *node; ///< Wskaźnik do Node 
    Edge *edge; ///< Wskaźnik do Edge
    int height; ///< Wysokość wierzchołka
    struct sAvlNode *left; ///< Wskaźnik do lewego wierzchołka
    struct sAvlNode *right; ///< Wskaźnik do prawego wierzchołka
    struct sAvlNode *father; ///< Wskaźnik na ojca
} AvlNode;

/**
 * Typ będący wskaźnikiem na typ AvlNode
 */
typedef AvlNode* AvlTree;

/** @brief Tworzy nową strukturę.
 * Tworzy nowy wierzchołek drzewa AVL, ustawia: key na @p key; node na @p nodeArg;
 * edge na @p edgeArg; height na 0; left, right oraz father na NULL.
 * @param[in] key   – klucz jaki ma zostać ustawiony;
 * @param[in] nodeArg  – wskaźnik na Node jaki ma zostać ustawiony;
 * @param[in] edgeArg  – wskaźnik na Edge jaki ma zostać ustawiony;
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
AvlNode* newAvlNode(int key, Node *nodeArg, Edge *edgeArg);

/** @brief Usuwa wierzchołek.
 * Usuwa wierzchołek, na który wskazuje @p avlNode.
 * @param[in] avlNode   – wskaźnik na usuwany wierzchołek.
 */
void deleteAvlNode(AvlNode *avlNode);

/** @brief Usuwa drzewo AVL.
 * Usuwa całe drzewo AVL ukorzenione w @p avl.
 * @param[in] avl   – korzeń drzewa, które ma zostać usunięte.
 */
void deleteAvlTree(AvlTree avl);

/** @brief Znajduje wierzchołek o zadanym kluczu.
 * Znajduje wskaźnik na wierzchołek w drzewie @p avl, o kluczu równym @p key.
 * @param[in] avl   – drzewo AVL, w którym następuje wyszukiwanie;
 * @param[in] key   – klucz, który jest szukany;
 * @return Wskaźnik na znaleziony wierzchołek lub NULL, jeżeli taki wierzchołek nie
 * istnieje. 
 */
AvlNode* findAvlKey(AvlTree avl, int key);

/** @brief Znajduje wierzchołek o zadanym kluczu i przechowujący konkretny wierzchołek.
 * Znajduje wskaźnik na wierzchołek w drzewie @p avl, o kluczu równym @p key i przechowujący
 * wskaźnik na wierzchołek @p node.
 * @param[in] avl   – drzewo AVL, w którym następuje wyszukiwanie;
 * @param[in] key   – klucz, który jest szukany;
 * @param[in] node  – wierzchołek, który ma być przechowywany w drzewie;
 * @return Wskaźnik na znaleziony wierzchołek lub NULL, jeżeli taki wierzchołek nie
 * istnieje.
 */
AvlNode* findAvlNode(AvlTree avl, int key, Node *node);

/** @brief Znajduje element o najmniejszym kluczu.
 * Znajduje w drzewie @p avl element o najmniejszym kluczu.
 * @param[in] avl   – drzewo avl, które jest przeszukiwane;
 * @return Element o najmniejszym kluczu lub NULL, jeżeli drzewo jest puste.
 */
AvlNode* findMinimumKey(AvlTree avl);

/** @brief Dodaje wierzchołek do drzewa AVL.
 * Dodaje wierzchołek do drzewa @p avl, o kluczu równym @p key i wskazujący na @p node oraz
 * @p edge.
 * @param[in,out] avl   – drzewo AVL, do którego dodawany jest wierzchołek;
 * @param[in] key       – dodawany klucz;
 * @param[in] node      – dodawany wskaźnik na Node;
 * @param[in] edge      – dodawany wskaźnik na Edge;
 * @return Wartość @p true, jeżeli wierzchołek został dodany.
 * Wartość @p false, jeżeli nie udało się zaalokować pamięci.
 */
bool addAvlKey(AvlTree* avl, int key, Node *node, Edge *edge);

/** @brief Usuwa wierzchołek z drzewa.
 * Usuwa wierzchołek @p foundNode z drzewa @p avl.
 * @param[in,out] avl   – drzewo AVL, z którego usuwany jest wierzchołek;
 * @param[in] foundNode – usuwany wierzchołek.
 */
void removeAvlNode(AvlTree* avl, AvlNode *foundNode);

/** @brief Usuwa wierzchołek o zadanym kluczu.
 * Usuwa wierzchołek w drzewie @p avl, o kluczu równym @p key lub nic nie robi, jeżeli
 * taki wierzchołek nie istnieje.
 * @param[in,out] avl   – drzewo AVL, w którym następuje wyszukiwanie;
 * @param[in] key       – klucz, który jest szukany.
 */
void removeAvlKey(AvlTree* avl, int key);

/** @brief Zmienia wartości node->father w każdym wierzchołku drzewa.
 * Dla każdego wierzchołka drzewa @p avl zmiena wartość node->father na @p nodeArg.
 * @param[in,out] avl   – drzewo, którego wierzchołki są aktualizowane;
 * @param[in] nodeArg   – wierzchołek, który ma zostać przypisany.
 */
void changeNodes(AvlTree avl, Node *nodeArg);

/** @brief Wykonuje leftRotation
 * Wykonuje leftRotation w drzewie @p avl w poniższy sposób:
 * <pre>
 *     ..                                ..    
 *       \                                 \
 *      father                           avlNode  
 *     /    \           -------->        /     \
 * left2  avlNode                     father  right   
 *        /     \                     /    \
 *      left1  right               left2  left1
 * </pre>
 * @param[in,out] avl   – wskaźnik na drzewo avl, w którym znajduje się @p avlNode;
 * @param[in] avlNode   – wskaźnik na wierzchołek w drzewie AVL.
 */
void leftRotation(AvlTree* avl, AvlNode *avlNode);

/** @brief Wykonuje rightRotation
 * Wykonuje rightRotation w drzewie @p avl w poniższy sposób:         
 * <pre>
 *      ..                            ..      
 *        \                             \            
 *      father                       avlNode               
 *       /    \      -------->       /     \        
 *  avlNode   right2               left  father          
 *   /    \                              /    \
 * left  right1                      right1  right2              
 *                                                   
 * </pre>
 * @param[in,out] avl   – wskaźnik na drzewo avl, w którym znajduje się @p avlNode;
 * @param[in] avlNode   – wskaźnik na wierzchołek w drzewie AVL.
 */
void rightRotation(AvlTree *avl, AvlNode *avlNode);

/** @brief Balansuje drzewo avl.
 * Balansuje drzewo @p avl w wierzchołku @p avlNode.
 * @param[in,out] avl   – drzewo, które jest balansowane;
 * @param[in] avlNode   – wierzchołek, w którym balansowanie zachodzi.
 */
void balance(AvlTree *avl, AvlNode *avlNode);

/** @brief Zwraca wysokońć wierzchołka.
 * Zwraca wysokość wierzchołka wskazywanego przez @p avlNode. Jeżeli @p avlNode wskazuje
 * na NULL, to zwraca -1.
 * @param[in] avlNode   – wierzchołek, którego wysokość jest zwracana;
 * @return Wysokość wierzchołka @p avlNode lub -1 jeżeli @p avlNode wskazuje na NULL. 
 */
int getHeight(AvlNode *avlNode);

/** @brief Aktualizuje wysokość wierzchołka.
 * Aktualizuje wysokość wierzchołka wskazywanego przez @p avlNode.
 * @param[in,out] avlNode   – wskaźnik na aktualizowany wierzchołek.
 */
void updateHeight(AvlNode *avlNode);

/** @brief Sprawdza, czy wierzchołek jest lewym dzieckiem, czy nie.
 * Sprawdza, czy wierzchołek wskazywany przez @p avlNode jest lewym dzieckiem,
 * czy nie.
 * @param[in] avlNode   – sprawdzany wierzchołek;
 * @return Wartość @p true, jeżeli wierzchołek jest lewym dzieckiem lub
 * wartość @p false, jeżeli jest prawym dzieckiem lub korzeniem drzewa.
 */
bool isLeft(AvlNode *avlNode);

#endif /* AVL_H */