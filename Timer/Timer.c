#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define LED_Pin (1<<24)
#define MAX_STRING 50

volatile  unsigned int * ADC_CR= (unsigned int*) 0xfffd8000; //Control Register
volatile	unsigned int * ADC_CDR0 = (unsigned int*) 0xfffd8040; // Channel Data Register 4
 volatile	unsigned int * ADC_SR = (unsigned int*) 0xfffd801c; // Status Register
volatile  unsigned int * ADC_LCDR= (unsigned int*) 0xfffd8020; //Last Converted Data Register

volatile  unsigned int * TC_CCR = (unsigned int*) 0xfffA0000; //Control Register 
volatile  unsigned int * TC_CMR = (unsigned int*) 0xfffA0004; //Channel Mode Register
volatile  unsigned int * TC_RA = (unsigned int*) 0xfffA0014; // Register A 
volatile  unsigned int * TC_RC = (unsigned int*) 0xfffA001C; //Register C
volatile  unsigned int * TC_SR = (unsigned int*) 0xfffA0020; //Status Register 
volatile  unsigned int * TC_BCR = (unsigned int*) 0xfffA00c0; //Block Control Register


char* itoa(int i, char b[]);
void setup();
void blink();
void delay0();
char get_char();
void put_char(char ch);
void write_majid();
void putString(char* str);
char* addCharacter(char input, int* isFinished);
void doubleNum(char input);

 
int main()
	{ 
	short int hasRecived();
	double mean=0,meanSqured=0,sigma=0;
	int i=0,curentNum=0;
	int converted=0;
	int converted2=0;
	int numberEntered;
	char strConverted[MAX_STRING];
	char meanChar[MAX_STRING];
	char sigmaChar[MAX_STRING];	
	char strSum1[MAX_STRING];
	char strSum2[MAX_STRING];
	double sumSqured=0,sum=0;
	int n=0;
	
 
	
	setup();
	converted= (int)* ADC_LCDR;
	converted2=(int)* ADC_CDR0;
	
		
 putString("\r\n");		
 putString("Test\r\n"); // \r 13, return Carrige \n line feed
	

 
	
 for (;;)
	{
	  char recived_char;
	
		blink();
		delay0();
		
		if(hasRecived()==1)
		{
		  recived_char=get_char();
			put_char(recived_char);
		
		}
   
			

	
	
	
	// Timer
	
	// Generat 1 khz square wave
		
	* TC_CCR =0x05; //softwear triger
	* TC_CMR =0x0046c004; // reset on RC comper mach
	* TC_RA =0x2c; // decimal 44, duty cycle 50%
	* TC_RC =0x58;// decimal 88
  }
	
	return 0;
}

short int hasRecived(){
	volatile unsigned int * DBGU_SR = (unsigned int*) 0xfffff214; // Status Register DBGU
  const unsigned int RXRDY=1;
	if((* DBGU_SR & RXRDY)==1)
		return 1;
	else
		return 0;
	
}


 

char* addCharacter(char input, int* isFinished)
{
	static char buffer [MAX_STRING];
  static	int counter=0;
	
	if (input=='\r')
	{
		counter=0; // sscanf  Salam12 =? Salam, 12
		*isFinished=1;
		
		return buffer;
	}
	

	
	buffer [counter]=input;
	counter++;
	*isFinished=0;
	return buffer;
}

void putString(char* str)
{
	int i=0;
	while(*(str+i)!=0)
	{
		put_char(*(str+i));
		i++;
	}
	
	// print all characters till you hit 0
}

char get_char()
{
	
	volatile unsigned int * DBGU_SR = (unsigned int*) 0xfffff214; // Status Register DBGU
	volatile unsigned int * DBGU_RHR = (unsigned int*) 0xfffff218; //Receive Holding Register
	const unsigned int RXRDY=1;
	char recived_char=0;
	
	
	while ((* DBGU_SR & RXRDY)==0);// wait for a charracter to be received 
	
	recived_char=* DBGU_RHR;
	return recived_char;
}

void put_char(char ch)
{
	
	unsigned int * DBGU_THR = (unsigned int*) 0xfffff21c; // Transmit Holding Register
	volatile unsigned int * DBGU_SR = (unsigned int*) 0xfffff214; // Status Register DBGU
	const unsigned int RXTDY=2;
	
	while ((* DBGU_SR & RXTDY)==0); // wait for transmist to be ready
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
	long int k, j;
	for ( k=0; k<10000; k++)
	{
		for ( j=0; j<100; j++)
		{
		}
	}
}

void setup(){
	
	const unsigned int MainStable=1;
	const unsigned int PLL_LOCK=0x04;
	int counter;
	

  // pmc
	unsigned int * CKGR_MOR= (unsigned int*) 0xfffffc20; //Main Oscillator Register
	volatile unsigned int * PMC_SR= (unsigned int*) 0xfffffc68; // Status Register
	unsigned int * CKGR_PLLR = (unsigned int*) 0xfffffc2c; // PLL Register 
	unsigned int * PMC_MCKR = (unsigned int*) 0xfffffc30; // Master Clock Register
	unsigned int * PMC_PCER = (unsigned int*) 0xfffffc10; //Peripheral Clock Enable Register
	
	

	
	// pIO
	unsigned int * PIO_PDR = (unsigned int*) 0xfffff404;   // PIO desiable register 
	unsigned int * PIO_ASR = (unsigned int * ) 0xfffff470;  //Peripheral A Select Register 
	unsigned int * PIO_BSR = (unsigned int * ) 0xfffff474;  //Peripheral B Select Register 
	
	
	
		unsigned int * PIO_Enable = (unsigned int*) 0xfffff400;  // enable ports
	unsigned int * PIO_OSR = (unsigned int * ) 0xfffff410 ;   // Output Status Register
	//DBGU
	
	unsigned int *DBGU_CR = (unsigned int*) 0xfffff200; //Control Register 
	volatile unsigned int * DBGU_SR = (unsigned int*) 0xfffff214; // Status Register DBGU
	unsigned int  * DBGU_MR = (unsigned int*) 0xfffff204; // Mode Register 
	unsigned int * DBGU_RHR = (unsigned int*) 0xfffff218; //Receive Holding Register
	unsigned int * DBGU_THR = (unsigned int*) 0xfffff21c; // Transmit Holding Register
	unsigned int * DBGU_BRGR = (unsigned int*) 0xfffff220; // Baud Rate Generator Register
	
	//ADC
	
	volatile  unsigned int * ADC_CR= (unsigned int*) 0xfffd8000; //Control Register 
	volatile  unsigned int * ADC_MR= (unsigned int*) 0xfffd8004; // Mode Register
  volatile	unsigned int * ADC_CHER = (unsigned int*) 0xfffd8010; // Channel Enable Register 
  volatile	unsigned int * ADC_SR = (unsigned int*) 0xfffd801c; // Status Register
	volatile  unsigned int * ADC_LCDR= (unsigned int*) 0xfffd8020; //Last Converted Data Register
  volatile	unsigned int * ADC_CDR0 = (unsigned int*) 0xfffd8040; // Channel Data Register 4
	
	// Timer
	
	
//  volatile  unsigned int * TC_CCR = (unsigned int*) 0xfffA0000; //Control Register 
//  volatile  unsigned int * TC_CMR = (unsigned int*) 0xfffA0004; //Channel Mode Register
//  volatile  unsigned int * TC_RA = (unsigned int*) 0xfffA0014; // Register A 
//  volatile  unsigned int * TC_RC = (unsigned int*) 0xfffA001C; //Register C
//  volatile  unsigned int * TC_SR = (unsigned int*) 0xfffA0020; //Status Register 
/   volatile  unsigned int * TC_BCR = (unsigned int*) 0xfffA00c0; //Block Control Register
//	
//	
//	
//	// Timer
//	
//	* TC_CCR =0x05;
//	* TC_CMR =0x0046c004;
//      * TC_RA =0x2c;
//	* TC_RC =0x58;// decimal 88


	
	//ADC
	
	* ADC_MR=0x0f3f0900; // 
	* ADC_CHER=0x80; // CH7 is selected
	* ADC_CR=0x2; // start conversion
	
	
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
		
	* PMC_PCER=(1<<12); //enable timer 0
	
	//Setup PIO
	
	 * PIO_PDR = 0x600; ;  //p 9 and p 10 (DTXD and DrXD) were desabled from I/O
	 * PIO_ASR= 0x600; //p 9 and p 10 (DTXD and DrXD) were selected for Peripheral A
	 * PIO_PDR =1<<0; // enable peripheral for pin0
 	 * PIO_BSR=(1<<0); // a0 for peripheral a0
	
	 
	  // setup DBGU
	  
	 * DBGU_BRGR=0x24A; // Baud Rate=9600 -> cd=585.93~586 = 0x24A
	 * DBGU_MR=0x800; // Mode Register: no parity : PAR=4, normal mod : CHMODE=0
	 *DBGU_CR=0x50; // Control Register set to enable transmiter and reciver
		
}


char* itoa(int i, char b[]){
    char const digit[] = "0123456789";
    char* p = b;
	int shifter = i;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}











