#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define LED_Pin (1<<24)
#define MAX_STRING 50
#define UART_BUFFER_LENGTH 100

char Buffer_int[UART_BUFFER_LENGTH];
char Buffer_int2[UART_BUFFER_LENGTH];
int kk=0;
int bb=0;
int aa=0;


// Timer

volatile  unsigned int * TC_CCR = (unsigned int*) 0xfffA0000; //Control Register 
volatile  unsigned int * TC_CMR = (unsigned int*) 0xfffA0004; //Channel Mode Register
volatile  unsigned int * TC_RA = (unsigned int*) 0xfffA0014; // Register A 
volatile  unsigned int * TC_RC = (unsigned int*) 0xfffA001C; //Register C
volatile  unsigned int * TC_SR = (unsigned int*) 0xfffA0020; //Status Register 
volatile  unsigned int * TC_BCR = (unsigned int*) 0xfffA00c0; //Block Control Register
volatile  unsigned int * TC_CV = (unsigned int*) 0xfffA0010; //Counter ValueRegister
volatile  unsigned int *AIC_ICCR = (unsigned int*) 0xFFFFF128; //Interrupt Clear Command Register
volatile  unsigned int *AIC_EOICR = (unsigned int*) 0xFFFFF130; // End of Interrupt Command Register


volatile  unsigned int * ADC_CR= (unsigned int*) 0xfffd8000; //Control Register
volatile  unsigned int * ADC_CDR0 = (unsigned int*) 0xfffd8040; // Channel Data Register 4
volatile  unsigned int * ADC_SR = (unsigned int*) 0xfffd801c; // Status Register
volatile  unsigned int * ADC_LCDR= (unsigned int*) 0xfffd8020; //Last Converted Data Register
volatile  unsigned int * ADC_MR= (unsigned int*) 0xfffd8004; // Mode Register


__irq void IRQ_Handler_ADC();
__irq void IRQ_Handler_sys();
unsigned char HasReceived2();
void put_char_int(char ch);
char get_char_int();
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
int k;
 int writeCounter;
 int readCounter;
 int writeCounter2;
 int readCounter2;


	
 
 
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
	char tmp[100];
		
		
		/* 
										Iitialization
		*/

  k=0;
	writeCounter=0;
	readCounter=0;

	writeCounter2=0;
	readCounter2=0;
	
	
	setup();
	
	converted= (int)* ADC_LCDR;
	converted2=(int)* ADC_CDR0;
	
	
  // Generat 1 khz square wave
	* TC_CCR =0x05; //softwear triger
	* TC_CMR =0x0046c004; // reset on RC compermach
	* TC_RA =0x2c; // decimal 44
	* TC_RC =0x58;// decimal 88
		
 putString("\r\n");		
 putString("Test\r\n"); // \r 13, return Carrige \n line feed
	
 putString("enter number to calculate average and  standard deviation:\r\n");	
 
 
 for(;;) //asyncron put char and getchar
 {
	 if (HasReceived2()==1)
	 {
		 put_char_int(get_char_int());
	 }
//		putchar(get_char_int());
	 
//	 delay0();
//  	 delay0();
//    sprintf(tmp, "wirteCounter:%d\r\n", writeCounter);	 
//  	 putString(tmp);
 }
 
	
 for (;;)
	{
		int finished;
	  char recived_char;
		int number=0;
		int isFinished=0;
		char *strNumber;
		int h=0;
		blink();
		delay0();
		
		
		if(hasRecived()==1)
		{
			delay0();
		  recived_char= get_char();
			put_char(recived_char);
			putString("\r\n");
			strNumber=addCharacter(recived_char,&isFinished);
	  	
			
		}
		
    if(isFinished==1)
		{
			number=atoi(strNumber);
			putString("you entered: \r\n");
			putString(strNumber);
			putString("\r\n");
			
			for(i=0;i<number;i++)
			{
				 * ADC_MR=1<<0; // start conversion
	      	while((* ADC_SR & (1<<16))==0){}; // waite untill receive
	      	converted= (int)* ADC_LCDR;
					sprintf(strConverted,"%d\r\n\r\n",converted);
					putString(strConverted);

					 
					sum+=1.0*converted;
					sprintf(strSum1,"%lf\r\n",sum);
					putString(strSum1);

					 
					sumSqured+=1.0* (converted*converted);
					sprintf(strSum2,"%lf\r\n\r\n",sumSqured);
					putString(strSum2);
			}
			mean=sum/number;
			meanSqured=sumSqured/number;
			sigma=sqrt(meanSqured-(mean*mean));
			sprintf(meanChar,"%lf",mean);
			sprintf(sigmaChar,"%lf",sigma);

			putString("\r\n");		
			putString("mean is : ");
			putString(meanChar);
			putString("\r\n");	
			putString("sigma is : ");
			putString(sigmaChar);		

		}
			
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
	const unsigned int RXRDY1=1;
	char recived_char=0;
	
	
	while ((* DBGU_SR & RXRDY1)==0);// wait for a charracter to be received 
	
	recived_char=* DBGU_RHR;
	return recived_char;
}

unsigned char HasReceived2()
{
	int compare;
	
  compare= writeCounter - readCounter;
	compare = compare<0 ? compare+UART_BUFFER_LENGTH : compare;
	
	if (compare>0)
		return 1;
	else
		return 0;
}

char get_char_int()
{
	char out;
	int compare =0;
	
	do
	{
		compare= writeCounter - readCounter;
		compare = compare<0 ? compare+UART_BUFFER_LENGTH : compare;
	}
	while(compare<=0);

	out = Buffer_int[readCounter++];
	
	if (readCounter>=UART_BUFFER_LENGTH)
		readCounter=0;
	    
	return out;	
}

void put_char(char ch)
{
	
	unsigned int * DBGU_THR = (unsigned int*) 0xfffff21c; // Transmit Holding Register
	volatile unsigned int * DBGU_SR = (unsigned int*) 0xfffff214; // Status Register DBGU
	const unsigned int RXTDY2=2;
	
	while ((* DBGU_SR & RXTDY2)==0); // wait for transmist to be ready
	* DBGU_THR=ch;
}

void put_char_int(char ch)
{
	
	volatile unsigned int * DBGU_IER = (unsigned int*) 0xfffff208; //Interrupt transmit Enable Register
	
	Buffer_int2[writeCounter2++]=ch;

	if(writeCounter2>=UART_BUFFER_LENGTH)
		writeCounter2=0;
	
	* DBGU_IER=2;  //important pay attention: at the end of function must enable transmit interupt
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
		for ( j=0; j<10; j++)
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
	unsigned int * PMC_PCER = (unsigned int*) 0xfffffc10; //Peripheral Clock Enable Register
	volatile unsigned int * PMC_IER= (unsigned int*) 0xfffffc60; // Interrupt Enable Register
	
	// pIO
	unsigned int * PIO_PDR = (unsigned int*) 0xfffff404;   // PIO desiable register
	unsigned int * PIO_ASR = (unsigned int * ) 0xfffff470;  //Peripheral A Select Register 
	
		unsigned int * PIO_Enable = (unsigned int*) 0xfffff400;  // enable ports
	unsigned int * PIO_OSR = (unsigned int * ) 0xfffff410 ;   // Output Status Register
	//DBGU
	
	unsigned int *DBGU_CR = (unsigned int*) 0xfffff200; //Control Register 
	volatile unsigned int * DBGU_IER = (unsigned int*) 0xfffff208; //Interrupt Enable Register
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
	volatile  unsigned int * ADC_IER= (unsigned int*) 0xfffd8024; //Interrupt Enable Register
	
	
	//AIC : interupt
	volatile  unsigned int * AIC_SMR1= (unsigned int*)0xFFFFF004; //Source Mode Register 1
	volatile  unsigned int * AIC_SVR1= (unsigned int*) 0xFFFFF084; //Source Vector Register
	volatile  unsigned int * AIC_SMR4= (unsigned int*)0xFFFFF010; //Source Mode Register 4
	volatile  unsigned int * AIC_SVR4= (unsigned int*) 0xFFFFF090; //Source Vector Register 
	volatile	unsigned int * AIC_IECR = (unsigned int*) 0xFFFFF120; //Interrupt Enable Command Register


	
	
	 
	const unsigned int MainStable=1;
	const unsigned int PLL_LOCK=0x04;
	int counter;
	
	//ADC
	
	* ADC_MR=0x0f3f0900; // 
	* ADC_CHER=0x10; // CH7 is selected
	* ADC_IER=0x10;  // interupt   CH4 is enabaledde
//	* ADC_CR=0x2; // start conversion


// AIC : interupt
  *AIC_SMR1=0x67; // priority high=7 , posative age triger 0110
	*AIC_SMR4=0x62; // priority =2 , posative age triger 0110
	*AIC_SVR4=(unsigned int)IRQ_Handler_ADC; 
	*AIC_SVR1=(unsigned int)IRQ_Handler_sys;
	*AIC_IECR =0x12; // interupt pid=4 for adc is enabaled and system interupt for dbgu reciver
	

	
	
	
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
	* PMC_PCER=(1<<12); //enable timer 0
	
	*PMC_MCKR=0x3;          //  master clock set to PLL
	//* PMC_IER=0x4; //enabaled iterupt pll
	
	//Setup PIO
	
	 * PIO_PDR = 0x600; ;  //p 9 and p 10 (DTXD and DrXD) were desabled from I/O
	 * PIO_ASR= 0x600; //p 9 and p 10 (DTXD and DrXD) were selected for Peripheral A
	 
	  // setup DBGU
	  
	 * DBGU_BRGR=0x24A; // Baud Rate=9600 -> cd=585.93~586 = 0x24A
	 * DBGU_MR=0x800; // Mode Register: no parity : PAR=4, normal mod : CHMODE=0
	 *DBGU_CR=0x50; // Control Register set to enable transmiter and reciver
	 * DBGU_IER=0x1;// transmit and recived interupt are enabaled
		
		
}

__irq void IRQ_Handler_ADC()
{
	volatile	unsigned int *AIC_EOICR = (unsigned int*) 0xFFFFF130; // End of Interrupt Command Register

	
	//int i=6;
 // putString("adc has finished adc is:");
	*AIC_EOICR=0x10;
	
	
}
__irq void IRQ_Handler_sys()
{
	volatile unsigned int * DBGU_IDR = (unsigned int*) 0xfffff20c; //Interrupt disable Register
  volatile unsigned int * DBGU_IMR = (unsigned int*) 0xfffff210; //Interrupt Mask Register
	volatile unsigned int * DBGU_RHR = (unsigned int*) 0xfffff218; //Receive Holding Register
	volatile	unsigned int *AIC_EOICR = (unsigned int*) 0xFFFFF130; // End of Interrupt Command Register
	volatile unsigned int * DBGU_THR = (unsigned int*) 0xfffff21c; // Transmit Holding Register
	volatile unsigned int * DBGU_SR = (unsigned int*) 0xfffff214; // Status Register DBGU
	
	
	
 if(((*DBGU_SR) & (1<<0))==1)	// if any char recived from DGBU
 {
	Buffer_int[writeCounter++]=*DBGU_RHR;
	
	if(writeCounter>=UART_BUFFER_LENGTH)
	{
	  	writeCounter=0;
	}
 }
 
if(((*DBGU_SR) & (1<<1))==2 &&( writeCounter2>readCounter2 || ((writeCounter2-readCounter2) == (1- UART_BUFFER_LENGTH)) ))	// if any char is ready to transmit from DGBU to computer
{
	//bb++;
	*DBGU_THR=Buffer_int2[readCounter2++];
	
	if(readCounter2>=UART_BUFFER_LENGTH)
		readCounter2=0;
	
	if (writeCounter2==readCounter2) // disable transmit interupt
		 *DBGU_IDR=2;
	
}

	*AIC_EOICR=0x10;
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











