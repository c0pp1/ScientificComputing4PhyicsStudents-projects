import matplotlib.pyplot as plt
import numpy as np
import math
import os

module_dir = os.path.dirname(__file__)

def read_data(filename):
    with open(filename, 'r') as file:
        lines = file.readlines()
    data = []
    for line in lines:
        if line.strip():
            data.append(list(map(float, line.split('\t'))))

    return np.array(data, dtype=np.float64)

def main():
    data_fname = os.path.join(module_dir, '../../build/04-discrete-math/C++/result.txt')
    data = read_data(data_fname)
    x = data[:, 0]
    y = data[:, 2]

    plt.figure(figsize=(10, 6))
    plt.plot(x, abs(y), marker='o', linestyle='-', color='tab:blue')
    plt.title('Relative error VS number of samples')
    plt.xlabel('Number of samples')
    plt.ylabel('Relative error')
    plt.xscale('log')
    plt.yscale('log')
    plt.savefig(os.path.join(module_dir, 'plot.png'), dpi=300)

if __name__ == "__main__":
    main()