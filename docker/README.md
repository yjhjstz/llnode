## docker readme

### pull
```shell
docker login
docker pull registry.cn-hangzhou.aliyuncs.com/alinode/llnode
```

### build

```shell
cd docker
docker build --no-cache -t="llnode" .
```


### run
- mkdir /root/dir/core
- put a core.xxx.xxx into /root/dir/core
- docker run -it -d -p 2000:2000 -v /root/dir/core:/data llnode

### inspect 
docker exec -it {container id} bash

### vistor
http://ip:2000
