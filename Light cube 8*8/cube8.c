#include<reg52.h>

#define uchar unsigned char
#define uint unsigned int
uchar xx,yy,zz,layer,icnt,rxcnt;
uchar display[8][8];
void delay(uchar z)
{
	uchar x,y;
	for(x=z;x>0;x--)
		for(y=100;y>0;y--);
}
void init()
{
	TMOD=0x21;//定时器1 方式2 用于产生波特率；定时器0 方式1 用于刷新显示
	TH1=0XFF;  //
	TL1=0XFF;
	TH0=0xC0;
	TL0=0;
	TR1=1;		//开定时器1
	TR0=1;

	SCON=0X50;//串口工作于方式1 可接收REN=1
	PCON|=0x80;//波特率加倍 

	EA=1;		//开全局中断
	ES=1;		//开串口中断
	ET0=1;		//开定时器0中断
	PS=1;		//配置串口中断优先
}
/*void single_led(uchar x,uchar y,uchar z,uchar state)
{
	uchar t0,t1;
	t0=0x01<<x;
	t1=~t0;
	if(1==state)
	display[y][z]=display[y][z]|t0;
	else
	display[y][z]=display[y][z]&t1;
}
void all_led(uchar state)
{
	uchar i,j;
	for(i=0;i<8;i++)
	{	
		for(j=0;j<8;j++)
		{
			if(1==state)
			display[j][i]=0xff;
			if(0==state)
			display[j][i]=0x00;
		}
	}
}  */
void main()
{
	uchar i;
	rxcnt=0;
	init();				//初始化
	while(1)
	{		
	//	P1=0;
		for (i=0;i<8;i++)
		{
			P2=1<<i;		//锁存端
			delay(10);
			P0=display[layer][i];//位选端
			delay(10);
		}
		
		P1=1<<layer;		//开显示
			//P1=layer;
//	all_led(0);
	if (layer<7) 
		layer++;
	else 
		layer=0


	}


}
void rxd() interrupt 4		  //串口中断服务程序
{								  
		uchar temp=0;			
		RI=0;
		++rxcnt;
		if(rxcnt>=2)
		{			
			temp=SBUF;			//读出接收到的数据
			display[xx][yy]=temp;	 //按顺序存入显示数组中
			xx++;
			if(xx==8)				
			{
				xx=0;
				yy++;
				if(yy==8)
				yy=0;
			}
		}
		if(rxcnt>=65)
		rxcnt=0;	
}
void timer0_isr(void) interrupt 1
{
	TH0=0xC0;
	TL0=0;
	if(icnt<21)
		icnt++;
	else
	{
		icnt=0;
		if(layer<=7)
		layer++;
		else
		layer=0;
	}
}
