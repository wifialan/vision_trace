#!/usr/bin/env python
# -*- coding: utf-8 -*-
import route
import simplejson as json
import numpy as np
import sys

with open('path_node.json', 'r') as file:
    json_data = json.load(file)
json_data_index = ['1st line', '2nd line', '3rd line', '4th line', '5th line', '6th line']
print(json_data)
row_pointer = 0
col_pointer = 0
crossroad_coord = []
cross_entry_coord = 0
cross_exit_coord = 0
cross_entry_coord_bak = []
spin_crossroad_coord = 0
path_plan_indicate = []
target_node_coord = [[0, 0], [0, 0]]
target_node_coord_origin = [[0, 0], [0, 0]]
spin_path_node = 0
len_row = len(json_data)
len_col = len(json_data["1st line"])
forward = 0
backward = 0
select_path = 0
# 创建计算路径节点个数列表
number_of_route_node = []  # np.zeros(shape=[1, len_row], dtype=int)
calc_node_counter_array_reverse = []
# 通过读取json文件可以得知需要创建数组的维度
path_node = np.zeros(shape=[len_row, len_col], dtype=int)
path_node_bak = np.zeros(shape=[len_row, len_col], dtype=int)
# 开始写入json文件里面的数据
for row in range(len(json_data)):
    # 将读取json文件里面的数据存入路径节点矩阵中
    # row_stack 方法是直接在矩阵后面追加一行，因此后续应有删除前面的空行程序
    path_node = np.row_stack((path_node, json_data[json_data_index[row]]))
    path_node = np.delete(path_node, 0, axis=0)
print(path_node)
for i in range(len_row):
    for j in range(len_col):
        path_node_bak[i][j] = path_node[i][j]
for n in range(0, len_col - 1):
    if path_node[1][n] == 0 and path_node[1][n + 1] != 0:
        cross_entry_coord = n
    if path_node[1][n] != 0 and path_node[1][n + 1] == 0:
        cross_exit_coord = n + 1
print('岔道口入口坐标：', end='')
print(cross_entry_coord)
print('岔道口出口坐标：', end='')
print(cross_exit_coord)
# write path node distribute info to file
path_node_distribute_info = open("path_node_distribute_info.txt", 'w')
for i in range(len_row):
    for j in range(len_col):
        if path_node[i][j] != 0 and path_node[i][j] != 99:
            path_node_distribute_info.write(str(path_node[i][j]) + ',')
    path_node_distribute_info.write('\n')
path_node_distribute_info.close()
# write crossraod node to file
if len_row > 1:
    crossraod_node = open("crossraod_node.txt", 'w')
    crossraod_node.write(str(path_node[0][cross_entry_coord]) + ',')
    crossraod_node.write(str(path_node[0][cross_exit_coord]))
    for i in range(0, len_row):
        j = 1
        while path_node[i][cross_exit_coord - j] == 99:
            j = j + 1
        crossraod_node.write('\n' + str(path_node[i][cross_entry_coord + 1]) + ',')
        crossraod_node.write(str(path_node[i][cross_exit_coord - j]))
    crossraod_node.close()
