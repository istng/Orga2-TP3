import numpy as np
import scipy.interpolate as interp
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import mpl_toolkits.mplot3d.axes3d as axes3d


def manta3D(X, Y, Z, ejex, ejey, titulo):

	plotx,ploty, = np.meshgrid(np.linspace(np.min(X),np.max(X),10),\
	                           np.linspace(np.min(Y),np.max(Y),10))
	plotz = interp.griddata((X,Y),Z,(plotx,ploty),method='linear')
	
	fig = plt.figure()
	ax = fig.add_subplot(111, projection='3d')
	ax.plot_surface(plotx,ploty,plotz,cmap='viridis')  # or 'hot'
	
	plt.title(titulo)
	plt.xlabel(ejex)
	plt.ylabel(ejey)


	plt.show()
	plt.close()


def puntosConDesvio3D(fx, fy, fz, zerror, ejex, ejey, titulo):
	fig = plt.figure(dpi=100)
	ax = fig.add_subplot(111, projection='3d')
	
	#error data
	xerror = []
	yerror = []
	for i in range(0,len(zerror)):
		xerror.append(0)
		yerror.append(0)
	
	#plot points
	ax.plot(fx, fy, fz, linestyle="None", marker="o")
	
	#plot errorbars
	for i in np.arange(0, len(fx)):
	    ax.plot([fx[i]+xerror[i], fx[i]-xerror[i]], [fy[i], fy[i]], [fz[i], fz[i]], marker="_")
	    ax.plot([fx[i], fx[i]], [fy[i]+yerror[i], fy[i]-yerror[i]], [fz[i], fz[i]], marker="_")
	    ax.plot([fx[i], fx[i]], [fy[i], fy[i]], [fz[i]+zerror[i], fz[i]-zerror[i]], marker="_")
	
	#configure axes

	plt.title(titulo)
	plt.xlabel(ejex)
	plt.ylabel(ejey)

	plt.show()
	plt.close()


def puntosConDesvio(xs, ys, err, XS, YS, ERR, labelm, labelM, ejex, ejey, titulo):
	plt.errorbar(xs, ys, err, linestyle='None', marker='^', label=labelm)
	plt.errorbar(XS, YS, ERR, linestyle='None', marker='o', label=labelM)
	plt.legend(bbox_to_anchor=(0, 0), loc=1, borderaxespad=0.)
	plt.title(titulo)
	plt.xlabel(ejex)
	plt.ylabel(ejey)
	plt.show()
	plt.close()



def barrasConDesvio(xs, ys, err, XS, YS, ERR, ancho, labelm, labelM, ejex, ejey, titulo):
	offset = ancho #esto es re desprolijo
	plt.bar([e + offset for e in xs],  [e for e in ys], ancho, color="blue", label=labelm, yerr=err, ecolor="cyan")
	plt.bar([e for e in XS], [e for e in YS], ancho, color="red", label=labelM, yerr=ERR, ecolor="magenta")
	plt.legend(bbox_to_anchor=(0, 0), loc=1, borderaxespad=0.)
	plt.xlabel(ejex)
	plt.ylabel(ejey)
	plt.show()
	plt.close()