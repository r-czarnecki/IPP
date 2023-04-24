/** @file
 * Interfejs struktury przechowującej krawędzie grafu .
 */

#ifndef EDGE_H
#define EDGE_H

#include <stdbool.h>
#include "list.h"

/// @cond
typedef struct sNode Node;
/// @endcond

/** 
 * Struktura przechowująca krawędź grafu.
 */
typedef struct sEdge {
    List routes; ///< Lista numerów dróg krajowych, do których należy dana krawędź
    int year; ///< Rok budowy
    int length; ///< Długość drogi
    Node *nodeA; /**<  Wskaźnik na wierzchołki przy końcach krawędzi, przy czym zachodzi
                 @p nodeA < @p nodeB, gdzie nodeA i nodeB to adresy tych krawędzi */
    Node *nodeB; /**<  Wskaźnik na wierzchołki przy końcach krawędzi, przy czym zachodzi
                 @p nodeA < @p nodeB, gdzie nodeA i nodeB to adresy tych krawędzi */
} Edge;

/**
 * Struktura będąca listą krawędzi.
 */
typedef struct sEdgeListElement {
    Edge* edge; ///< Wskaźnik na krawędź.
    int i; ///< Zmienna pomocnicza typu int
    struct sEdgeListElement* next; ///< Wskaźnik na następny element listy.
    struct sEdgeListElement* prev; ///< Wskaźnik na poprzedni element listy.
    struct sEdgeListElement* last; ///< Wskaźnik na ostatni element listy.
} EdgeListElement;

/**
 * Typ będący wskaźnikiem na typ EdgeListElement.
 */
typedef EdgeListElement* EdgeList;

/** @brief Tworzy nową strukturę.
 * Tworzy nową krawędź, ustawia: routes na NULL; nodeA i nodeB na @p nodeA i @p nodeB.
 * @param[in] nodeA     – wierzchołek na jednym z końców krawędzi;
 * @param[in] nodeB     – wierzchołek na drugim końcu krawędzi;
 * @param[in] year      – rok budowy;
 * @param[in] length    – długość drogi;
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
Edge* newEdge(Node* nodeA, Node* nodeB, int year, int length);

/** @brief Usuwa krawędź.
 * Usuwa krawędź, na którą wskazuje @p edge.
 * @param[in] edge   – wskaźnik na krawędź przeznaczoną do usunięcia.
 */
void deleteEdge(Edge* edge);

/** @brief Tworzy nową strukturę.
 * Tworzy nowy element listy krawędzi, ustawia edge na @p edge, next, prev i last na NULL.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
EdgeListElement* newEdgeListElement(Edge* edge);

/** Usuwa listę krawędzi.
 * Usuwa listę krawędi @p list oraz każdą krawędź w niej przechowywaną.
 * @param[in] list  – lista przeznaczona do usunięcia.
 */
void deleteEdgeList(EdgeList list);

/** @brief Tworzy krawędź między wierzchołkami.
 * Tworzy krawędź między wierzchołkami @p node1 i @p node2 o długości @p lenght
 * i o roku wybudowania równym @p year.
 * @param[in] node1     – jeden z wierzchołków;
 * @param[in] node2     – drugi z wierzchołków;
 * @param[in] length    – długość dodawanej krawędzi;
 * @param[in] year      – rok wybudowania dodawanej krawędzi;
 * @return Wartość @p true, jeżeli krawędź została dodana.
 * Wartość @p false, jeżeli nie udało się zaalokować pamięci.
 */
bool addEdge(Node *node1, Node *node2, int length, int year);

/** @brief Dodaje krawędź do listy krawędzi.
 * Dodaje do listy krawędzi @p list krawędź @p edge.
 * @param[in,out] list  – wskaźnik na listę krawędzi, do której dodawana jest krawędź;
 * @param[in] edge      – dodawana krawędź;
 * @return Wartość @p true, jeżeli krawędź została dodana.
 * Wartość @p false, jeżeli nie udało się zaalokować pamięci.
 */
bool addEdgeToList(EdgeList* list, Edge *edge);

/** @brief Znajduje element listy przechowujący daną krawędź.
 * Znajduje wskaźnik na element listy @p list przechowujący krawędź pomiędzy @p nodeA oraz @p nodeB.
 * @param[in] list  – lista krawędzi, w której następuje przeszukiwanie;
 * @param[in] nodeA – wierzchołek na jednym końcu szukanej krawędzi;
 * @param[in] nodeB – wierzchołek na drugim końcu szukanej krawędzi;
 * @return Wskaźnik na element listy @p list przechowujący szukaną krawędź lub NULL, jeżeli taka
 * krawędź nie istnieje na liście.
 */
EdgeListElement* findElement(EdgeList list, Node* nodeA, Node* nodeB);

/** @brief Znajduje krawędź w liście.
 * Znajduje wskaźnik na krawędź pomiędzy @p nodeA oraz @p nodeB przechowywaną w @p list.
 * @param[in] list  – lista krawędzi, w której następuje przeszukiwanie;
 * @param[in] nodeA – wierzchołek na jednym końcu szukanej krawędzi;
 * @param[in] nodeB – wierzchołek na drugim końcu szukanej krawędzi;
 * @return Wskaźnik na krawędź przechowywaną w @p list lub NULL, jeżeli taka
 * krawędź nie istnieje na liście.
 */
Edge *findEdge(EdgeList list, Node* nodeA, Node* nodeB);

/** @brief Usuwa krawędź między wierzchołkami.
 * Usuwa krawędź między wierzchołkami @p node1 i @p node2
 * @param[in] node1     – jeden z wierzchołków;
 * @param[in] node2     – drugi z wierzchołków.
 */
void removeEdge(Node *node1, Node *node2);

/** @brief Usuwa listę krawędzi.
 * Usuwa listę krawędzi bez usuwania krawędzi w niej przechowywanych.
 * @param[in] list      – lista przeznaczona do usunięcia.
 */
void freeEdgeList(EdgeList list);

/** @brief Usuwa krawędź z listy.
 * Usuwa krawędź kończącą się wierzchołkami @p nodeA i @p nodeB z listy @p list.
 * @param[in,out] list  – lista, z której usuwamy krawędź;
 * @param[in] nodeA – wierzchołek na jednym końcu usuwanej krawędzi;
 * @param[in] nodeB – wierzchołek na drugim końcu usuwanej krawędzi.
 */
void removeEdgeFromList(EdgeList* list, Node* nodeA, Node* nodeB);

/** @brief Dodaje drogę krajową do listy dróg krajowych danej krawędzi. 
 * Dodaje drogę krajową nr @p route do listy dróg krajowych krawędzi, na którą wskazuje @p edge.
 * @param[in,out] edge  – aktualizowana krawędź;
 * @param[in] route     – numer dodawanej drogi krajowej;
 * @return Wartość @p true, jeżeli droga krajowa została dodana.
 * Wartość @p false, jeżeli nie udało się zaalokować pamięci.
 */
bool addRoute(Edge* edge, int route);

/** @brief Usuwa drogę krajową z listy dróg krajowych danej krawędzi.
 * Usuwa drogę krajową nr @p route z listy dróg krajowych krawędzi, na którą wskazuje @p edge, lub
 * nic nie robi, jeżeli taki numer drogi nie jest przechowywany w liście.
 * @param[in,out] edge  – aktualizowana krawędź;
 * @param[in] route     – numer usuwanej drogi krajowej;
 */
void removeRouteFromEdge(Edge* edge, int route);

#endif /* EDGE_H */