# Crypto-rsa_d

 连接靶机，得到如下信息
```shell
p=14785247
q=16612481
e=65537
d=?
d=
```
即我们已知 `p, q, e` , 需要我们求解d。

payload 如下：

```python
import gmpy2
# input your own number
p=14785247
q=16612481
e=65537
#n = p * q

# solve part
phi_n = (p - 1) * (q - 1)
d =gmpy2.invert(e, phi_n)
print(d)
```

得到 `d` 的值与 nc 交互即得 flag