# Pwn-nc_shell
签到题，连接题目靶机，得到如下提示：

```text
Do you know netcat and shell?
try execute command 'ls'
```

输入`ls`后，发现存在`flag`文件，于是使用指令：

```shell
cat flag
```

得到flag。