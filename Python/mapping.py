'''
* Team Id : HC#169
* Author List : Harsh Khatri
* Filename: mapping.py
* Theme: Homecoming
* Functions: check_habitat(),combination()
* Global Variables: file,xl,df1,habitat,sorted_list
'''
'''
loads the mapping table and creates mapping of animals in the arena to the available habitats in 
the arena image
'''

import pandas as pd
import numpy as np

# Assign spreadsheet filename to `file`
file = 'Animal_Habitat_Mapping.xlsx'


'''
* Function Name: check_habitat()
* Input: hab: habitat name
* Output: string 'valid'/'invalid'
* Logic:  checks and returns if the habitat is valid or invalid 
* Example Call:check_habitat('forest')
'''

def check_habitat(hab):
    return(d[hab][0])


'''
* Function Name: combination()
* Input: habitat_list,animal_list
* Output: combinations,max number of habitat linked to any one of animals
* Logic: it checks the mapping list and finds available habitats for a given animal 
         that are present in the arena
* Example Call:combination(habitat_list,animal_list) #habitats and animals present in the arena
'''

def combination(habitat_list,animal_list):
    comb={}
    for key in animal_list:
        hab_list=[]
        for each in d:
            if d[each][0]=='valid':
                for animals in d[each][1]:
                    if animals== animal_list[key]:
                        for hab in habitat_list:
                            if(each==habitat_list[hab]):
                                hab_list.append([hab,each])
        comb[key]=hab_list
    max_len=0
    for animal in sorted(comb):
        if max_len<len(comb[animal]):
            max_len=len(comb[animal])
    return(comb,max_len)


################################################################################
# Load spreadsheet
# preprocess data
# get mapping
################################################################################
xl = pd.ExcelFile(file)
df1 = xl.parse('Mapping')


habitat=[['a','a']]

for i in range(25):
    if(df1.loc[i]['Animal1'] is np.nan):
        animal=[]
        for each in df1.iloc[i][1:]:
            if 'nan' not in str(each):
                animal.append(str(each))
        habitat.append([df1.iloc[i]['Habitats'],'Invalid',animal])
    else:
        animal=[]
        for each in df1.iloc[i][1:]:
            if 'nan' not in str(each):
                animal.append(str(each))
        habitat.append([df1.loc[i]['Habitats'],'valid',animal])

sorted_list=[] 

for i in habitat:
    if i[1]=='Invalid':
        sorted_list.append(i)
for i in habitat:
    if i[1]=='valid':
        sorted_list.append(i)
for each in sorted_list:
    each[0]=each[0].replace(' ','_')

d = {key: [st,val] for (key, st,val) in sorted_list}
hab_num={key : 0 for (key) in d}


if __name__ == "__main__":
    pass
################################################################################
################################################################################