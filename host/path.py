# -*- coding:utf-8 -*-
# using python 3.8
import fun
import simplejson as json
import numpy as np


path = fun.PathLine()
path.print()
data = path.find_coordinate(1, 6)
path.path_plan(data)
# with open('path_node.json', 'r') as file:
#     dat = json.load(file)
# print(len(dat))
# print(dat["1st line"])


