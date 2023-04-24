#include "map.h"
#include "input.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#undef NDEBUG

#include <assert.h>

int main() {
    Map *map = newMap();
    int last = 0;

    while(true) {
        int q = getFunctionName(&last);
        if(!handleInput(map, q, &last))
            break;
    }

    deleteMap(map);
    return 0;
}
