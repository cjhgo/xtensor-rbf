
f=open('s.txt')
d=f.readlines()

for l in d:
  l = l.strip()
  name = l.split('/')[-1].split('so')[0]+'so'
  print(f"cp {l} /tmp/lib/{name}")