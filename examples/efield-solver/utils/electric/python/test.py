import numpy as np
import scipy as sp
import pylab as p
import mpl_toolkits.mplot3d.axes3d as p3
import matplotlib.pyplot as plt
import sys, os
import scipy.sparse
import numpy as np
import matplotlib.pyplot as plt
sys.path.append(os.path.join(os.environ['PETSC_DIR'], 'bin', 'pythonscripts'))
import PetscBinaryIO
io = PetscBinaryIO.PetscBinaryIO()
mat = io.readBinaryFile('./a.dat', mattype='scipy.sparse')

a = mat[0]
#plt.spy(a)
#plt.show()

def f(x,y):
    #return 2*((1-6*x**2)*(y**2)*(1-y**2) + (1-6*y**2)*(x**2)*(1-x**2))
    #return 8*(np.pi**2)*np.sin(2*np.pi*x)*np.sin(2*np.pi*y)
    t1 = (np.pi**2)*np.cos(np.pi*x)
    t2 = (np.cos(np.pi*y) - 1) + np.cos(np.pi*y)
    return t1*t2

def sol(x,y):
    #return ((x**2) - (x**4))*((y**4) - (y**2))
    #return np.sin(2*np.pi*x) * np.sin(2*np.pi*y)
    return np.cos(np.pi*x)*(np.cos(np.pi*y) - 1)


nx = 5
ny = nx
hx = 1./(nx-1)
hy = 1./(ny-1)

#X, Y = np.meshgrid(np.linspace(0,1,nx), np.linspace(0,1,ny))

#b = np.loadtxt('./b.txt', skiprows=5, comments='Process')
#u = np.loadtxt('./x.txt', skiprows=5, comments='Process')
#newb = np.zeros(b.shape[0])


for j in range(1,5):#(ny):
    for i in range(1,5):#(nx):
        x = i*hx
        y = j*hy
        ind = i*ny + j
        #print hx*hy*f(x,y), b[ind]
        #print sol(x,y), u[ind]


#fig = p.figure()
#ax = p3.Axes3D(fig)
#ax.plot_wireframe(X,Y,b.reshape((ny,nx)))
#p.show()
s = a - a.T

def row(i):
    global a
    d = a.data[a.indptr[i]:a.indptr[i+1]]
    cols = a.indices[a.indptr[i]:a.indptr[i+1]]
    for k in range(len(d)):
        if abs(d[k]) > 1e-7:
            print '(', i, cols[k], ')', '->', d[k]


def rowv(a, i):
    d = a.data[a.indptr[i]:a.indptr[i+1]]
    cols = a.indices[a.indptr[i]:a.indptr[i+1]]
    for k in range(len(d)):
        if abs(d[k]) > 1e-7:
            print '(', i, cols[k], ')', '->', d[k]

def plot():
    global a
    plt.spy(a, precision=1e-7)
    plt.show()


def sym():
    global s
    plt.spy(s, precision=1e-7)
    plt.show()
