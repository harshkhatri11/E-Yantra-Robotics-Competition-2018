'''
* Team Id : HC#169
* Author List : Harsh Khatri
* Filename: connect.py
* Theme: Homecoming
* Functions: find_connection(),find_alternate_route(),try_changing()
* Global Variables: combination_list[]
'''

'''
* places the animals into the closest available habitat
* if any animal is left un placed while having availabe habitats which are full
* it tries to optimize it so that most number of animals can be placed in the arena for a given configuration
'''
import pathfinder

'''
* Function Name: try_changing()
* Input: combination,max_len,habitat_list,habitats,hab
* Output: boolean true,false
* Logic:  for each animal placed in the habitat (hab) it cheaks if the animal can be placed
*         in an other location ,if it is possible it changes that animals loaction
*         and returns True else it returns False
* Example Call:try_changing(combination,max_len,habitat_list,habitats,hab)
'''
# it checks if there is any other habitat available for the animals in that habitat and if true it replaces them
combination_list=[]
def try_changing(combination,max_len,habitat_list,habitats,hab):
    if(len(combination[hab])<1):
        return(0,False)
    else :
        for habt in combination_list:
            if habt[1]==combination[hab][0][0]:
                flag=find_alternate_route(combination,max_len,habitat_list,habitats,[habt[0]])
                if flag:
                    return(1,flag)
                else:
                    return(0,False)

'''
* Function Name: find_alternate_route()
* Input: combination,max_len,habitat_list,habitats,ani
* Output: boolean true,false
* Logic:  animal which have available habitats in the arena but those habitat are full with
*         other animals are given to this function,then it tries to rearrange placed animals
*         to make room for this animal
* Example Call:find_alternate_route(combination,max_len,habitat_list,habitats,ani)
'''

def find_alternate_route(combination,max_len,habitat_list,habitats,ani):
    print('alternate',ani,end=' ')
    for each in combination_list:
        temp=each[1]
        for comb in combination[each[0]]:
            if comb[0]==temp:
                combination[each[0]].remove(comb)
    for each in combination[ani[0]]:
        if(habitats[each[0]]<2):
            for temp in combination_list:
                if(temp[0]==ani[0]):
                    habitats[temp[1]]-=1
                    temp[1]=each[0]
                    habitats[each[0]]+=1
                    break
            return(True)
        else:
            for hab in combination_list:
                if hab[1]==each[0]:
                    change,flag=try_changing(combination,max_len,habitat_list,habitats,hab[0])
                    if(flag):
                        print('success')
                        for temp in combination_list:
                            if(temp[0]==hab[0]):
                                habitats[temp[1]]-=1
                                temp[1]=combination[hab[0]][0][0]
                                habitats[temp[1]]+=1
                                break
                        flag=True
                        for temp in combination_list:
                            if temp[0]==ani[0]:
                                flag=False
                        if flag:
                            temp=[ani[0],each[0],0]
                            combination_list.append(temp)
                            habitats[each[0]]+=1    
                        else:
                            for temp in combination_list:
                                if(temp[0]==ani[0]):
                                    habitats[temp[1]]-=1
                                    temp[1]=each[0]
                                    habitats[temp[1]]+=1
                                    break
                        return(True)
    print('failed')
    return(False)

'''
* Function Name: find_connection()
* Input: combination,max_len,habitat_list
* Output: combination_list
* Logic:  finds all the available habitat for a animal and places the animal in 
*         the closest available habitat which is not full
* Example Call:find_connection(combination,max_len,habitat_list)
'''

def find_connection(combination,max_len,habitat_list):
    hab={key:0 for key in habitat_list}
    habitats=hab
    min_distance=100000
    for i in range(1,max_len+1):
        for animal in combination:
            temp=[0,0,0]
            min_distance=100000
            if len(combination[animal])==i:
                for hab in combination[animal]:
                    ani=[animal]
                    habt=[hab[0]]
                    axy=pathfinder.aniloc_to_component(ani)
                    hxy=pathfinder.habloc_to_component(habt)
                    disstance=pathfinder.distance(axy,hxy)
                    if(disstance<min_distance):
                        if habitats[hab[0]]<2:
                            temp=[animal,hab[0],disstance]
                            min_distance=disstance
                if(temp[0]!=0):
                    combination_list.append(temp)
                    habitats[temp[1]]+=1
                else:
                    find_alternate_route(combination,max_len,habitat_list,habitats,ani)
    for each in combination_list:
        axy=pathfinder.aniloc_to_component([each[0]])
        hxy=pathfinder.habloc_to_component([each[1]])
        disstance=pathfinder.distance(axy,hxy)
        each[2]=disstance  
    return(combination_list)
    