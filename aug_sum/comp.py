f = open("res.txt")
line = f.readline()
d = {}
note=''
while line:
    items = line.split('\t')
    if len(items) < 3:
        if items[0]=='AUG\n':
            note='A'
        if items[0]=='NoAug\n':
            note='N'
        line = f.readline()
        continue
    tmp = items[1].split('=')
    name = tmp[1]

    tmp = items[2].split('=')
    time = tmp[1]

    tmp = items[3].split('=')
    n = tmp[1]

    tmp = items[4].split('=')
    m = tmp[1]

    test=name+n+m+note
    if (not d.has_key(test)):
        d[test] = {}
    d[test]['name'] = name
      
    tmp = items[6].split('=')
    p = int(tmp[1])
    #print p

    if (p==1):
        d[test]['seq']=time
    else:
        d[test]['para']=time
    d[test]['n'] = n
    d[test]['m'] = m
#    print d

    line = f.readline()
f.close()

fout = open('data.txt','w')
for i in d.keys():
    if i[-1] == 'A':
        fout.write('Augmented\t')
    if i[-1] == 'N':
        continue
    fout.write('test='+d[i]['name']+'\t')
    fout.write('n='+d[i]['n']+'\t')
    fout.write('m='+d[i]['m']+'\t')
    if (d[i].has_key('seq')):
        fout.write('sequential_time='+d[i]['seq']+'\t')                
    if (d[i].has_key('para')):                
        fout.write('parallel_time='+d[i]['seq']+'\t')
    if (d[i].has_key('para') and d[i].has_key('seq')):
        speed=float(d[i]['seq'])/float(d[i]['para'])
        fout.write('speedup='+str(speed))
    fout.write('\n')

fout.write('\n')
for i in d.keys():
    if i[-1] == 'A':
        continue
    if i[-1] == 'N':
        fout.write('Non-augmented\t')
    fout.write('test='+d[i]['name']+'\t')
    fout.write('n='+d[i]['n']+'\t')
    fout.write('m='+d[i]['m']+'\t')
    if (d[i].has_key('seq')):
        fout.write('sequential_time='+d[i]['seq']+'\t')                
    if (d[i].has_key('para')):                
        fout.write('parallel_time='+d[i]['seq']+'\t')
    if (d[i].has_key('para') and d[i].has_key('seq')):
        speed=float(d[i]['seq'])/float(d[i]['para'])
        fout.write('speedup='+str(speed))
    fout.write('\n')

fout.close()
    
