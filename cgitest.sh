#!/bin/sh

counter=1

while [ $counter -le 10 ]
#while [ $counter ]
do
	curl localhost:1024/hello --output -
	curl localhost:1024/hello/asfsadf --output -
	curl localhost:1024/hello/hello.cgi --output -
	curl localhost:1024/hello/hello.php --output -
	curl localhost:1024/hello/hello.py --output -
	curl localhost:1024/hello2 --output -
	curl localhost:1024/hello2/asdfsdf --output -
	curl localhost:1024/hello2/hello.cgi --output -
	curl localhost:1024/hello2/hello.php --output -
	curl localhost:1024/hello2/hello.py --output -
	curl localhost:1024/hello3 --output -
	curl localhost:1024/hello3/jsfdh --output -
	curl localhost:1024/hello3/hello.cgi --output -
	curl localhost:1024/hello3/hello.php --output -
	curl localhost:1024/hello3/hello.py --output -
	((counter++))
done;
