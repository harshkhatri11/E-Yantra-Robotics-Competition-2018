 /*
 * Team Id: <169>
 * Author List: krishnaraj solanki,Harsh khatri
 * Filename: <animal_pickup_and_drop.c>
 * Theme: <eYRC-homecoming>
 * Functions: animal_coordinate(),habitat_coordinate(),reset_coordinates(),
 			  change_direction_right(),change_direction_left(),distance()
			  change_direction_to(),coordinate_next_node(),goto_location()
 * Global Variables:hab[5][5]
 */ 



//########################################################################################################################
//SERVO CONTROL FUNCTIONS
//########################################################################################################################

//Configure PORTB 5 pin for servo motor 1 operation2
void servo1_pin_config (void)
{
 DDRB  = DDRB | 0x20;  //making PORTB 5 pin output
 PORTB = PORTB | 0x20; //setting PORTB 5 pin to logic 1
}

//Configure PORTB 6 pin for servo motor 2 operation
void servo2_pin_config (void)
{
 DDRB  = DDRB | 0x40;  //making PORTB 6 pin output
 PORTB = PORTB | 0x40; //setting PORTB 6 pin to logic 1
}

//Configure PORTB 7 pin for servo motor 3 operation
void servo3_pin_config (void)
{
 DDRB  = DDRB | 0x80;  //making PORTB 7 pin output
 PORTB = PORTB | 0x80; //setting PORTB 7 pin to logic 1
}

//Initialize the ports
void port_init_servo(void)
{
 servo1_pin_config(); //Configure PORTB 5 pin for servo motor 1 operation
 servo2_pin_config(); //Configure PORTB 6 pin for servo motor 2 operation 
 servo3_pin_config(); //Configure PORTB 7 pin for servo motor 3 operation  
}

//TIMER1 initialization in 10 bit fast PWM mode  
//prescale:256
// WGM: 7) PWM 10bit fast, TOP=0x03FF
// actual value: 52.25Hz 
void timer1_init(void)
{
 TCCR1B = 0x00; //stop
 TCNT1H = 0xFC; //Counter high value to which OCR1xH value is to be compared with
 TCNT1L = 0x01;	//Counter low value to which OCR1xH value is to be compared with
 OCR1AH = 0x03;	//Output compare Register high value for servo 1
 OCR1AL = 0xFF;	//Output Compare Register low Value For servo 1
 OCR1BH = 0x03;	//Output compare Register high value for servo 2
 OCR1BL = 0xFF;	//Output Compare Register low Value For servo 2
 OCR1CH = 0x03;	//Output compare Register high value for servo 3
 OCR1CL = 0xFF;	//Output Compare Register low Value For servo 3
 ICR1H  = 0x03;	
 ICR1L  = 0xFF;
 TCCR1A = 0xAB; /*{COM1A1=1, COM1A0=0; COM1B1=1, COM1B0=0; COM1C1=1 COM1C0=0}
 					For Overriding normal port functionality to OCRnA outputs.
				  {WGM11=1, WGM10=1} Along With WGM12 in TCCR1B for Selecting FAST PWM Mode*/
 TCCR1C = 0x00;
 TCCR1B = 0x0C; //WGM12=1; CS12=1, CS11=0, CS10=0 (Prescaler=256)
}


//Function to rotate Servo 1 by a specified angle in the multiples of 1.86 degrees
void servo_1(unsigned char degrees)  
{
 float PositionPanServo = 0;
  PositionPanServo = ((float)degrees / 1.86) + 35.0;
 OCR1AH = 0x00;
 OCR1AL = (unsigned char) PositionPanServo;
}


//Function to rotate Servo 2 by a specified angle in the multiples of 1.86 degrees
void servo_2(unsigned char degrees)
{
 float PositionTiltServo = 0;
 PositionTiltServo = ((float)degrees / 1.86) + 35.0;
 OCR1BH = 0x00;
 OCR1BL = (unsigned char) PositionTiltServo;
}

//Function to rotate Servo 3 by a specified angle in the multiples of 1.86 degrees
void servo_3(unsigned char degrees)
{
 float PositionServo = 0;
 PositionServo = ((float)degrees / 1.86) + 35.0;
 OCR1CH = 0x00;
 OCR1CL = (unsigned char) PositionServo;
}

//servo_free functions unlocks the servo motors from the any angle 
//and make them free by giving 100% duty cycle at the PWM. This function can be used to 
//reduce the power consumption of the motor if it is holding load against the gravity.

void servo_1_free (void) //makes servo 1 free rotating
{
 OCR1AH = 0x03; 
 OCR1AL = 0xFF; //Servo 1 off
}

void servo_2_free (void) //makes servo 2 free rotating
{
 OCR1BH = 0x03;
 OCR1BL = 0xFF; //Servo 2 off
}

void servo_3_free (void) //makes servo 3 free rotating
{
 OCR1CH = 0x03;
 OCR1CL = 0xFF; //Servo 3 off
} 
//########################################################################################################################
//########################################################################################################################


//########################################################################################################################
//ANIMAL pickup and drop routins
//########################################################################################################################


unsigned char current_angle=0;
void corner_pickup();
void side_pickup();
void drop();

/*
* Function Name: left_back()
* Input: None
* Output: None
* Logic: turns the robot 90 deg (till it finds a black line) from the current_direction,
*		 use left turn,performs this without changing the direction variable of the coordinate system
* Example Call: left_back()
*
*/

void left_back()
{
	int Center = ADC_Conversion1(2);	//Getting data of Center WL Sensor
	int C_thresh=15;
	Center = ADC_Conversion1(2);	//Getting data of Center WL Sensor
	left_degrees(35);
	Center = ADC_Conversion1(2);	//Getting data of Center WL Sensor
	while(Center<C_thresh)
	{
		left_degrees(1);
		Center = ADC_Conversion1(2);	//Getting data of Center WL Sensor
	}		
}

/*
* Function Name: right_back()
* Input: None
* Output: None
* Logic: turns the robot 90 deg (till it finds a black line) from the current_direction,
*		 use right turn,performs this without changing the direction variable of the coordinate system
* Example Call: right_back()
*
*/

void right_back()
{
	int Center = ADC_Conversion1(2);	//Getting data of Center WL Sensor
	int C_thresh=15;
	Center = ADC_Conversion1(2);	//Getting data of Center WL Sensor
	right_degrees(45);
	Center = ADC_Conversion1(2);	//Getting data of Center WL Sensor
	while(Center<C_thresh)
	{
		right_degrees(1);
		Center = ADC_Conversion1(2);	//Getting data of Center WL Sensor
	}		
}

/*
* Function Name: left_conner()
* Input: None
* Output: None
* Logic: turns the robot left till it finds a black line from the current_direction,
*		 use left turn,performs this without changing the direction variable of the coordinate system
*		 used for conner pickups
* Example Call: left_conner()
*
*/

void left_conner()
{
	int Center = ADC_Conversion1(2);	//Getting data of Center WL Sensor
	int C_thresh=15;
	Center = ADC_Conversion1(2);	//Getting data of Center WL Sensor
	soft_left_degrees(1);
	Center = ADC_Conversion1(2);	//Getting data of Center WL Sensor
	while(Center<C_thresh)
	{
		soft_left_degrees(1);
		Center = ADC_Conversion1(2);	//Getting data of Center WL Sensor
	}
}

/*
* Function Name: right_conner()
* Input: None
* Output: None
* Logic: turns the robot left till it finds a black line from the current_direction,
*		 use right turn,performs this without changing the direction variable of the coordinate system
*		 used for conner pickups
* Example Call: right_conner()
*
*/

void right_conner()
{
	int Center = ADC_Conversion1(2);	//Getting data of Center WL Sensor
	int C_thresh=15;
	Center = ADC_Conversion1(2);	//Getting data of Center WL Sensor
	soft_right_degrees(1);
	Center = ADC_Conversion1(2);	//Getting data of Center WL Sensor
	while(Center<C_thresh)
	{
		soft_right_degrees(1);
		Center = ADC_Conversion1(2);	//Getting data of Center WL Sensor
	}
	
			
}

/*
* Function Name: pickup_animal()
* Input: None
* Output: None
* Logic: function is called to pickup animal it checks the current position of the robot and performs 
*		 which ever animal pickup routine is appropriate i.e if it a conner or a side pickup and 
*		 according to the direction the robot if facing currently it calls the approprate pickup routine
* Example Call: pickup_animal()
*
*/

void pickup_animal()
{
	int present_direction;
	present_direction=current_direction;
		
	//####### corner nodes ###########
	if(current_x==5&&current_y==5)
	{	
		forward_mm(35);
		if(current_direction!=0)
		{
			left_conner();
			corner_pickup();			
		}
		else
		{
			right_conner();
			corner_pickup();	
		}
		servo_reset1();
		right_back();
		current_direction=2;
		
	}
	else if(current_x==5&&current_y==0)
	{	
		forward_mm(35);
		if(current_direction!=1)
		{
			left_conner();
			corner_pickup();
			servo_reset1();
		}
		else
		{
			right_conner();
			corner_pickup();
		}
		servo_reset1();
		right_back();
		current_direction=3;
		
		
	}
	else if(current_x==0&&current_y==0)
	{
		forward_mm(35);
		if(current_direction!=2)
		{
			left_conner();
			corner_pickup();
		}
		else
		{
			right_conner();
			corner_pickup();
		}
		servo_reset1();
		right_back();
		current_direction=0;
		
		
	}
	else if(current_x==0&&current_y==5)
	{
		forward_mm(35);
		if(current_direction!=3)
		{
			left_conner();
			corner_pickup();
		}
		else
		{
			right_conner();
			corner_pickup();	
		}
		servo_reset1();
		right_back();
		current_direction=1;
		
	}
	
	//####### side nodes #########
	else if(current_x==0)
	{
		if(current_direction==3)
		{
			forward_mm(20);
			corner_pickup();
			back_mm(20);
		}
		else if(current_direction!=2)
		{
			
			change_direction_to(0);
			side_pickup_left();
		}
		else 
		{	
			side_pickup_right();
		}
		change_direction_to(present_direction);
		
		
	}
	else if(current_x==5)
	{
		if(current_direction==1)
		{
			forward_mm(20);
			corner_pickup();
			back_mm(20);
		}
		else if(current_direction!=0)
		{
			change_direction_to(2);
			side_pickup_left();
		}
		else
		{
			side_pickup_right();
		} 
		change_direction_to(present_direction);
	
	}
	else if(current_y==0)
	{
		if(current_direction==2)
		{
			forward_mm(20);
			corner_pickup();
			back_mm(20);
		}
		else if(current_direction!=3)
		{
			change_direction_to(1);
			side_pickup_right();
		}
		else
		{
			side_pickup_left();
		}
		change_direction_to(present_direction);
	
	}
	else if(current_y==5)
	{
		if(current_direction==0)
		{
			forward_mm(20);
			corner_pickup();
			back_mm(20);
		}
		else if(current_direction!=3)
		{
			change_direction_to(1);
			side_pickup_left();
		}
		else
		{
			side_pickup_right();
		}
		change_direction_to(present_direction);
	
	}
}

//hab stores the number of animal places in any of the habitat,default value at the start of the run is 0 
//for all habitats as there are no animal in any habitat at the start of the run
int hab[5][5]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

/*
* Function Name: drop_animal()
* Input: None
* Output: None
* Logic: drops the animal the robot is currently holding in the appropriate habitat, it checks at which conner 
*		 of the habitat the robot is currently at and which direction does it need to drop the animal box,
*		 it also checks weather or not there is another animal present in the habitat so that it can changes the drop angle 
*		 by a little bit so that both the animal can be placed correctly in the habitat
* Example Call: drop_animal();
*
*/

void drop_animal(int node)
{
	int present_direction;
	if(node==1)
	{
		if(hab[get_x()][get_y()]!=1)
		{
			if(current_direction==0)
			{
				drop(1);
				hab[get_x()][get_y()]=1;
			}
			else if(current_direction==1)
			{
				drop(1);
				hab[get_x()][get_y()]=1;
			}
			else
			{
				back_drop();
				hab[get_x()][get_y()]=1;
			}
		}
		else
		{
			if(current_direction==0)
			{
				drop(0);
				hab[get_x()][get_y()]=1;
			}
			else if(current_direction==1)
			{
				drop(0);
				hab[get_x()][get_y()]=1;
			}
			else
			{
				back_drop();
				hab[get_x()][get_y()]=1;
			}
		}
		
	}
	else if(node==2)
	{
		current_direction--;
		if(current_direction==-1){current_direction=3;}
		
		if(hab[get_x()][get_y()-1]!=1)
		{
			if(current_direction==0)
			{
				drop(1);
				hab[get_x()][get_y()-1]=1;
			}
			else if(current_direction==1)
			{
				drop(1);
				hab[get_x()][get_y()-1]=1;
			}
			else
			{
				back_drop();
				hab[get_x()][get_y()-1]=1;
			}
		}
		else
		{
			if(current_direction==0)
			{
				drop(0);
				hab[get_x()][get_y()-1]=1;
			}
			else if(current_direction==1)
			{
				drop(0);
				hab[get_x()][get_y()-1]=1;
			}
			else
			{
				back_drop();
				hab[get_x()][get_y()-1]=1;
			}
		}
		current_direction++;
		if(current_direction==4){current_direction=0;}
		
	}
	else if(node==3)
	{
		current_direction++;
		if(current_direction==4){current_direction=0;}
		current_direction++;
		if(current_direction==4){current_direction=0;}
		
		if(hab[get_x()-1][get_y()-1]!=1)
		{
			if(current_direction==0)
			{
				drop(1);
				hab[get_x()-1][get_y()-1]=1;
			}
			else if(current_direction==1)
			{
				drop(1);
				hab[get_x()-1][get_y()-1]=1;
			}
			else
			{
				back_drop();
				hab[get_x()-1][get_y()-1]=1;
			}
		}
		else
		{
			if(current_direction==0)
			{
				drop(0);
				hab[get_x()-1][get_y()-1]=1;
			}
			else if(current_direction==1)
			{
				drop(0);
				hab[get_x()-1][get_y()-1]=1;
			}
			else
			{
				back_drop();
				hab[get_x()-1][get_y()-1]=1;
			}
		}
		current_direction--;
		if(current_direction==-1){current_direction=3;}
		current_direction--;
		if(current_direction==-1){current_direction=3;}
		
	}
	else if(node==4)
	{
		present_direction=current_direction;	
		current_direction++;
		if(current_direction==4){current_direction=0;}
		if(hab[get_x()-1][get_y()]!=1)
		{
			if(current_direction==0)
			{
				drop(1);
				hab[get_x()-1][get_y()]=1;
			}
			else if(current_direction==1)
			{
				drop(1);
				hab[get_x()-1][get_y()]=1;
			}
			else
			{
				back_drop();
				hab[get_x()-1][get_y()]=1;
			}
		}
		else
		{
			if(current_direction==0)
			{
				drop(0);
				hab[get_x()-1][get_y()]=1;
			}
			else if(current_direction==1)
			{
				drop(0);
				hab[get_x()-1][get_y()]=1;
			}
			else
			{
				back_drop();
				hab[get_x()-1][get_y()]=1;
			}
		}
		current_direction--;
		if(current_direction==-1){current_direction=3;}
		
		
	}
	
}

/*
* Function Name: servo_grab()
* Input: None
* Output: None
* Logic: servo routine to pickup an animal box
* Example Call: servo_grab()
*
*/

void servo_grab()
{
	servo_1(0);
	_delay_ms(1000);
	servo_2(145);
	_delay_ms(1000);
	servo_2_free();
	servo_1(200);
	_delay_ms(1000);
	free_servo();
}

/*
* Function Name: servo_reset()
* Input: None
* Output: None
* Logic: resets all servos positions
* Example Call: servo_reset()
*
*/

void servo_reset()
{
	servo_1(200);
	servo_2(200);
	unsigned char i=current_angle;
	while (i != 0)
 	{
  		servo_3(i);
 	 	_delay_ms(10);
		if(i>0){i--;}
		else{i++;}
 	}
	current_angle=0;
	_delay_ms(1000);
	free_servo();
}

/*
* Function Name: servo_reser1()
* Input: None
* Output: None
* Logic: servo routine to reset only 2 servos and not the griper servo
* Example Call: servo_reset1()
*
*/

void servo_reset1()
{
	servo_1(200);
	unsigned char i=current_angle;
	while (i != 0)
 	{
  		servo_3(i);
 	 	_delay_ms(10);
		if(i>0){i--;}
		else{i++;}
 	}
	current_angle=0;
	_delay_ms(1000);
	free_servo();
}

/*
* Function Name: free_servo()
* Input: None
* Output: None
* Logic: servo routine to  an free all servos
* Example Call: free_servo()
*
*/

void free_servo()
{
	servo_1_free(); 
 	servo_2_free();
 	servo_3_free();
}

/*
* Function Name: servo_drop()
* Input: None
* Output: None
* Logic: servo routine to drop an animal box
* Example Call: servo_drop()
*
*/

void servo_drop()
{	
	servo_1(110);
	_delay_ms(500);
	servo_2(200);
	_delay_ms(500);
	servo_1(200);
	_delay_ms(500);
	free_servo();

}
/*
* Function Name: corner_pickup()
* Input: None
* Output: None
* Logic: turns the arm straight ahead to pickup the animal placed in the corner
* Example Call: corner_pickup()
*
*/

void corner_pickup()
{
	back_mm(20);
	unsigned char i=current_angle;
	while (i != 95)
 	{
  		servo_3(i);
 	 	_delay_ms(10);
		if(i>95){i--;}
		else{i++;}
 	}
	current_angle=95;
	servo_grab();
	forward_mm(20);
	_delay_ms(100);
}

/*
* Function Name: side_pickup_right()
* Input: None
* Output: None
* Logic: routine to pickup animal when the box is on the right of the robot
* Example Call: side_pickup_right()
*
*/

void side_pickup_right()
{
	unsigned char i=current_angle;
	while (i != 10)
 	{
  		servo_3(i);
 	 	_delay_ms(10);
		if(i>10){i--;}
		else{i++;}
 	}
	current_angle=10;
	servo_grab();
	

}

/*
* Function Name: side_pickup_left()
* Input: None
* Output: None
* Logic: routine to pickup animal when the box is on the left of the robot
* Example Call: side_pickup_left()
*
*/

void side_pickup_left()
{
	unsigned char i=current_angle;
	while (i != 175)
 	{
  		servo_3(i);
 	 	_delay_ms(10);
		if(i>175){i--;}
		else{i++;}
 	}
	current_angle=175;
	servo_grab();
}
/*
* Function Name: drop()
* Input: int n :which corner of the habitat is the robot at  
* Output: None
* Logic: routine to change the arms positions so that the animal is droped in the correct location 
*		 according the the corner of the habitat
* Example Call: drop()
*
*/

void drop(int n)
{
	if(current_direction==0)
	{
		if(n==0)
		{
			unsigned char i=current_angle;
			while (i != 50)
	 		{
	  			servo_3(i);
	 		 	_delay_ms(10);
				if(i>50){i--;}
				else{i++;}
	 		}
			current_angle=50;
		
		}
		else
		{
			unsigned char i=current_angle;
			while (i != 40)
	 		{
	  			servo_3(i);
	 		 	_delay_ms(10);
				if(i>40){i--;}
				else{i++;}
	 		}
			current_angle=40;
		}
	}
	else
	{
		if(n==0)
		{
			unsigned char i=current_angle;
			while (i != 135)
	 		{
	  			servo_3(i);
	 		 	_delay_ms(10);
				if(i>135){i--;}
				else{i++;}
	 		}
			current_angle=135;		
		}
		else
		{
			unsigned char i=current_angle;
			while (i != 145)
	 		{
	  			servo_3(i);
	 		 	_delay_ms(10);
				if(i>145){i--;}
				else{i++;}
	 		}
			current_angle=145;
		}
	}
	_delay_ms(1000);
	servo_drop();
}
/*
* Function Name: back_drop()
* Input: None  
* Output: None
* Logic: drops the animal when the habitat is behind the robot
* Example Call: back_drop()
*
*/

void back_drop()
{
	if(current_direction==2)
	{
		unsigned char i=current_angle;
		while (i != 200)
 		{
  			servo_3(i);
 		 	_delay_ms(10);
			if(i>200){i--;}
			else{i++;}
 		}
		current_angle=200;
	}
	else
	{
		unsigned char i=current_angle;
		while (i != 0)
 		{
  			servo_3(i);
 		 	_delay_ms(10);
			if(i>0){i--;}
			else{i++;}
 		}
		current_angle=0;
	}
	servo_drop();
}

/*
* Function Name: ADC_Conversion_animal()
* Input: unsigned char ch:channel number of the sensor  
* Output: unsigned char digital value of the analog value given by the sensor
* Logic: converts the analog value of the sensor to a digital value
* Example Call: ADC_Conversion_animal()
*
*/

unsigned char ADC_Conversion_animal(unsigned char Ch)
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

