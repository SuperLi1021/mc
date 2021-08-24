#include<SN8F5702.h> 
#define LED1  P00
#define LED2  P02
void func1();
void func2();
 
/*============================����Ϊ�������������============================*/
 
//����۸���.�ڱ����в�δ���������뻻��,����ʵ�����е������ж��ٸ�,
//�Ͷ�����ٸ������,���ɶඨ����ٶ���
#define MAX_TASKS 5
 
//�����ջָ��
unsigned char idata task_sp[MAX_TASKS];
 
//���ջ��.��Ͳ�������2��,����ֵΪ12.
//Ԥ������:��2Ϊ����,ÿ����һ�㺯������,��2�ֽ�.
//��������ܷ����ж�,��Ҫ�ټ����ж���Ҫ��ջ��.
//��Сջ��ķ���:1.������Ƕ���ӳ��� 2.���ӳ���ǰ���ж�.
#define MAX_TASK_DEP 12
 
unsigned char idata task_stack[MAX_TASKS][MAX_TASK_DEP];//�����ջ.
 
unsigned char task_id;		//��ǰ������
 
 
//�����л�����(���������)
void task_switch()
{
    task_sp[task_id] = SP;		//���浱ǰ�����ջָ��
 
    if (++task_id == MAX_TASKS)	//������л�����һ������
        task_id = 0;
 
    SP = task_sp[task_id];		//��ϵͳ��ջָ��ָ���¸������˽ջ
}
 
 
 
 
//����װ�뺯��.��ָ���ĺ���(����1)װ��ָ��(����2)���������.
//����ò���ԭ����������,��ԭ����ʧ,��ϵͳ�����ᷢ������.
//��������ĺ�����ַ�ĵ��ֽں͸��ֽڷֱ�����
//task_stack[�����][0]��task_stack[�����][1]��
void task_load(unsigned int fn, unsigned char tid)
{
    //task_sp[tid] = task_stack[tid][1];
    task_sp[tid] = task_stack[tid] + 1;
    task_stack[tid][0] = (unsigned int)fn & 0xff;
    task_stack[tid][1] = (unsigned int)fn >> 8;
}
 
//��ָ��������ʼ�����������.���øú��,����������.
#define os_start(tid) {task_id = tid,SP = task_sp[tid];return;}
 
 
 
 
/*============================����Ϊ���Դ���============================*/
 
 
unsigned char stra[3], strb[3];//�����ڴ�鸴�Ʋ��Ե�����.
 
 
//��������:�����ڴ��.ÿ����һ���ֽ��ͷ�CPUһ��
void task1()
{
    //ÿ����һ���ֽ��ͷ�CPUһ��,����ѭ���ı������뿼�Ǹ���
    static unsigned char i;//������������ǰ��staticȥ��,�ᷢ��ʲô��?
    i = 0;
 
    while (1) //�������Ϊ��ѭ��,�����˳�����,����ϵͳ�����
    {
        stra[i] = strb[i];
        if (++i == sizeof(stra))
            i = 0;
 
        //����i�������Խ��task_switch(),��������붨��Ϊ��̬(static),
        //���������ᱻ���������޸�,��Ϊ����һ��������Ҳ���õ��ñ�����ռ�õĵ�ַ.
        task_switch();//�ͷ�CPUһ���,�����������л�������.���ȥ������,���Ľ�����Զ���ᱻ���õ�
    }
}
 
//��������:�����ڴ��.ÿ����һ���ֽ��ͷ�CPUһ��.
void task2()
{
    //ÿ����һ���ֽ��ͷ�CPUһ��,����ѭ���ı������뿼�Ǹ���
    static unsigned char i;//������������ǰ��staticȥ��,���ᷢ����������.
    //task1()��task2()�ᱻ���������䵽ͬһ���ڴ��ַ��,����������ͬʱ����ʱ,i��ֵ�ͻᱻ�������������ȥ
    i = 0;
 
    while (1) //�������Ϊ��ѭ��,�����˳�����,����ϵͳ�����
    {
        stra[i] = strb[i];
        if (++i == sizeof(stra))
            i = 0;
 
        //����i�������Խ��task_switch(),��������붨��Ϊ��̬(static),
        //���������ᱻ���������޸�,��Ϊ����һ��������Ҳ���õ��ñ�����ռ�õĵ�ַ.
        task_switch();//�ͷ�CPUһ���,�����������л�������.���ȥ������,���Ľ�����Զ���ᱻ���õ�
    }
}
 
//��������:�����ڴ��.�����������ֽں��ͷ�CPUһ��.
void task3()
{
    //����ȫ���ֽں���ͷ�CPU,����ѭ���ı������뿼�Ǹ���
    unsigned char i;//�������ǰ����Ҫ��static,
    //��Ϊ�������������ڲ�û���ͷŹ�CPU
 
    while (1) //�������Ϊ��ѭ��,�����˳�����,����ϵͳ�����
    {
        i = sizeof(stra);
        do
        {
            stra[i-1] = strb[i-1];
        }
        while (--i);
 
        //����i���������������ʹ��,�������趨��Ϊ��̬.
        //���������Զ���Ϊ�Ĵ�����(regiter)
        task_switch();//�ͷ�CPUһ���,�����������л�������.���ȥ������,
        //���Ľ�����Զ���ᱻ���õ�
    }
}
 
 
 
/*
my first task
*/
 
void func1()
{
    static unsigned char data i;
    i = 0;
 
    while (1)
    {
            //25ms
        if (i < 250)
        {
            i++;
        }
        if (i >= 250)
        {
            LED1 = ~LED1;
            i = 0;
        }
        task_switch();
    }
}
 
//������������j=10,���ȴ�1ms
void func2()
{
    static unsigned int data j;
    j = 0;
 
    while (1)
    {
        //65ms
        if (j < 650)
        {
            j++;
        }
        if (j >= 650)
        {
            LED2 = !LED2;
            j = 0;
        }
        task_switch();
    }
}
 
/*
keyscan task
void task3()
{
	while(1)
	{
        i = 5;
        do
		{
                sigl = !sigl;
        }while(--i);
        task_switch();
    }
}
*/
 
/*
void func1()
{
    register char data i;
    while(1)
	{
        i = 5;
        do
		{
                sigl = !sigl;
        }while(--i);
        task_switch();
    }
}
void func2()
{
    register char data i;
    while(1)
	{
        i = 5;
        do
		{
                sigl = !sigl;
        }while(--i);
        task_switch();
    }
}
*/
 
void main()
{WDTR = 0x5A; 
	P0M=0XFF;
    //�����ʾ���ﲢû�п�������Ļ��뻻��,��������۱���ȫ������,����ϵͳ�����.
    //����װ������������,����ڶ���MAX_TASKSʱҲ���붨��Ϊ5
    task_load(task1, 0);//��task1����װ��0�Ų�
    task_load(task2, 1);//��task2����װ��1�Ų�
    task_load(task3, 2);//��task3����װ��2�Ų�
    task_load(func1, 3);//��task3����װ��3�Ų�
    task_load(func2, 4);//��task3����װ��4�Ų�
 
    os_start(0);//�����������,����0�Ųۿ�ʼ����.������Ϊ1,����������1�Ų�.
    //���øú��,�������������ٷ���main(),Ҳ����˵,�������֮���������䶼����ִ�е�.
}
