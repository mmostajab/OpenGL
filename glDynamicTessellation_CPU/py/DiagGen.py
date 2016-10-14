import matplotlib.pyplot as plt
import numpy as np
from matplotlib2tikz import save as tikz_save
from matplotlib.backends.backend_pdf import PdfPages

input_list = []
result_list = []

with open('TrianglesPerSecond.txt') as f:
    w, h  = map(int, f.readline().split())
    input_list.append(w)
    print("1 === ")
    print(w)

    input_list.append(h)
    print("2 === ")
    print(h)



pp = PdfPages('Result.pdf')
plt.figure(figsize=(12,9), dpi=300)

results  = plt.plot(input_list, result_list, '-g', marker = 'o', label='Ray tracing')
plt.plot(input_list, result_list, 'go')

plt.xlabel('Number of triangles')
plt.ylabel('Computation time (mSecs)')

plt.xscale('log')
plt.yscale('log')
plt.legend(bbox_to_anchor=(0.68, 0.98), loc=2, borderaxespad=0.)

plt.grid(True)
plt.savefig("SubtractionEvaluation1.svg", figsize=(12,9), dpi=300)
plt.savefig("SubtractionEvaluation1.png", figsize=(12,9), dpi=300)
tikz_save('SubtractionEvaluation1.tex', figureheight = '\\figureheight', figurewidth = '\\figurewidth');
pp.savefig(plt.gcf());
plt.show()


pp.close()
