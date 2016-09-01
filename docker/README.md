## Docker llnode readme

### Pull image
```shell
docker login
docker pull registry.cn-hangzhou.aliyuncs.com/alinode/llnode
```

### Build

```shell
cd docker
docker build --no-cache -t="llnode" .
```

### Run
- mkdir /root/dir/core
- put a core.xxx.xxx into /root/dir/core
- put nodejs binary into /root/dir/binary
- docker run -it -d -p 2000:2000 -v /root/dir/core:/data llnode

### Docker Inspect 
docker exec -it {container_id} bash

### Demo Usage
- 生成 coredump, 记住 node.js version
- TODO 获取对应版本的 node.js binary (此 demo 是用 node-v4.2.6 生成）
- docker run
- 访问 http://ip:2000

### Support Commands API
```shell
parallels@ubuntu:/media/psf/Home/Work/llnode-api$ node
> const llnode_module = require('../build/Release/llnode_module');
undefined
> llnode_module
{ loadDump: [Function: loadDump],
  getThreadCount: [Function: getThreadCount],
  getFrameCount: [Function: getFrameCount],
  getFrame: [Function: getFrame],
  jsstack: [Function: jsstack],
  nodeinfo: [Function: nodeinfo],
  findjsobjects: [Function: findjsobjects],
  jsprint: [Function: jsprint],
  findjsinstances: [Function: findjsinstances] }
> 
```
