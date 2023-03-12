#!/bin/bash

if [ -z "$(git status --porcelain)" ]; then
    echo "No changes to commit."
    exit 0;
else
    git status
    git add .
    read -p "Please enter commit massage: " commit_message
    if  [[ -z $commit_message ]]; then
        echo "Commit message is empty, exiting."
        exit 1
    fi
    git commit -am "$commit_message"
    git push
fi
