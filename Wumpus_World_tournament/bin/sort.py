# Author: Vincent Ho
# CS 199: Spring 2016

import operator

AIs_dict = {}

with open("Tournament_Results.txt", "r") as results:
    for line in results:
        split = line.split(":")
        AIs_dict[split[0].strip()] = float(split[1].strip())

AIs_dict_sorted = sorted(AIs_dict.items(), key=operator.itemgetter(1), reverse=True)

with open("Tournament_Results.txt", "w") as results:
    for k, v in AIs_dict_sorted:
        # print("{}: {}".format(k, str(v)))
        results.write("{}: {}\n".format(k, str(v)))