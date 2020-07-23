'''
* Team Id : HC#169
* Author List : Harsh Khatri
* Filename: pathfinder.py
* Theme: Homecoming
* Functions: distance(),habloc_to_component(),aniloc_to_component(),optimize()
* Global Variables: none
'''

'''
finds the distance between animal and habitat and optimizes the list so that least amount of distance is traveled
by the robot while performaing the task
'''

'''
* Function Name: distance()
* Input: a :- animal loaction,h :- habitat_loaction
* Output: distance
* Logic:  distance between the 2 is sun of diffenrence in x component and y component
* Example Call:distance(animal_loaction,habitat_loaction) : distance('A1','5')
'''

def distance(a,h):
    ax,ay=a[0]
    hx,hy=h[0]
    difx=dify=0
    if(ax>hx):
        difx=ax-hx
    else:
        difx=hx-ax
    if(ay>hy):
        dify=ay-hy
    else:
        dify=hy-ay
    return(difx+dify)

'''
* Function Name: habloc_to_component()
* Input:  habitat_loaction eg:-10
* Output: (x,y) 
* Logic:  calculates the x and y component of the habitat loaction and returns them
* Example Call:habloc_to_component(3)
'''

def habloc_to_component(habitat):
    comp=[]
    for loc in habitat:
        locx=int(loc)%5-1
        if(locx<0):
            locx=4
        i=0
        while(i*5<int(loc)):
            i+=1

        locy=i-1
        comp.append([locx,locy])
    return comp

'''
* Function Name: aniloc_to_component()
* Input:  animal_loaction eg:-B6
* Output: (x,y) 
* Logic:  calculates the x and y component of the animal loaction and returns them
* Example Call:aniloc_to_component('F3')
'''

def aniloc_to_component(animal):
    comp=[]
    for loc in animal:
        locx=loc[0]
        locy=int(loc[1])-1
        comp.append([ord(locx)-65,locy])
    return comp

'''
* Function Name: optimize()
* Input:  animal_list,habutat_list,eg:-['A1','A4','F2'],['3','21','12']
* Output: optimized_animal_list,optimized_habitat_list,cost(distance_traveled) 
* Logic:  calculates the distance needed to be traveled for the input list the optimizes the sequence
*         to take least amount of distance traveled by the robot and returns them
* Example Call:optimize(animal_list,habitat_list)
'''

def optimize(animal_list,habitat_list):
    dec = dict(zip(animal_list, habitat_list))
    hab=[]
    ani=[]
    hab=[]
    n=str(len(animal_list))
    ani=animal_list
    hab=habitat_list
    a=aniloc_to_component(ani)
    h=habloc_to_component(hab)
    i=0
    final=[]
    for loca,loch in zip(a,h):
        ax,ay=loca
        hx,hy=loch
        if(ax>hx):
            difx=ax-hx
        else:
            difx=hx-ax
        if(ay>hy):
            dify=ay-hy
        else:
            dify=hy-ay
        final.append([ani[i],hab[i],difx+dify])
        i+=1
    graph=[[0]*int(n) for i in range(int(n))]
    for i in range(int(n)):
        for j in range(int(n)):
            if(i != j):
                a=[final[i][1]]
                b=[final[j][0]]
                graph[i][j]=distance(aniloc_to_component(b),habloc_to_component(a))+final[j][2]
    start=0
    num=1
    seq=[-1 for i in range(int(n))]
    seq[0]=final[0][0]
    next_=start
    while(num!=int(n)):
        min_=10000
        flag=2
        for i in range(int(n)):
            flag=2
            if(graph[start][i]<min_ and graph[start][i]!=0 ):
                for j in seq:
                    if(final[i][0]==j):
                        flag=1  
                if(flag==2):
                    next_=i
                    seq[num]=final[i][0]
                    min_=graph[start][i]
        start=next_
        num+=1
    
    default_cost=0
    current_loc=0
    for j in animal_list:
        for i in range(int(n)):
            if(final[i][0]==j):
                default_cost+=graph[current_loc][i]
                current_loc=i
    greedy_cost=0
    current_loc=0
    for j in seq:
        for i in range(int(n)):
            if(final[i][0]==j):
                greedy_cost+=graph[current_loc][i]
                current_loc=i
    output_animal=[]
    output_habitat=[]
    if(greedy_cost<default_cost):
        for each in seq:
            output_habitat.append(dec[each])
            output_animal.append(each)
        return(output_animal,output_habitat,greedy_cost)
    else:
        return(animal_list,habitat_list,default_cost)
