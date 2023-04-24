#include "input.h"
#include "node.h"
#include "character.h"
#include <stdio.h>
#include <stdlib.h>

#define ABS(a) ((a < 0) ? -(a) : (a))

static const char QUERIES[7][20] = {"addRoad", "repairRoad", "getRouteDescription", "newRoute", \
                                    "extendRoute", "removeRoad", "removeRoute"};
static int nrOfQuery = 1;

const char* readNextArg(int *lastChar) {
    char *str = malloc(2*sizeof(char));
    int length = 1, pos = 0;
    int readChar = getchar();
    *lastChar = readChar;
    if(str == NULL)
        return NULL;
    else    
        str[0] = 0;

    while(readChar != '\n' && readChar != ';' && readChar != EOF) {
        if(readChar == 0)
            readChar = 1;
        if(pos == length) {
            bool ok = true;
            char *tmp = realloc(str, (2*length + 1)*sizeof(char));
            if(tmp != NULL)
                str = tmp;
            length *= 2;

            if(!ok) {
                *lastChar = 0;
                free(str);
                return NULL;
            }
        }

        str[pos] = readChar;
        str[pos + 1] = 0;
        pos++;
        readChar = getchar();
        *lastChar = readChar;
    }

    return str;
}

int charToInt(const char *c, bool *error) {
    *error = true;
    if(c == NULL) {
        *error = false;
        return 0;
    }
        
    int readChar = *c;
    int result = 0;
    int limit = 2147483647;
    int length = 0;
    bool negative = false;
    bool firstDigit = true;
    if(*c == 0 || (*c == '-' && *(c + 1) == 0)) {
        *error = false;
        return 0;
    }

    if(readChar == '-') {
        negative = true;
        limit = -2147483648;
        c++;
        readChar = *c;
    }

    while(readChar != 0) {
        if(readChar < '0' || readChar > '9') {
            *error = false;
            return 0;
        }
        if(firstDigit && readChar == '0') {
            c++;
            readChar = *c;
            continue;
        }

        firstDigit = false;
        length++;
        readChar -= '0';
        if(length > 10) {
            *error = false;
            return 0;
        }
        else if(length == 10) {
            if(!negative && result > limit / 10) {
                *error = false;
                return 0;
            }
            else if(negative && result < limit / 10) {
                *error = false;
                return 0;
            }
            else if(result == limit / 10 && readChar > ABS(limit % 10)) {
                *error = false; 
                return 0;
            }
        }
        result *= 10;
        result += (negative) ? -1 * readChar : readChar;
        c++;
        readChar = *c;
    }

    return result;
}

int getFunctionName(int *lastChar) {
    int last = 0;
    const char* str = readNextArg(&last);
    bool error;
    int num = charToInt(str, &error);
    int result = 0;
    *lastChar = last;

    if(str == NULL)
        return 0;
    
    for(int i = 0; i < 7; i++) {
        bool equal = true;
        const char *tmp = str;
        for(int j = 0; j < 20; j++) {
            if(QUERIES[i][j] == 0 && *tmp == 0)
                break;
            
            if(QUERIES[i][j] != *tmp) {
                equal = false;
                break;
            }

            tmp++;
        }

        if(equal && last == ';') {
            free((void*)str);
            return -(i + 1);
        }
    }

    if(*str == '#' || (*str == 0 && (last == '\n' || last == EOF)))
        result = -8;
    else if(last != ';')
        result = 0;
    else if(error && num > 0)
        result = num;
    else 
        result = 0;

    free((void*)str);
    return result;
}

void readUntilEndOfLine(int *lastChar) {
    while(*lastChar != '\n' && *lastChar != EOF)
        *lastChar = getchar();
}

bool readTwoCities(const char **node1, const char **node2, int *last) {
    *node2 = NULL;
    *node1 = readNextArg(last);
    if(node1 == NULL || !isGoodCityName(*node1) || *last != ';')
        return false;

    *node2 = readNextArg(last);
    if(node2 == NULL || !isGoodCityName(*node2))
        return false;
    
    return true;
}

bool isGoodCityName(const char *city) {
    if(*city == 0)
        return false;

    while(*city != 0) {
        if(*city == ';' || (*city > 0 && *city <= 31)) 
            return false;
        city++;
    }
    return true;
}

bool handleInput(Map *map, int fun, int *lastChar) {
    int last = *lastChar;
    bool ok = true;
    
    if(fun == 0)
        ok = false;
    else if(fun == -1) {
        const char *node1, *node2;
        int length, year;
        bool error;
        ok = readTwoCities(&node1, &node2, &last);
        if(ok && last == ';') {
            const char *num = readNextArg(&last);
            length = charToInt(num, &error);
            free((void*)num);
            if(last == ';' && length > 0 && error) {
                num = readNextArg(&last);
                year = charToInt(num, &error);
                free((void*)num);
                if(last == '\n' && year != 0 && error)
                    ok = addRoad(map, node1, node2, length, year);
                else
                    ok = false;
            }
            else
                ok = false;
        }
        else
            ok = false;
        free((void*)node1);
        free((void*)node2);
    }
    else if(fun == -2) {
        const char *node1, *node2;
        int year;
        bool error;
        ok = readTwoCities(&node1, &node2, &last);
        if(ok && last == ';') {
            const char *num = readNextArg(&last);
            year = charToInt(num, &error);
            free((void*)num);
            if(last == '\n' && year != 0 && error)
                ok = repairRoad(map, node1, node2, year);
            else 
                ok = false;
        }
        else
            ok = false;

        free((void*)node1);
        free((void*)node2);
    }
    else if(fun == -3) {
        const char *str = readNextArg(&last);
        bool error;
        int route = charToInt(str, &error);
        free((void*)str);
        if(route < 0 || !error)
            ok = false;
        else {
            str = NULL;
            if(last == '\n')
                str = getRouteDescription(map, route);
            if(str != NULL)
                printf("%s\n", str);
            else 
                ok = false;

            free((void*)str);
        }
    }
    else if(fun == -4) {
        const char *str = readNextArg(&last);
        bool error;
        int route = charToInt(str, &error);
        free((void*)str);
        if(route < 0 || !error || last != ';')
            ok = false;
        else {
            const char *str2;
            error = readTwoCities(&str, &str2, &last);
            if(error && last == '\n')
                ok = newRoute(map, route, str, str2);
            else 
                ok = false;

            free((void*)str);
            free((void*)str2);
        }
    }
    else if(fun == -5) {
        const char *str = readNextArg(&last);
        bool error;
        int route = charToInt(str, &error);
        free((void*)str);
        if(route < 0 || !error || last != ';')
            ok = false;
        else {
            str = readNextArg(&last);
            if(str == NULL || last != '\n' || !isGoodCityName(str))
                ok = false;
            else
                ok = extendRoute(map, route, str);
            free((void*)str);
        }
    }
    else if(fun == -6) {
        const char *city1, *city2;
        bool error = readTwoCities(&city1, &city2, &last);
        if(!error || last != '\n')
            ok = false;
        else
            ok = removeRoad(map, city1, city2);

        free((void*)city1);
        free((void*)city2);
    }
    else if(fun == -7) {
        const char *str = readNextArg(&last);
        bool error;
        int route = charToInt(str, &error);
        free((void*)str);
        if(route < 0 || !error)
            ok = false;
        else 
            ok = removeRoute(map, route);
    }
    else if(fun == -8)
        ok = true;
    else {
        if(fun < 0)
            ok = false;
        else 
            ok = createRoute(map, fun, &last);
        if(last != '\n')
            ok = false;
    }

    readUntilEndOfLine(&last);
    if(!ok)
        fprintf(stderr, "ERROR %d\n", nrOfQuery);
    if(last == EOF)
        return false;
    nrOfQuery++;

    return true;
}