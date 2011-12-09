import os
import pickle

import numpy as np
import random
import matplotlib as mpl
import matplotlib.pyplot as plt

from matplotlib import rc
#rc('font',**{'family':'serif'})
#rc('text', usetex=True)
#rc('text.latex',unicode=True)
#rc('text.latex',preamble='\usepackage[utf8]{inputenc}')
#rc('text.latex',preamble='\usepackage[russian]{babel}')



res_dir = r'res'
pic_dir = r'pic/'


folders = [r'../../images/50/orig',
           r'../../images/50/rot33',
           r'../../images/50/rot130',
           r'../../images/50/scale12',
           r'../../images/50/scale2']

ivf_w_params = [r'none', r'tfidf']
ivf_n_params = [r'l1', r'l2']
#q_d_params = [r'l1', r'l2', r'cos']
q_d_params = [r'l1', r'cos']
clust = [3,5,10]
leavs = [20, 50, 100]
leavs3 = [20, 50, 100, 200, 300]




def fname(path):
    return os.path.basename(path)

def res_file(f,d,w,n,cl,lv):
    return "%(f)s__%(d)s__%(w)s__%(n)s__%(cl)d__%(lv)d.res" % \
        {'f':fname(f), 'd':d, 'w':w, 'n':n, 'cl':cl, 'lv':lv}

def load(f,d,w,n,cl,lv):
    res_f = res_file(f,d,w,n,cl,lv)
    res_f = res_dir + r'/' + res_f
    print(res_f)
    rf = open(res_f, 'rb')
    res = pickle.load(rf)
    rf.close()
    return res

###################

def color(f):
    c = {'orig':'k', 'rot33':'g', 'rot130':'r', 'scale12':'c', 'scale2':'m'}
    return c[fname(f)]
def line(f):
    c = {'orig':'-', 'rot33':'-', 'rot130':'--', 'scale12':'-.', 'scale2':':'}
    return c[fname(f)]

def coli(f):
    return color(f)+line(f)

def norm(r, m=None):
    if not m: m = max(r)
    return [a / m for a in r]


def res_by_good_mark_d():
    f = folders[0]
    d = q_d_params[0]
    w = ivf_w_params[1]
    n = ivf_n_params[1]
    cl = 3
    lv = 100

    lvs_arr = leavs3
    fig = plt.figure()
 
    counter = 1

    for n in ivf_n_params:
        for d in q_d_params:
        #for cl in clust:
            tit = "D = " + d + " N = " + n
            plt.subplot(2,2,counter)
            for f in folders:
                res = []
                for lv in lvs_arr:
                    r = load(f,d, w,n,cl,lv)
                    res.append(r[1][2])
                    pass
                print(f,res)
                plt.plot(lvs_arr, res, 'o'+coli(f), label=fname(f))
                plt.axis([0,max(lvs_arr),0,55])
                plt.xlabel("Leaves")
                plt.ylabel("Good count")
                plt.legend(loc=0, title=tit)

                pass
            #fig.savefig(pic_dir + "mark__n_" +n+"__d_" + d + ".png")
            counter += 1
            pass
        pass
    fig.savefig(pic_dir + "d.png")
    return


def res_by_good_mark_w():
    f = folders[0]
    d = q_d_params[0]
    w = ivf_w_params[1]
    n = ivf_n_params[0]
    cl = 3
    lv = 100

    lvs_arr = leavs3
    for w in ivf_w_params:
    #for d in q_d_params:
    #for cl in clust:
        tit = "W = " + w
        fig = plt.figure()
        for f in folders:
            res = []
            for lv in lvs_arr:
                r = load(f,d, w,n,cl,lv)
                res.append(r[1][2])
                pass
            print(f,res)
            plt.plot(lvs_arr, res, 'o'+coli(f), label=fname(f))
            plt.axis([0,max(lvs_arr),0,55])
            plt.xlabel("Leaves")
            plt.ylabel("Good count")
            plt.legend(loc=0, title=tit)

            pass
        fig.savefig(pic_dir + "mark__w_" + w + ".png")
        pass
    return

def res_by_good_mark_cl():
    f = folders[0]
    d = q_d_params[0]
    w = ivf_w_params[1]
    n = ivf_n_params[0]
    cl = 3
    lv = 100

    lvs_arr = leavs
    #for w in ivf_w_params:
    #for d in q_d_params:
    for cl in clust:
        tit = "Clusters = " + str(cl)
        fig = plt.figure()
        for f in folders:
            res = []
            for lv in lvs_arr:
                r = load(f,d, w,n,cl,lv)
                res.append(r[1][2])
                pass
            print(f,res)
            plt.plot(lvs_arr, res, 'o'+coli(f), label=fname(f))
            plt.axis([0,max(lvs_arr),0,55])
            plt.xlabel("Leaves")
            plt.ylabel("Good count")
            plt.legend(loc=0, title=tit)

            pass
        fig.savefig(pic_dir + "mark__cl_" + str(cl) + ".png")
        pass
    return

def res_by_gap_d():
    f = folders[0]
    d = q_d_params[0]
    w = ivf_w_params[1]
    n = ivf_n_params[1]
    cl = 3
    lv = 100

    lvs_arr = leavs3
    for n in ivf_n_params:
        for d in q_d_params:
        #for cl in clust:
            tit = "D = " + d + " N = " + n
            fig = plt.figure()

            for f in folders:
                res = []
                resf = []
                
                for lv in lvs_arr:
                    r = load(f,d, w,n,cl,lv)
                    s = 0
                    sf = 0
                    cnt = 0.000001
                    cntf = 0.000001
                    for a in r[1][0].iteritems():
                        it = a[1]
                        diff = abs(float(it[0][1]) - float(it[1][1]))
                        sf += diff
                        cntf += 1
                        if r[1][1][a[0]]:
                            s += diff
                            cnt += 1
                        else:
                            #print("skip", s, sf)
                            pass
                        pass
                    s = s/cnt
                    sf = sf/cntf
                    res.append(s)
                    resf.append(sf)
                    pass
                #print(f,res)
                z = list(res)
                z.extend(resf)
                res = norm(res, max(z))
                resf = norm(resf, max(z))
                plt.plot(lvs_arr, res, 'o'+coli(f), label=fname(f) + " good")
                plt.plot(lvs_arr, resf, '^'+coli(f), label=fname(f))
                #plt.axis([0,max(lvs_arr),0,55])
                plt.legend(loc=0, title=tit)
                plt.xlabel("Leaves")
                plt.ylabel("Sum gap")
                #plt.title(u"Разность между первым и вторым ответами от количества листьев")

                pass

            fig.savefig(pic_dir + "gap__n_" + n + "__d_" + d + ".png")
            pass
        pass
    return

#res_by_good_mark_d()
#res_by_good_mark_w()
#res_by_good_mark_cl()
res_by_gap_d()

plt.show()

