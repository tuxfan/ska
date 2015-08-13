import numpy as np

def f(x, y):
    #return np.sin(np.pi*(x+y+1))*(np.sin(np.pi*(y - x + 1)))
    return np.sin(.25*np.pi*(x+y+1))*(.25*np.sin(np.pi*(y - x + 1)))
