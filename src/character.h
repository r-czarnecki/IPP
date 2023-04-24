/** @file
 * Interfejs klasy przechowującej listę znaków .
 */

#ifndef CHARACTER_H
#define CHARACTER_H

#include <stdbool.h>

/** @brief Lista przechowująca kolejne znaki.
 * Struktura przechowująca znak oraz wskaźniki do elementu następnego, poprzedniego
 * i ostatniego elementu listy. Każda lista kończy się wartownikiem przechowującym 
 * znak -1.  
 */
typedef struct sCharacter {
    int c; ///< Przechowywany znak.
    struct sCharacter *next; ///< Wskaźnik na następny element listy.
    struct sCharacter *prev; ///< Wskaźnik na poprzedni element listy.
    struct sCharacter *last; ///< Wskaźnik na ostatni element listy.
} Character;

/** @brief Dodaje znak do struktury.
 * Dodaje @p c jako ostatni znak listy rozpoczynającej się elementem, na który 
 * wskazuje @p character.
 * @param[in] c             – znak, który ma zostać dodany;
 * @param[in,out] character – wkaźnik na strukturę, do której ma zostać dodany znak;
 * @return Wartość @p 1, jeżeli znak został dodany.
 * Wartość @p 0, jeżeli nie udało się zaalokować pamięci.
 */
int addChar(int c, Character *character);

/** @brief Tworzy nową strukturę.
 * Tworzy strukturę przechowującą znak -1.
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */ 
Character *newChar();

/** @brief Usuwa strukturę.
 * Usuwa strukturę wskazywaną przez @p character
 * @param[in] character     – wskaźnik na usuwaną strukturę.
 */
void deleteChar(Character *character);

/** @brief Tworzy strukturę Character przechowującą int
 * Tworzy strukturę Character przechowującą liczbę @p n
 * @param[in] n     – liczba mająca być przechowywana w strukturze;
 * @return Wskaźnik na utworzoną strukturę lub NULL, gdy nie udało się
 * zaalokować pamięci.
 */
Character *intToChar(int n);

/** @brief Łączy jedną strukturę do drugiej.
 * Dodaje strukturę @p char2 na koniec @p char1.
 * @param[in,out] char1     – struktura, do której dołączamy drugą strukturę;
 * @param[in] char2         – dołączana struktura;
 */
void appendChar(Character *char1, Character *char2);

/** @brief Zwraca ilość znaków przechowywanych w strukturze @p character.
 * @param[in] character     – wskaźnik na rozpatrywaną strukturę
 * @return Ilość znaków przechowywanych w strukturze @p character.
 */
int charLength(Character *character);

/** @brief Zamienia Character na C string.
 * Zamienia strukturę wskazywaną przez @p character na C string
 * @param[in] character     – zamieniana struktura;
 * @return Wskaźnik na utworzony C string lub NULL, jeżeli nie udało się
 * zaalokować pamięci.
 */
const char *toStr(Character *character);

/** @brief Zwraca kopię struktury.
 * Zwraca kopię struktury o początku wskazywanym przez @p character.
 * @param[in] character     – wskaźnik na kopiowaną strukturę;
 * @return Wskaźnik na utworzoną kopię lub NULL, jeżeli nie udało się
 * zaalokować pamięci.
 */
Character *getCopy(Character *character);

#endif