
/*
 * Team Id: <169>
 * Author List: krishnaraj solanki,khushboo patel,Prayushi rana
 * Filename: <serial_communication_usb-rs232.c>
 * Theme: <eYRC-homecoming>
 * Functions: buzzer_pin_config (),port_init(),buzzer_on (),buzzer_off (),uart2_init(),
 			SIGNAL(SIG_USART2_RECV),init_devices(),have_d(),char get_data(),
			get_data_array(),get_length(),
 * Global Variables:unsigned char data;int array_length;int data_array[10];
					int i;int have_data;
 */ 



unsigned char data; //to store received data from UDR1
int array_length=40;
int data_array[10];
int i=0;
int have_data=0;
void buzzer_pin_config (void)
{
 DDRC = DDRC | 0x08;		//Setting PORTC 3 as outpt
 PORTC = PORTC & 0xF7;		//Setting PORTC 3 logic low to turnoff buzzer
}


//Function to initialize ports
void port_init()
{
	buzzer_pin_config();
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

//Function To Initialize UART2
// desired baud rate:9600
// actual baud rate:9600 (error 0.0%)
// char size: 8 bit
// parity: Disabled
void uart2_init(void)
{
 UCSR2B = 0x00; //disable while setting baud rate
 UCSR2A = 0x00;
 UCSR2C = 0x06;
 UBRR2L = 0x5F; //set baud rate lo
 UBRR2H = 0x00; //set baud rate hi
 UCSR2B = 0x98;
}

int ack=0;
unsigned char data1;
SIGNAL(SIG_USART2_RECV) 		// ISR for receive complete interrupt
{
		
		data = UDR2; 				//making copy of data from UDR2 in 'data' variable 
		UDR2 = data1;
		data_array[i++]=data-48;
		if(i==array_length)
		{
			i=0;
		}
		buzzer_on();
		buzzer_off();
		have_data=1;//UDR2 = data;
		
	 				//echo data back to PC
	//buzzer_on();
	
	//buzzer_off();
}


//Function To Initialize all The Devices
void init_devices()
{
 cli(); //Clears the global interrupts
 port_init();  //Initializes all the ports
 uart2_init(); //Initailize UART1 for serial communiaction
 sei();   //Enables the global interrupts
}

/*
* Function Name: have_d
* Input: none
* Output: 0 if no data is reviced else data
* Logic: changes values after data is being recived through serial communication
* 
* Example Call: if(have_d()==0);
*
*/
int have_d(void)
{
	int temp=have_data;
	if(temp==0){ack=0;}
	return(temp);
	

}
/*
* Function Name: get_data
* Input: none
* Output: returns the recived data
* Logic: returns the reviced data and resets the variable to 0
*
* Example Call: input=get_data();
*
*/
unsigned char get_data(void)
{
	
	if(have_data==1){have_data=0;}
	return(data);
	
}
/*
* Function Name: get_data_array
* Input: none
* Output: returns the jth element from the input array
* Logic: returns the jth element without changing its values
* 
* Example Call: data=get_data_array(2);
*
*/

int get_data_array(int j)
{
	return(data_array[j]);
}
/*
* Function Name: get_length
* Input: none
* Output: length of the input array
* Logic: returns the index variable which is always at the end of the array
* 
* Example Call: length =get_length();
*
*/

int get_length()
{
	return(i);
}
