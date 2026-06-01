#!/bin/bash

agents=("random" "alphabeta" "mcts")

for file in *.py; do
    echo "Running $file..."
    
    for ((i=0; i<${#agents[@]}; i++)); do
        a1="${agents[$i]}"
        
        for ((j=i; j<${#agents[@]}; j++)); do
            a2="${agents[$j]}"
            
            echo "  -> $a1 vs $a2"
            python3 "$file" "$a1" "$a2"
        done
    done
done