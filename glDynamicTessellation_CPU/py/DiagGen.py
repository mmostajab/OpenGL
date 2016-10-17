import matplotlib.pyplot as plt
import numpy as np
from matplotlib2tikz import save as tikz_save
from matplotlib.backends.backend_pdf import PdfPages

input_list_gl = []
result_list_gl = []

input_list_ifx = []
result_list_ifx = []

with open('TrianglesPerSecond.txt') as f:
    for line in f:
        w, h  = map(float, line.split())
        input_list_gl.append(w)
        #print("1 === ")
        #print(w)

        result_list_gl.append(h)
        #print("2 === ")
        #print(h)

with open('TrianglesPerSecond_ifx.txt') as f:
    for line in f:
        w, h  = map(float, line.split())
        input_list_ifx.append(w)
        #print("1 === ")
        #print(w)

        result_list_ifx.append(h)
        #print("2 === ")
        #print(h)


pp = PdfPages('Comparison.pdf')
plt.figure(figsize=(12,9), dpi=300)

results_gl  = plt.plot(input_list_gl, result_list_gl, '-g', marker = 'o', label='OpenGL')
plt.plot(input_list_gl, result_list_gl, 'go')

results_ifx = plt.plot(input_list_ifx, result_list_ifx, '-b', marker = '^', label='iFX')
plt.plot(input_list_ifx, result_list_ifx, 'b^')

plt.xlabel('Number of triangles')
plt.ylabel('Computation time (mSecs)')

#plt.xscale('log')
#plt.yscale('log')
plt.legend(bbox_to_anchor=(0.68, 0.98), loc=2, borderaxespad=0.)

plt.grid(True)
plt.savefig("Comparison.svg", figsize=(12,9), dpi=300)
plt.savefig("Comparison.png", figsize=(12,9), dpi=300)
tikz_save('Comparison.tex', figureheight = '\\figureheight', figurewidth = '\\figurewidth');
pp.savefig(plt.gcf());
plt.show()


pp.close()
