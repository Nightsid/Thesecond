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
sbit BUZZER = P1^7;  //��������������
sbit Trig = P1^0;
sbit Echo = P1^1;
sbit KEY_OUT_1 = P1^2;  //���󰴼���ɨ���������1
sbit KEY_OUT_2 = P1^3;  //���󰴼���ɨ���������2
sbit KEY_OUT_3 = P1^4;  //���󰴼���ɨ���������3
sbit KEY_OUT_4 = P1^5;  //���󰴼���ɨ���������4
sbit KEY_OUT_5 = P1^6;  //���󰴼���ɨ���������4

unsigned long S=0;
bit      flag =0;
unsigned int DIstance,time;
char H = 20;//�趨����������
char L = 10;//�趨��С��������

void Lcd1602_Delay1ms(uint c)   //��� 0us
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

void LcdWriteCom(uchar com)	  //д������
{
	LCD1602_E = 0;     //ʹ�� 
	LCD1602_RS = 0;	   //ѡ��������
	LCD1602_RW = 0;	   //ѡ��д��
	LCD1602_DATAPINS = com;     //��������
	Lcd1602_Delay1ms(1);		//�ȴ������ȶ�

	LCD1602_E = 1;	          //д��ʱ��
	Lcd1602_Delay1ms(5);	  //����ʱ��
	LCD1602_E = 0;
}	   	   
void LcdWriteData(uchar dat)			//д��һ���ֽ�����
{
	LCD1602_E = 0;	//ʹ������
	LCD1602_RS = 1;	//ѡ����������
	LCD1602_RW = 0;	//ѡ��д��

	LCD1602_DATAPINS = dat; //д������
	Lcd1602_Delay1ms(1);

	LCD1602_E = 1;   //д��ʱ��
	Lcd1602_Delay1ms(5);   //����ʱ��
	LCD1602_E = 0;
}
		   
void LcdInit()						  //LCD��ʼ��
{
 	LcdWriteCom(0x38);  //����ʾ16*2
	LcdWriteCom(0x0c);  //����ʾ����ʾ���
	LcdWriteCom(0x06);  //дһ��ָ���1
	LcdWriteCom(0x01);  //����
	LcdWriteCom(0x80);  //��������ָ�����
}


//��ָ��λ����ʾһ���ַ�
void DisplayOneChar(unsigned char X, unsigned char Y, unsigned char DData)
{
 Y &= 0x1;
 X &= 0xF; //����X���ܴ���15��Y���ܴ���1
 if (Y) X |= 0x40; //��Ҫ��ʾ�ڶ���ʱ��ַ��+0x40;
 X |= 0x80; // ���ָ����
 LcdWriteCom(X); //���ﲻ���æ�źţ����͵�ַ��
 LcdWriteData(DData);
}
//��ָ��λ����ʾһ���ַ�
void DisplayListChar(unsigned char X, unsigned char Y, unsigned char code *DData)
{
 unsigned char ListLength;

  ListLength = 0; 
 Y &= 0x1;//y=y&0x1,y����������
 X &= 0xF; //x=x&0xF����X���ܴ���15��Y���ܴ���1��x����������
 while (DData[ListLength]>=0x20) //�������ִ�β���˳�����������(ASCII��)�ɴ�ӡ�ַ���Χ,�˴��Ǽ���DData�еĵ����ַ��Ƿ������lcd�ϴ�ӡ����
  {
   if (X <= 0xF) //X����ӦС��0xF
    {
     DisplayOneChar(X, Y, DData[ListLength]); //��ʾ�����ַ�
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
void  StartModule() 		         //����ģ��
{
	  Trig=0;     //��ʼ��ʱ��������TR�����趨Ϊ�͵�ƽ,��ΪSRF04�����������������TR���ŵ������ش�����
	  Trig=1;			//����һ��ģ��
	  _nop_();    //һ��_nop_�����ӳ�1us����SRF04�Ĵ���ʱ������Ϊ10US
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
	  Trig=0;   //��Trig���ŵ�ƽ��Ϊ�͵�ƽ���ȴ���һ�ε������ش����ź�
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
		DisplayOneChar(8, 1, (char)(L/100+'0')); //��λ
	  DisplayOneChar(9, 1, (char)(L/10%10+'0')); //ʮλ
	  DisplayOneChar(10, 1, (char)(L%10+'0'));    //��λ
		DisplayOneChar(11, 1, ('-'));    //.
		DisplayOneChar(12, 1, (char)(H/100+'0')); //��λ
	  DisplayOneChar(13, 1, (char)(H/10%10+'0')); //ʮλ
	  DisplayOneChar(14, 1, (char)(H%10+'0'));    //��λ
  
}
void Sr04_Init()
{
	 TMOD=0x01;	//��T0Ϊ��ʽ1��GATE=0,��ʱT0��ʱ��ֻ�ܵ�TR0�Ŀ��ƣ�TR0Ϊ1��������ʱ����
	 TH0=0;   //T0��ʱ��Ϊ16λ��1����������TH0��TL0��ɣ�װ�ؼ�����ֵ������TH0Ϊ��8λ��TL0Ϊ�Ͱ�λ��
	 TL0=0;   //�˴���ʾ������ΧΪ65536us,��0.065536s
	 ET0=1;   //����T0�ж�
	 EA=1;		//�������ж�;
}
void Conut(void)
{
	 StartModule();
	 while(!Echo);		//���������Ľ��նˣ�Echo�����յ����صĳ�����ʱ��Echo��ƽ��Ϊ�ߵ�ƽ��Echoδ���յ��ź�ʱΪ�㣬�����յ��ز��źź�Echo���Ż�����һ��ʱ����ͬ�ĸߵ�ƽ�ź�
	 TR0=1;			    //TCON��ʱ�������е�һλ���ݣ��ڴ�Ϊ1�������������Sr04_Init()����������
	 while(Echo);			//����EchoΪ�ߵ�ƽ��ʱ�䣬��Ϊ�շ��źŵ�ʱ�䡣
	 TR0=0;				//�رռ���
	 time=TH0*256+TL0; //��ʱ������,T0��ʱ��������Ϊ16λ�Ķ��������ݣ�����TH0������Ϊ�߰�λ��TL0������Ϊ�Ͱ�λ
	 TH0=0;
	 TL0=0;
	 DIstance =(time*1.7)/100;     //�������CM
	 S = DIstance+1;
	 if((S>=H)||flag==1) //s�ǲ����ľ��룬flagΪ1������T0��ʱ���������жϣ�����ʱ���õ�ʱ�䳬���˼�ʱ��Χ���������˲������룬��ʱ����LCD������ʾ��������Ϊ0
	 {	 
	  flag=0;
	  DisplayOneChar(9, 0, '0');
	  DisplayOneChar(10, 0, '.');	//��ʾ��
	  DisplayOneChar(11, 0, '0');
	  DisplayOneChar(12, 0, '0');
	  DisplayOneChar(13, 0, 'C');	//��ʾC
		DisplayOneChar(14, 0, 'M');	//��ʾM
	 }
	 else if(S>=L && S<H)
	 {
		DisplayListChar(0,1,"normal-");
	  DisplayOneChar(9, 0, (char)(S/100+'0')); //��λ
	  //DisplayOneChar(1, 1, '.');	//��ʾ��
	  DisplayOneChar(10, 0, (char)(S/10%10+'0')); //ʮλ
	  DisplayOneChar(11, 0, (char)(S%10+'0'));    //��λ
		DisplayOneChar(12, 0, '.');	//��ʾ��
		DisplayOneChar(13, 0, '0');	//��ʾ��
	  DisplayOneChar(14, 0, 'C');	//��ʾC
		DisplayOneChar(15, 0, 'M');	//��ʾM
		 
	 }
	 else
	 {
		
	  DisplayOneChar(9, 0, (char)(S/100+'0')); //��λ
	  //DisplayOneChar(1, 1, '.');	//��ʾ��
	  DisplayOneChar(10, 0, (char)(S/10%10+'0')); //ʮλ
	  DisplayOneChar(11, 0, (char)(S%10+'0'));    //��λ
		DisplayOneChar(12, 0, '.');	//��ʾ��
		DisplayOneChar(13, 0, '0');	//��ʾ��
	  DisplayOneChar(14, 0, 'C');	//��ʾC
		DisplayOneChar(15, 0, 'M');	//��ʾM
	 }
	
	 if(S <= L)
	 {	
		 delay_ms(100);
		 DisplayListChar(0,1,"warning");
		 LEDYELLOW = 0;           //�Ƶ���
		 LEDGREEN = 1;
		 LEDRED = 1;
		 BUZZER = 1;
		 delay_ms(100);
		 DisplayListChar(0,1,"       ");
		 BUZZER = 1;												//������ ��~��~��
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
		 LEDGREEN = 0 ;               //�̵���
		 BUZZER = 1;		 //������
		 delay_ms(100);
	 }
	 else if(S > H||flag==1)
	 {
		 delay_ms(100);
		 DisplayListChar(0,1,"warning");
		 //LcdWriteCom(0x01);  //����
		 LEDGREEN = 1;
		 LEDYELLOW = 1;
		 delay_ms(100);
		 DisplayListChar(0,1,"       ");
	   LEDRED = 0;                //�����		
     BUZZER = 0;
     delay_ms(500);		 //��������
	 }
	 
}
void zd0() interrupt 1 		 //�жϺ�1��Ӧ���ж�ԴΪTO,��T0��ʱ�����ʱ����������෶Χ�������Զ����ø��жϺ���.
{
    flag=1;							 //�ж������־
}


void main()
{
	BUZZER = 0;  //����������ת
	Sr04_Init();
	LcdInit();
	DisplayListChar(0,0,"distance:");
	
	while(1)
	{
		key_set();
		Conut();
	}
}

