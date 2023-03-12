#!/bin/bash

git status
git add .
read -p "Please enter commit massage: " commit_message
git commit -am "$commit_message"
git push
