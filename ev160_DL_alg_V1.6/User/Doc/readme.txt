2018年12月5日 15：28
 ev160_DL_alg_V1.6与protocol程序融合
 问题：原ev160_DL_alg_V1.6代码，在触发后通过Send_debug_message();	上传信息格式与405板子上传数据不符，matlab打印不出来波形。
 解决：USART输出TTL电平，需外接一个TTL转232的转换头
 
2018.12.6 15：56
 问题：UART5在原程序配置后不能够正常工作
 解决：原程序把UART5 RX\TX引脚配置成了其它模式，需要注释掉。
 
 2018.12.7 14：57
 从机发送阈值和上报命令测试完成可接收，并分别存放在结构体变量g_tCarDoorState、g_tCarState中，在函数UcAnalysis_RecSlave_Data函数中进行接收
 
 2018.12.8  11：00
 程序框架已经搭建完成，测试框架时发现调用函数Master_Send_Data时发送数据有误，解决：Master_Send_Data_Frame函数中ucSendData[i]=*packdata改为ucSendData[i]=*packdata++;