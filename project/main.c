/*********************************************************************/
/*��˾���ƣ� �������ʿ����ܵ����ɷ����޹�˾                          */
/*ģ �� ���� ��ȡ�����ź�                                          */
/*�� �� �ˣ� LICHAOFAN     ���ڣ�2021-08-13                          */
/*�� �� �ˣ� LICHAOFAN     ���ڣ�2021-08-13                          */
/*���������� ��ȡ�����ź�                                              */
/*����˵����                                                         */
/*�汾��
/**********************************************************************/
#include <intrins.h>    
#include <SN8F5702.h>
#include "display.h"
#include "sys.h"

/********************************����ȫ���ֽڱ���*********************/
unsigned int PinT;
unsigned char Level,PinBit,SWFlag,HWFlag,sw;
bit SWFlags;
/********************************����ȫ��λ����***********************/

/*********************************************************************
* �� �� ���� main
* ���������� ������
* ����˵���� ������
* ���ú����� 
* ȫ�ֱ����� PinT
* �� �룺 ��
* �� �أ� ��
* �� �� �ߣ�LICHAOFAN ���ڣ�2021-08-13
* �� �� �ߣ�LICHAOFAN ���ڣ�2021-08-13
/*�汾��
/**********************************************************************/
void main(void) 
{	
    WDTR = 0x5A;         			  //���Ź���λ  
    InitT0T1();       				  //��ʱ����ʼ��
    InitT2();
    P1M=0xfE;         			    //io�˿ڳ�ʼ��
    P0M=0XEe;	
    P0OC=0X10;
    P2M=0X00;
    P05=0;
   // P2UR=0XFF;
    Level=70;
    SWFlags=1;
    while (1) 
  	{  WDTR = 0x5A;
           	switch(PinT)													//��ʾ���յ����źţ�
	  		{   
	  				case 0x0277: Level=1 ;display(Level);break;
	  				case 0x0239: Level=20;display(Level);break;
	  				case 0x025F: Level=50;display(Level);break;
	  				case 0x027E: Level=70;display(Level);break;
					default:display(Level);break; 
	  		}
            if(sw==1)														//�Խ��յ����д���Ҫ����źŽ��д�����ʾ			
			{    sw=0;
            	switch(PinT)						
	  	    	{ 	case 0x026F: if(Level>=20)Level=Level-10;;display(Level);break;
				  																//+	
	  	         	case 0x027B: SWFlags=~SWFlags;display(Level);break;					
					   															//����
                    case 0x027D: if(Level<=60)Level=Level+10;display(Level);break;
																				//-
                    default:display(Level);break; 
                }
            } 
			if(SWFlags==0)
			{   
				if((P1&0X01)!=0x01)											//����ң�ؿ����밴�����ؾ����ڴ�״̬
				{															//���������				
				
					SWFlag=1;

				}
				else														//����������
				{	SWFlag=0;Level=70;   }
			}
			else
		{	SWFlag=0;Level=70; }
        if((P1&0X01)==0x01)
            Level=70;


  	}
}





