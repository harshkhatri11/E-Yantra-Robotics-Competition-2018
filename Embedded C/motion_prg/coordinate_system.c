/*
 * Team Id: <169>
 * Author List: krishnaraj solanki,khushboo patel
 * Filename: <coordinate_system.c>
 * Theme: <eYRC-homecoming>
 * Functions: animal_coordinate(),habitat_coordinate(),reset_coordinates(),
 			  change_direction_right(),change_direction_left(),distance()
			  change_direction_to(),coordinate_next_node(),goto_location()
 * Global Variables:current_direction;int current_x,current_y;
 */ 

/*
 * the cooridnate system works on the global variables and has functions to change this 
 * variable according to the rules set it those function.
 * it tracks the robots position and direction throught out the task 
*/

int current_direction;//0-North,1-east,2-south,3-west
int current_x,current_y;//current node

unsigned char ADC_Conversion1(unsigned char Ch)
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

/*
 * Function Name: aniaml_coordinate
 * Input: special animal loaction according to the tasks naming system
 * 		 refernece to the x and y variable to store values in them
 * Output: none
 * Logic: converts the first character of the location to x coordinate and
		 converts the second character of the location to y coordinate
 * Example Call: aniaml_corrdinate(location,&x,&y);
 *
*/

void animal_coordinate(char *animal,int *x,int *y)
{	int locx,locy;
	locx=animal[0]-65;
	locy=animal[1]-49;
	*x=locx;
	*y=locy;
}

/*
 * Function Name: habitat_coordinate
 * Input: special habitat loaction according to the tasks naming system
 * 		 refernece to the x and y variable to store values in them
 * Output: none
 * Logic: x coordinate = habitat_loaction%5-1 if x=-1 then x=0
		 y coordinate = number of 5 that can be placed in the habitats number
 * Example Call: habitat_corrdinate(location,&x,&y);
 *
*/

void habitat_coordinate(char *habitat,int *x,int *y)
{
	int locx,locy;
	int data=0;
	for(int i=0;habitat[i]!='\0';i++)
	{
		data=data*10+(int)(habitat[i]-48);
	}
	locx=data;
	locx=locx%5-1;
	if(locx<0){
		locx=4;
	}
	int i=0;
	locy=data;
	while(i*5<locy){i++;}
	locy=i-1;
	*x=locx;
	*y=locy;
}

/*
* Function Name: reset_coordinate()
* Input: none
* Output: none
* Logic: resets current_x and current_y to 0
* Example Call: reset_coordinate();
*
*/

void reset_coordinates()
{
	current_direction=0;
	current_x=0;current_y=0;
}

/*
* Function Name: chage_direction_right()
* Input: none
* Output: none
* Logic: turns the current_direction by one right eg:North:-East (0 :- 1)
* Example Call: chage_direction_right();
*
*/

void change_direction_right()
{
	current_direction++;
	current_direction%=4;
}

/*
* Function Name: chage_direction_left()
* Input: none
* Output: none
* Logic: turns the current_direction by one left eg:East :- North (1 :- 0)
* Example Call: chage_direction_right();
*
*/

void change_direction_left()
{
	
	current_direction--;
	if(current_direction==-1)
	{
		current_direction=3;
	}
}

/*
* Function Name: distance
* Input: start_x,start_y,end_x,end_y,&dif_x,&dif_y
* Output: none
* Logic: calculates the diffenrece in the x,y coordinate of the current position and
*		 the destination and stores the x component difference in dif_x and y component
*		 difference in dif_y it can pe positive or negative according to the start and 
*		 end loaction
* Example Call: distance(0,1,5,4,&dif_x,&dif_y);
*
*/

void distance(int start_x,int start_y,int end_x,int end_y,int *dif_x,int *dif_y)
{
	*dif_x=end_x-start_x;
	*dif_y=end_y-start_y;
}

/*
* Function Name: get_x()
* Input: none
* Output: int
* Logic: reutrns the current_x 
* Example Call: a=get_x();
*
*/

int get_x()
{
	return(current_x);
}
/*
* Function Name: get_y()
* Input: none
* Output: int
* Logic: reutrns the current_y 
* Example Call: b=get_y();
*
*/

int get_y()
{
	return(current_y);
}

/*
* Function Name: change_direction_to()
* Input: dir ,the direction the robot has to turn
* Output: None
* Logic: turns the robot from the current_direction to the intended direction
* Example Call: change_direction_to(2),//2:-south
*
*/

void change_direction_to(int dir)
{
	velocity(250,250);
	int	Left = ADC_Conversion1(3);	//Getting data of Left WL Sensor
	int Center = ADC_Conversion1(2);	//Getting data of Center WL Sensor
	int Right = ADC_Conversion1(1);	//Getting data of Right WL Sensor
	int C_thresh=15;
	Left = ADC_Conversion1(3);	//Getting data of Left WL Sensor
	Center = ADC_Conversion1(2);	//Getting data of Center WL Sensor
	Right = ADC_Conversion1(1);	//Getting data of Right WL Sensor

	int temp=current_direction;
	while(current_direction!=dir)
	{
		if(temp==3&&dir==0)
		{
			soft_right_degrees(45); //Rotate robot left by 90 degrees
			Center = ADC_Conversion1(2);	//Getting data of Center WL Sensor
			while(Center<C_thresh)
			{
				soft_right_degrees(1);
				Center = ADC_Conversion1(2);	//Getting data of Center WL Sensor
			}
			change_direction_right();
		}
		else if(temp==0&&dir==3)
		{
			soft_left_degrees(45); //Rotate robot left by 90 degrees
			Center = ADC_Conversion1(2);	//Getting data of Center WL Sensor
			while(Center<C_thresh)
			{
				soft_left_degrees(1);
				Center = ADC_Conversion1(2);	//Getting data of Center WL Sensor
			}
			change_direction_left();
			
		}
		else if((temp==0&&dir==2)||(temp==2&&dir==0))
		{
			if((current_x==0&&(temp==0&&dir==2))||(current_x==5&&(temp==2&&dir==0)))
			{
				right_180();
			}
			else
			{
				left_180();
			}
		}
		else if((temp==1&&dir==3)||(temp==3&&dir==1))
		{
			if((current_y==5&&(temp==1&&dir==3))||(current_y==0&&(temp==3&&dir==1)))
			{
				right_180();
			}
			else
			{
				left_180();
			}
		}
		else if(temp<dir)
		{
			soft_right_degrees(45);
			Center = ADC_Conversion1(2);	//Getting data of Center WL Sensor
			while(Center<C_thresh)
			{
				soft_right_degrees(1);
				Center = ADC_Conversion1(2);	//Getting data of Center WL Sensor
			}
			
			change_direction_right();
		}
		else
		{
			soft_left_degrees(45);
			Center = ADC_Conversion1(2);	//Getting data of Center WL Sensor
			while(Center<C_thresh)
			{
				left_degrees(1);
				Center = ADC_Conversion1(2);//Getting data of Center WL Sensor
			}
			
			change_direction_left();
		}
	}
	stop();
}

/*
* Function Name: left_180()
* Input: None
* Output: None
* Logic: turns the robot 180 deg from the current_direction,use left turn for 180
* Example Call: left_180()
*
*/


void left_180()
{
	int	Left = ADC_Conversion1(3);	//Getting data of Left WL Sensor
	int Center = ADC_Conversion1(2);	//Getting data of Center WL Sensor
	int Right = ADC_Conversion1(1);	//Getting data of Right WL Sensor
	int C_thresh=15;
	Left = ADC_Conversion1(3);	//Getting data of Left WL Sensor
	Center = ADC_Conversion1(2);	//Getting data of Center WL Sensor
	Right = ADC_Conversion1(1);	//Getting data of Right WL Sensor

	soft_left_degrees(45);
	Center = ADC_Conversion1(2);	//Getting data of Center WL Sensor
	while(Center<C_thresh)
	{
		soft_left_degrees(1);
		Center = ADC_Conversion1(2);	//Getting data of Center WL Sensor
	}
	change_direction_left();
	soft_left_2_degrees(45);
	Center = ADC_Conversion1(2);	//Getting data of Center WL Sensor
	while(Center<C_thresh)
	{
		soft_left_2_degrees(1);
		Center = ADC_Conversion1(2);	//Getting data of Center WL Sensor
	}
	change_direction_left();
			
}

/*
* Function Name: right_180()
* Input: None
* Output: None
* Logic: turns the robot 180 deg from the current_direction,use right turn for 180
* Example Call: right_180()
*
*/

void right_180()
{
	int	Left = ADC_Conversion1(3);	//Getting data of Left WL Sensor
	int Center = ADC_Conversion1(2);	//Getting data of Center WL Sensor
	int Right = ADC_Conversion1(1);	//Getting data of Right WL Sensor
	int C_thresh=15;
	Left = ADC_Conversion1(3);	//Getting data of Left WL Sensor
	Center = ADC_Conversion1(2);	//Getting data of Center WL Sensor
	Right = ADC_Conversion1(1);	//Getting data of Right WL Sensor

	soft_right_degrees(45);
	Center = ADC_Conversion1(2);	//Getting data of Center WL Sensor
	while(Center<C_thresh)
	{
		soft_right_degrees(1);
		Center = ADC_Conversion1(2);	//Getting data of Center WL Sensor
	}
	change_direction_right();
	soft_right_2_degrees(45);
	Center = ADC_Conversion1(2);	//Getting data of Center WL Sensor
	while(Center<C_thresh)
	{
		soft_right_2_degrees(1);
		Center = ADC_Conversion1(2);	//Getting data of Center WL Sensor
	}
	change_direction_right();
			
}

/*
* Function Name: coordinate_next_node()
* Input: None
* Output: None
* Logic: changes the x/y component of the loaction based on the direction of the robot
* Example Call: coordinate_next_node()
*
*/

void coordinate_next_node()
{
	if(current_direction==0){current_y++;}
	else if(current_direction==1){current_x++;}
	else if(current_direction==2){current_y--;}
	else if(current_direction==3){current_x--;}
}

/*
* Function Name: goto_loaction()
* Input: end_x,end_y: x and y coordinates of the destination location
* Output: None
* Logic:	takes the robot from the current loaction to the dezired loaction
* Example Call: goto_location(3,2);
*
*/

void goto_location(int end_x,int end_y)
{
	
	int dif_x,dif_y;
	//finds the distance between the current location to the end loaction 
	distance(current_x,current_y,end_x,end_y,&dif_x,&dif_y);
	
	//reduces the x difference to 0 
	if(dif_x<0)
	{	
		change_direction_to(3);
		while(dif_x<0)
		{
			forward_mm(20);
			_delay_ms(100);
			next_node();
			coordinate_next_node();
			dif_x++;
		}
	}
	else if(dif_x>0)
	{
		change_direction_to(1);
		while(dif_x>0)
		{
			forward_mm(20);
			_delay_ms(100);
			next_node();
			coordinate_next_node();
			dif_x--;
		}
	}
	//reduces the y difference to 0
	if(dif_y<0)
	{
		change_direction_to(2);
		while(dif_y<0)
		{
			forward_mm(20);
			_delay_ms(100);
			next_node();
			coordinate_next_node();
			dif_y++;
		}
	}
	else if(dif_y>0)
	{
		change_direction_to(0);
		while(dif_y>0)
		{
			forward_mm(20);
			_delay_ms(100);
			next_node();
			coordinate_next_node();
			dif_y--;
		}
	}
}
