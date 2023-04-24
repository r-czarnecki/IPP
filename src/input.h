/** @file
 * Funkcje związane z obsługą wejścia.
 */
#ifndef INPUT_H
#define INPUT_H

#include "map.h"
#include <stdbool.h>

/** @brief Wczytuje kolejny argument z wejścia.
 * Wczytuje kolejny argument aż do znaku ';', końca linii
 * lub końca pliku. Ustawia @p lastChar na ostatni wczytany znak.
 * @param[out] lastChar     – ostatni wczytany znak;
 * @return Wskaźnik na napis przechowujący wczytany argument lub NULL,
 * jeżeli nie udało się zaalokować pamięci.
 */
const char* readNextArg(int *lastChar);

/** @brief Tworzy liczbę z napisu.
 * Tworzy liczbę z napisu wskazywanego przez @p c. Ustawia @p error na wartość @p false,
 * jeżeli napis nie jest poprawną liczbą mieszczącą się w typie int, w przeciwnym wypadku
 * ustawia error na wartość @p true.
 * @param[in] c         – napis reprezentujący liczbę;
 * @param[out] error    – zmienna opisana powyżej;
 * @return Wartość liczby lub 0, jeżeli wczytany wyraz nie składa 
 * się z cyfr od 0 do 9 lub nie mieści się w zmiennej typu int.
 */
int charToInt(const char *c, bool *error);

/** @brief Określa, które polecenie jest wywoływane.
 * Wczytuje znaki do napotkania ';', końca linii lub końca pliku i określa,
 * które polecenie ma być wywołane. Ustawia @p lastChar na ostatni wczytany znak.
 * @return 0 - jeżeli nazwa nie jest poprawnym poleceniem, nie kończy się
 * znakiem ';', lub nie udało się zaalokować potrzebnej pamięci \n -1 - jeżeli wczytano polecenie addRoad
 * \n -2 - jeżeli wczytano polecenie repairRoad \n -3 - jeżeli wczytano polecenie getRouteDescription \n 
 * -4 - jeżeli wczytano polecenie newRoute \n -5 - jeżeli wczytano polecenie extendRoute \n
 * -6 - jeżeli wczytano polecenie removeRoad \n -7 - jeżeli wczytano polecenie removeRoute \n
 * -8 - jeżeli wczytano pustą linię, lub linię zaczynającą się znakiem '#' \n
 * liczbę całkowitą z przedziału [1,999] - jeżeli wczytano polecenie stworzenia drogi krajowej.
 */
int getFunctionName(int *lastChar);

/** @brief Wczytuje znaki z wejścia aż do napotkania znaku końca linii lub końca pliku.
 * Wczytuje znaki z wejścia aż do napotkania znaku końca linii lub końca pliku.
 * Ustawia @p lastChar na ostatni wczytany znak.
 * @param[in,out] lastChar     – ostatni wczytany znak.
 */
void readUntilEndOfLine(int *lastChar);

/** @brief Wczytuje nazwy dwóch miast.
 * Wczytuje nazwy dwóch miast oddzielone i kończące się znakiem ';'
 * @param[out] node1    – pierwsze z wczytanych miast;
 * @param[out] node2    – drugie z wczytanych miast;
 * @param[out] last     – ostatnia wczytana listera;
 * @return Wartość @p true, jeżeli udało się znaleźć oba miasta lub wartość
 * @p false, jeżeli nie są oddzielone znakiem ';', nie są poprawnymi nazwami miast 
 * lub nie udało się zaalokować potrzebnej pamięci
 */
bool readTwoCities(const char **node1, const char **node2, int *last);

/** @brief Sprawdza, czy napis jest poprawną nazwą miasta.
 * @param[in] city      – sprawdzany napis;
 * @return Wartość @p true jeżeli napis jest poprawną nazwą miasta lub
 * wartość @p false w przeciwnym przypadku.
 */
bool isGoodCityName(const char *city);

/** @brief Wykonuje wczytaną funkcję.
 * Wykonuję funkcję reprezentowaną przez @p fun (według opisu wyjścia funkcji getFunctionName)
 * @param[in,out] map       – mapa, w której wykonywane będą operacje;
 * @param[in] fun           – funkcja mająca się wykonać;
 * @param[in] lastChar      – ostatnia wczytana litera;
 * @return Zwraca wartość @p false, jeżeli funkcja doszła do końca pliku, zwraca wartość @p true
 * w przeciwnym przypadku.
 */
bool handleInput(Map *map, int fun, int *lastChar);

#endif /* INPUT_H */