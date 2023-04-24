#!/bin/bash

declare -A lengths

if [[ -z "$1" ]] || [[ -z "$2" ]]
then
    exit 1
fi

if ! [[ -f "$1" ]]
then
    exit 1
fi

for arg in "${@:2}" 
do
    if ! [[ "$arg" =~ ^[0-9]+$ ]]
    then
        exit 1
    fi
done

while read p; do
    ROUTE=$p
    OK=1
    NR=0
    LENGTH=0
    NUM=0
    ARGS=0
    for (( i=0; i<${#ROUTE}; i++ )) do
        CHAR=${ROUTE:$i:1}
        if [[ $CHAR =~ [0-9] ]]
        then
            let "NUM *= 10"
            let "NUM += CHAR"
        elif [[ "$CHAR" = ';' ]]
        then
            let "ARGS += 1"
            if [[ $ARGS -eq 1 ]]
            then
                NR=$NUM
                EXIST=0
                for arg in "${@:2}"
                do
                    if [[ $NR -eq $arg ]]
                    then
                        EXIST=1
                    fi
                done

                if [[ $EXIST -eq 0 ]]
                then
                    OK=0
                    break
                fi
            elif [[ $(($ARGS%3)) -eq 0 ]]
            then
                let "LENGTH += NUM"
            fi
            NUM=0
        fi 
    done

    if [[ $OK -eq 1 ]]
    then
        lengths[$NR]=$LENGTH
    fi
done < "$1"

for arg in "${@:2}" 
do
    if [[ -v "lengths[$arg]" ]]
    then
        echo "$arg;${lengths[$arg]}"
    fi
done


exit 0


