import os
import sys
import urlparse

argv = sys.argv

if (len(argv) < 4):
	raise "Not enough arguments"

file_list = argv[1]
query_in  = argv[2]
query_out = argv[3]
res_out   = argv[4]


f = open(file_list, 'rt')
fl = f.read().splitlines()
f.close()

f = open(query_in, 'rU')
query = f.readline()
f.close()

query = os.path.abspath(query).replace(r"cygdrive/d", r"D:")


f = open(query_out, 'rU')
ql = f.read().splitlines()
f.close()

res = [(fl[int(b[0])],b[1]) for b in [a.split() for a in ql]]

def to_url(path):
	return urlparse.urlunparse( ('file', '', path, '', '', '') ) 


f = open(res_out, 'w')
f.write(r'<html><head></head><body>')

f.write('<p>Query<br/><img border="1" src="')
f.write(to_url(query))
f.write('"></p><hr/>')

f.write('<p>Results</p><p><table border="1">')
for r in res:
	url = to_url(r[0])
	print(url)

	f.write('<tr><td><img src="')
	f.write(url)
	f.write('"></td><td>')
	f.write(r[1])
	f.write('</td></tr>')
	pass

f.write(r'</table></p></body></html>')
f.close()