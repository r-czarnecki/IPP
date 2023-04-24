/** @file
 * Interfejs struktury przechowującej drzewo radix 
 */
#ifndef RADIX_H
#define RADIX_H

#include "node.h"
#include "character.h"

/// @cond
typedef struct Map Map;
/// @endcond

/**
 * Typ będący wskaźnikiem na typ Node.
 */
typedef Node* RadixTree;

/** @brief Znajduje nazwę miasta w drzewie.
 * Znajduje nazwę miasta wskazywaną przez @p cityName. Ustawia @p c na wskaźnik na ostatni znak nazwy.
 * Ustawia @p found na 1 jeżeli znaleziono odpowiednią nazwę, a na 0 w przeciwnym wypadku.
 * Ustawia @p character na pozyję na której wystąpiła różnica, lub NULL jeżeli różnica nie wystąpiła.
 * @param[in] node          – wskaźnik na początek drzewa radix
 * @param[in,out] c         – wskaźnik na wskaźnik na zmienną typu char opisaną powyżej
 * @param[in,out] found     – wskaźnik na zmienną typu int opisaną powyżej
 * @param[in,out] character – wskaźnik na wskaźnik na strukturę typu Character opisaną powyżej
 * @param[in] cityName      – wskaźnik na nazwę miasta
 * @return Wskaźnik na wierzchołek, w którym zakończyło się przeszukiwanie, lub NULL
 * jeżeli nazwa miasta nie spełnia specyfikacji.
 */
Node *findWord(Node *node, const char **c, int *found, Character **character, const char *cityName);

/** @brief Dodaje nazwę miasta do drzewa radix.
 * Dodaje nazwę miasta wskazywaną przez @p cityName do drzewa ukorzenionego w @p node w mapie @p map.
 * Ustawia @c na ostatni znak nazwy.
 * @param[in] map           – mapa przechowująca wierzchołek @p node
 * @param[in,out] node      – korzeń drzewa radix
 * @param[in,out] c         – ostatni znak nazwy
 * @param[in] cityName      – wskaźnik na nazwę miasta
 * @param[in,out] addedNode – wskaźnik na dodany wierzchołek
 * @return 0 jeżeli udało się dodać, 1 jeżeli nie udało się dodać z powodu braku pamięci lub
 * 2 jeżeli nazwa miasta nie spełnia specyfikacji.
 */
int addWord(Map *map, Node *node, int *c, const char *cityName, Node **addedNode);

/** @brief Znajduje miasto w drzewie.
 * Znajduje miasto o nazwie @p city w drzewie ukorzenionym w @p node.
 * @param[in] node      – drzewo, w którym następuje przeszukiwanie;
 * @param[in] city      – wskaźnik na napis reprezentujący nazwę szukanego miasta;
 * @return Wskaźnik na znalezione miasto lub NULL, jeżeli miasto o podanej nazwie nie istnieje
 * lub jego nazwa nie spełnia specyfikacji.
 */
Node *findCity(Node *node, const char *city);

/** @brief Rozbija słowo na dwie części.
 * Dzieli słowo przechowywane przez wierzchołek wskazywany przez @p node w punkcie word.
 * @param[in] map           – mapa przechowująca wierzchołek @p node;
 * @param[in,out] node      – wskaźnik na dzielony wierzchołek;
 * @param[in,out] word      – wskaźnik na miejsce, w którym ma zostać przedzielone słowo;
 * @return 0 - jeżeli się udało lub 1 - jeżeli nie udało się zadeklarować pamięci
 */
int splitNode(Map *map, Node *node, Character *word);

/** @brief Łączy wierzchołek ze swoim dzieckiem.
 * Łączy wierzchołek wskazywany przez @p node ze swoim dzieckiem nie jest osobnym miastem.
 * @param[in,out] nodeA – wierzchołek, który będzie łączony ze swoim dzieckiem.
 */
void mergeNode(Node *nodeA);

/** @brief Zwraca nazwę miasta.
 * Zwraca nazwę miasta reprezentowanego przez @p node jako strukturę typu Character.
 * @param[in] node      – wierzchołek reprezentujący miasto, którego nazwa jest szukana;
 * @return Wskaźnik na utworzoną strukturę typu Character lub NULL, jeżeli nie udało się
 * zaalokować pamięci.
 */
Character *getCityName(Node *node);

/** @brief Usuwa wierzchołek z drzewa.
 * Usuwa wierzchołek wskazywany przez @p node w drzewie @p tree.
 * @param[in] node      – wskaźnik na usuwany wierzchołek.
 */
void removeRadixNode(Node* node);

/** @brief Usuwa poddrzewo.
 * Usuwa poddrzewo ukorzenione w @p node.
 * @param[in] node  – korzeń poddrzewa przeznaczonego do usunięcia
 */
void deleteTree(Node *node);

#endif