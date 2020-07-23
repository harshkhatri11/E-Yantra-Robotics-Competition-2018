
/*
 * Team Id: <169>
 * Author List: Harsh Khatri,Prayushi Rana
 * Filename: <serial_communication_usb-rs232.c>
 * Theme: <eYRC-homecoming>
 * Functions: buzzer_pin_config (),port_init(),buzzer_on (),buzzer_off (),uart2_init(),
 			SIGNAL(SIG_USART2_RECV),init_devices(),have_d(),char get_data(),
			get_data_array(),get_length(),
 * Global Variables:unsigned char data;int array_length;int data_array[10];
					int i;int have_data;
 */ 


#include <math.h> //included to support power function
#include "lcd.c"

void port_init();
void timer5_init();
void velocity(unsigned char, unsigned char);
void motors_delay();

unsigned char ADC_Conversion(unsigned char);
unsigned char ADC_Value;
unsigned char flag = 0;
unsigned char Left_white_line = 0;
unsigned char Center_white_line = 0;
unsigned char Right_white_line = 0;

//ADC pin configuration
void adc_pin_config (void)
{
 DDRF = 0x00;
 PORTF = 0x00;
 DDRK = 0x00;
 PORTK = 0x00;
}

//Function to configure ports to enable robot's motion
void motion_pin_config (void)
{
 DDRA = DDRA | 0x0F;
 PORTA = PORTA & 0xF0;
 DDRL = DDRL | 0x18;   //Setting PL3 and PL4 pins as output for PWM generation
 PORTL = PORTL | 0x18; //PL3 and PL4 pins are for velocity control using PWM.
}

//Function to Initialize PORTS
void lcd_port_config (void)
{
 DDRC = DDRC | 0xF7; //all the LCD pin's direction set as output
 PORTC = PORTC & 0x80; // all the LCD pins are set to logic 0 except PORTC 7
}
void port_init()
{
	lcd_port_config();
	//lcd_init();
	adc_pin_config();
	buzzer_pin_config();
	motion_pin_config();
}


// Timer 5 initialized in PWM mode for velocity control
// Prescale:256
// PWM 8bit fast, TOP=0x00FF
// Timer Frequency:225.000Hz
void timer5_init()
{
	TCCR5B = 0x00;	//Stop
	TCNT5H = 0xFF;	//Counter higher 8-bit value to which OCR5xH value is compared with
	TCNT5L = 0x01;	//Counter lower 8-bit value to which OCR5xH value is compared with
	OCR5AH = 0x00;	//Output compare register high value for Left Motor
	OCR5AL = 0xFF;	//Output compare register low value for Left Motor
	OCR5BH = 0x00;	//Output compare register high value for Right Motor
	OCR5BL = 0xFF;	//Output compare register low value for Right Motor
	OCR5CH = 0x00;	//Output compare register high value for Motor C1
	OCR5CL = 0xFF;	//Output compare register low value for Motor C1
	TCCR5A = 0xA9;	/*{COM5A1=1, COM5A0=0; COM5B1=1, COM5B0=0; COM5C1=1 COM5C0=0}
 					  For Overriding normal port functionality to OCRnA outputs.
				  	  {WGM51=0, WGM50=1} Along With WGM52 in TCCR5B for Selecting FAST PWM 8-bit Mode*/

	TCCR5B = 0x0B;	//WGM12=1; CS12=0, CS11=1, CS10=1 (Prescaler=64)
}

void adc_init()
{
	ADCSRA = 0x00;
	ADCSRB = 0x00;		//MUX5 = 0
	ADMUX = 0x20;		//Vref=5V external --- ADLAR=1 --- MUX4:0 = 0000
	ACSR = 0x80;
	ADCSRA = 0x86;		//ADEN=1 --- ADIE=1 --- ADPS2:0 = 1 1 0
}

//Function For ADC Conversion
unsigned char ADC_Conversion(unsigned char Ch)
{
	unsigned char a;
	if(Ch>7)
	{
		ADCSRB = 0x08;
	}
	Ch = Ch & 0x07;
	ADMUX= 0x20| Ch;
	ADCSRA = ADCSRA | 0x40;		//Set start conversion bit
	while((ADCSRA&0x10)==0);	//Wait for conversion to complete
	a=ADCH;
	ADCSRA = ADCSRA|0x10; //clear ADIF (ADC Interrupt Flag) by writing 1 to it
	ADCSRB = 0x00;
	return a;
}

//Function To Print Sesor Values At Desired Row And Coloumn Location on LCD
void print_sensor(char row, char coloumn,unsigned char channel)
{
	int v=0;
	ADC_Value = ADC_Conversion(channel);
	/*if(ADC_Value<60)
	{
		v=0;
	}
	else
	{
		v=1;
	}
	*/
	v=ADC_Value;
	lcd_print(row, coloumn, v, 3);
	
}

//Function for velocity control
void velocity (unsigned char left_motor, unsigned char right_motor)
{
	OCR5AL = (unsigned char)left_motor;
	OCR5BL = (unsigned char)right_motor;
}

//Function used for setting motor's direction
void motion_set (unsigned char Direction)
{
 unsigned char PortARestore = 0;

 Direction &= 0x0F; 		// removing upper nibbel for the protection
 PortARestore = PORTA; 		// reading the PORTA original status
 PortARestore &= 0xF0; 		// making lower direction nibbel to 0
 PortARestore |= Direction; // adding lower nibbel for forward command and restoring the PORTA status
 PORTA = PortARestore; 		// executing the command
}

void forward (void)
{
  motion_set (0x06);
}
void right (void)
{
  motion_set (0x05);
}
void backward(void)
{
	motion_set(0x09);
}
void stop (void)
{
  motion_set (0x00);
}

void init_devices (void)
{
 	cli(); //Clears the global interrupts
	port_init();
	adc_init();
	timer5_init();
	sei();   //Enables the global interrupts
}
void buzzer_pin_config (void)
{
 DDRC = DDRC | 0x08;		//Setting PORTC 3 as output
 PORTC = PORTC & 0xF7;		//Setting PORTC 3 logic low to turnoff buzzer
}

void buzzer_on (void)
{
 unsigned char port_restore = 0;
 port_restore = PINC;
 port_restore = port_restore | 0x08;
 PORTC = port_restore;
}

void buzzer_off (void)
{
 unsigned char port_restore = 0;
 port_restore = PINC;
 port_restore = port_restore & 0xF7;
 PORTC = port_restore;
}

//Main Function
int main(){
	move(1);
	turn_r();
	move(1);
	turn_r();
	move(1);
	turn_r();
	move(1);
	
}
int turn_r()
{
	int flag=1;
	int f=0;
	int white=30;
	
	while(flag==1)
	{
		
			//f++;
			right();
			velocity(200,200);
		//if(f>4){flag++;}	
		Left_white_line = ADC_Conversion(3);	//Getting data of Left WL Sensor
		Center_white_line = ADC_Conversion(2);	//Getting data of Center WL Sensor
		Right_white_line = ADC_Conversion(1);	//Getting data of Right WL Sensor
		flag++	;
		_delay_ms(500);	
		Center_white_line = ADC_Conversion(2);	//Getting data of Center WL Sensor
		while(Center_white_line<white)
		{
			_delay_ms(50);
			Center_white_line = ADC_Conversion(2);	//Getting data of Center WL Sensor
		
		}
		
	
	}		//delay
	stop();
	
}

	
int move(int node)
{
	init_devices();
	lcd_set_4bit();
	lcd_init();
	int threshold=50;
	int white=30;
	int l_thresh=30;
	int c_thresh=30;
	int flag=1;
	int f=0;
	int  node_counter=0;
	
	while(flag==1&&node_counter<node)
	{
		

		Left_white_line = ADC_Conversion(3);	//Getting data of Left WL Sensor
		Center_white_line = ADC_Conversion(2);	//Getting data of Center WL Sensor
		Right_white_line = ADC_Conversion(1);	//Getting data of Right WL Sensor

		f=0;

		print_sensor(1,1,3);	//Prints value of White Line Sensor1
		print_sensor(1,5,2);	//Prints Value of White Line Sensor2
		print_sensor(1,9,1);	//Prints Value of White Line Sensor3
		/*
		if(Center_white_line<c_thresh&&f==0)
		{
			f=1;
			forward();
			velocity(100,120);
		}
		if(Center_white_line>c_thresh&&f==0)
		{
			f=1;
			forward();
			velocity(120,100);
		}
		if(Left_white_line>threshold&&
		Center_white_line>threshold&&
		Right_white_line>threshold&&f==0)
		{
			f=1;
			//forward();
			//velocity(200,200);
			stop();
			flag=2;
			buzzer_on();
			_delay_ms(10);		//delay
			buzzer_off();
			node_counter++;
		}
		*/
		
		if(Left_white_line<threshold&&
		Center_white_line<threshold&&
		Right_white_line>threshold&&f==0)
		{
			f=1;
			forward();
			velocity(180,150);
		}
		if(Left_white_line<threshold&&
		Center_white_line>threshold&&
		Right_white_line>threshold&&f==0)
		{
			f=1;
			forward();
			velocity(200,100);
		}
		
		if(Left_white_line<threshold&&
		Center_white_line>threshold&&
		Right_white_line<threshold&&f==0)
		{
			f=1;
			forward();
			velocity(200,200);
		}
		
		if(Left_white_line>threshold&&
		Center_white_line<threshold&&
		Right_white_line<threshold&&f==0)
		{
			f=1;
			forward();
			velocity(150,180);
		}
		if(Left_white_line>threshold&&
		Center_white_line>threshold&&
		Right_white_line<threshold&&f==0)
		{
			f=1;
			forward();
			velocity(100,200);
		}
		
		if(Left_white_line>threshold&&
		Center_white_line>threshold&&
		Right_white_line>threshold&&f==0)
		{
			f=1;
			forward();
			velocity(200,200);
			buzzer_on();
			_delay_ms(10);		//delay
			buzzer_off();
			node_counter++;
		}
		
		if(Left_white_line<white&&
		Center_white_line<white&&
		Right_white_line<white&&f==0)
		{
			f=1;
			flag++;
			stop();
			//velocity(200,200);
			buzzer_on();
			_delay_ms(100);		//delay
			buzzer_off();
			//forward();
			//	velocity(0,0);
		}
		
	}
			f=1;
			flag++;
			stop();
			//velocity(200,200);
			buzzer_on();
			_delay_ms(100);		//delay
			buzzer_off();
			//forward();
			//	velocity(0,0);
		
	lcd_print(2, 1, node_counter, 3);
	lcd_print(2, 5, node_counter, 3);
	lcd_print(2, 9, node_counter, 3);

	forward();
	velocity(200,200);
	_delay_ms(700);		//delay
	stop();		
}
