#include <LPC214X.H>
int adc_get_val(void);
int main ()
{
	int a;
	PINSEL0=0x30000;
	IODIR0=IODIR0 | 0x10;
	AD1CR=0x200104;
	while(1)
	{
		a = adc_get_val();
		if(a>512)
		{
			IOSET0=0x10;
		}
		else
		{
			IOCLR0=0x10;
		}
	}
}

int adc_get_val()
{
	int val ;
	AD1GDR = AD1GDR & 0X7EfEfff;
	AD1CR = AD1GDR | (1<<24);
	while (! (AD1GDR & (1<<31)));
	val =(AD1GDR & 0xffc0 )>>6;
	return val ;
}
