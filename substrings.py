g= open("data.txt","r+")
s=""
for aline in g:
    m=aline.rstrip('\n')
    s=s+m
g.close()


for i in range(0,3000,4):
        v=s[i:(i+4)]
        print(v)
        v=v[::-1]
        v=v[0:3]
        v="0x"+v
        x=int(v,16)
        print((x/10.5))

