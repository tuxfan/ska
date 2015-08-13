#!/usr/bin/env python

import matplotlib.pyplot as plt
import numpy as np

eigs = np.loadtxt('data/eigs.txt',dtype=complex)
print 'Minimum eigval: ', np.min(np.abs(eigs))
plt.scatter(eigs.real, eigs.imag)
#plt.xlim(0)
plt.show()
