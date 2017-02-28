#!/bin/python

from sys import argv
from collections import defaultdict
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.font_manager import FontProperties

FNAME = argv[1]
FNAME_BASE = FNAME.split(".")[0]

GRAPH_NAMES = ["average", "best case", "worst case", "relative", "#wins"]
BASE_ALGO = "one_for_loop_1if"

def statistics(dat):
  algorithms = dict(enumerate(dat.keys()))

  to_ret = defaultdict(dict)
  for algo, rst in dat.items():
    for size, times in rst.items():
      sorted_times = sorted(times)
      to_ret[algo][size] = [sum(times) / len(times), sorted_times[0], sorted_times[int(-len(sorted_times)/10)]]

  for size in dat[BASE_ALGO].keys():
    base_avg = to_ret[BASE_ALGO][size][0]
    for algo in algorithms.values():
      avg_this = to_ret[algo][size][0]
      to_ret[algo][size].append(avg_this / base_avg)

  for size in dat[BASE_ALGO].keys():
    for algo in algorithms.values():
      to_ret[algo][size].append(0)

    all_times = [dat[algo][size] for algo in dat.keys()]
    for measurement in zip(*all_times):
      min_idx, _ = min(enumerate(measurement), key=lambda x: x[1])
      to_ret[algorithms[min_idx]][size][-1] += 1

  return to_ret

if __name__ == "__main__":
  dat = defaultdict(dict)
  with open(FNAME, "r") as fp:
    for line in fp:
      algo, size, *rest = line.split(";")
      # remove first five measurements
      dat[algo][int(float(size))] = [int(i) for i in rest[5:]]

  dat = statistics(dat)

  fig, ax = plt.subplots(nrows=5, sharex=True, figsize=(7, 9))
  for method, info in dat.items():
    for i in range(5):
      to_draw = list(sorted(info.items(), key=lambda x:x[0]))
      to_draw_y = np.array([d[i] for _, d in to_draw])
      to_draw_x = np.array([size for size, _ in to_draw])

      ax[i].plot(to_draw_x, to_draw_y, linewidth=0.5, label=method)
      ax[i].set_xscale("log")
      ax[i].set_yscale("log" if i < 3 else "linear")
      ax[i].grid(True)
      ax[i].set_ylabel(GRAPH_NAMES[i])

  plt.subplots_adjust(hspace = 0.5)
  ax[2].legend(loc='right', fontsize='small')
  plt.suptitle("Performance of second smallest element algorithms")
  ax[0].set_title("Filename: " + FNAME)

  plt.savefig(FNAME_BASE + ".png")
  plt.savefig(FNAME_BASE + ".svg")
