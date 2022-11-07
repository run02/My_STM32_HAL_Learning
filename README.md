# RCC 时钟配置

本节主要是将之前默认的高速内部时钟换成板子上的RCC时钟. 基于第二个按键播放音乐的实验,仅做了更换时钟的处理. 通过播放出的音乐可以体验到两者时钟精度上的差别.

最后使用git查看RCC配置改变了代码的哪些地方.

## 使能RCC时钟

![image-20221107091650332](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221107091650332.png)

习惯性的把中断打开,说不定有用.

![image-20221107093216298](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221107093216298.png)

## 配置时钟树

![image-20221107091547995](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221107091547995.png)

### 时钟树图简介

![image-20221107091309591](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221107091309591.png)



### 配置完成后生成



## 接线

手头上这块板子用外部时钟的时候,需要给COM口供电. 这样看来外部晶振的电源来源于COM口.

来回切换插口比较麻烦,这里建议给板子连两条线.一条连COM口用于看串口打印,一条连SWD用于下载调试.

![image-20221107100803713](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221107100803713.png)





## 代码: 播放音乐体验RCC与HSI的精度差异

> RCC时钟更加精准,以实验2播放音乐为例,本来播放出来的音乐很慢,因为那个微秒级别的延时并不精准.
>
> 这里换成RCC提供时钟,放出来的音乐稍微好听了一些.

您以尝试切换到这个分支并下载代码,按K2播放音乐,体验HSI时钟和RCC时钟的区别.

您可以轻松使用Git来展示把时钟从HSI换成RCC修改了代码的哪些地方.

```shell
git diff lab2-tubeDisplay
```

您可以一直按回车键查看下一处不同,按q再回车退出.