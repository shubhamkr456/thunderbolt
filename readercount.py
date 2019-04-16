f= open("sss.txt","r+")
x_previous=0
t_previous=0
def time(v):
    return (int(v[0])+int(v[1]))

for aline in f:
    v=aline.split()
    x= int(v[0])
    t=v[2].split(":")
    
    if((x_previous+1)!= x):
        if(x_previous==0):
            pass
        else:
            
            print("Data was missed between %d & %d and the time elapsed between them was % minutes", x_previous, t_previous, t_previous)
       
    x_previous=x

f.close()
