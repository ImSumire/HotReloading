#!/bin/sh

WATCHED="src/lib.cpp"
LAST=$(stat -c %Y "$WATCHED")

while true; do
    sleep 1
    MOD=$(stat -c %Y "$WATCHED")

    if [ "$MOD" -ne "$LAST" ]; then
        printf "\033[30mFile modified. Compiling...\033[0m"
        ./scripts/lib.sh
        printf "\033[32m ✓\033[0m\n"
        LAST=$MOD
    fi
done
