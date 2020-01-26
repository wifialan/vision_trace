# -*- coding:utf-8 -*-
# using python 3.8
import numpy as np
import simplejson as json


class PathLine:
    def __init__(self):
        with open('path_node.json', 'r') as file:
            self.json_data = json.load(file)
        self.json_data_index = ['1st line', '2nd line', '3rd line', '4th line', '5th line', '6th line']
        print(self.json_data)
        self.row_pointer = 0
        self.col_pointer = 0
        self.crossroad_coord = []
        self.path_plan_indicate = []
        self.target_node_coord = [[0, 0], [0, 0]]
        self.len_row = len(self.json_data)
        self.len_col = len(self.json_data["1st line"])
        # 创建计算路径节点个数列表
        self.calc_node_counter_array = []  # np.zeros(shape=[1, self.len_row], dtype=int)
        # 通过读取json文件可以得知需要创建数组的维度
        self.path_node = np.zeros(shape=[self.len_row, self.len_col], dtype=int)
        # 开始写入json文件里面的数据
        for row in range(len(self.json_data)):
            # 将读取json文件里面的数据存入路径节点矩阵中
            # row_stack 方法是直接在矩阵后面追加一行，因此后续应有删除前面的空行程序
            self.path_node = np.row_stack((self.path_node, self.json_data[self.json_data_index[row]]))
            self.path_node = np.delete(self.path_node, 0, axis=0)

    def find_coordinate(self, current_node, target_node):
        # 寻找当前节点的坐标
        loc = np.where(self.path_node == current_node)
        self.target_node_coord[0][0] = loc[0][0]
        self.target_node_coord[0][1] = loc[1][0]
        # 寻找目标节点的坐标
        loc = np.where(self.path_node == target_node)
        self.target_node_coord[1][0] = loc[0][0]
        self.target_node_coord[1][1] = loc[1][0]
        print(self.target_node_coord)
        # 开始寻找节点最少路径
        # 首先检测当前点和目标点是否同在同一条线上
        if (self.target_node_coord[0][0] == self.target_node_coord[1][0]) and (self.target_node_coord[0][0] == 0):
            # 在同一条路线上
            # 第一条路线上的节点长度
            tmp = self.target_node_coord[0][1] - self.target_node_coord[1][1]
            self.calc_node_counter_array.insert(0, abs(tmp))
            print(self.calc_node_counter_array)
            # 第二条路线上的节点长度
            find_crossroad_flag = 0
            find_crossroad_coord_flag = 0
            for row in range(1, self.len_row):
                for i in range(self.len_col):
                    if self.path_node[row][i] != 0:
                        # 找到岔道入口节点位置，位置在 i 处
                        print('第' + str(row) + '条岔道入口节点坐标：[' + str(row) + ',' + str(i) + ']')
                        crossroad_entry_ordinate = i
                        if find_crossroad_coord_flag == 0:
                            self.crossroad_coord.append(crossroad_entry_ordinate - 1)
                            find_crossroad_coord_flag = 1
                        for j in range(i, self.len_col):
                            if self.path_node[row][j] == 0:
                                # 找到岔道出口节点位置，开始判断这条岔道中的空节点
                                blank_node = 0
                                for k in range(j, 0, -1):
                                    if self.path_node[row][k] == 99:
                                        blank_node = blank_node + 1
                                print('第' + str(row) + '条岔道出口节点坐标：[' + str(row) + ',' + str(j - blank_node - 1) + ']')
                                print('第' + str(row) + '条岔道中空节点个数：' + str(blank_node))
                                crossroad_exit_ordinate = j - blank_node - 1
                                find_crossroad_flag = 1
                                break
                    if find_crossroad_flag == 1:
                        find_crossroad_flag = 0
                        break
                tmp = abs(self.target_node_coord[0][1] - crossroad_entry_ordinate) + abs(
                    crossroad_exit_ordinate - crossroad_entry_ordinate) + \
                      abs(self.target_node_coord[1][1] - crossroad_exit_ordinate - blank_node)
                self.calc_node_counter_array.insert(row, abs(tmp))

            for i in range(self.len_row):
                print('第' + str(i + 1) + '条路线经过节点数：' + str(self.calc_node_counter_array[i]))
            # 确保有不止一条路线
            if self.len_row > 1:
                mix_data = self.calc_node_counter_array[0]
                mix_data_coordinate = 0
                for i in range(1, self.len_row):
                    tmp_data_2 = self.calc_node_counter_array[i]
                    if mix_data <= tmp_data_2:
                        mix_data = mix_data
                    else:
                        mix_data = tmp_data_2
                        mix_data_coordinate = i
            select_path = mix_data_coordinate + 1
            print('第' + str(select_path) + '条路线经过节点最少')
            print('道路分岔点坐标为：' + str(self.crossroad_coord[0]))

        if (self.target_node_coord[0][0] != self.target_node_coord[1][0]) and (self.target_node_coord[0][0] == 0):
            select_path = self.target_node_coord[1][0] + 1
            # 寻找路线分岔点
            for i in range(self.len_col):
                if self.path_node[self.target_node_coord[1][0]][i] != 0:
                    self.crossroad_coord.append(i - 1)
                    print('道路分岔点坐标为：' + str(self.crossroad_coord[0]))
                    break
            for j in range(select_path - 1):
                self.calc_node_counter_array.append(0)
            path_plan_node_couner = abs(self.crossroad_coord[0] - \
                                        self.target_node_coord[0][1]) + \
                                    abs(self.crossroad_coord[0] - \
                                        self.target_node_coord[1][1])
            print(path_plan_node_couner)
            self.calc_node_counter_array.append(path_plan_node_couner)

        return select_path

    def path_plan(self, select_path):
        print('按照第' + str(select_path) + '条路线开始规划')
        if select_path > 1:  # 除第一条路线外，其余都要经过岔路口
            up_counter = abs(
                self.path_node[0][self.target_node_coord[0][1]] - self.path_node[0][self.crossroad_coord[0]])
            for i in range(up_counter):
                self.path_plan_indicate.append("up")
            print('前进步数：' + str(up_counter))
            right_counter = select_path - 1  # 第二条路线原地右转一次，第二条路线原地右转两次，依次类推
            for i in range(right_counter):
                self.path_plan_indicate.append("right")
            print('右转步数：' + str(right_counter))
            up_counter = abs(self.calc_node_counter_array[select_path - 1] - up_counter)
            for i in range(up_counter):
                self.path_plan_indicate.append("up")
            print('前进步数：' + str(up_counter))
            print(self.path_plan_indicate)

    def print(self):
        for i in range(self.len_row):
            print("[", end='')
            for j in range(self.len_col - 1):
                print(str(self.path_node[i][j]) + ', ', end='')
            print(str(self.path_node[i][self.len_col - 1]), end='')
            print(']')
        print()
