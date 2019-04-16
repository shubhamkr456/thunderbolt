import matplotlib.pyplot as plt

g= open("data.txt","r+")

s=""
for aline in g:
    m=aline.rstrip('\n')
    s=s+m
g.close()
y=[]
y1=[]
t=[]

for i in range(0,3500,12):
        v=s[i+2:i+4]+s[i:(i+2)]
        
        
        x=int(v,16)
        x=x & 0x0FFF
        y.append(x/10.5)

for i in range(0,3500,12):
        v=s[i:(i+4)]
        
        v=v[::-1]
        v=v[0:3]
        v="0x"+v
        x=int(v,16)
        y1.append((x/10.5))




for i in range(0,len(y)):
    t.append(i)

plt.plot(t,y,'b',t,y1,'y')
plt.grid()
plt.show()


