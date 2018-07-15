
#include<REG51.h>

const unsigned char code flook [][5] =
{
	{ 0x3E, 0x51, 0x49, 0x45, 0x3E }, // 0
	
	{ 0x00, 0x42, 0x7F, 0x40, 0x00 }, // 1
	
	{ 0x42, 0x61, 0x51, 0x49, 0x46 }, // 2
	
	{ 0x21, 0x41, 0x45, 0x4B, 0x31 }, // 3	 
	
	{ 0x18, 0x14, 0x12, 0x7F, 0x10 }, // 4
	
	{ 0x27, 0x45, 0x45, 0x45, 0x39 }, // 5
	
	{ 0x3C, 0x4A, 0x49, 0x49, 0x30 }, // 6
	
	{ 0x01, 0x71, 0x09, 0x05, 0x03 }, // 7
	
	{ 0x36, 0x49, 0x49, 0x49, 0x36 }, // 8
	
	{ 0x06, 0x49, 0x49, 0x29, 0x1E } // 9
};

const unsigned char code colon[]={0x00,0x18,0x00};

unsigned char a,b,c,d,e,f;

bit tog;

void Int0(void) interrupt 0 using 2
{
	EX0=0;   // Avoid another INT1* until we are completely finished...	
	TH0=0xA4;	
	TR0=1;    // Start timer 0
}


void timer0(void) interrupt 1  using 3
{

	static unsigned char i,k;

	k++;

	if(k==12)	
	{
	
		k=0;
		
		if(i<5)	  //hours ten digit	
		{		
		P0=~(flook[f][i]);		
		i++;		
		}
	
		else if(i==5)//space		
		{		
		P0=0xFF;		
		i++;		
		}
	
		else if(i<11)//space		
		{		
		P0=~(flook[(e%10)][i-6]);		
		i++;							
		}
		
		else if(i<14)//colon		
		{		
			if(tog==1)			
			{			
				P0=~(colon[i-11]);			
			}
						
			else			
			{			
				P0=0xFF;			
			}
			i++;
		}
		
		else if(i<19)//min tens		
		{								 
			P0=~(flook[d][i-14]);		
			i++;		
		}
		
		else if(i==19)//space		
		{		
			P0=0xFF;				
			i++;	
		}
		
		else if(i<25)//min one								 
		{
		
			P0=~(flook[c][i-20]);		
			i++;		
		}
				
		else if(i<28)//colon		
		{		
			if(tog==1)
			
			{			
				P0=~(colon[i-25]);			
			}
			
			else			
			{			
				P0=0xFF;			
			}
			i++;
		}
	
		else if(i<33)//sec tens
		{
		
			P0=~(flook[b][i-28]);
			i++;
		}
		
		else if(i==33)//space
		{
			P0=0xFF;
			i++;
		}
		
		else if(i<39)//sec ones		
		{
			P0=~(flook[a][i-34]);
			i++;
		}

		else		
		{
			TR0=0;
			P0=0xFF;
			EX0=1;
			i=0;
		}	
	}
}

void delay()
{
	int fi,fj;
	
	for(fi=0;fi<160;fi++)
	{
		for(fj=0;fj<1250;fj++)	;
	}
}

void main()

{
	TMOD=0x02;
	
	TH0=0xA4;
	
	EX0   = 1;                      /* External interrupt 0 enable              */
	
	IT0   = 1;                      /* External interrupt 0 Edge sesitive       */
	
	ET0   = 1;                      /* Enable Timer 0 interrupts                */
	
	PT0   = 1;                      /* Timer 0 high priority                    */
	
	PX0   = 1;                      /* External interrupt 0 high priority       */
	
	/*-----------------------------------------------
	
	Configure INT0 (external interrupt 0) to generate
	
	an interrupt on the falling-edge of /INT0 (P3.2).
	
	Enable the EX0 interrupt and then enable the
	
	global interrupt flag.
	
	-----------------------------------------------*/
	
	EX0 = 1;   // Enable EX0 Interrupt
	
	EA = 1;    // Enable Global Interrupt Flag
	
	P0=0xFF;
	
	P3=0xFF;
		
		a=0;	  //sec one's
		
		b=0;	  //sec ten's
		
		c=0;	  //min one's
		
		d=0;	  //min ten's
		
		e=0;	  //hr one's
		
		f=0;	  //hr ten's
		
		tog=0;
	
	while(1)	
	{	
		delay();
		
		tog=~tog;
		
		a++;
		
		if(a>9)
		
		{		
			a=0;			
			b++;		
		}
		
		if(b>5)		
		{		
			b=0;		
			c++;		
		}
		
		if(c>9)		
		{		
			c=0;		
			d++;		
		}
		
		if(d>5)		
		{		
			d=0;		
			e++;		
		}
				
		if(e>12)		
		{		
			e=0;		
		}
		
		if(e>9)		
		{		
			f=1;		
		}
		
		else		
		{		
			f=0;		
		}	
	}
}