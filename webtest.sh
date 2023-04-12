#!bin/sh

#404
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
echo -e "\\n\\n400 ERROR\\n"
curl -X asdf localhost:1024
echo -e "\\n" 
curl -X get	 localhost:8080
echo -e "\\n" 
curl -X GET! localhost:1024/home
echo -e "\\n" 
curl -X POSTxx localhost:1024
echo -e "\\n" 
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
