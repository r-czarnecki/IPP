/** @file
 * Funkcje i struktury związane z algorytmem Dijkstry 
 */

#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <stdbool.h>
#include "list.h"

/// @cond
typedef struct Map Map;
typedef struct sNode Node;
/// @endcond

/**
 * Struktura przechowująca najważniejsze informacje wymagane do działania
 * algorytmu Dijkstry.
 */
typedef struct sDijkstraData {
    int distance; ///< Odległość od wierzchołka, z którego ostatnio wywołany został algorytm Dijkstry
    int year; ///< Rok najdawniej wybudowanej drogi w optymalnej ścieżce
    long long int visited; ///< Wartość, która trzyma numer wywołania Dijkstry, która dotarła do tego wierzchołka
    int nrOfCandidates; ///< Ilość optymalnych ścieżek prowadzących do tego wierzchołka
    Node *bestNode; ///< Wskaźnik na poprzedni wierzchołek w optymalnej ścieżce
} DijkstraData;

/** @brief Tworzy nową strukturę.
 * Tworzy nową strukturę, ustawia: distance, year, visited i nrOfCandidates na 0 oraz bestNode na NULL.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
DijkstraData* newDijkstraData();

/** @brief Wywołuje algorytm Dijkstry.
 * Wywołuje algorytm Dijkstry z wierzchołka, na którego wskazuje @p node. Wyznaczona trasa nie może przechodzić przez
 * drogi należące do drogi krajowej o numerze @p route
 * @param[in] map   – wskaźnik na mapę przechowującą miasto @p node;
 * @param[in] node  – wierzchołek startowy;
 * @param[in] route – droga krajowa, przez którą nie może przechodzić wyznaczona trasa;
 * @return Wartość @p true, jeżeli algorytm wykonał się pomyślnie lub wartość @p false, jeżeli nie udało się zaalokować
 * odpowienio wiele pamięci.
 */
bool Dijkstra(Map *map, Node *node, int route);

/** Tworzy listę wierzchołków na optymalnej ścieżce.
 * Tworzy listę kolejnych wierzchołków leżących na ścieżce od @p node do wierzchołka, z którego
 * ostatnio był wywoływany algorytm Dijkstry. Jeżeli @p reversed jest równe @p true, to wierzchołki
 * będą dodane na listę w odwrotnej kolejności. W przeciwnym wypadku ich kolejność pozostanie
 * niezmieniona.
 * @param[in] node      – wskaźnik na wierzchołek, od którego rozpoczyna się konstrukcja listy;
 * @param[in] reversed  – zmienna decydująca o tym, czy wierzchołki na liście mają być w odwrotnej kolejności, czy nie
 * @return Wskaźnik na utworzoną listę wierzchołków lub NULL, jeżeli nie istnieje ścieżka między 
 * @p node, a wierzchołkiem, z którego ostatnio wywoływany był algorytm Dijkstry.
 */
List getNodes(Node *node, bool reversed);

/** Aktualizuje stan dróg krajowych.
 * Aktualizuje wierzchołki wymienione na liście @p list tak, aby struktura reprezentacji dróg krajowych
 * była poprawna. Jeżeli na liście napotka element, którego wartość @p node = NULL, a @p integer jest niezerowy, to
 * od tego momentu ustalany będzie przebieg drogi krajowej numer @p integer. Jeżeli element listy ma wskaźnik @p node, który
 * nie wskazuje na NULL oraz wskaźnik @p node kolejnego elementu listy nie wskazuje na NULL, to do aktualnie aktualizowanej
 * ścieżki krajowej dodawana jest krawędź od obecnego elementu listy do następnego. Pierwszy element listy ma 
 * zawsze @p node równy NULL.
 * @param[in] list  – lista opisana powyżej.
 * @return Wartość @p true, jeżeli aktualizacja powiodła się, lub @p false, jeżeli nie udało się zaalokować 
 * potrzebnej pamięci.
 */
bool UpdateRoutes(List list);

/** @brief Sprawdza czy wierzchołek @p node w mapie @p map został odwiedzony podczas ostatniego wywołania Dijsktry.
 * @param[in] map   – mapa zawierająca wierzchołek @p node;
 * @param[in] node  – sprawdzany wierzchołek;
 * @return Wartość @p true jeżeli wierzchołek @p node został odwiedzony lub wartość @p false w przeciwnym
 * przypadku.
 */
bool isVisited(Map *map, Node* node);

/** @brief Ustawia na ile optymalnych możliwości można dojść do danego wierzchołka.
 * Ustawia dijkstraData->nrOfCandidates struktury @p node w mapie @p map. Jeżeli istnieje 
 * dokładnie jeden sposób dojścia do @p node to ustawia wspomnianą wartość na 1, w przeciwynym 
 * wypadku ustawia wartość na inną liczbę.
 * @param[in] map   – mapa zawierająca wierzchołek @p node;
 * @param[in,out] node  – wierzchołek, którego dijkstraData jest aktualizowana;
 * @param[in] route     – numer drogi krajowej, przez który nie może przechodzić optymalna ścieżka.
 */
void getNrOfCandidates(Map *map, Node *node, int route);

#endif /* DIJKSTRA_H */