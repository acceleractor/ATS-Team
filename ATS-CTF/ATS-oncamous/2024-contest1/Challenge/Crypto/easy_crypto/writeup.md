# Writeup for easy_crypto
这是一个简单的凯撒加密，通过爆破，发现位移为4时为flag格式字符串。
## 最终flag
**flag{welcome_to_crypto_world}**

## 爆破脚本：

exp.py
```python
c = "jpek{aipgsqi_xs_gvctxs_asvph}"

def caiser(it, st):
    anw = ""
    for i in st:
        if ord(i) > ord('z') or ord(i) < ord('a'):
            anw += i
            continue
        tmp = ord(i) - it
        if tmp < ord('a'):
            anw += chr(tmp + 26)
        else:
            anw += chr(tmp)
    return anw

for i in range(1, 26):
    print(i, caiser(i, c))
```