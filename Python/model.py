'''
* Team Id : HC#169
* Author List : Krishnaraj solanki
* Filename: model.py
* Theme: Homecoming
* Functions: image_loader(image_name),predict_animal(image),predict_habitat(image),predict_list(model,img_path,animal_habitat),main()
* Global Variables: device
'''

import torch
import torch.nn as nn
import numpy as np
from torchvision import datasets, models, transforms
import os
import argparse
from PIL import Image

device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
'''
* Function Name: image_loader
* Input: image path
* Output: tensor of type float and on device
* Logic:    reads PIL image,
            applies transforms on it,
            makes it a 4-D tensor eg[1,3,224,224],
            return tensor
* Example Call:image_loader(image_path)
'''

def image_loader(image_name):
    data_transforms=transforms.Compose([transforms.Resize(256),
                                        transforms.CenterCrop(224),
                                        transforms.ToTensor(),
        transforms.Normalize([0.485, 0.456, 0.406], [0.229, 0.224, 0.225])])
    """load image, returns cuda tensor"""
    image = Image.open(image_name)#reads PIL image
    image = data_transforms(image).float()#transforms PIL image
    image = image.unsqueeze(0) #model expects a 4d tensor
    return image.to(device)  #reuturns tensor to which ever device is avaliable


'''
* Function Name: predict_animal
* Input:    image_path,animal_model
* Output:   predicted animal (string)
* Logic:    get tensor through image_loader(),
            input tensor to model,
            get predicted_index by the max value in the output tensor
            animal_class  at the index is returned
* Example Call:predict_animal(image,animal_model)
'''

def predict_animal(image,animal_model):
    animal_classes=['arctic fox', 'bear', 'bee', 'butterfly', 'cat', 'cougar', 'cow',
     'coyote', 'crab', 'crocodile', 'deer', 'dog', 'eagle', 'elephant', 'fish', 'frog',
      'giraffe', 'goat', 'hippo', 'horse', 'kangaroo', 'lion', 'monkey', 'otter', 'panda',
       'parrot', 'penguin', 'raccoon', 'rat', 'seal', 'shark', 'sheep', 'skunk', 'snake',
       'snow leopard', 'tiger', 'yak', 'zebra']
    img=image_loader(image)
    outputs=animal_model(img)
    _, preds = torch.max(outputs, 1)
    return animal_classes[preds[0]]


'''
* Function Name: predict_habitat
* Input:    image_path,habitat_model
* Output:   predicted habitat (string)
* Logic:    get tensor through image_loader(),
            input tensor to model,
            get predicted_index by the max value in the output tensor
            habitat_class at the index is returned
* Example Call:predict_habitat(image,model)
'''

def predict_habitat(image,habitat_model):
    habitat_classes=['baseball_diamond', 'basketball_court', 'beach', 'chaparral',
     'circular_farmland', 'cloud', 'commercial_area', 'dense_residential', 'desert',
      'forest', 'golf_course', 'harbor', 'island', 'lake', 'meadow', 'medium_residential',
     'mountain', 'rectangular_farmland', 'river', 'sea_ice', 'snowberg', 'sparse_residential',
      'thermal_power_station', 'wetland']
    img=image_loader(image)
    outputs=habitat_model(img)
    _, preds = torch.max(outputs, 1)
    return habitat_classes[preds[0]]


'''
* Function Name: predict_list
* Input:    model,img_path,animal_habitat=True/False
* Output:   predicted dictionary of animal/habitat with location
* Logic:    loads passed model,
            for images in passed directory predict animal/habitat,
            make a dictionary of predicted animal/habitat with their loaction as keys,
            return dictionary
* Example Call:predict_list(habitat_model,habitat_images_path,animal_habitat=False)
'''

def predict_list(model,img_path,animal_habitat=True):
    animal_list={}
    habitat_list={}

    '''load animal/habitat model'''
    if animal_habitat:
        animal_model=torch.load(model)
        animal_model.eval()
        animal_model=animal_model.to(device)
    else:
        habitat_model=torch.load(model)
        habitat_model.eval()
        habitat_model=habitat_model.to(device)
    '''predict labels'''
    for img in os.listdir(img_path):
        if animal_habitat:
            animal=predict_animal(os.path.join(img_path,img),animal_model)
            animal_list[img[:-4]]=animal
        else:
            habitat=predict_habitat(os.path.join(img_path,img),habitat_model)
            habitat_list[img[:-4]]=habitat
    if animal_habitat:
        return(animal_list)
    else:
        return(habitat_list)


'''
* Function Name: main
* Input:    NONE
* Output:   prints predicted animal and habitat
* Logic:    Call predict_animal,predict_habitat functions and pass respective images to them
'''

if __name__ == '__main__':
    animal=predict_animal(args.a)
    habitat=predict_habitat(args.ha)
    print(args.a,':-',animal,'\n',args.ha,':-',habitat)
    #loader(args.path)
