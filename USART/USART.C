
#define LED_Pin (1<<24)
#include <stdio.h>
#include <string.h>

unsigned int * DBGU_THR = (unsigned int*) 0xfffff21c; // Transmit Holding Register
unsigned int * DBGU_RHR = (unsigned int*) 0xfffff218; //Receive Holding Register



void setup();
void blink();
void delay0();
short int if_ready_reciver();
char get_char();
void put_char(char ch);
//void write_majid();

 
int main()
{
	char *save;
	short int k=1;
        short int true_recived=0;
	char recived_char=0;

	
	setup();
	
		
 for (;;)
	{
		
		blink();
		delay0();
		if(if_ready_reciver()==1)
		{ 
			
			delay0();
			recived_char=get_char();
			delay0();
			put_char(recived_char);
			delay0();
				
		}
		
	
	
	}
	
	return 0;
}



//reciver is ready?
short int if_ready_reciver()
{ 
	short int a=0;
	volatile unsigned int * DBGU_SR = (unsigned int*) 0xfffff214; // Status Register DBGU
	const unsigned int RXRDY=1;
	if((* DBGU_SR & RXRDY)==1) 
	{
		a=1;
	}
	return a;
}

char get_char()
{
	unsigned int * DBGU_RHR = (unsigned int*) 0xfffff218; //Receive Holding Register
	char recived_char=0;
	recived_char=* DBGU_RHR;
	return recived_char;
}

void put_char(char ch)
{
	unsigned int * DBGU_THR = (unsigned int*) 0xfffff21c; // Transmit Holding Register
	* DBGU_THR=ch;
}


void blink()
{
	
  unsigned int * PIO_SODR = (unsigned int * ) 0xfffff430 ;
	unsigned int * PIO_CODR = (unsigned int * ) 0xfffff434 ;
	
	*PIO_SODR = LED_Pin;
		 delay0();
		
        *PIO_CODR = LED_Pin;
		delay0();
}

void delay0()
{
	long int i, j;
	for ( i=0; i<10000; i++)
	{
		for ( j=0; j<100; j++)
		{
		}
	}
}

void setup(){
	
	// SETUP LED
	

  // pmc
	unsigned int * CKGR_MOR= (unsigned int*) 0xfffffc20; //Main Oscillator Register
	volatile unsigned int * PMC_SR= (unsigned int*) 0xfffffc68; // Status Register
	unsigned int * CKGR_PLLR = (unsigned int*) 0xfffffc2c; // PLL Register 
	unsigned int * PMC_MCKR = (unsigned int*) 0xfffffc30; // Master Clock Register
	
	// pIO
	unsigned int * PIO_PDR = (unsigned int*) 0xfffff404;   // PIO desiable register
	unsigned int * PIO_ASR = (unsigned int * ) 0xfffff470;  //Peripheral A Select Register 
	
		unsigned int * PIO_Enable = (unsigned int*) 0xfffff400;  // enable ports
	unsigned int * PIO_OSR = (unsigned int * ) 0xfffff410 ;   // Output Status Register
	//DBGU
	
	unsigned int *DBGU_CR = (unsigned int*) 0xfffff200; //Control Register 
	volatile unsigned int * DBGU_SR = (unsigned int*) 0xfffff214; // Status Register DBGU
	unsigned int  * DBGU_MR = (unsigned int*) 0xfffff204; // Mode Register 
	unsigned int * DBGU_RHR = (unsigned int*) 0xfffff218; //Receive Holding Register
	unsigned int * DBGU_THR = (unsigned int*) 0xfffff21c; // Transmit Holding Register
	unsigned int * DBGU_BRGR = (unsigned int*) 0xfffff220; // Baud Rate Generator Register
	
	
	 
	const unsigned int MainStable=1;
	const unsigned int PLL_LOCK=0x04;
	int counter;
	
	
	// SETUP LED
	
	*PIO_Enable=LED_Pin;
	
	*PIO_OSR=LED_Pin;
	
	// SETUP PMC
	
	
	*CKGR_MOR= 0xff01; // Main Oscillator set
	while((*PMC_SR & MainStable)==0); // wait for oscillator
	
	for (counter=0; counter<10000; counter++);
	
	*CKGR_PLLR= 0x5A3f08;  // PLL Register set, DIV=9, MUL=90 
	
	while((*PMC_SR & PLL_LOCK)==0) {};  // wait for lock
	
  	for (counter=0; counter<10000; counter++);
	
	
	*PMC_MCKR=0x3;          //  master clock set to PLL
	
	//Setup PIO
	
	 * PIO_PDR = 0x600; ;  //p 9 and p 10 (DTXD and DrXD) were desabled from I/O
	 * PIO_ASR= 0x600; //p 9 and p 10 (DTXD and DrXD) were selected for Peripheral A
	 
	  // setup DBGU
	  
	 * DBGU_BRGR=0x24A; // Baud Rate=9600 -> cd=585.93~586 = 0x24A
	 * DBGU_MR=0x800; // Mode Register: no parity : PAR=4, normal mod : CHMODE=0
	 *DBGU_CR=0x50; // Control Register set to enable transmiter and reciver
		
}










