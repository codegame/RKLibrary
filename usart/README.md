![RKLibrary](..\\doc\\Logo_Mini.png)

RK Usart 串口驱动库
===========

[Documentation](http://)
------

概览
------
 
[USART](https://en.wikipedia.org/wiki/Universal_synchronous_and_asynchronous_receiver-transmitter) **串口驱动库**专门为单片系统的串口通讯操作提供简化使用的开源库。

源代码使用开源BSD2发布。

------
支持设备
------
 STM32F0xx,STM32F1xx,STM32F3xx,STM32F4xx
 Artery AT32F421x,AT32F423x

------
功能特性
------

**1.收发使用DMA和空闲中断**

**2.支持不定长数据发送**

**3.支持最多16个串口同时工作**

**4.支持收发CRC16透明校验**

**5.支持自定义CRC16校验算法**

**6.支持校验错误自动请求重发**

**7.支持自定义缓冲区大小**

 
------
使用方法
------

**1.下载RKLibrary可以放置到任意目录，在IDE中设置绝对引用路径：**
`"...RKLibrary\inc"` 和`"...RKLibrary\usart"`，并且把`rk_usart.c`添加到工程项目列表中。
<br>

**2.修改中断处理文件(stm32f1xx_it 或 at32fxxx_int.c)**

 1.添加 `#include "rk_usart.h"`。

 2.找到Usart中断函数：`void USARTx_IRQHandler(void)`添加：
  `RK_Usart_RxCallback(RK_Usart_Phy_x);` 其中`RK_Usart_Phy_x`为真实的串口序号，如果真实串口对象为Usart2则填入`RK_Usart_Phy_2`。

<br>

**3.在需要调用的文件中对库进行引用初始化**

1.添加 ```c #include "rk_usart.h"```。
<br>

2.添加申明串口对象数组和串口对象：
```c
 RK_Usart_HandleTypeDef RKL_Usart2; //这里申请Usart2的串口。
 RK_Usart_Handles RK_Usart_Handle ={NULL};
```
<br>

3.添加串口接收回调函数：
```c
void MyUsart2_Rx(uint8_t * Data, uint16_t Size)
{ 
//这里处理收到的数据包
  RK_Usart_DMA_Transmit(RK_Usart_Phy_2,(uint8_t*)Data, Size); //演示收到数据直接发送
  return;
}
```
<br>

4.添加初始化代码：
```c
//Artery AT32 :
void  Usart_Init(void)
{
  RKL_Uart2.PhyId = RK_Usart_Phy_2; //内部串口序列编号
  RKL_Uart2.usart = USART2;   //使用串口对象
  RKL_Uart2.tx_dma = DMA1_CHANNEL4; //DMA Tx通道
  RKL_Uart2.rx_dma = DMA1_CHANNEL5; //DMA Rx通道
  RKL_Uart2.RXCallback = MyUsart2_Rx; //设置用户RX接收回调函数
  RKL_Uart2.tx_crc = true; //CRC 校验和
  RKL_Uart2.rx_crc = true; //CCRC 校验和，校验失败则丢掉包
  RKL_Uart2.resend = true; //错误重发
  RKL_Uart2.tx_Size =128; //发送缓冲大小Byte
  RKL_Uart2.rx_Size =128; //接收缓冲大小Byte
  RK_Usart_Handle[RKL_Uart2.PhyId -1]= &RKL_Uart2; //添加对象结构到RKL_Uart管理句柄  
  RK_Usart_Init(&RK_Usart_Handle);
}

//STM32:
void  Usart_Init(void)
{
  RKL_Uart2.PhyId = RK_Usart_Phy_2; //内部串口序列编号
  RKL_Uart2.usart = &huart1;         //使用串口对象
  RKL_Uart2.RXCallback = MyUsart2_Rx; //设置用户RX接收回调函数
  RKL_Uart2.tx_crc = true; //CRC 校验和
  RKL_Uart2.rx_crc = true; //CRC 校验和，校验失败则丢掉包
  RKL_Uart2.resend = true; //错误重发
  RKL_Uart2.tx_Size = 128; //发送缓冲大小Byte
  RKL_Uart2.rx_Size = 128; //接收缓冲大小Byte
  RK_Usart_Handle[RKL_Uart2.PhyId -1]= &RKL_Uart2; //添加对象结构到RKL_Uart管理句柄 
  RK_Usart_Init(&RK_Usart_Handle);
}
```

5.调用使用：

```c
  Usart_Init();
  char _putdata[] = "RK Usart library v0.1 \r\n";
  RK_Usart_DMA_Transmit(RK_Usart_Phy_2,(uint8_t*)_putdata, strlen(_putdata));
```



------
其它问题
------
<br>

***QQ讨论群:740605493***



<br>
<br>
<br>
<br>
<br>
<br>

