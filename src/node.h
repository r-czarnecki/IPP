/** @file
 * Struktura przechowująca wierzchołek drzewa radix 
 */
#ifndef NODE_H
#define NODE_H

#include "character.h"
#include "dijkstra.h"
#include "edge.h"
#include "avl.h"

/** @brief Wierzchołek miasta (oraz wierzchołek radix tree).
 * Struktura przechowująca informacje o wierzchołku w sensie pozycji w strukturze
 * radix tree oraz jako miasto.
 */
typedef struct sNode {
    Character *word; ///< Wskaźnik na ciąg liter przechowywany w drzewie radix
    int numberOfChildren; ///< Ilość dzieci w drzewie radix
    struct sNode *father; ///< Wskaźnik na ojca w drzewie radix
    long long int id; ///< Numer unikalny dla każdego wierzchołka będącego miastem
    AvlTree children; ///< Drzewo AVL dzieci w drzewie radix
    AvlTree routes; /**< @brief Drzewo AVL dróg krajowych wychodzących z wierzchołka
                    Kluczem są numery dróg krajowych, a trzymane są kolejne wierzchołki dróg */
    EdgeList edges; ///< Wskaźnik na listę krawędzi do sąsiadujących miast
    DijkstraData* dijkstraData; ///< Informacje potrzebne do działania Dijkstry
} Node;

/** @brief Tworzy nową strukturę.
 * Tworzy nowy wierzchołek, ustawia: word na newChar(); numberOfChildren i id na 0;
 * father, children, routes, edges i dijkstraData na null.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
Node *newNode();

/** @brief Usuwa strukturę.
 * Usuwa wierzchołek wskazywany przez @p node.
 * @param[in] node  – wierzchołek przeznaczony do usunięcia.
 */
void deleteNode(Node *node);

/** @brief Dodaje wierzchołek do drzewa AVL children.
 * Dodaje do drzewa children przechowywanego w wierzchołku wskazywanym przez @p nodeA
 * wierzchołek, na który wskazuje @p nodeB i który przetrzymuje wyraz zaczynający
 * się literą @p c.
 * @param[in,out] nodeA – wskaźnik na wierzchołek, którego drzewo children jest aktualizowane;
 * @param[in] nodeB     – wskaźnik na dodawany wierzchołek;
 * @param[in] c         – litera rozpoczynająca wyraz przechowywany w @p nodeB;
 * @return Wartość @p true, jeżeli udało się dodać wierzchołek lub wartość @p false,
 * jeżeli nie udało się zaalokować pamięci.
 */
bool addChild(Node *nodeA, Node *nodeB, int c);

/** @brief Usuwa wierzchołek z drzewa AVL children.
 * Z drzewa children w wierzchołku wskazywanym przez @p node usuwa wierzchołek, który
 * przetrzymuje wyraz zaczynający się literą @p c.
 * @param[in,out] node  – wierzchołek, którego drzewo children jest aktualizowane;
 * @param[in] c         – litera rozpoczynająca wyraz przechowywany w usuwanym wierzchołku.
 */
void removeChild(Node *node, int c);

#endif