# python2 wp

这是源代码

``` python
import random
import sys
evalFunc = eval
openFunc = open
#这里是一些黑名单 防止直接通过eval函数执行获取flag
blackFuncList = ['compile', 'eval', 'exec', 'open']
#在标准库中删除这些函数
for m in blackFuncList:
    del __builtins__.__dict__[m]
#这里是一个审计钩子函数 用来防止直接调用os.system等函数
def setAuditHook(event, _):
    blackEvents = set({'pty.spawn', 'os.system', 'os.exec', 'os.posix_spawn','os.spawn','subprocess.Popen'})
    if event in blackEvents:
        print('不能使用黑名单的函数: {}'.format(event))
        raise RuntimeError('不能使用黑名单的函数: {}'.format(event))
def guesser():
    gameScore = 0
    print('你能猜到这个数字吗? 在 1 到 9999999999999之间 > ')
    rightAnswer = random.randint(1, 9999999999999)
    inputData = evalFunc(input(''))
    if inputData == rightAnswer:
        gameScore += 1
    return gameScore
def main():
    print('猜数字游戏开始了！')
    gameScore = guesser()
    if gameScore == 1:
        print('哇 你真的猜对了')
        print(openFunc('flag').read())
    else:
        print('猜错了')

sys.addaudithook(setAuditHook)
main()
```

python 3.8将audit hook机制引入了sys模块：虽然机制引入的本意当然是给沙箱执行提供安全保障，但是单纯的audit hook也不能做到完全的安全。<br>
所以，在这个题目里面，我们绝对不能直接调用pty.spawn、os.system、os.exec、os.posix_spawn、os.spawn、subprocess.Popen这些直接RCE的函数。并且，我们也拿不到compile、eval、exec、open函数。<br>
但是，我们这里只需要正确得到随机数，就能得到flag了；而此处随机数的生成是采用random.randint来产生的。python的random模块使用梅森旋转法来生成随机数。由于程序未限制我们拿到random模块，所以我们可以先拿到random模块，再random.getstate()拿到随机数生成器的状态，再通过random.setstate()置随机数生成器状态为生成随机数之前的状态，最后random.randint生成一模一样的随机数。
这里可以打开python环境 <br>

``` python
import random
random.getstate()#获取到当前生成器的状态
>>> (3, (2147483648, 308560697, 2707160061, 2892231551, 1203077281, 496705666, 3352689338, 186298562, 1668728382, 2755178086, 1894974830, , 883374571, 1020022572, 3017421553, 772174260, 2787699012, 1494686473, 3771534924, 713614801, 2043562381, 2459288056, 522318326, 229661453, ..., 2196466854, 624), None)
#获取到这样一个元组 元组第一位3和最后一位None是不会变的 然后第二位是一个 624个32位整数 最后一个表示生成到了624个随机数
random.getrandbits(32)#生成一位随机数
random.getstate()#获取到当前生成器的状态 
>>> (3, (2411276621, 2410770473, 236721643, 4181272992, 702571343, 3050262225, 473098656, 2529570108, 227701176, 1258953209, 840257660, 201416638, 1132069705, 1478522551, 1950047014, 1318631913, 1915426248, 2501255582, 3663871423, 2006538022, 2669760750, 2339305302,  1405172338, 640385032, ..., 405370500, 1), None)
#这时候发现计数器变成了1  况且生成的随机数表也发生了变化(用梅森旋转法转过来了) 所以只要把state中的1改为0 就能确保下一个数字生成的和上一个数字一样了
```

所以这题分为四个步骤<br>

```python
import random
state=random.getstate()[1] #这里直接截取元组第二位 前面的3和后面的none是固定的
new_state=list(state[:624])+[0] #这里截取前面生成出来的包含随机数的元组的前624个32位随机数 不包括最后的 1 然后 再拼接上最后一位 0 这样就把state改成最开始生成随机数之前的样子了
random.setstate((3,tuple(new_state),None)) #设置新的state
random.randint(1,9999999999999)
```

但是，题目中只给了一行代码去实现四个操作<br>
python 3.8还引入了海象运算符:=：在表达式左侧应用海象运算符，可以将该表达式的值赋给某个变量。另外，我们还可以用一个list来装这些表达式，这样表达式的值就会从左至右依次计算，就像我们写程序一样一行一行地执行。再最后使用```[-1]```索引获取最后的返回值。
所以得到最后的payload

``` python
[random:=__import__('random'),state:=random.getstate()[1],new_state:=list(state[:624])+[0],rd:=random.setstate((3,tuple(new_state),None)),rand:=random.randint(1,9999999999999)][-1]
```


