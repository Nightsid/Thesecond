#include<reg51.h>
#include <intrins.h>
#define uchar unsigned char
#define uint unsigned int
#define LCD1602_DATAPINS P0
sbit LCD1602_E=P2^2;
sbit LCD1602_RW=P2^1;
sbit LCD1602_RS=P2^0;
sbit LEDRED = P2^5;
sbit LEDGREEN = P2^6;
sbit LEDYELLOW = P2^7;
sbit BUZZER = P1^7;  //蜂鸣器控制引脚
sbit Trig = P1^0;
sbit Echo = P1^1;
sbit KEY_OUT_1 = P1^2;  //矩阵按键的扫描输出引脚1
sbit KEY_OUT_2 = P1^3;  //矩阵按键的扫描输出引脚2
sbit KEY_OUT_3 = P1^4;  //矩阵按键的扫描输出引脚3
sbit KEY_OUT_4 = P1^5;  //矩阵按键的扫描输出引脚4
sbit KEY_OUT_5 = P1^6;  //矩阵按键的扫描输出引脚4

unsigned long S=0;
bit      flag =0;
unsigned int DIstance,time;
char H = 20;//设定最大测量距离
char L = 10;//设定最小测量距离

void Lcd1602_Delay1ms(uint c)   //误差 0us
{
  uchar a,b;
	for (; c>0; c--)
	{
		 for (b=199;b>0;b--)
		 {
		  	for(a=1;a>0;a--);
		 }      
	}
}

void LcdWriteCom(uchar com)	  //写入命令
{
	LCD1602_E = 0;     //使能 
	LCD1602_RS = 0;	   //选择发送命令
	LCD1602_RW = 0;	   //选择写入
	LCD1602_DATAPINS = com;     //放入命令
	Lcd1602_Delay1ms(1);		//等待数据稳定

	LCD1602_E = 1;	          //写入时序
	Lcd1602_Delay1ms(5);	  //保持时间
	LCD1602_E = 0;
}	   	   
void LcdWriteData(uchar dat)			//写入一个字节数据
{
	LCD1602_E = 0;	//使能清零
	LCD1602_RS = 1;	//选择输入数据
	LCD1602_RW = 0;	//选择写入

	LCD1602_DATAPINS = dat; //写入数据
	Lcd1602_Delay1ms(1);

	LCD1602_E = 1;   //写入时序
	Lcd1602_Delay1ms(5);   //保持时间
	LCD1602_E = 0;
}
		   
void LcdInit()						  //LCD初始化
{
 	LcdWriteCom(0x38);  //开显示16*2
	LcdWriteCom(0x0c);  //开显示不显示光标
	LcdWriteCom(0x06);  //写一个指针加1
	LcdWriteCom(0x01);  //清屏
	LcdWriteCom(0x80);  //设置数据指针起点
}


//按指定位置显示一个字符
void DisplayOneChar(unsigned char X, unsigned char Y, unsigned char DData)
{
 Y &= 0x1;
 X &= 0xF; //限制X不能大于15，Y不能大于1
 if (Y) X |= 0x40; //当要显示第二行时地址码+0x40;
 X |= 0x80; // 算出指令码
 LcdWriteCom(X); //这里不检测忙信号，发送地址码
 LcdWriteData(DData);
}
//按指定位置显示一串字符
void DisplayListChar(unsigned char X, unsigned char Y, unsigned char code *DData)
{
 unsigned char ListLength;

  ListLength = 0; 
 Y &= 0x1;//y=y&0x1,y是限制行数
 X &= 0xF; //x=x&0xF限制X不能大于15，Y不能大于1，x是限制列数
 while (DData[ListLength]>=0x20) //若到达字串尾则退出，即超出了(ASCII码)可打印字符范围,此处是检验DData中的单个字符是否可以在lcd上打印出来
  {
   if (X <= 0xF) //X坐标应小于0xF
    {
     DisplayOneChar(X, Y, DData[ListLength]); //显示单个字符
     ListLength++;
     X++;
    }
  }
}

void delay_ms(unsigned int z)
{
   unsigned int i,j;
   for(i=z;i>0;i--)
      for(j=110;j>0;j--);
}
void  StartModule() 		         //启动模块
{
	  Trig=0;     //初始化时超声波的TR引脚设定为低电平,因为SRF04发出超声波测距是由TR引脚的上升沿触发的
	  Trig=1;			//启动一次模块
	  _nop_();    //一个_nop_函数延迟1us，而SRF04的触发时间至少为10US
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_();
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_();
	  Trig=0;   //将Trig引脚电平降为低电平，等待下一次的上升沿触发信号
}
void key_set()
{
		
	if(KEY_OUT_1 == 0)
	{
		delay_ms(50);
		L = L + 1;
	}
	else if (KEY_OUT_2 == 0)
	{
		delay_ms(50);
		L = L - 1;
	}
	else if (KEY_OUT_3 == 0)
	{
		delay_ms(50);
		H = H + 1;
	}
	else if (KEY_OUT_4 == 0)
	{
		delay_ms(50);
		H = H - 1;
	}
	else if (KEY_OUT_5 == 0)
	{
		delay_ms(50);
		H = 60;
		 L = 50;
	}
		DisplayOneChar(8, 1, (char)(L/100+'0')); //百位
	  DisplayOneChar(9, 1, (char)(L/10%10+'0')); //十位
	  DisplayOneChar(10, 1, (char)(L%10+'0'));    //个位
		DisplayOneChar(11, 1, ('-'));    //.
		DisplayOneChar(12, 1, (char)(H/100+'0')); //百位
	  DisplayOneChar(13, 1, (char)(H/10%10+'0')); //十位
	  DisplayOneChar(14, 1, (char)(H%10+'0'));    //个位
  
}
void Sr04_Init()
{
	 TMOD=0x01;	//设T0为方式1，GATE=0,此时T0定时器只受到TR0的控制，TR0为1则启动定时器；
	 TH0=0;   //T0定时器为16位加1计数器，由TH0和TL0组成，装载计数初值，其中TH0为高8位，TL0为低八位。
	 TL0=0;   //此处表示计数范围为65536us,即0.065536s
	 ET0=1;   //允许T0中断
	 EA=1;		//开启总中断;
}
void Conut(void)
{
	 StartModule();
	 while(!Echo);		//当超声波的接收端（Echo）接收到返回的超声波时，Echo电平置为高电平，Echo未接收到信号时为零，且在收到回波信号后Echo引脚会生成一段时间相同的高电平信号
	 TR0=1;			    //TCON定时控制器中的一位数据，在此为1开启计数，结合Sr04_Init()函数分析。
	 while(Echo);			//计算Echo为高电平的时间，即为收发信号的时间。
	 TR0=0;				//关闭计数
	 time=TH0*256+TL0; //计时结束后,T0定时器的数据为16位的二进制数据，其中TH0的数据为高八位，TL0的数据为低八位
	 TH0=0;
	 TL0=0;
	 DIstance =(time*1.7)/100;     //算出来是CM
	 S = DIstance+1;
	 if((S>=H)||flag==1) //s是测量的距离，flag为1代码着T0计时器发生了中断，即计时所得的时间超出了计时范围，代表超出了测量距离，此时则在LCD屏上显示测量距离为0
	 {	 
	  flag=0;
	  DisplayOneChar(9, 0, '0');
	  DisplayOneChar(10, 0, '.');	//显示点
	  DisplayOneChar(11, 0, '0');
	  DisplayOneChar(12, 0, '0');
	  DisplayOneChar(13, 0, 'C');	//显示C
		DisplayOneChar(14, 0, 'M');	//显示M
	 }
	 else if(S>=L && S<H)
	 {
		DisplayListChar(0,1,"normal-");
	  DisplayOneChar(9, 0, (char)(S/100+'0')); //百位
	  //DisplayOneChar(1, 1, '.');	//显示点
	  DisplayOneChar(10, 0, (char)(S/10%10+'0')); //十位
	  DisplayOneChar(11, 0, (char)(S%10+'0'));    //个位
		DisplayOneChar(12, 0, '.');	//显示点
		DisplayOneChar(13, 0, '0');	//显示点
	  DisplayOneChar(14, 0, 'C');	//显示C
		DisplayOneChar(15, 0, 'M');	//显示M
		 
	 }
	 else
	 {
		
	  DisplayOneChar(9, 0, (char)(S/100+'0')); //百位
	  //DisplayOneChar(1, 1, '.');	//显示点
	  DisplayOneChar(10, 0, (char)(S/10%10+'0')); //十位
	  DisplayOneChar(11, 0, (char)(S%10+'0'));    //个位
		DisplayOneChar(12, 0, '.');	//显示点
		DisplayOneChar(13, 0, '0');	//显示点
	  DisplayOneChar(14, 0, 'C');	//显示C
		DisplayOneChar(15, 0, 'M');	//显示M
	 }
	
	 if(S <= L)
	 {	
		 delay_ms(100);
		 DisplayListChar(0,1,"warning");
		 LEDYELLOW = 0;           //黄灯亮
		 LEDGREEN = 1;
		 LEDRED = 1;
		 BUZZER = 1;
		 delay_ms(100);
		 DisplayListChar(0,1,"       ");
		 BUZZER = 1;												//蜂鸣器 嘀~嘀~嘀
		 delay_ms(100);
		 BUZZER = 0;	
		 delay_ms(100);
	 }
	 else if(S>L && S<= H)
	 {
		 LEDYELLOW = 1;
		 LEDRED = 1;
		 delay_ms(100);
		 DisplayListChar(0,1,"       ");
		 LEDGREEN = 0 ;               //绿灯亮
		 BUZZER = 1;		 //蜂鸣器
		 delay_ms(100);
	 }
	 else if(S > H||flag==1)
	 {
		 delay_ms(100);
		 DisplayListChar(0,1,"warning");
		 //LcdWriteCom(0x01);  //清屏
		 LEDGREEN = 1;
		 LEDYELLOW = 1;
		 delay_ms(100);
		 DisplayListChar(0,1,"       ");
	   LEDRED = 0;                //红灯亮		
     BUZZER = 0;
     delay_ms(500);		 //蜂鸣器响
	 }
	 
}
void zd0() interrupt 1 		 //中断号1对应的中断源为TO,当T0定时器溢出时（即超过测距范围），则自动调用该中断函数.
{
    flag=1;							 //中断溢出标志
}


void main()
{
	BUZZER = 0;  //蜂鸣器不运转
	Sr04_Init();
	LcdInit();
	DisplayListChar(0,0,"distance:");
	
	while(1)
	{
		key_set();
		Conut();
	}
}

