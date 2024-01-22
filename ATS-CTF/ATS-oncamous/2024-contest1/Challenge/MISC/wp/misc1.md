## MISC1
这道题我把压缩包伪加密，压缩包爆破，base64隐写，0宽字符隐写合在了一起，综合难度比较高。  
### Step1
提示说这个压缩包好像加密了又好像没有加密，一般来说压缩包有密码应该先看看是不是伪加密  
使用010editor打开  
发现0x504B0304这是压缩包的头文件  
然后0x1400这是压缩包的解压所需的版本  
然后0x0000这里是全局方位标记表示有无加密  
如果是0000表示没有加密 如果是0900就是有加密  
这里是0000说明压缩包没有加密 所以我们可以认为这个压缩包是伪加密的  
然后看到末尾0x504B0102这里这是压缩包的压缩源文件目录区 这里 然后是1400 1400 这是压缩和解压的所需的版本  
之后是0900这就是和上面对应得全局方式位标记 0900表示加密0000表示未加密 和之前是不一样的 所以把09改成00就可以解压了  
[具体可以看这篇文章](https://www.cnblogs.com/0yst3r-2046/p/11890498.html)  
### Step2
提示说有一个八位数字密码 直接爆破  
密码是11451400  
### Step3
提示说放到vscode里可以看到看不见的东西  
用vscode或者vim notepad++打开他发现有一串特殊的字符  
考虑是0宽字符隐写(我这里用了一个比较常见的网站进行0宽字符隐写 因为一般情况下对应法则不一样 我就选择了一个常用的网站 其他网站应该也能解)  
[网站](https://yuanfux.github.io/zero-width-web/)  
得到一半的flag   _1s_funny}
### Step4
一般看到一些字母数字符号组成的字符串 且末尾用=结尾的 是base64编码的字符  
[这里贴一个base64的介绍](https://www.jyshare.com/front-end/693/)  
[工具](https://gchq.github.io/CyberChef/)  
这里用了两次的base64编码 所以解码两次 ~~(由于隐写的缘故一些比较短的字符比如a变成Ja导致出现了一些乱码 我的失误很抱歉)~~  
解码之后的后半段发现是please try Base64 steganography考虑是base64隐写  
具体的base64隐写可以看这篇文章[里面有介绍和代码](https://cloud.tencent.com/developer/article/2090375)  
这边贴一个我自己的脚本  
``` python
import base64

table = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/'
file = open("flag.txt")
flag=''
tmpbin=''

for line in file.readlines():
    line=line.strip('\n')
    if(line[-1]=='='):
        if(line[-2]=='='):
            i=table.index(line[-3])
            b=bin(i)[2:]
            b=b.zfill(6)
            print(line)
            print(b)
            print(b[-4:]+'\n')

            tmpbin+=b[-4:]
        else:
            i = table.index(line[-2])
            b = bin(i)[2:]
            b = b.zfill(6)
            print(line)
            print(b)
            print(b[-2:]+'\n')

            tmpbin+=b[-2:]

length= len(tmpbin)/8
for i in range(int(length)):
    flag+=chr(int(tmpbin[i*8:i*8+8],2))

print(flag)

```
直接得到前半flag  flag{M1sc

拼接一下 flag{M1sc_1s_funny}