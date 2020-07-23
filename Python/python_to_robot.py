'''
* Team Id : HC#169
* Author List : Krishnaraj solanki
* Filename: python_to_robot.py
* Theme: Homecoming
* Functions: None
* Global Variables: none
'''

'''
serialy communicates the data in the output file to the robot 
'''

import msvcrt
import os
from time import sleep
import time
import serial           # import the module

ComPort = serial.Serial('COM3') # open COM3
ComPort.baudrate = 9600 # set Baud rate to 9600
ComPort.bytesize = 8    # Number of data bits = 8
ComPort.parity   = 'N'  # No parity
ComPort.stopbits = 1    # Number of Stop bits = 1


os.system('cls' if os.name == 'nt' else 'clear')
input_file=open('output.txt','r')

seconds = time.ctime()
string=input_file.read()+'#'
print(string)
length=len(string)
print(length)

i=0
flag=1
per=0
dif=0

#inputd = msvcrt.getch()
while flag==1:
    data = bytearray(string[i], 'utf-8')
    if(i%2==0):
        os.system('cls' if os.name == 'nt' else 'clear')
    
        
        for j in range(int(per)):
            print('#', end='')
            dif+=1
        while dif<100:
            print(' ', end='')
            dif+=1
        print('| ',int(per),' %')
        print(' \n',seconds)
        print('\n')
        print(string)

    No = ComPort.write(data) 
    #out=ComPort.read()
    sleep(0.3)
    if data==bytearray(b'#'):
        flag=2
    
    i+=1
    per=i*100/length
    dif=0
os.system('cls' if os.name == 'nt' else 'clear')

for j in range(int(per)):
    print('#', end='')
    dif+=1
while dif<100:
    print(' ', end='')
    dif+=1
print('| ',int(per),'%')
print('\nstart:-',seconds)
print(' ')
seconds1 = time.ctime()
print('\nend:-',seconds1)
print(' ')
print(string)
    
ComPort.close()
