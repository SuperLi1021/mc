#include<SN8F5702.h> 
#define LED1  P00
#define LED2  P02
void func1();
void func2();
 
/*============================以下为任务管理器代码============================*/
 
//任务槽个数.在本例中并未考虑任务换入换出,所以实际运行的任务有多少个,
//就定义多少个任务槽,不可多定义或少定义
#define MAX_TASKS 5
 
//任务的栈指针
unsigned char idata task_sp[MAX_TASKS];
 
//最大栈深.最低不得少于2个,保守值为12.
//预估方法:以2为基数,每增加一层函数调用,加2字节.
//如果其间可能发生中断,则还要再加上中断需要的栈深.
//减小栈深的方法:1.尽量少嵌套子程序 2.调子程序前关中断.
#define MAX_TASK_DEP 12
 
unsigned char idata task_stack[MAX_TASKS][MAX_TASK_DEP];//任务堆栈.
 
unsigned char task_id;		//当前活动任务号
 
 
//任务切换函数(任务调度器)
void task_switch()
{
    task_sp[task_id] = SP;		//保存当前任务的栈指针
 
    if (++task_id == MAX_TASKS)	//任务号切换到下一个任务
        task_id = 0;
 
    SP = task_sp[task_id];		//将系统的栈指针指向下个任务的私栈
}
 
 
 
 
//任务装入函数.将指定的函数(参数1)装入指定(参数2)的任务槽中.
//如果该槽中原来就有任务,则原任务丢失,但系统本身不会发生错误.
//将各任务的函数地址的低字节和高字节分别入在
//task_stack[任务号][0]和task_stack[任务号][1]中
void task_load(unsigned int fn, unsigned char tid)
{
    //task_sp[tid] = task_stack[tid][1];
    task_sp[tid] = task_stack[tid] + 1;
    task_stack[tid][0] = (unsigned int)fn & 0xff;
    task_stack[tid][1] = (unsigned int)fn >> 8;
}
 
//从指定的任务开始运行任务调度.调用该宏后,将永不返回.
#define os_start(tid) {task_id = tid,SP = task_sp[tid];return;}
 
 
 
 
/*============================以下为测试代码============================*/
 
 
unsigned char stra[3], strb[3];//用于内存块复制测试的数组.
 
 
//测试任务:复制内存块.每复制一个字节释放CPU一次
void task1()
{
    //每复制一个字节释放CPU一次,控制循环的变量必须考虑覆盖
    static unsigned char i;//如果将这个变量前的static去掉,会发生什么事?
    i = 0;
 
    while (1) //任务必须为死循环,不得退出函数,否则系统会崩溃
    {
        stra[i] = strb[i];
        if (++i == sizeof(stra))
            i = 0;
 
        //变量i在这里跨越了task_switch(),因此它必须定义为静态(static),
        //否则它将会被其它进程修改,因为在另一个进程里也会用到该变量所占用的地址.
        task_switch();//释放CPU一会儿,让其它进程有机会运行.如果去掉该行,则别的进程永远不会被调用到
    }
}
 
//测试任务:复制内存块.每复制一个字节释放CPU一次.
void task2()
{
    //每复制一个字节释放CPU一次,控制循环的变量必须考虑覆盖
    static unsigned char i;//如果将这个变量前的static去掉,将会发生覆盖问题.
    //task1()和task2()会被编译器分配到同一个内存地址上,当两个任务同时运行时,i的值就会被两个任务改来改去
    i = 0;
 
    while (1) //任务必须为死循环,不得退出函数,否则系统会崩溃
    {
        stra[i] = strb[i];
        if (++i == sizeof(stra))
            i = 0;
 
        //变量i在这里跨越了task_switch(),因此它必须定义为静态(static),
        //否则它将会被其它进程修改,因为在另一个进程里也会用到该变量所占用的地址.
        task_switch();//释放CPU一会儿,让其它进程有机会运行.如果去掉该行,则别的进程永远不会被调用到
    }
}
 
//测试任务:复制内存块.复制完所有字节后释放CPU一次.
void task3()
{
    //复制全部字节后才释放CPU,控制循环的变量不须考虑覆盖
    unsigned char i;//这个变量前不需要加static,
    //因为在它的作用域内并没有释放过CPU
 
    while (1) //任务必须为死循环,不得退出函数,否则系统会崩溃
    {
        i = sizeof(stra);
        do
        {
            stra[i-1] = strb[i-1];
        }
        while (--i);
 
        //变量i在这里已完成它的使命,所以无需定义为静态.
        //你甚至可以定义为寄存器型(regiter)
        task_switch();//释放CPU一会儿,让其它进程有机会运行.如果去掉该行,
        //则别的进程永远不会被调用到
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
 
//经过仿真计算得j=10,即等待1ms
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
    //在这个示例里并没有考虑任务的换入换出,所以任务槽必须全部用完,否则系统会崩溃.
    //这里装载了三个任务,因此在定义MAX_TASKS时也必须定义为5
    task_load(task1, 0);//将task1函数装入0号槽
    task_load(task2, 1);//将task2函数装入1号槽
    task_load(task3, 2);//将task3函数装入2号槽
    task_load(func1, 3);//将task3函数装入3号槽
    task_load(func2, 4);//将task3函数装入4号槽
 
    os_start(0);//启动任务调度,并从0号槽开始运行.参数改为1,则首先运行1号槽.
    //调用该宏后,程序流将永不再返回main(),也就是说,该语句行之后的所有语句都不被执行到.
}
