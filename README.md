# 实验5 IIC总线读写EEPROM

这次实验主要是学习使用IIC总线,用IIC协议向EEPROM中写一些数据,然后读出来数据,并且串口打印.

如果是用固件库的话,因为ST官方当时的IIC驱动给的有bug,要自己用软件实现IIC,从头手写一遍的话非常麻烦.

但是,HAL库不存在这个问题,而且上层接口特别完善,如果之前有过使用IIC的经历,仅需几分钟就可以做完这个实验.

## 实验目的

1.掌握I2C总线的工作原理；

2.掌握利用STM32单片机GPIO模拟I2C总线的程序设计方法；

3.掌握单片机读写AT24C02 EEPROM的程序设计方法。

## 实验内容

利用STM32F103VBT6单片机读写AT24C02 EEPROM。

具体要求：

首先STM32F103VBT6单片机向AT24C02 EEPROM一次性连续写入若干个数，

然后STM32F103VBT6单片机再从AT24C02 EEPROM中将之前面写入的全部数读出来，并通过串口输出到PC机上。

## 原理

### 一段潦草但是易懂的话描述IIC

所谓IIC其实和UART串口差不多,也是两根线,但是IIC的两根线有一根是信号线,所以说数据只能从一根线上发送.这样的话两个设备共用一根线,就像是两个人用对讲机的一个频道:同一时间只能一个人说话. 为保证通信的质量,在刚开始传输的时候, 需要从设备应答一下表示收到了,不然发了一串发现对牛弹琴,就很亏.

然后如果有一串数据,就可以传输给从机了.

发完了之后发送一串结束信息告诉从机表示发送结束.

有的从机比如说AT24C02,专门做存储用的,一下子可能会写很多数据.数据就存在从机连续的的寄存器里.

![image-20221107161111350](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221107161111350.png)



### 原理图

![image-20221107161258290](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221107161258290.png)



![image-20221107161322475](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221107161322475.png)



![image-20221107161422483](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221107161422483.png)



## 代码

自己写的代码加起来也就这么点,HAL库封装的太好了,向IIC读写内容直接一句话搞定.

- 写

  ```c
  HAL_I2C_Mem_Write(&hi2c1,//iic的结构体,主要是需要知道哪个引脚,还有频率
                    ADDR_EEPROM_Write,//从设备地址
                    0,//从设备寄存器地址
                    I2C_MEMADD_SIZE_8BIT,//从设备寄存器的宽度(一般是8或16位)
                    i2c_tx_buffer,//要写的数组
                    sizeof i2c_tx_buffer,//数组长度
                    0xff//超时时间,如果超时了没发完就不发了
                   );
  ```

  

- 读

  ```c
     HAL_I2C_Mem_Read(&hi2c1, //同上
     					ADDR_EEPROM_Read,//IIC设备地址
                      0, //从设备内存地址
                      I2C_MEMADD_SIZE_8BIT,//从设备寄存器宽度
                      i2c_rx_buffer,//接收数据的内存数组
                      sizeof i2c_rx_buffer, //接收多少个
                      0xff //超时hi见,超时了就不读了
                      );
  ```



```c
#include "string.h"
#define ADDR_EEPROM_Write 0xA0
#define ADDR_EEPROM_Read 0xA1
uint8_t i2c_tx_buffer[]={'h','e','l','l','o','\0'};
uint8_t i2c_rx_buffer[8];
int main(void)
{
    HAL_UART_Transmit(&huart1,"write i2c: hello",sizeof "write i2c: hello",0xff);
      /*HAL库一句话搞定写内存*/
    HAL_I2C_Mem_Write(&hi2c1,ADDR_EEPROM_Write,
                                     0,
                                     I2C_MEMADD_SIZE_8BIT,
                                     i2c_tx_buffer,
                                     sizeof i2c_tx_buffer,0xff);
    HAL_UART_Transmit(&huart1,i2c_tx_buffer,sizeof i2c_tx_buffer,1);
    while (1)
    {
        /*HAL库一句话搞定读内存*/
        HAL_I2C_Mem_Read(&hi2c1, 
                         ADDR_EEPROM_Read,
                         0, 
                         I2C_MEMADD_SIZE_8BIT,
                         i2c_rx_buffer,
                         sizeof i2c_rx_buffer, 0xff);
        HAL_Delay(2000);
        HAL_UART_Transmit(&huart1,"read EEPROM data: ",sizeof "read EEPROM data: ",1);
        HAL_UART_Transmit(&huart1,i2c_rx_buffer, strlen(i2c_tx_buffer),1);  
    }
}
```



## 实验结果

![image-20221107161605991](https://my-blogs-imgs-1312546167.cos.ap-nanjing.myqcloud.com//image-20221107161605991.png)





## 参考

[【STM32】HAL库 STM32CubeMX教程十二---IIC(读取AT24C02 )_Z小旋的博客-CSDN博客_hal_i2c_master_transmit](https://blog.csdn.net/as480133937/article/details/105259075)



