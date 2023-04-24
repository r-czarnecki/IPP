#include <stdlib.h>
#include "character.h"

int addChar(int c, Character *character) {
    if(character == NULL)
        return 0;
    Character *new = newChar();
    if (new == NULL)
        return 0;
    
    new->prev = character->last;
    character->last->c = c;
    character->last->next = new;
    character->last = new;
    return 1;
}

Character *newChar() {
    Character *new = malloc(sizeof(Character));
    if (new == NULL)
        return NULL;

    new->c = -1;
    new->next = NULL;
    new->prev = NULL;
    new->last = new;
    return new;
}

void deleteChar(Character *character) {
    while (character != NULL) {
        Character *pom = character->next;
        free(character);
        character = pom;
    }
}

Character *intToChar(int n) {
    Character *new = newChar();
    if(new == NULL)
        return NULL;
    
    if(n == 0) {
        int tmp = addChar('0', new);
        if(tmp == 0)
            return NULL;
    }
    bool negative = false;
    
    while(n != 0) {
        if(n < 0) {
            n *= -1;
            negative = true;
        }
        int digit = n % 10;
        n /= 10;
        Character *tmp = malloc(sizeof(Character));
        if(tmp == NULL) {
            deleteChar(new);
            return NULL;
        }
        tmp->c = digit + '0';
        tmp->next = new;
        tmp->prev = NULL;
        tmp->last = new->last;
        new = tmp;
    }
    if(negative) {
        Character *neg = newChar();
        int memory = addChar('-', neg);
        if(neg == NULL || memory == 0) {
            deleteChar(neg);
            deleteChar(new);
            return NULL;
        }
        appendChar(neg, new);
        return neg;
    }

    return new;
}

void appendChar(Character *char1, Character *char2) {
    if(char1 == NULL || char2 == NULL)
        return;
    if(char2->next != NULL)
        char2->next->prev = char1->last;
    char1->last->c = char2->c;
    char1->last->next = char2->next;
    char1->last = char2->last;
    free(char2);
}

int charLength(Character *character) {
    int length = 0;
    for(Character *ch = character; ch != NULL; ch = ch->next)
        length++;
    return length;
}

Character *getCopy(Character *character) {
    Character *result = newChar();
    Character *tmp = result;
    if(result == NULL)
        return NULL;

    for(Character *ch = character; ch->c != -1; ch = ch->next) {
        tmp->c = ch->c;
        tmp->next = newChar();
        if(tmp->next == NULL) {
            deleteChar(result);
            return NULL;
        }
        tmp->next->prev = result;
        result->last = tmp->next;
        tmp = tmp->next;
    }

    return result;
}

const char *toStr(Character *character) {
    char *word = malloc(charLength(character) * sizeof(Character));
    if(word == NULL)
        return NULL;
        
    int tmp = 0;
    for(Character *ch = character; ch != NULL; ch = ch->next) {
        if(ch->c == -1)
            word[tmp] = '\0';
        else
            word[tmp] = ch->c;
        tmp++;
    }
    return word;
}