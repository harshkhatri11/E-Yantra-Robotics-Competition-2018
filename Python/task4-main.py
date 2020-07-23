'''
* Team Id : HC#169
* Author List : Krishnaraj solanki
* Filename: main.py
* Theme: Homecoming
* Functions: process_image(image),print_arena(image,animal_list,habitat_list),predict_list(animal_path,habitat_path),main()
* Global Variables: list1,list2,args,font
'''
#standard libaries
import cv2
import sys
import argparse
import numpy as np
import os
import shutil
# python scripts 

import model
import mapping
import pathfinder
import connect

# input argument to the main program
# input image path
# amod animal model
# hmod habitat model
# -s save output image

parser = argparse.ArgumentParser()
parser.add_argument('path',help='img path')
parser.add_argument('-amod','--amod',help='animal_model path eg(model/animal_model.pth)')
parser.add_argument('-hmod','--hmod',help='habitate_model path eg(model/habitat_model.pth)')
parser.add_argument('-s','--save',help='save output image')
args=parser.parse_args()


list1=[]    #list of animal square (aniam contours)
list2=[]    #list of habitat square(habitat contours)

font = cv2.FONT_HERSHEY_SIMPLEX


'''
* Function Name: predict_list
* Input: aniaml_model_path,habitat_model_path
* Output: animal dictionary ,habitat dictionary
* Logic:  calls model.predict_list() function with model,image_path,
            animal_habitat =True for predicting animals,
            animal_habitat =False for predicting habitats,
            predicts the animal and habitat in the image
* Example Call:predict_list(animal_model_path,habitat_model_path)
'''


def predict_list(animal_path,habitat_path):
    if args.amod:
        animal_model=args.amod
    elif os.path.isfile('resnet152_animal_model_v5.pth'):
        animal_model='resnet152_animal_model_v5.pth'
    if args.hmod:
        habitat_model=args.hmod
    elif os.path.isfile('resnet152_habitat_model.pth'):
        habitat_model='resnet152_habitat_model.pth'
    habitat_list=model.predict_list(habitat_model,habitat_path,animal_habitat=False)
    animal_list=model.predict_list(animal_model,animal_path,animal_habitat=True)
    return(habitat_list,animal_list)

'''
* Function Name: process_image
* Input: image
* Output: finds animal and habitat boxes in image
* Logic:  apply adaptiveThreshold on grayscaled image,
          find contours,find aniaml square contours and habitat square contours,
          save aniaml images in tempory animal folder,save habitat images in tempory folder
* Example Call:process_image(image)
'''

def process_image(image):
    img=cv2.imread(image)
    img1=img.copy()
    img2=img.copy()
    gray=cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
    thresh=cv2.adaptiveThreshold(gray,255,cv2.ADAPTIVE_THRESH_GAUSSIAN_C,cv2.THRESH_BINARY,11,2)
    _, contours, _ = cv2.findContours(thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    font = cv2.FONT_HERSHEY_COMPLEX

    for cnt in contours:
        flag=False
        approx = cv2.approxPolyDP(cnt, 0.01*cv2.arcLength(cnt, True), True)
        if len(approx)==4:
            if cnt.shape[0]:
                if flag==False:
                    perimeter=cv2.arcLength(cnt,True)
                    if  perimeter<600 and perimeter>527:
                        #cv2.drawContours(img, [approx], 0, (0,255,0), 3)
                        x = approx.ravel()[0]
                        y = approx.ravel()[1]
                        list1.append(cnt)
                    if  perimeter<1250 and perimeter>1200:
                        #cv2.drawContours(img, [approx], 0, (255,0,0), 3)
                        x = approx.ravel()[0]
                        y = approx.ravel()[1]
                        list2.append(cnt)
    """labels animals"""
    ax=5 #x cordinate for animal boxes
    ay=1 #y cordinate for animal boxes
    temp=[]
    x1=0 #previous cx
    y1=0 #previous cy
    try:
        os.makedirs('animals', 493 )
    except OSError:
        pass
    for value in list1:
        flag=True
        M=cv2.moments(value)
        cx=int(M['m10']/M['m00'])
        cy=int(M['m01']/M['m00'])
        x,y,w,h = cv2.boundingRect(value)
        cropped=thresh[y:y+h, x:x+w]
        crop=img[y+5:y+h-5, x+5:x+w-5]
        mean = cv2.mean(cropped)[:3]
        if (cx-x1)+(cy-y1)>-10 and(cx-x1)+(cy-y1)<10:
            flag=False

        if flag:
            if mean[0]<219:
                cv2.imwrite('animals/'+str(chr(ax+65)+str(ay))+'.jpg',crop)
            if ax>0 and (ay==1 or ay==6):
                ax=ax-1
            elif ax>0:
                ax=ax-5
            else:
                ax=5
                ay=ay+1
        x1=cx
        y1=cy
    try:
        os.makedirs('habitat', 493 )
    except OSError:
        pass
    b=5
    l=1
    counter=0
    for x in list2:
        flag=True
        M=cv2.moments(x)
        cx=int(M['m10']/M['m00'])
        cy=int(M['m01']/M['m00'])
        x,y,w,h = cv2.boundingRect(x)
        cropped=thresh[y:y+h, x:x+w]
        crop=img[y+5:y+h-5, x+5:x+w-5]
        mean = cv2.mean(cropped)[:3]
        if (cx-x1)+(cy-y1)>-10 and(cx-x1)+(cy-y1)<10:
            flag=False
        if flag:
            if mean[0]<223:
                cv2.imwrite('habitat/'+str(b)+'.jpg',crop)
                #if args.cc:                     #saves contours
                    #cv2.imwrite(args.cc+str(b)+'.jpg',crop)
            if b>5*(l-1)+1:
                b=b-1
            else:
                l=l+1
                b=5*l
        x1=cx
        y1=cy

'''
* Function Name: print_arena
* Input: image,habitat list,animal_list
* Output: displays processed image with animal and habitat names
* Logic:    read image,
            for aniaml square put_text animal names and location
            for habitat square put_text habitat names and loaction
* Example Call:print_arena(image,habitat_list,animal_list)
'''

def print_arena(image,habitat_list,animal_list):
    img=cv2.imread(image)
    img1=img.copy()
    img2=img.copy()
    gray=cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
    thresh=cv2.adaptiveThreshold(gray,255,cv2.ADAPTIVE_THRESH_GAUSSIAN_C,cv2.THRESH_BINARY,11,2)
    _, contours, _ = cv2.findContours(thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    font = cv2.FONT_HERSHEY_COMPLEX
    """labels animals"""
    ax=5 #x cordinate for animal boxes
    ay=1 #y cordinate for animal boxes
    temp=[]
    x1=0
    y1=0
    print('___________ANIMAL___________')
    for value in list1:
        flag=True
        M=cv2.moments(value)
        cx=int(M['m10']/M['m00'])
        cy=int(M['m01']/M['m00'])
        x,y,w,h = cv2.boundingRect(value)
        cropped=thresh[y:y+h, x:x+w]
        crop=img[y+5:y+h-5, x+5:x+w-5]
        mean = cv2.mean(cropped)[:3]
        if (cx-x1)+(cy-y1)>-10 and(cx-x1)+(cy-y1)<10:
            flag=False
        if flag:
            if mean[0]<219:
                print(str(chr(ax+65)+str(ay)),'\t:-\t',animal_list[str(chr(ax+65)+str(ay))])
                cv2.putText(img, str(chr(ax+65)+str(ay)+' '+str(animal_list[str(chr(ax+65)+str(ay))])), (x,y-30), font, 1.5, (0, 0, 0), 2, cv2.LINE_AA)
                cv2.rectangle(img,(x,y),(x+w,y+h),(255,0,0),5)
            if ax>0 and (ay==1 or ay==6):
                ax=ax-1
            elif ax>0:
                ax=ax-5
            else:
                ax=5
                ay=ay+1
        x1=cx
        y1=cy

    b=5
    l=1
    counter=0
    print('___________HABITAT___________')
    for x in list2:
        flag=True
        M=cv2.moments(x)
        cx=int(M['m10']/M['m00'])
        cy=int(M['m01']/M['m00'])
        x,y,w,h = cv2.boundingRect(x)
        cropped=thresh[y:y+h, x:x+w]
        crop=img[y+5:y+h-5, x+5:x+w-5]
        mean = cv2.mean(cropped)[:3]
        if (cx-x1)+(cy-y1)>-10 and(cx-x1)+(cy-y1)<10:
            flag=False
        if flag:
            if mean[0]<223:
                print(str(b),'\t:-\t',habitat_list[str(b)],'\t',mapping.check_habitat(habitat_list[str(b)]))
                if(mapping.check_habitat(habitat_list[str(b)])=='Invalid'):
                    cv2.rectangle(img,(x,y),(x+w,y+h),(0,0,255),5)
                    cv2.putText(img, str(str(b)+' '+str(habitat_list[str(str(b))])), (x+10,y+((b-5*(l-1)+1)*40)), font, 1.5, (0, 0, 255), 2, cv2.LINE_AA)
                
                else:
                    cv2.rectangle(img,(x,y),(x+w,y+h),(0,255,0),5)
                    cv2.putText(img, str(str(b)+' '+str(habitat_list[str(str(b))])), (x+10,y+((b-5*(l-1)+1)*40)), font, 1.5, (0, 255, 0), 2, cv2.LINE_AA)
                
            if b>5*(l-1)+1:
                b=b-1
            else:
                l=l+1
                b=5*l
        x1=cx
        y1=cy

    img2=cv2.resize(img,(800,800),interpolation=cv2.INTER_AREA)
    cv2.imshow("shapes", img2)
    cv2.waitKey(0)
    if args.save:
        cv2.imwrite(args.save,img)

'''
* Function Name: main
* Input: NONE
* Output: NONE
* Logic:deletes prexiting directory if any ,
        calls process_image(),predict_list(),print_arena(),
        creates output.txt to store the output list
        deletes tempory file created while program execution
'''

if __name__ == '__main__':
    file=open('output.txt','w')
    try:
        shutil.rmtree('habitat')
    except OSError:
        pass
    try:
        shutil.rmtree('animals')
    except OSError:
        pass
    process_image(args.path)
    habitat_list,animal_list=predict_list('animals','habitat')
    #print(habitat_list,animal_list)
    available_combination,max_len=mapping.combination(habitat_list,animal_list)
    pairs=connect.find_connection(available_combination,max_len,habitat_list)
    output_animal_list=[]
    output_habitat_list=[]
    print('_____________ANIMAL:-HABITAT___________________')
    for each in pairs:
        output_animal_list.append(each[0])
        output_habitat_list.append(each[1])
    output_animal_list,output_habitat_list,_=pathfinder.optimize(output_animal_list,output_habitat_list)
    for each in pairs:
        print(f"{each[0]}\t:-\t{each[1]}")
    
    for each in output_habitat_list:
        file.write(each)
        pos=file.tell()
        file.write(', ')
    file.seek(pos)
    file.write('\n')
    for each in output_animal_list:
        file.write(each)
        pos=file.tell()
        file.write(', ')
    file.seek(pos)
    file.write(' ')
    file.close()
    print("output list:-(output.txt)\n",output_habitat_list,' \n',output_animal_list)
    print_arena(args.path,habitat_list,animal_list)
    try:
        shutil.rmtree('habitat')
    except OSError:
        pass
    try:
        shutil.rmtree('animals')
    except OSError:
        pass
