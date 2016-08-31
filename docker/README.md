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
docker exec -it 84e03d1fce90 bash

### demo usage
- 生成 coredump, 记住 node.js version
- TODO 获取对应版本的 node.js binary (此 demo 是用 node-v4.2.6 生成）
- docker run
- 访问 http://ip:2000

