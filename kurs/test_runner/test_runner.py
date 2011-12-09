#!/usr/bin/env python

import os
import subprocess
import pickle

def check_output(*popenargs, **kwargs):
    if 'stdout' in kwargs:
        raise ValueError('stdout argument not allowed, it will be overridden.')
    process = subprocess.Popen(stdout=subprocess.PIPE, *popenargs, **kwargs)
    output, unused_err = process.communicate()
    retcode = process.poll()
    if retcode:
        cmd = kwargs.get("args")
        if cmd is None:
            cmd = popenargs[0]
        raise subprocess.CalledProcessError(retcode, cmd, output=output)
    return output

class CalledProcessError(Exception):
    def __init__(self, returncode, cmd, output=None):
        self.returncode = returncode
        self.cmd = cmd
        self.output = output
    def __str__(self):
        return "Command '%s' returned non-zero exit status %d" % (
            self.cmd, self.returncode)
# overwrite CalledProcessError due to `output` keyword might be not available
subprocess.CalledProcessError = CalledProcessError




###################################

folders = [r'../../images/50/orig',
           r'../../images/50/rot33',
           r'../../images/50/rot130',
           r'../../images/50/scale12',
           r'../../images/50/scale2']


res_dir = r'res'

file_list  = r'cache/file.list'
tree_file  = r'cache/hikm.tree'
ivf_file   = r'cache/ivf.file'

ivf_config  = r'ivf.config'
tree_config = r'tree.config'

query_maker = r'../kurs/bin/query_maker'

ivf_w_params = [r'none', r'tfidf']
ivf_n_params = [r'l1', r'l2']

#ivf_w_params = [r'none', r'bin', r'tf', r'tfidf']
#ivf_n_params = [r'none', r'l0', r'l1', r'l2']

q_d_params = [r'l1', r'l2', r'cos']

clust = [3,5,10]
leavs = [20, 50, 100]

###################################

def getIvfCmdParams(w, n):
    return r"IVF_CREATOR_OPTS='-W%(w)s -N%(n)s'" % \
        {'w': ivf_w_params[w], 'n': ivf_n_params[n] }

def writeIvfConfig(w, n):
    f = open(ivf_config, 'wt')
    f.write("weight = %(w)s\nnorm = %(n)s" % \
        {'w': ivf_w_params[w], 'n': ivf_n_params[n] })
    f.close()
    return "IVF_CREATOR_OPTS=-c" + ivf_config


#def writeTreeConfig(cl, lv):
#    f = open(tree_config, 'wt')
#    f.write("clustres = %(c)s\nleaves = %(l)s" % \
#        {'c': cl, 'l': lv })
#    f.close()
#    return "TREE_CREATOR_OPTS=-c" + tree_config

def writeIvfConfigS(w, n):
    f = open(ivf_config, 'wt')
    f.write("weight = %(w)s\nnorm = %(n)s" % \
        {'w': w, 'n': n })
    f.close()
    return "IVF_CREATOR_OPTS=-c" + ivf_config


def writeTreeConfigS(cl, lv):
    f = open(tree_config, 'wt')
    f.write("clustres = %(c)s\nleaves = %(l)s" % \
        {'c': cl, 'l': lv })
    f.close()
    return "TREE_CREATOR_OPTS=-c" + tree_config

####################################
fl = []
try:
    f = open(file_list, 'rt')
    fl = f.read().splitlines()
    f.close()
except IOError as e:
    print (e)
    pass


def num2file(num):
    return fl[int(num)]

####################################

def make_query(query, tree, ivf, dist):
    cmd = ' '.join([query_maker, '-i', query, '-t', tree, '-v', ivf, '-D', dist, '-o --'])
    print(cmd)
    res = check_output(cmd, shell=True)
    return res.splitlines()

def fname(path):
    return os.path.basename(path)

def test(dir, dist):
    files = os.listdir(dir)

    res = {}
    good = {}
    good_count = 0

    for f in files:
        qres = make_query(dir + r'/' + f, tree_file, ivf_file, dist)
        fqres = [(num2file(b[0]),b[1]) for b in [a.split() for a in qres]]
        res[f] = fqres
        g = f == fname(fqres[0][0])
        good[f] = g
        if g: 
            good_count += 1
            pass

        pass

    return res, good, good_count
#a,b,c, = test(folders[4])
#print (a,b,c)


def make_base(params = []):
    prm = ['-j4']
    prm.extend(params)
    cmd = ['make']
    cmd.extend(prm)
    cmd.append('base')
    cmd = ' '.join(cmd)
    print (cmd)
    res = ''
    try:
        res = check_output(cmd, stderr = subprocess.STDOUT, shell=True)
    except subprocess.CalledProcessError as e:
        print(e)
        print(e.output)
        return False
    print(res)
    return True

def make_testclean():
    cmd = ['make', 'testclean']
    try:
        res = check_output(cmd, stderr = subprocess.STDOUT)
    except subprocess.CalledProcessError as e:
        print(e)
        return False
    print(res)
    return True


#make_base([writeIvfConfig(1,1), writeTreeConfig(3,200)])

def res_file(f,d,w,n,cl,lv):
    return "%(f)s__%(d)s__%(w)s__%(n)s__%(cl)d__%(lv)d.res" % \
        {'f':fname(f), 'd':d, 'w':w, 'n':n, 'cl':cl, 'lv':lv}

def run_test(f, d, w, n, cl, lv):
    res_f = res_file(f,d,w,n,cl,lv)
    print(res_f)    

    rf = open(res_dir + r'/' + res_f, 'wb')

    make_testclean()
    make_base([writeIvfConfigS(w,n), writeTreeConfigS(cl, lv)])

    a,b,c = test(f, d)
    pickle.dump([ [f, d, w, n, cl, lv], [a,b,c] ], rf)

    rf.close()
    pass



def run_folder(f):
    #ivf_w_params.reverse()
    #ivf_n_params.reverse()
    #q_d_params.reverse()
    #clust.reverse()
    #leavs.reverse()
    #folders.reverse()

    for w in ivf_w_params:
        for n in ivf_n_params:
            for d in q_d_params:
                for cl in clust:
                    for lv in leavs:
                        run_test(f, d, w, n, cl, lv)
                        #return
                        pass
                    pass
                pass
            pass
        pass
    return


def run_all():
    for f in folders:
        pass
    return

try:
    run_all()
except subprocess.CalledProcessError as e:
    print(e)
    print(e.output)

os.system('pause')
