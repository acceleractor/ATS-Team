# Writeup for nc_problem

nc真是中文缩写吗？（bushi

小皮一下, nc (netcat) ,是个计算机网络公用程序，用来对网络连线TCP或者UDP进行读写 (come from Wikipedia) 。在这道题中，我们需要打开题目所给的环境，得到一个带端口的ip地址，这时我们输入命令 ```nc ip port```即可完成靶机的连接，这时我们只需 ```cat flag``` 得到 flag 即可