#!/bin/bash

while true; do
    echo -n "Press 'r' to reload: "
    read -n 1 key
    echo

    if [[ $key == "r" ]]; then
        clear
        echo "Running make && ./atm..."
        make && ./atm
        echo "Process finished. Waiting for 'r' to reload."
    fi
done
