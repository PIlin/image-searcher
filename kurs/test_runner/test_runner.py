import os
import subprocess


folders = [r'../../images/50/orig/',
           r'../../images/50/rot33/',
           r'../../images/50/rot130/',
           r'../../images/50/scale1_2/',
           r'../../images/50/scale2/']

file_list = r'cache/file.list'
tree_file = r'cache/hikm.tree'
ivf_file  = r'cache/ivf.file'

query_maker = r'../kurs/bin/query_maker.exe'

####################################
f = open(file_list, 'rt')
fl = f.read().splitlines()
f.close()

def num2file(num):
    return fl[int(num)]

####################################

def make_query(query, tree, ivf):
    cmd = ' '.join([query_maker, '-i', query, '-t', tree, '-v', ivf, '-o --'])
    print(cmd)
    res = subprocess.check_output(cmd)
    return res.splitlines()

def fname(path):
    return os.path.basename(path)

def test(dir):
    files = os.listdir(dir)

    res = {}
    good = {}
    good_count = 0

    for f in files:
        qres = make_query(dir + f, tree_file, ivf_file)
        fqres = [(num2file(b[0]),b[1]) for b in [a.split() for a in qres]]
        res[f] = fqres
        g = f == fname(fqres[0][0])
        good[f] = g
        if g: 
            good_count += 1
            pass

        break
        pass

    return res, good, good_count
a,b,c, = test(folders[4])
print (a,b,c)