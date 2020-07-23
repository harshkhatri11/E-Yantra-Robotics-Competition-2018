/*
 * Team Id: <169>
 * Author List: krishnaraj solanki,khushboo patel,Harsh khatri
 * Filename: <motion_prg.c>
 * Theme: <eYRC-homecoming>
 * Functions: lcd_port_comfig(void),in_device(void),adc_pin_config (void),adc_init(),in_devices(void)
 				,linear_distance_mm(),forward_mm(),back_mm(),angle_rotate(),left_degrees(),right_degrees(),
				soft_left_degrees(),soft_right_degrees(),soft_left_2_degrees(),soft_right_2_degrees(),
				ADC_Conversion(),next_node(void),
 * Global Variables: uint8_t EEMEM NonVolatileString [30],int counter,unsigned char data,
 */ 


#define F_CPU 14745600
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <math.h> //included to support power function
#include "lcd.c"
#include <avr/eeprom.h>
#include "Serial_Communication_USB-RS232.c"
#include "Velocity_Control_using_PWM.c"
#include "coordinate_system.c"
#include "animal_pickup_and_drop.c"
unsigned char data;
int counter=0;
volatile unsigned long int ShaftCountLeft = 0; //to keep track of left position encoder
volatile unsigned long int ShaftCountRight = 0; //to keep track of right position encoder
volatile unsigned int Degrees; //to accept angle in degrees for turning

void lcd_port_comfig(void);
void in_device(void);


void adc_pin_config (void)
{
 DDRF = 0x00;
 PORTF = 0x00;
 DDRK = 0x00;
 PORTK = 0x00;
}

void adc_init()
{
	ADCSRA = 0x00;
	ADCSRB = 0x00;		//MUX5 = 0
	ADMUX = 0x20;		//Vref=5V external --- ADLAR=1 --- MUX4:0 = 0000
	ACSR = 0x80;
	ADCSRA = 0x86;		//ADEN=1 --- ADIE=1 --- ADPS2:0 = 1 1 0
}
	void lcd_port_config (void)
	{
 	DDRC = DDRC | 0xF7; //all the LCD pin's direction set as output
 	PORTC = PORTC & 0x80; // all the LCD pins are set to logic 0 except PORTC 7
	}	

	void left_encoder_pin_config (void)
	{
		DDRE  = DDRE & 0xEF;  //Set the direction of the PORTE 4 pin as input
		PORTE = PORTE | 0x10; //Enable internal pull-up for PORTE 4 pin
	}

	//Function to configure INT5 (PORTE 5) pin as input for the right position encoder
	void right_encoder_pin_config (void)
	{
		DDRE  = DDRE & 0xDF;  //Set the direction of the PORTE 4 pin as input
		PORTE = PORTE | 0x20; //Enable internal pull-up for PORTE 4 pin
	}	


	//ISR for right position encoder
	ISR(INT5_vect)
	{
		ShaftCountRight++;  //increment right shaft position count
	}	


	//ISR for left position encoder
	ISR(INT4_vect)
	{
		ShaftCountLeft++;  //increment left shaft position count
	}
	
	void left_position_encoder_interrupt_init (void) //Interrupt 4 enable
	{
		cli(); //Clears the global interrupt
		EICRB = EICRB | 0x02; // INT4 is set to trigger with falling edge
		EIMSK = EIMSK | 0x10; // Enable Interrupt INT4 for left position encoder
		sei();   // Enables the global interrupt
	}	

	void right_position_encoder_interrupt_init (void) //Interrupt 5 enable
	{
		cli(); //Clears the global interrupt
		EICRB = EICRB | 0x08; // INT5 is set to trigger with falling edge
		EIMSK = EIMSK | 0x20; // Enable Interrupt INT5 for right position encoder
		sei();   // Enables the global interrupt
	}
void in_devices(void)
{
	cli();
	lcd_port_config();
	lcd_set_4bit();
	lcd_init();
	adc_pin_config();
	adc_init();
	init_devices();
		port_init_servo();
 		timer1_init();
	init_devices_motor();
	left_encoder_pin_config(); //left encoder pin config
	right_encoder_pin_config(); //right encoder pin config
	left_position_encoder_interrupt_init();
	right_position_encoder_interrupt_init();
	
	sei();

}


	//Function used for moving robot by specified distance
	void linear_distance_mm(unsigned int DistanceInMM)
	{
		float ReqdShaftCount = 0;
		unsigned long int ReqdShaftCountInt = 0;
	
		ReqdShaftCount = DistanceInMM / 5.338; // division by resolution to get shaft count
		ReqdShaftCountInt = (unsigned long int) ReqdShaftCount;
	
		ShaftCountRight = 0;
		while(1)
		{
			if(ShaftCountRight > ReqdShaftCountInt)
			{
				break;
			}
		}
		stop(); //Stop robot
	}

	//Function used for moving robot forward by specified distance
	void forward_mm(unsigned int DistanceInMM)
	{
		forward();
		linear_distance_mm(DistanceInMM);
	}
	
	//Function used for moving robot back by specified distance
	void back_mm(unsigned int DistanceInMM)
	{
		back();
		linear_distance_mm(DistanceInMM);
	}

	//Function used for turning robot by specified degrees
	void angle_rotate(unsigned int Degrees)
	{
		float ReqdShaftCount = 0;
		unsigned long int ReqdShaftCountInt = 0;	

		ReqdShaftCount = (float) Degrees/ 4.090; // division by resolution to get shaft count
		ReqdShaftCountInt = (unsigned int) ReqdShaftCount;
		ShaftCountRight = 0;
		ShaftCountLeft = 0;	

		while (1)
		{
			if((ShaftCountRight >= ReqdShaftCountInt) | (ShaftCountLeft >= ReqdShaftCountInt))
			break;
		}
		stop(); //Stop robot
	}
	
	//Function used for moving robot left by specified angle
	void left_degrees(unsigned int Degrees)
	{
		// 88 pulses for 360 degrees rotation 4.090 degrees per count
		left(); //Turn left
		angle_rotate(Degrees);
	}

	//Function used for moving robot right by specified angle
	void right_degrees(unsigned int Degrees)
	{
		// 88 pulses for 360 degrees rotation 4.090 degrees per count
		right(); //Turn right
		angle_rotate(Degrees);
	}

	
	//Function used for moving robot soft left by specified angle
	void soft_left_degrees(unsigned int Degrees)
	{
		// 176 pulses for 360 degrees rotation 2.045 degrees per count
		soft_left(); //Turn soft left
		Degrees=Degrees*2;
		angle_rotate(Degrees);
	}
	
	//Function used for moving robot soft right by specified angle
	void soft_right_degrees(unsigned int Degrees)
	{
		// 176 pulses for 360 degrees rotation 2.045 degrees per count
		soft_right();  //Turn soft right
		Degrees=Degrees*2;
		angle_rotate(Degrees);
	}
	
	//Function used for moving robot soft left backward by specified angle
	void soft_left_2_degrees(unsigned int Degrees)
	{
		// 176 pulses for 360 degrees rotation 2.045 degrees per count
		soft_left_2(); //Turn reverse soft left
		Degrees=Degrees*2;
		angle_rotate(Degrees);
	}	
	
	//Function used for moving robot soft right backward by specified angle
	void soft_right_2_degrees(unsigned int Degrees)
	{
		// 176 pulses for 360 degrees rotation 2.045 degrees per count
		soft_right_2();  //Turn reverse soft right
		Degrees=Degrees*2;
		angle_rotate(Degrees);
	}


/*
* Function Name:ADC_Conversion
* Input: unsigned char gives the channel of the sensor
* Output: unsigned char gives the digital value of the sensor's analog reading
* Logic: gets analog valuse from specified channal through the input variable and returns
* the value as output
* Example Call: ADC_Conversion(1)
*/
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




int c=0;
uint8_t EEMEM NonVolatileString [200];

/*
* Function Name: main
* Input: None
* Output: int to inform the caller that the program exited correctly or
* Incorrectly (C code standard)
* Logic: the main program is divided into 2 parts: 1 takes serial input
* from the computer and another performs the task on that input.this is achived be
* storing a variable in the EEPROM of the device so that it wont get lost while reset/restart
* of the robot. IF part takes serial input,ELSE performs the task,
* robot toggels between IF and ELSE every time it restarts/resets 
* Example Call: Called automatically by the Operating System
*
*/
int main(void){
		in_devices();
		servo_reset();
		lcd_cursor(1,1);
		lcd_string("Serial input  ");

	//stores the input string recivied via serial communication
	char string [200];
	
	//stores the input character recivied via serial communication
	unsigned char input_data;
	
	//used to clear the eeprom after the task is performed ones
	char nullstring[200];
	
	//input string index variable and flag
	int string_index=0,flag=1;
	
	//c is flag variable which toggels between IF and ELSE
	uint8_t c ;
	
	//reads flag variable data from eeprom at loaction 264
	c = eeprom_read_byte ((const uint8_t*)264);
	if(c==0x55)
	{
		//changes the valuse of flag in EEPROM so that next time program so in the ELSE statement
		//###########################
		c = 0x15 ;
		eeprom_write_byte (( uint8_t *) 264, c );
		//###########################
		
		lcd_cursor(1,1);
		lcd_string("Serial input :- ");
		lcd_cursor(2,1);
		string_index=0;
		while(flag==1)
		{
			while(have_d()==0){
				lcd_cursor(2,1);//infinite loop while there is no data recived
			}
			
			//gets the recived data and appends it to the input string
			input_data=get_data();
			string[string_index++]=input_data;
			string[string_index]='\0';
			
			//stops the input of data if latest input character is #
			if(input_data=='#')
			{
				flag=2;
			}
		}
		string[string_index]='\0';
		/*
		//prints the input data of lcd 
		//################################
		lcd_cursor(1,1);
		lcd_string("                ");
		lcd_cursor(2,1);
		lcd_string("                ");
		lcd_cursor(1,1);
		
		int j=0;
		while(string[j]!='\0'){
			if(string[j]=='\n'){
			lcd_cursor(1,1);
			}
			if(j>38){
			lcd_cursor(2,1);
			
			}
			else{
			lcd_wr_char(string[j]);
			}
			j++;
		}
		//################################
		*/
		//stored the input string in the EEPROM from 0 to 200th location
		eeprom_write_block(string,0,200);
	}

	/*
	*	ELSE part retrives the input string from the EEPROM and performs the task
	*	on that string
	*/
	else{
		//reading the input string
		char input_string [200];
		eeprom_read_block(input_string,0,200);

		//changing the flag variable value so that next time program switches to IF statement
		c = 0x55 ;
		eeprom_write_byte (( uint8_t *) 264, c );
		
		//clearing the string stored in EEPROM 
		eeprom_write_block(nullstring,0,200);//strlen(nullstring)	
		
		/*
		//##############################
		//clear lcd 
		lcd_cursor(1,1);
		lcd_string("                ");
		lcd_cursor(2,1);
		lcd_string("                ");
		lcd_cursor(1,1);
		//lcd_string(input_string);
		//#############################
		*/


		/*
			processing on the input string extrates the Animal and Habitat loaction 
			from the string and stores them in animal_obj and habitat_obj
		*/
		int i=0;//i:index pointer
		int k=0;//k:index pointer
		int animal_index=0,habitat_index=0;
		
		char animal_obj[20][5];
		char habitat_obj[20][5];

		while(input_string[i]!='#'&&input_string[i]!='\0'&&i<200)
		{
			if((input_string[i]>='a' && input_string[i]<='z')||(input_string[i]>='A' && input_string[i]<='Z'))
			{
				while(input_string[i]!=',' &&input_string[i]!='#' && i<200 && ((input_string[i]>='a' && input_string[i]<='z')||(input_string[i]>='A' && input_string[i]<='Z')||(input_string[i]>='0'&&input_string[i]<='9')))
				{
					animal_obj[animal_index][k]=input_string[i];
					k++;i++;
				}
				animal_obj[animal_index][k]='\0';
				
				k=0;animal_index++;
			}
			if(input_string[i]>='0'&&input_string[i]<='9')
			{
				
				while(input_string[i]!=','&&input_string[i]!='\n'&&i<200&&input_string[i]>='0'&&input_string[i]<='9')
				{
					habitat_obj[habitat_index][k]=input_string[i];
					k++;i++;
				}
				habitat_obj[habitat_index][k]='\0';
				k=0;habitat_index++;
				

			}
			i++;
		}

		//sets the X and Y coordinates of the robot to 0,0
		reset_coordinates();
		
		//for each Animal loaction it goes to the loaction pickups the box and goes to habitat loaction 
		//and drops the box  at the habitat loaction
		for(i=0;i<habitat_index;i++)
		{
			//ax:ay x:y coordinates of animal
			//hx:hy x:y coordinates of habitat
			
			int ax,ay,hx,hy;

			//gets x:y coordinates from arena specific code like A1:-x:0,y:0
			
			animal_coordinate(animal_obj[i],&ax,&ay);

			//special case if A1 needs to be picked up at the start of the run
			//rest fo the codes is generic 
			
			if(get_x()==0&&get_y()==0&&ax==0&&ay==0){goto_location(0,1);}
	
			goto_location(ax,ay);		
			stop();
			pickup_animal();

			_delay_ms(100);
			servo_reset1();
			
			//gets x:y coordinates from arena specific code like 2:-x:1,y:0
			
			habitat_coordinate(habitat_obj[i],&hx,&hy);
			drop_at(hx,hy);
			servo_reset();
		}
		
		//End of the Task continous Buzzer

		buzzer_on();
		_delay_ms(5000);
		buzzer_off();
	
	}

}

/*
* Function Name: drop_at
* Input:  		 int x int y the loaction of the habitat where animal needs to be droped
* Output: 		 none
* Logic:		 checks which conner  of the habitat is the closest to the current position 
* 				 of the robot and goes there then according the respective conner performs the drop 
* 				 sequence 
* Example Call:  drop_ar(2,4)
*
*/

void drop_at(int x,int y)
{
	if(get_x()>x&&get_y()>y)
	{
		goto_location(x+1,y+1);
		drop_animal(3);
	}
	else if(get_x()<x&&get_y()<y)
	{
		goto_location(x,y);
		drop_animal(1);
	}
	else if(get_x()>x)
	{
		goto_location(x+1,y);
		drop_animal(4);
	}
	else if(get_y()>y) 
	{
		goto_location(x,y+1);
		
		drop_animal(2);
	}
	else
	{
		goto_location(x,y);
		drop_animal(1);
	}
	
}

/*
* Function Name: next_node
* Input:  		 none
* Output: 		 none
* Logic:		 goes to the next node using line following sensors and stops
*
* Example Call:  next_node();
*
*/

int reset=1;
int next_node(void)
{
		//flag for exiting when a node is reached threshold values for 
		// left center and right line following sensor
		int flag=1;
		int L_thresh=15;
		int C_thresh=15;
		int R_thresh=15;
		int l_dir=0,r_dir=0;

		//variable to perform speed difference between left and right wheels
		int x=50,y=30;//50,30//70,50
		int counter=0;
		int n_flag=1;
		
		//specifiec the angle tried when all sensor are on white and the robot is
		// trying to find the black line
		int tryed=0;
		int tryed_delay=1;
		int	Left = ADC_Conversion(3);	//Getting data of Left WL Sensor
		int Center = ADC_Conversion(2);	//Getting data of Center WL Sensor
		int Right = ADC_Conversion(1);	//Getting data of Right WL Sensor
		
		//stores the value of the previous move made be the robot 
		int previous_move=0;
		int angle =0 ;
		int wait;
		while(flag==1){

			Left = ADC_Conversion(3);	//Getting data of Left WL Sensor
			Center = ADC_Conversion(2);	//Getting data of Center WL Sensor
			Right = ADC_Conversion(1);	//Getting data of Right WL Sensor
			
			/*
			//##############################
			//prints the sensor values on LCD
			lcd_print(1,1,Left,3);
			lcd_print(1,5,Center,3);
			lcd_print(1,9,Right,3);
			//###############################
			*/

			if(Left<L_thresh && Center<C_thresh && Right>R_thresh){
				velocity(250,250-x);
				forward();
				tryed=0;
				angle=0;
				previous_move=1;
			}
			else if(Left<L_thresh && Center>C_thresh && Right>R_thresh){
				velocity(250,250-y);
				forward();
				tryed=0;
				angle=0;
				previous_move=1;
				
			}
			else if (Left<L_thresh && Center>C_thresh && Right<R_thresh){
				if(previous_move==0)
				{
					velocity(250,250);
				}
				else if(previous_move==1)
				{
					velocity(220,250);
				}
				else if(previous_move==2)
				{
					velocity(250,220);
				}
				forward();
				tryed=0;
				angle=0;
				previous_move=0;
			}
			else if(Left>L_thresh && Center<C_thresh && Right<R_thresh){
				velocity(250-x,250);
				forward();
				tryed=0;
				angle=0;
				previous_move=2;
			}
			else if(Left>L_thresh && Center>C_thresh && Right<R_thresh){
				velocity(250-y,250);
				forward();
				tryed=0;
				angle=0;
				previous_move=2;
			}
			else if(Center<C_thresh && Right<R_thresh && Left<L_thresh){
				
				
					velocity(200,200);
					int rot=5;	
					if(previous_move==1)
					{
						int ang=4;
						if(tryed%ang<ang/2)
						{
							right_degrees(rot);
						}
						
						else
						{
							left_degrees(rot);
						}
					}
					else
					{
						int ang=4;
						if(tryed%ang<ang/2)
						{
							left_degrees(rot);
						}
						else
						{
							right_degrees(rot);
						} 
					}
					if (angle==tryed)
					{
						angle=0;
						tryed++;
					}
					if(angle<tryed)
					{
						angle++;
					}
					
			}
			if((Left>L_thresh+40 ||Right>R_thresh+40) &&Center>C_thresh+40 &&n_flag==1){//&&Center>C_thresh &&n_flag==1
				counter++;
				n_flag=2;
				stop();
				flag=2;
				_delay_ms(100);
			}
			if(n_flag==2&&reset>100){
				n_flag=1;
				reset=1;
			}
			if(n_flag==2){
				reset++;
			}
		}
}
