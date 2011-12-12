#-------------------------------------------------------------------------------
# Name:        module1
# Purpose:
#
# Author:      ilknur
#
# Created:     10/12/2011
# Copyright:   (c) ilknur 2011
# Licence:     <your licence>
#-------------------------------------------------------------------------------
#!/usr/bin/env python

import vtk
origDataPath = '/Users/mn/programming/mnCALATK-build/bin/lungNodule/';
resPath = '/Users/mn/programming/mnCALATK-build/bin/';
origDataPrefix = 'lung-';

import nrrd as nd
import numpy as np
import scipy as sp
import pylab as P
import matplotlib.mlab as mlab
import matplotlib.pyplot as plt
import matplotlib.cm as cm
from matplotlib import rc
import os,sys
import Image

def displayGeometResults( inputFilePath, prefix ):

    I0Filename = inputFilePath + prefix + "I0-orig.nrrd"
    I1Filename = inputFilePath + prefix + "I1-orig.nrrd"
    I1EstFilename = inputFilePath + prefix + "orig-EstI1.nrrd"
    T1EstFilename = inputFilePath + prefix + "orig-EstT1.nrrd"
    T2EstFilename = inputFilePath + prefix + "orig-EstT2.nrrd"
    map0OutFilename = inputFilePath + prefix + "map0Out.nrrd"
    map1OutFilename = inputFilePath + prefix + "map1Out.nrrd"
    map2OutFilename = inputFilePath + prefix + "map2Out.nrrd"
    map12OutFilename = inputFilePath + prefix + "map12.nrrd"



    I0 = nd.Nrrd(I0Filename)
    I1 = nd.Nrrd(I1Filename)
    I1Est = nd.Nrrd(I1EstFilename)
    T1Est = nd.Nrrd(T1EstFilename)
    T2Est = nd.Nrrd(T2EstFilename)
    map0Out = nd.Nrrd(map0OutFilename)
    map1Out = nd.Nrrd(map1OutFilename)
    map2Out = nd.Nrrd(map2OutFilename)
    map12Out = nd.Nrrd(map12OutFilename)
    print "I0 size ", I0.data.shape

    ysize, xsize = I0.data.shape
    [gridX,gridY]=np.meshgrid(np.arange(1,xsize+1),np.arange(1,ysize+1))

    plt.figure()
    plt.title(prefix + " Geomet Results")
    plt.subplot(235)
    P.imshow(T1Est.data,cmap=cm.gray)
    v = plt.axis()
    plt.axis(v)
    ylim = plt.get(plt.gca(), 'ylim')
    CS = plt.contour(gridX,gridY,map1Out.data[0,:,:], 20, hold='on', colors='red')
    CS = plt.contour(gridX,gridY,map1Out.data[1,:,:], 20, hold='on', colors='red')
    plt.title('Est-T1')

    plt.subplot(236)
    P.imshow(T2Est.data,cmap=cm.gray)
    v = plt.axis()
    plt.axis(v)
    ylim = plt.get(plt.gca(), 'ylim')
    CS = plt.contour(gridX,gridY,map12Out.data[0,:,:], 20, hold='on', colors='red')
    CS = plt.contour(gridX,gridY,map12Out.data[1,:,:], 20, hold='on', colors='red')
    plt.title('Est-T2')

    #plt.figure()
    plt.subplot(234)
    P.imshow(I1Est.data,cmap=cm.gray)
    v = plt.axis()
    plt.axis(v)
    ylim = plt.get(plt.gca(), 'ylim')
    CS = plt.contour(gridX,gridY,map1Out.data[0,:,:], 20, hold='on', colors='red')
    CS = plt.contour(gridX,gridY,map1Out.data[1,:,:], 20, hold='on', colors='red')
    plt.title('Est-I1')

    plt.subplot(231)
    P.imshow(I0.data,cmap=cm.gray)
    v = plt.axis()
    plt.axis(v)
    ylim = plt.get(plt.gca(), 'ylim')
    plt.title('Orig-I0')

    plt.subplot(232)
    P.imshow(I1.data,cmap=cm.gray)
    v = plt.axis()
    plt.axis(v)
    ylim = plt.get(plt.gca(), 'ylim')
    plt.title('Orig-I1')
    outputFileName1 = inputFilePath + prefix + "ALL.png"
    plt.savefig(outputFileName1)

def main():
    inputFilePath = "TEST1\\"
    prefix = list()
    prefix.append("run1-res-")

    for p in prefix:
        displayGeometResults( inputFilePath, p )

    pass

if __name__ == '__main__':
    main()
