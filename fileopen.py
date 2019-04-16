f= open("shub.txt", "r+")
g= open("data.txt","a+")
s=""
for aline in f:
    v=aline.split("-")
    
    g.write(v[3])
    print(s)
f.close()
g.close()