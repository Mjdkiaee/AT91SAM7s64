
#define LED_Pin (1<<24);

void setup();
void blink();
void delay0();


int main()
{

	setup();
	
	
	for (;;)
	{
		blink();
		delay0();
	}
	
	
	return 0;
}
// uart 2 
void blink()
{
	
 volatile  unsigned int * PIO_SODR = (unsigned int * ) 0xfffff430 ;
 volatile	unsigned int * PIO_CODR = (unsigned int * ) 0xfffff434 ;
	
	*PIO_SODR = LED_Pin;
		 delay0();
		
        *PIO_CODR = LED_Pin;
		delay0();
}

void delay0()
{
	long int i, j;
	for ( i=0; i<1000; i++)
	{
		for ( j=0; j<1000; j++)
		{
		}
	}
}

void setup(){
	
	// SETUP LED
	
	volatile unsigned int * PIO_Enable = (unsigned int*) 0xfffff400;
	volatile unsigned int * PIO_OSR = (unsigned int * ) 0xfffff410 ;

	
	
	// SETUP LED
	
	*PIO_Enable=LED_Pin;
	
	*PIO_OSR=LED_Pin;

	
}