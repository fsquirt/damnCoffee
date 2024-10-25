# damnCoffee

好不容易拿了个shell发现有杀软啥工具也运行不了，那还渗透个damn啊，下班饮coffee啦

damnCoffee，一款简单的后渗透免杀加载器，轻松Bypass AV/EDR，已测360，其余杀软自测

[![pAwgNLt.png](https://s21.ax1x.com/2024/10/25/pAwgNLt.png)](https://imgse.com/i/pAwgNLt)

## 使用方法

在[Releases](https://github.com/fsquirt/damnCoffee/releases)中下载damnCoffee本体和damnHelper，准备一张正常的png图片，启动damnHelper。文本和文件两种方式向png文件插入shellcode，将文件拖拽至文本框可快速输入文件路径，插入后将含有shellcode的png图片与damnCoffee上传至同一位置，将png文件名修改为`j.png`，然后enjoy it~

**注意:必须是正常的png图片**，damnCoffee会根据png的结尾寻找shellcode的开头

[![pAwgtsI.png](https://s21.ax1x.com/2024/10/25/pAwgtsI.png)](https://imgse.com/i/pAwgtsI)

## 注意事项

Releases中的damnCoffee为64位，如需加载32位shellcode自行修改137行修改线程上下文处代码并重新编译

此加载器开源，如果报毒了那纯属**非常非常正常**的现象。如果遇到bug和报毒的情况请截屏提交issue
