# Writeup for easy_rsa
题目中已知 $p$, $q$, $e$, $c$, $p$ 和 $q$ 为两个大质数，根据此我们可以计算出$n = p * q$ 和 $phi(n) = (p - 1) * (q - 1)$, 其中 $phi()$ 为欧拉函数。然后我们根据 $phi(n)$ 和公钥 $e$ 计算出私钥 $d$，其中 $d$ 为 $e\ (mod\ n)$ 的逆元，我们可以使用扩展欧几里得算法计算; 最终根据解密公式 $m(明文) = c^d\ mod\ n$ 解出最后明文。当然，作为一道赛题来说的话，我们可以直接google到相关网址帮忙计算，我这里将计算代码贴出，供大家参考（代码很烂，各位大佬轻喷）。 
## 最终flag 
**flag{crypto_is_just_a_math_problem}**

## 计算脚本

```python
from Crypto.Util.number import *

p = 157173933678865829796588771859730626751979694456452522968152651069129392799258217265185848448685585775544813903532906730614939030283007417606598779500056952754181261190054483026447877747920219900804090686875343878530050195569754923626003932685350379021185849905416227380846266989977230446347850752132780995851
q = 118137767140663032316717597565829355889298910256704336671094654247806607566526109499776038326918920524704233758475838430215153530040405269217410220826570758329054694465812191453918843350294115299488478793133177286138570853777934505763392200709694896022800642350666804081284587102146286116419267715475787118431
e = 65537
c = 1244139621221798336011360777421383618270693953475688231676631467752812850168611669256137287976524541754051790468466155884867478079178451985211020451119319459880266560291961483221117813697171271969713905268051600082796733118818786832199939743042295618497558011083468549923528995469347146484713544146105249963392344122452750729993525077680496331781965676800289439912777970469174249411291922332016851536533041294776774291868620601249388837363759409881738311233126615418586444913637689452364713121292232437577065849021283077035346442966749362596589942604455352794718895095326303151696878518630642608238234148501381581264

n = p * q
phi_n = (p - 1) * (q - 1)

d = inverse(e, phi_n)

m = pow(c, d, n)
print(f"m = {long_to_bytes(m)}")
