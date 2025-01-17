#!/bin/sh

WATCHED="src/lib.cpp"
LAST=$(stat -c %Y "$WATCHED")
DELAY=0.02

FULL_DELAY=$(echo "4 * 0.01666666 + $DELAY + 0.1" | bc -l)

printf "Watching \033[4m$WATCHED\033[0m! \033[30mApproximate update delay: 0$FULL_DELAY seconds\033[0m\n"

while true; do
    sleep $DELAY
    MOD=$(stat -c %Y "$WATCHED")

    if [ "$MOD" -ne "$LAST" ]; then
        printf "\033[30mFile modified. Compiling...\033[0m"
        ./scripts/lib.sh
        printf "\033[32m ✓\033[0m\n"
        LAST=$MOD
    fi
done
