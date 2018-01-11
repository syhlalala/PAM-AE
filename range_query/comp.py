f = open("res.txt")
fout = open('data.txt','w')
line1 = f.readline()
line2 = f.readline()

items1 = line1.split('\t')
items2 = line2.split('\t')

build_par = float(items1[7][11:])
build_seq = float(items2[7][11:])
build_spd = build_seq/build_par

query_par = float(items1[8][11:])
query_seq = float(items2[8][11:])
query_spd = query_seq/query_par

fout.write(items1[1]+', '+items1[2]+', '+items1[3]+', '+items1[4]+', ')
fout.write("sequential build=" + str(build_seq) + ", parallel build=" + str(build_par) + ", build speedup=" + str(build_spd)+'\n')
fout.write(items1[1]+', '+items1[2]+', '+items1[3]+', '+items1[4]+', ')
fout.write("sequential query=" + str(query_seq) + ", parallel query=" + str(query_par) + ", query speedup=" + str(query_spd)+'\n')


line1 = f.readline()
line2 = f.readline()

items1 = line1.split('\t')
items2 = line2.split('\t')

build_par = float(items1[7][11:])
build_seq = float(items2[7][11:])
build_spd = build_seq/build_par

query_par = float(items1[8][11:])
query_seq = float(items2[8][11:])
query_spd = query_seq/query_par

fout.write(items1[1]+', '+items1[2]+', '+items1[3]+', '+items1[4]+', ')
fout.write("sequential build=" + str(build_seq) + ", parallel build=" + str(build_par) + ", build speedup=" + str(build_spd)+'\n')
fout.write(items1[1]+', '+items1[2]+', '+items1[3]+', '+items1[4]+', ')
fout.write("sequential query=" + str(query_seq) + ", parallel query=" + str(query_par) + ", query speedup=" + str(query_spd)+'\n')

fout.close()
