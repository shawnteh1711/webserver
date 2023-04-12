#!/bin/sh

counter=1

#while [ $counter -le 10 ]
while [ $counter ]
do
	curl localhost:1024/hello
	curl localhost:1024/hello/asfsadf
	curl localhost:1024/hello/hello.cgi
	curl localhost:1024/hello/hello.php
	curl localhost:1024/hello/hello.py
	curl localhost:1024/hello2
	curl localhost:1024/hello2/asdfsdf
	curl localhost:1024/hello2/hello.cgi
	curl localhost:1024/hello2/hello.php
	curl localhost:1024/hello2/hello.py
	curl localhost:1024/hello3
	curl localhost:1024/hello3/jsfdh
	curl localhost:1024/hello3/hello.cgi
	curl localhost:1024/hello3/hello.php
	curl localhost:1024/hello3/hello.py
	((counter++))
done;
