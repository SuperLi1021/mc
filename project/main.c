/*********************************************************************/
/*公司名称： 深圳市朗科智能电气股份有限公司                          */
/*模 块 名： 获取红外信号                                          */
/*创 建 人： LICHAOFAN     日期：2021-08-13                          */
/*修 改 人： LICHAOFAN     日期：2021-08-13                          */
/*功能描述： 获取红外信号                                              */
/*其他说明：                                                         */
/*版本：
/**********************************************************************/
#include <intrins.h>    
#include <SN8F5702.h>
#include "display.h"
#include "sys.h"

/********************************定义全局字节变量*********************/
unsigned int PinT;
unsigned char Level,PinBit,SWFlag,SWFlags,HWFlag,sw;
/********************************定义全局位变量***********************/

/*********************************************************************
* 函 数 名： main
* 功能描述： 主函数
* 函数说明： 主函数
* 调用函数： 
* 全局变量： PinT
* 输 入： 无
* 返 回： 无
* 设 计 者：LICHAOFAN 日期：2021-08-13
* 修 改 者：LICHAOFAN 日期：2021-08-13
/*版本：
/**********************************************************************/
void main(void) 
{	
    WDTR = 0x5A;         			  //看门狗复位  
    InitT0T1();       				  //定时器初始化
    InitT2();
    P1M=0xfE;         			    //io端口初始化
    P0M=0XEf;	
    P0OC=0X10;
    P2M=0X00;
    Level=60;
    while (1) 
  	{  WDTR = 0x5A;
           	switch(PinT)													//显示接收到的信号，
	  		{   
	  				case 0x0277: Level=1 ;display(Level);break;
	  				case 0x0239: Level=20;display(Level);break;
	  				case 0x025F: Level=50;display(Level);break;
	  				case 0x027E: Level=60;display(Level);break;
					default:display(Level);break; 
	  		}
            if(sw==1)														//对接收到的有次数要求的信号进行处理并显示			
			{    sw=0;
            	switch(PinT)						
	  	    	{ 	case 0x026F: if(Level>=20)Level=Level-10;;display(Level);break;
				  																//+	
	  	         	case 0x027B: SWFlags=~SWFlags;display(Level);break;					
					   															//开关
                    case 0x027D: if(Level<=50)Level=Level+10;display(Level);break;
																				//-
                    default:display(Level);break; 
                }
            } 
			if(SWFlags==0xff)
			{
				if((P1&0X01)==0x01)											//仅当遥控开关与按键开关均处于打开状态
				{															//才启动电机				
					
					SWFlag=1;
				}
				else														//否则电机不动
					SWFlag=0;
			}
			else
			SWFlag=0;


  	}
}





