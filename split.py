f= open('Salidadatos.dat', 'r')
lines = f.readlines()
f.seek(0)
result=[]
posx=[]
posy=[]

x={}
y={}

for x in lines:
	x = x.strip()
	result=x.split('	',4)
	posx.append(result[1])
	posy.append(result[2])

cin=0
cout=0
f.close()
nlineas =len(lines)

for i in range(-1,nlineas):
	if float(posx[i])>=-30 and float(posx[i])<=30 :
		if float(posy[i])>=-30 and float(posy[i])<=30 :
			cin=cin+1
	else :
		cout=cout+1

print 'cin: ', cin, 'cout: ', cout
