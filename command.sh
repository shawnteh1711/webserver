#!bin/sh
while true; do
    echo "Enter command to execute:"
    read command

    if [ "$command" == "exit" ]; then
        break
    elif [ "$command" == "404" ]; then
        echo -e "404 ERROR\\n"
        curl localhost:1024/wfd
        echo -e "\\n" 
        curl localhost:7777/wfd
        echo -e "\\n" 
        curl localhost:8080/wfd
        echo -e "\\n" 
        curl localhost:3333/wfd
        echo -e "\\n" 
        curl localhost:1024/home/sadf34fd
        echo -e "\\n" 
    elif [ "$command" == "400" ]; then
        echo -e "\\n\\n400 ERROR\\n"
        curl -X asdf localhost:1024
        echo -e "\\n" 
        curl -X get	 localhost:8080
        echo -e "\\n" 
        curl -X GET! localhost:1024/home
        echo -e "\\n" 
        curl -X POSTxx localhost:1024
        echo -e "\\n" 
    elif [ "$command" == "405" ]; then
        echo -e "\\n\\n405 ERROR\\n"
        curl -X GET localhost:1024/upload
        echo -e "\\n" 
        curl -X DELETE localhost:1024/home/upload/asdf
        echo -e "\\n" 
        curl -X GET localhost:8080/test/asdfsdf
        echo -e "\\n" 
        curl -X GET localhost:8080/test
        echo -e "\\n" 
        curl -X GET localhost:8080/test2/
        echo -e "\\n" 
        curl -X DELETE localhost:8080/test2/zxvc
        echo -e "\\n" 
        curl -X DELETE localhost:8080/example/zxvc
        echo -e "\\n" 
        curl -X DELETE localhost:1024/upload/lol.txt
        echo -e "\\n"
    elif [ "$command" == "500" ]; then
        echo -e "\\n\\n500 ERROR\\n"
        curl -X POST localhost:8080/test
        echo -e "\\n" 
        curl -X POST localhost:8080/test/index.html
        echo -e "\\n" 
        curl -X POST localhost:8080/test/
        echo -e "\\n" 
        curl -X POST localhost:1024/autoindex
        echo -e "\\n" 
        curl -X DELETE localhost:1024/autoindex
        echo -e "\\n" 
        curl -X DELETE localhost:1024/autoindex/asdf
        echo -e "\\n"
    elif [ "$command" == "get" ]; then
        curl -X GET localhost:1024/home 
    elif [ "$command" == "post" ]; then
        curl -X POST -H "Content-Type: plain/text" --data "BODY IS HERE write something shorter or longer than body limit" http://localhost:1024/home --output -
    elif [ "$command" == "upload" ]; then
        curl -d "file=/Users/steh/Documents/own_folder/webserver/test.txt" localhost:1024/upload
    elif [ "$command" == "delete" ] ; then
        curl -X DELETE localhost:1024/test.txt
    elif [ "$command" == "limit" ]; then
        curl -X POST -H "Content-Type: plain/text" --data "BODY IS HERE write something shorter or longer than body limit" http://localhost:3333 --output -
    elif [ "$command" == "cgi get" ]; then
        curl -X GET localhost:1024/hello --output -
    elif [ "$command" == "cgi post" ]; then
        curl -X POST http://localhost:1024/hello\?name=shawn --output -
    elif [ "$command" == "siege" ] ; then
        ./siege/bin/siege -b http://localhost:1024/empty
    else
        echo "Invalid command."
    fi
done
