f= open("sss.txt","r+")
x_previous=0
t_previous=[]
t_count=0
def time(v):
    return (int(v[0])*60+int(v[1]))

for aline in f:
    v=aline.split()
    x= int(v[0])
    t=v[2].split(":")
    
    if((x_previous+1)!= x):
        if(x_previous==0):
            pass
        else:
            if(t[0]> t_previous[0]):
                t_count=time(t)-time(t_previous)
            else:
                t_count= (24+int(t[0]))*60+int(t[1])- time(t_previous)
                
            
            print("Data was missed between {} & {} and the time elapsed between them was {} minutes".format( x_previous, x, t_count))
       
    x_previous=x
    t_previous=t

f.close()
