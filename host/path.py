# -*- coding:utf-8 -*-
# using python 3.8
import route
import simplejson as json
import numpy as np
import sys

print(sys.path)

path = route.PathRoute()
# path.path_node_restore()
if 0:
    data_1 = path.get_path_info(1, 5)
# tmp = path.plan_path_route(1, 9)
# path.path_node_restore()
else:
    path_node = 9
    arry_len = path_node * (path_node - 1)
    data = [[] for _ in range(arry_len)]
    k = 0
    for i in range(1, path_node + 1):
        for j in range(1, path_node + 1):
            if i == j:
                continue
            tmp = path.get_path_info(i, j)
            data[k].append(str(i) + ' → ' + str(j))
            for j in range(len(tmp)):
                data[k].append(tmp[j])
            tmp.clear()
            k = k + 1
    print('************************************')
    for i in range(0, arry_len):
        print(data[i])


# data_2 = path.find_coordinate(1, 7)
# path.path_plan(data)
# with open('path_node.json', 'r') as file:
#     dat = json.load(file)
# print(len(dat))
# print(dat["1st line"])


