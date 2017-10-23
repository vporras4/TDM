name = 'workfile.txt'  				#Nombre del archivo

a = open(name, 'r+') 
b = a.readlines()	
a.seek(0)					#Remover encabezado
a.writelines(b[253:])
a.truncate()

'''
a.close()
lines = open(name).readlines()
open(name, 'w').writelines(lines[253:])
f = open(name,"r+")
'''
a.seek(0)
d = a.readlines()
a.seek(0)					#Remover e-
for i in d:
	if not i.startswith('e'):
		a.write(i)
a.truncate()
a.close()
