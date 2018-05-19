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
	TMOD=0x21;//��ʱ��1 ��ʽ2 ���ڲ��������ʣ���ʱ��0 ��ʽ1 ����ˢ����ʾ
	TH1=0XFF;  //
	TL1=0XFF;
	TH0=0xC0;
	TL0=0;
	TR1=1;		//����ʱ��1
	TR0=1;

	SCON=0X50;//���ڹ����ڷ�ʽ1 �ɽ���REN=1
	PCON|=0x80;//�����ʼӱ� 

	EA=1;		//��ȫ���ж�
	ES=1;		//�������ж�
	ET0=1;		//����ʱ��0�ж�
	PS=1;		//���ô����ж�����
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
	init();				//��ʼ��
	while(1)
	{		
	//	P1=0;
		for (i=0;i<8;i++)
		{
			P2=1<<i;		//�����
			delay(10);
			P0=display[layer][i];//λѡ��
			delay(10);
		}
		
		P1=1<<layer;		//����ʾ
			//P1=layer;
//	all_led(0);
	if (layer<7) 
		layer++;
	else 
		layer=0


	}


}
void rxd() interrupt 4		  //�����жϷ������
{								  
		uchar temp=0;			
		RI=0;
		++rxcnt;
		if(rxcnt>=2)
		{			
			temp=SBUF;			//�������յ�������
			display[xx][yy]=temp;	 //��˳�������ʾ������
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
