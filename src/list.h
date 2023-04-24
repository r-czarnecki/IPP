/** @file 
 * Interfejs listy przechowującej elementy typu int i/lub Node* 
 */

#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

/// @cond
typedef struct sNode Node;
/// @endcond

/** @brief Struktura przechowująca element listy.
 * Element listy przechowujący zmienną typu int i wskaźnik do elementu typu Node.
 */
typedef struct sListElement {
    int integer; ///< Zmienna typu int
    Node *node; ///< Wskaźnik na typ Node
    struct sListElement *next; ///< Wskaźnik do następnego elementu listy
    struct sListElement *prev; ///< Wskaźnik do poprzedniego elementu listy
    struct sListElement *last; ///< Wskaźnik do ostatniego elementu listy
} ListElement;

/**
 * Typ będący wskaźnikiem na typ ListElement.
 */
typedef ListElement* List;

/** @brief Tworzy nową strukturę.
 * Tworzy nową strukturę i ustawia wartości: integer na @p intArg; node na @p nodeArg, next i prev na NULL
 * oraz last na samą siebie.
 * @param[in] intArg    – wartość, którą ma przyjąć integer
 * @param[in] nodeArg   – wskaźnik na strukturę typu node, który ma przyjąć nodeArg
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci. 
 */
ListElement* newListElement(int intArg, Node *nodeArg);

/** @brief Usuwa element listy.
 * Usuwa element listy @p list, na który wskazuje @p element oraz łączy sąsiadujące z nim elementy.
 * @param[in] list     – lista, z której usuwany jest element;
 * @param[in] element   – wskaźnik na element listy przeznaczony do usunięcia.
 */
void removeListElement(List *list, ListElement *element); //Argument może być NULL

/** @brief Usuwa całą listę.
 * Usuwa listę zaczynając od elementu wskazywanego przez @p list.
 * @param[in] list      – lista przeznaczona do usunięcia.
 */
void deleteList(List list);

/** @brief Dodaje element na początek listy.
 * Tworzy element o wartości integer równej @p intArg oraz o node równym @p nodeArg,
 * a następnie dodaje go na początek listy.
 * @param[in,out] list  – lista, do której dodajemy element;
 * @param[in] intArg   – wartość integer nowego elementu;
 * @param[in] nodeArg      – wskaźnik node nowego elementu;
 * @return Wartość @p true, jeżeli element został dodany.
 * Wartość @p false, jeżeli nie udało się zaalokować pamięci.
 */
bool addFrontElement(List* list, int intArg, Node *nodeArg);

/** @brief Dodaje element na koniec listy.
 * Tworzy element o wartości integer równej @p intArg oraz o node równym @p nodeArg,
 * a następnie dodaje go na koniec listy.
 * @param[in,out] list  – lista, do której dodajemy element
 * @param[in] intArg   – wartość integer nowego elementu;
 * @param[in] nodeArg      – wskaźnik node nowego elementu;
 * @return Wartość @p true, jeżeli element został dodany.
 * Wartość @p false, jeżeli nie udało się zaalokować pamięci.
 */
bool addBackElement(List* list, int intArg, Node *nodeArg);

/** @brief Łączy dwie listy.
 * Dodaje listę @p listB na koniec listy @p listA.
 * @param[in,out] listA – lista, do której dołączamy drugą listę;
 * @param[in] listB     – lista, którą dołączamy do pierwszej listy.
 */
void mergeLists(List* listA, List listB);

/** @brief Znajduje pierwszy element listy o danej wartości zmiennej int.
 * Znajduje pierwszy element listy @p list przechowujący zmienną typu int o wartości @p find.
 * @param[in] list      – lista, w kórej będzie odbywać się przeszukiwanie;
 * @param[in] find      – wartość int, która będzie szukana;
 * @return Wskaźnik na pierwszy element listy przechowujący wartość @p find lub NULL, gdy taki
 * element nie istnieje.
 */
ListElement* findInt(List list, int find);

#endif /* LIST_H */
