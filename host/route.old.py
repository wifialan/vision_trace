# -*- coding:utf-8 -*-
# using python 3.8
import numpy as np
import simplejson as json


class PathRoute:
    def __init__(self):
        with open('/home/alan/work/vision_trace/host/path_node.json', 'r') as file:
            self.json_data = json.load(file)
        self.json_data_index = ['1st line', '2nd line', '3rd line', '4th line', '5th line', '6th line']
        print(self.json_data)
        self.row_pointer = 0
        self.col_pointer = 0
        self.crossroad_coord = []
        self.cross_entry_coord = 0
        self.cross_exit_coord = 0
        self.cross_entry_coord_bak = []
        self.spin_crossroad_coord = 0
        self.path_plan_indicate = []
        self.target_node_coord = [[0, 0], [0, 0]]
        self.target_node_coord_origin = [[0, 0], [0, 0]]
        self.spin_path_node = 0
        self.len_row = len(self.json_data)
        self.len_col = len(self.json_data["1st line"])
        self.forward = 0
        self.backward = 0
        self.select_path = 0
        # 创建计算路径节点个数列表
        self.number_of_route_node = []  # np.zeros(shape=[1, self.len_row], dtype=int)
        self.calc_node_counter_array_reverse = []
        # 通过读取json文件可以得知需要创建数组的维度
        self.path_node = np.zeros(shape=[self.len_row, self.len_col], dtype=int)
        self.path_node_bak = np.zeros(shape=[self.len_row, self.len_col], dtype=int)
        # 开始写入json文件里面的数据
        for row in range(len(self.json_data)):
            # 将读取json文件里面的数据存入路径节点矩阵中
            # row_stack 方法是直接在矩阵后面追加一行，因此后续应有删除前面的空行程序
            self.path_node = np.row_stack((self.path_node, self.json_data[self.json_data_index[row]]))
            self.path_node = np.delete(self.path_node, 0, axis=0)
        print(self.path_node)
        for i in range(self.len_row):
            for j in range(self.len_col):
                self.path_node_bak[i][j] = self.path_node[i][j]
        for n in range(0, self.len_col - 1):
            if self.path_node[1][n] == 0 and self.path_node[1][n + 1] != 0:
                self.cross_entry_coord = n
            if self.path_node[1][n] != 0 and self.path_node[1][n + 1] == 0:
                self.cross_exit_coord = n + 1
        print('岔道口入口坐标：', end='')
        print(self.cross_entry_coord)
        print('岔道口出口坐标：', end='')
        print(self.cross_exit_coord)

    def get_path_info(self, current_node, target_node):
        self.target_node_coord = [[0, 0], [0, 0]]
        loc = np.where(self.path_node == current_node)
        print(self.target_node_coord)
        self.target_node_coord[0][0] = loc[0][0]
        self.target_node_coord[0][1] = loc[1][0]
        # 寻找目标节点的坐标
        loc = np.where(self.path_node == target_node)
        self.target_node_coord[1][0] = loc[0][0]
        self.target_node_coord[1][1] = loc[1][0]
        print('A点坐标：', end='')
        print(self.target_node_coord[0][1])
        print('B点坐标：', end='')
        print(self.target_node_coord[1][1])
        # 找出岔路口坐标
        path_node = self.plan_path_route(current_node, target_node)
        print(path_node)
        if (self.target_node_coord[0][0] == self.target_node_coord[1][0] == 0) and \
                min(self.target_node_coord[0][1], self.target_node_coord[1][1]) <= self.cross_entry_coord <= \
                self.cross_exit_coord <= max(self.target_node_coord[0][1], self.target_node_coord[1][1]):
            if self.target_node_coord[0][1] < self.target_node_coord[1][1]:
                print('前三个节点为正向')
                if path_node.index((min(min(path_node[0:self.len_row]), path_node[self.len_row]))) < 3:
                    print('选择正向')
                    self.forward = 1
                    self.backward = 0
                    self.select_path = path_node.index(min(path_node[0:self.len_row])) + 1
                    print('按照第' + str(self.select_path) + '条路线规划')
                else:
                    print('选择反向')
                    self.forward = 0
                    self.backward = 1
                    self.select_path = 1
                    print('按照第' + str(self.select_path) + '条路线规划')
            else:
                print('第一个节点为正向')
                if path_node.index(min(path_node[0], min(path_node[1:self.len_row + 1]))) == 0:
                    print('选择正向')
                    self.forward = 1
                    self.backward = 0
                    self.select_path = 0 + 1
                    print('按照第' + str(self.select_path) + '条路线规划')
                else:
                    print('选择反向')
                    self.forward = 0
                    self.backward = 1
                    self.select_path = path_node.index(min(path_node[1:self.len_row + 1]))
                    print('按照第' + str(self.select_path) + '条路线规划')
        else:
            # 剩下的这种情况只有两种路径：正向路径和反向路径
            if path_node.index(min(path_node)) == 0:
                print('选择正向')
                self.forward = 1
                self.backward = 0
                self.select_path = 0
            else:
                print('选择反向')
                self.forward = 0
                self.backward = 1
                self.select_path = 0

        route = self.path_plan()
        return route

    def plan_path_route(self, current_node, target_node):
        # 寻找当前节点的坐标
        loc = np.where(self.path_node == current_node)
        self.target_node_coord[0][0] = loc[0][0]
        self.target_node_coord[0][1] = loc[1][0]
        # 寻找目标节点的坐标
        loc = np.where(self.path_node == target_node)
        self.target_node_coord[1][0] = loc[0][0]
        self.target_node_coord[1][1] = loc[1][0]
        print('***********************************')
        print(str(current_node) + '和' + str(target_node) + '的坐标为：', end='')
        print(self.target_node_coord)
        print('***********************************')
        self.number_of_route_node.clear()
        # 起点A和终点B直接的路线有如下三种模型：
        # 1.直达模式：A -- x -- x -- B
        # 2.岔道到达：A -- X -- X -- B
        #                 |   (| or |)
        #                 C -- D -- E
        # 3.岔道未达：A -- X -- X -- B
        #                 |
        #                 C -- X -- X --

        # 首先判断第一种路线模式，确保A和B直接没有岔道，属于直达模式。
        # 共有两种情况，1：路线只有一条 2：路线有多条，但A和B直接只有一条到达路线
        if self.len_row == 1:
            # 直接计算两点间节点个数
            tmp = abs(self.target_node_coord[0][1] - self.target_node_coord[1][1])
            self.number_of_route_node.append(tmp)
            return self.number_of_route_node
        else:
            # 验证A和B两点间没有岔道算法
            self.cross_entry_coord = [[], []]
            self.cross_exit_coord = [[], []]
            self.cross_entry_coord[0] = self.target_node_coord[0][0]
            self.cross_exit_coord[0] = self.target_node_coord[0][0]
            self.cross_entry_coord = 0
            self.cross_exit_coord = 0
            blank = [[] for _ in range(self.len_row)]
            # 找出路线的入口坐标和出口坐标
            for n in range(0, self.len_col - 1):
                if self.path_node[1][n] == 0 and self.path_node[1][n + 1] != 0:
                    self.cross_entry_coord = n
                if self.path_node[1][n] != 0 and self.path_node[1][n + 1] == 0:
                    self.cross_exit_coord = n + 1
            print('岔道口入口坐标：', end='')
            print(self.cross_entry_coord)
            print('岔道口出口坐标：', end='')
            print(self.cross_exit_coord)
            # Case 1：A和B皆在第一条路线上
            if self.target_node_coord[0][0] == self.target_node_coord[1][0] == 0:
                print("A和B皆在第一条线路上")
                # 检验A和B间是否有闭环岔道算法
                if min(self.target_node_coord[0][1], self.target_node_coord[1][1]) <= self.cross_entry_coord <= \
                        self.cross_exit_coord <= max(self.target_node_coord[0][1], self.target_node_coord[1][1]):
                    print('A和B在同一条线上且之间存在闭环岔路口')
                    print('检测起始点A是否在目的地B的左边')
                    addr = self.target_node_coord[0][1]
                    dest = self.target_node_coord[1][1]
                    tmp_min = min(self.target_node_coord[0][1], self.target_node_coord[1][1])
                    tmp_max = max(self.target_node_coord[0][1], self.target_node_coord[1][1])
                    self.target_node_coord[0][1] = tmp_min
                    self.target_node_coord[1][1] = tmp_max
                    if self.target_node_coord[0][1] < self.target_node_coord[1][1]:
                        print('A在B的左边，按照闭环岔道口计算节点')
                        cross_node = [[] for _ in range(self.len_row)]
                        blank = [[] for _ in range(self.len_row)]
                        for i in range(self.len_row):
                            if i != self.target_node_coord[0][0]:
                                blank[i] = 0
                        # 第一阶段算法已经验证过，A和B之间存在闭环岔道，应找出岔道入口和出口坐标
                        blank[self.target_node_coord[0][0]] = 0
                        for i in range(self.cross_entry_coord, self.cross_exit_coord):
                            if self.path_node[self.target_node_coord[0][0]][i] == 99:
                                blank[self.target_node_coord[0][0]] = blank[self.target_node_coord[0][0]] + 1
                        cross_node[self.target_node_coord[0][0]] = abs(self.cross_exit_coord - self.cross_entry_coord) - \
                                                                   blank[self.target_node_coord[0][0]] - 1
                        print("A和B之间的伪节点个数为：", end='')
                        print(blank)

                        # 从岔路口坐标间寻找岔路口的节点个数
                        for i in range(self.len_row - 1):
                            exist_node_flag = 0
                            for j in range(self.cross_entry_coord + 1, self.cross_exit_coord):
                                if self.path_node[abs(self.len_row - self.target_node_coord[0][0] - i - 1)][j] != 0:
                                    # 如果节点不为0，说明存在闭环节点，若节点为99说明为伪节点
                                    if self.path_node[abs(self.len_row - self.target_node_coord[0][0] - i - 1)][j] != 99:
                                        exist_node_flag = exist_node_flag + 1
                            cross_node[abs(self.len_row - self.target_node_coord[0][0] - i - 1)] = exist_node_flag
                        print('A到B之间的岔道部分节点个数分别为：', end='')
                        print(cross_node)
                        # 计算A到B之间所有路线的节点总个数
                        for i in range(self.len_row):
                            self.number_of_route_node.append(
                                abs(self.target_node_coord[0][1] - self.cross_entry_coord) + \
                                cross_node[i] + abs(
                                    self.target_node_coord[1][1] - self.cross_exit_coord) + 1)
                        print('从A到B各个路线花费的步数为：')
                        print(self.number_of_route_node)
                        print('开始反向规划')
                        blank = 0
                        for i in range(0, self.target_node_coord[0][1]):
                            if self.path_node[self.target_node_coord[1][0]][i] == 99:
                                blank = blank + 1
                        for i in range(self.target_node_coord[1][1], self.len_col):
                            if self.path_node[self.target_node_coord[0][0]][i] == 99:
                                blank = blank + 1
                        print('A和B之间的伪节点数为：', end='')
                        print(blank)
                        tmp = self.target_node_coord[0][1] + abs(self.target_node_coord[1][1] - self.len_col) - blank
                        self.number_of_route_node.append(tmp)
                        print('从A到B需要的步数为：', end='')
                        print(self.number_of_route_node[3])
                        self.target_node_coord[0][1] = addr
                        self.target_node_coord[1][1] = dest
                        print('---------------------------------')
                        if addr < dest:
                            print('从A到B需要的正向步数为：', end='')
                            for i in range(self.len_row - 1):
                                print(str(self.number_of_route_node[i]) + ',', end='')
                            print(self.number_of_route_node[self.len_row - 1])
                            print('从A到B需要的反向步数为：', end='')
                            print(self.number_of_route_node[self.len_row])
                        else:
                            self.number_of_route_node.insert(0, self.number_of_route_node[self.len_row])
                            self.number_of_route_node.pop(self.len_row + 1)
                            print('从A到B需要的正向步数为：', end='')
                            print(self.number_of_route_node[0])
                            print('从A到B需要的反向步数为：', end='')
                            for i in range(1, self.len_row):
                                print(str(self.number_of_route_node[i]) + ',', end='')
                            print(self.number_of_route_node[self.len_row])
                else:
                    print('A和B在同一条线上且之间没有闭环岔路口')
                    if self.target_node_coord[0][1] < self.target_node_coord[1][1]:
                        print('A在B左边')
                        blank = 0
                        for i in range(self.target_node_coord[0][1], self.target_node_coord[1][1]):
                            if self.path_node[self.target_node_coord[0][0]][i] == 99:
                                blank = blank + 1
                        tmp = abs(self.target_node_coord[0][1] - self.target_node_coord[1][1]) - blank
                        print('A和B之间的伪节点数为：', end='')
                        print(blank)
                        self.number_of_route_node.append(tmp)
                        print('从A到B需要的正向步数为：', end='')
                        print(self.number_of_route_node[0])
                        print('开始反向规划')
                        blank = 0
                        for i in range(0, self.target_node_coord[0][1]):
                            if self.path_node[self.target_node_coord[0][0]][i] == 99:
                                blank = blank + 1
                        for i in range(self.target_node_coord[1][1], self.len_col):
                            if self.path_node[self.target_node_coord[1][0]][i] == 99:
                                blank = blank + 1
                        tmp = self.target_node_coord[0][1] + abs(self.target_node_coord[1][1] - self.len_col) - blank
                        print('A和B之间的伪节点数为：', end='')
                        print(blank)
                        self.number_of_route_node.append(tmp)
                        print('从A到B需要的反向步数为：', end='')
                        print(self.number_of_route_node[1])
                        print('从A到B需要的正向步数为：', end='')
                        print(self.number_of_route_node[0])
                        print('从A到B需要的反向步数为：', end='')
                        print(self.number_of_route_node[1])
                    else:
                        print('A在B右边')
                        blank = 0
                        for i in range(self.target_node_coord[0][1], self.len_col):
                            if self.path_node[self.target_node_coord[0][0]][i] == 99:
                                blank = blank + 1
                        for i in range(0, self.target_node_coord[1][1]):
                            if self.path_node[self.target_node_coord[0][0]][i] == 99:
                                blank = blank + 1
                        tmp = self.target_node_coord[1][1] + abs(self.target_node_coord[0][1] - self.len_col) - blank
                        print('A和B之间的伪节点数为：', end='')
                        print(blank)
                        self.number_of_route_node.append(tmp)
                        print('从A到B需要的正向步数为：', end='')
                        print(self.number_of_route_node[0])
                        print('开始反向规划')
                        blank = 0
                        for i in range(self.target_node_coord[1][1], self.target_node_coord[0][1]):
                            if self.path_node[self.target_node_coord[0][0]][i] == 99:
                                blank = blank + 1
                        tmp = abs(self.target_node_coord[0][1] - self.target_node_coord[1][1]) - blank
                        print('A和B之间的伪节点数为：', end='')
                        print(blank)
                        self.number_of_route_node.append(tmp)
                        print('从A到B需要的反向步数为：', end='')
                        print(self.number_of_route_node[1])
                        print('从A到B需要的正向步数为：', end='')
                        print(self.number_of_route_node[0])
                        print('从A到B需要的反向步数为：', end='')
                        print(self.number_of_route_node[1])

                return self.number_of_route_node
            # Case 2: A和B皆不在第一条路线上，也就是说，A和B都在岔道上面
            elif self.target_node_coord[0][0] != 0 and self.target_node_coord[1][0] != 0:
                print('这里需要考虑A和B皆不在第一行上面的情况')
                # 这种情况路线分为两种情况：
                # 1. A和B在同一条路线上
                # 2. A和B不在同一条路线上

                if self.target_node_coord[0][0] == self.target_node_coord[1][0]:
                    print('A和B在同一条路线上，不用考虑岔道口')
                    # 寻找A和B之间的空白节点
                    blank = 0
                    for i in range(self.target_node_coord[0][1], self.target_node_coord[1][1]):
                        if self.path_node[self.target_node_coord[0][0]][i] == 99:
                            blank = blank + 1
                    tmp = abs(self.target_node_coord[0][1] - self.target_node_coord[1][1]) - blank
                    print('A和B之间的伪节点数为：', end='')
                    print(blank)
                    self.number_of_route_node.append(tmp)
                    print('从A到B需要的步数为：', end='')
                    print(self.number_of_route_node[0])
                    if self.target_node_coord[0][1] < self.target_node_coord[1][1]:
                        print('A在B左边')
                        self.number_of_route_node.append(99)
                    else:
                        print('A在B右边')
                        self.number_of_route_node.insert(0, 99)
                else:
                    print('A和B不在同一条路线上，需要考虑岔道口的出口坐标')
                    print('出口坐标为：', end='')
                    print(self.cross_exit_coord)
                    # 寻找A和B之间的空白节点
                    blank = 0
                    for i in range(min(self.target_node_coord[0][1], self.cross_exit_coord), \
                                   max(self.target_node_coord[0][1], self.cross_exit_coord)):
                        if self.path_node[self.target_node_coord[0][0]][i] == 99:
                            blank = blank + 1
                    for i in range(min(self.target_node_coord[1][1], self.cross_exit_coord), \
                                   max(self.target_node_coord[1][1], self.cross_exit_coord)):
                        if self.path_node[self.target_node_coord[1][0]][i] == 99:
                            blank = blank + 1
                    print('A和B之间的伪节点数为：', end='')
                    print(blank)
                    tmp = abs(self.target_node_coord[0][1] - self.cross_exit_coord) + \
                          abs(self.target_node_coord[1][1] - self.cross_exit_coord) - blank
                    self.number_of_route_node.append(tmp)
                    print('从A到B需要的正向步数为：', end='')
                    print(self.number_of_route_node[0])
                    print('开始反向规划')
                    blank = 0
                    for i in range(min(self.target_node_coord[0][1], self.cross_entry_coord), \
                                   max(self.target_node_coord[0][1], self.cross_entry_coord)):
                        if self.path_node[self.target_node_coord[0][0]][i] == 99:
                            blank = blank + 1
                    for i in range(min(self.target_node_coord[1][1], self.cross_entry_coord), \
                                   max(self.target_node_coord[1][1], self.cross_entry_coord)):
                        if self.path_node[self.target_node_coord[1][0]][i] == 99:
                            blank = blank + 1
                    print('A和B之间的伪节点数为：', end='')
                    print(blank)
                    tmp = abs(self.target_node_coord[0][1] - self.cross_entry_coord) + \
                          abs(self.target_node_coord[1][1] - self.cross_entry_coord) - blank
                    self.number_of_route_node.append(tmp)
                    print('从A到B需要的反向步数为：', end='')
                    print(self.number_of_route_node[1])
                    print('从A到B需要的正向步数为：', end='')
                    print(self.number_of_route_node[0])
                    print('从A到B需要的反向步数为：', end='')
                    print(self.number_of_route_node[1])

                return self.number_of_route_node
            # Case 3: A和B有一个在第一行，另一个不在第一行
            elif (self.target_node_coord[0][0] == 0 and self.target_node_coord[1][0] != 0) \
                    or (self.target_node_coord[1][0] == 0 and self.target_node_coord[0][0] != 0):
                print('A和B有一个在第一行，有一个不在第一行')
                # 这种情况分为两类路线
                # 1. A和B之间存在岔道入口
                # 2. A和B之间存在岔道出口
                if self.target_node_coord[0][0] == 0:
                    print('A在第一行上面')
                    if self.target_node_coord[0][1] <= self.cross_entry_coord:
                        print('A和B之间存在岔道入口')
                        print('入口坐标为：', end='')
                        print(self.cross_entry_coord)
                        # 寻找A和B之间的空白节点
                        blank = 0
                        for i in range(min(self.target_node_coord[0][1], self.cross_entry_coord), \
                                       max(self.target_node_coord[0][1], self.cross_entry_coord)):
                            if self.path_node[self.target_node_coord[0][0]][i] == 99:
                                blank = blank + 1
                        for i in range(min(self.target_node_coord[1][1], self.cross_entry_coord), \
                                       max(self.target_node_coord[1][1], self.cross_entry_coord)):
                            if self.path_node[self.target_node_coord[1][0]][i] == 99:
                                blank = blank + 1
                        print('A和B之间的伪节点数为：', end='')
                        print(blank)
                        tmp = abs(self.target_node_coord[0][1] - self.cross_entry_coord) + \
                              abs(self.target_node_coord[1][1] - self.cross_entry_coord) - blank
                        self.number_of_route_node.append(tmp)
                        print('从A到B需要的正向步数为：', end='')
                        print(self.number_of_route_node[0])
                        print('开始反向规划，需经过岔道出口')
                        print('岔道出口坐标为：', end='')
                        print(self.cross_exit_coord)
                        blank = 0
                        for i in range(0, self.target_node_coord[0][1]):
                            if self.path_node[self.target_node_coord[0][0]][i] == 99:
                                blank = blank + 1
                        for i in range(self.cross_exit_coord, self.len_col):
                            if self.path_node[self.target_node_coord[0][0]][i] == 99:
                                blank = blank + 1
                        for i in range(min(self.target_node_coord[1][1], self.cross_exit_coord), \
                                       max(self.target_node_coord[1][1], self.cross_exit_coord)):
                            if self.path_node[self.target_node_coord[1][0]][i] == 99:
                                blank = blank + 1
                        print('A和B之间的伪节点数为：', end='')
                        print(blank)
                        tmp = abs(self.target_node_coord[0][1]) + \
                              abs(self.len_col - self.cross_exit_coord) + \
                              abs(self.target_node_coord[1][1] - self.cross_exit_coord) - blank
                        self.number_of_route_node.append(tmp)
                        print('从A到B需要的反向步数为：', end='')
                        print(self.number_of_route_node[1])
                        print('从A到B需要的正向步数为：', end='')
                        print(self.number_of_route_node[0])
                        print('从A到B需要的反向步数为：', end='')
                        print(self.number_of_route_node[1])

                    elif self.cross_entry_coord < self.target_node_coord[0][1] < self.cross_exit_coord:
                        print('A和B之间存在岔道出口，A在入口和出口之间')
                        print('出口坐标为：', end='')
                        print(self.cross_exit_coord)
                        # 寻找A和B之间的空白节点
                        blank = 0
                        for i in range(min(self.target_node_coord[0][1], self.cross_exit_coord), \
                                       max(self.target_node_coord[0][1], self.cross_exit_coord)):
                            if self.path_node[self.target_node_coord[0][0]][i] == 99:
                                blank = blank + 1
                        for i in range(min(self.target_node_coord[1][1], self.cross_exit_coord), \
                                       max(self.target_node_coord[1][1], self.cross_exit_coord)):
                            if self.path_node[self.target_node_coord[1][0]][i] == 99:
                                blank = blank + 1
                        print('A和B之间的伪节点数为：', end='')
                        print(blank)
                        tmp = abs(self.target_node_coord[0][1] - self.cross_exit_coord) + \
                              abs(self.target_node_coord[1][1] - self.cross_exit_coord) - blank
                        self.number_of_route_node.append(tmp)
                        print('从A到B需要的正向步数为：', end='')
                        print(self.number_of_route_node[0])
                        print('开始反向规划，需经过岔道出口')
                        print('岔道入口坐标为：', end='')
                        print(self.cross_entry_coord)
                        blank = 0
                        for i in range(self.cross_entry_coord, self.target_node_coord[0][1]):
                            if self.path_node[self.target_node_coord[0][0]][i] == 99:
                                blank = blank + 1
                        for i in range(min(self.target_node_coord[1][1], self.cross_entry_coord), \
                                       max(self.target_node_coord[1][1], self.cross_entry_coord)):
                            if self.path_node[self.target_node_coord[1][0]][i] == 99:
                                blank = blank + 1
                        print('A和B之间的伪节点数为：', end='')
                        print(blank)
                        tmp = abs(self.target_node_coord[0][1] - self.cross_entry_coord) + \
                              abs(self.target_node_coord[1][1] - self.cross_entry_coord) - blank
                        self.number_of_route_node.append(tmp)
                        print('从A到B需要的反向步数为：', end='')
                        print(self.number_of_route_node[1])
                        print('从A到B需要的正向步数为：', end='')
                        print(self.number_of_route_node[0])
                        print('从A到B需要的反向步数为：', end='')
                        print(self.number_of_route_node[1])
                    else:
                        print('A和B之间存在岔道出口，A在出口右边')
                        print('入口坐标为：', end='')
                        print(self.cross_entry_coord)
                        print('出口坐标为：', end='')
                        print(self.cross_exit_coord)
                        # 寻找A和B之间的空白节点
                        blank = 0
                        for i in range(self.target_node_coord[0][1], self.len_col):
                            if self.path_node[self.target_node_coord[0][0]][i] == 99:
                                blank = blank + 1
                        for i in range(0, self.cross_entry_coord):
                            if self.path_node[self.target_node_coord[0][0]][i] == 99:
                                blank = blank + 1
                        for i in range(self.cross_entry_coord, self.target_node_coord[1][1]):
                            if self.path_node[self.target_node_coord[1][0]][i] == 99:
                                blank = blank + 1
                        print('A和B之间的伪节点数为：', end='')
                        print(blank)
                        tmp = abs(self.target_node_coord[0][1] - self.len_col) + self.cross_entry_coord + \
                              abs(self.target_node_coord[1][1] - self.cross_entry_coord) - blank
                        self.number_of_route_node.append(tmp)
                        print('从A到B需要的正向步数为：', end='')
                        print(self.number_of_route_node[0])
                        print('开始反向规划，需经过岔道出口')
                        print('岔道出口坐标为：', end='')
                        print(self.cross_exit_coord)
                        blank = 0
                        for i in range(self.cross_exit_coord, self.target_node_coord[0][1]):
                            if self.path_node[self.target_node_coord[0][0]][i] == 99:
                                blank = blank + 1
                        for i in range(min(self.target_node_coord[1][1], self.cross_entry_coord), \
                                       max(self.target_node_coord[1][1], self.cross_entry_coord)):
                            if self.path_node[self.target_node_coord[1][0]][i] == 99:
                                blank = blank + 1
                        print('A和B之间的伪节点数为：', end='')
                        print(blank)
                        tmp = abs(self.target_node_coord[0][1] - self.cross_exit_coord) + \
                              abs(self.target_node_coord[1][1] - self.cross_exit_coord) - blank
                        self.number_of_route_node.append(tmp)
                        print('从A到B需要的反向步数为：', end='')
                        print(self.number_of_route_node[1])
                        print('从A到B需要的正向步数为：', end='')
                        print(self.number_of_route_node[0])
                        print('从A到B需要的反向步数为：', end='')
                        print(self.number_of_route_node[1])
                else:
                    print('B在第一行上面')
                    print('A和B之间存在岔道出口')
                    print('出口坐标为：', end='')
                    print(self.cross_exit_coord)
                    if self.target_node_coord[1][1] < self.cross_exit_coord:
                        print('B在出口左边')
                        # 寻找B和出口之间的空白节点
                        blank = 0
                        for i in range(min(self.target_node_coord[1][1], self.cross_exit_coord), \
                                       max(self.target_node_coord[1][1], self.cross_exit_coord)):
                            if self.path_node[self.target_node_coord[1][0]][i] == 99:
                                blank = blank + 1
                        print('B和出口之间的伪节点数为：', end='')
                        print(blank)
                        tmp_1 = abs(self.target_node_coord[1][1] - self.cross_exit_coord) - blank
                        # 寻找B和矩阵左边界之间的空白节点
                        blank = 0
                        for i in range(0, self.target_node_coord[1][1]):
                            if self.path_node[self.target_node_coord[1][0]][i] == 99:
                                blank = blank + 1
                        print('B和矩阵左边界之间的伪节点数为：', end='')
                        print(blank)
                        tmp_2 = abs(self.len_col - self.cross_exit_coord) + self.target_node_coord[1][1] - blank
                        blank = 0
                        for i in range(self.target_node_coord[0][1], self.cross_exit_coord):
                            if self.path_node[self.target_node_coord[0][0]][i] == 99:
                                blank = blank + 1
                        print('A和出口之间的伪节点数为：', end='')
                        print(blank)
                        tmp = abs(self.target_node_coord[0][1] - self.cross_exit_coord) - blank + min(tmp_1, tmp_2)
                        self.number_of_route_node.append(tmp)
                        print('从A到B需要的步数为：', end='')
                        print(self.number_of_route_node[0])
                        print('开始反向规划，需经过岔道出口')
                        print('岔道入口坐标为：', end='')
                        print(self.cross_entry_coord)
                        if self.target_node_coord[1][1] < self.cross_entry_coord:
                            print('B在入口左边')
                            blank = 0
                            for i in range(self.cross_entry_coord, self.target_node_coord[0][1]):
                                if self.path_node[self.target_node_coord[0][0]][i] == 99:
                                    blank = blank + 1
                            print('A和入口之间的伪节点数为：', end='')
                            print(blank)
                            tmp = abs(self.target_node_coord[0][1] - self.cross_entry_coord) + \
                                  self.target_node_coord[1][1] - blank
                            self.number_of_route_node.append(tmp)
                            print('从A到B需要的反向步数为：', end='')
                            print(self.number_of_route_node[1])
                            print('从A到B需要的正向步数为：', end='')
                            print(self.number_of_route_node[0])
                            print('从A到B需要的反向步数为：', end='')
                            print(self.number_of_route_node[1])
                        elif self.cross_entry_coord < self.target_node_coord[1][1] < self.cross_exit_coord:
                            print('B在入口和出口之间')
                            blank = 0
                            for i in range(self.cross_entry_coord, self.target_node_coord[0][1]):
                                if self.path_node[self.target_node_coord[0][0]][i] == 99:
                                    blank = blank + 1
                            for i in range(self.cross_entry_coord, self.target_node_coord[1][1]):
                                if self.path_node[self.target_node_coord[1][0]][i] == 99:
                                    blank = blank + 1
                            print('A和B之间的总伪节点数为：', end='')
                            print(blank)
                            tmp = abs(self.target_node_coord[0][1] - self.cross_entry_coord) + \
                                  abs(self.target_node_coord[1][1] - self.cross_entry_coord) - blank
                            self.number_of_route_node.append(tmp)
                            print('从A到B需要的反向步数为：', end='')
                            print(self.number_of_route_node[1])
                            print('从A到B需要的正向步数为：', end='')
                            print(self.number_of_route_node[0])
                            print('从A到B需要的反向步数为：', end='')
                            print(self.number_of_route_node[1])
                    elif self.target_node_coord[1][1] >= self.cross_exit_coord:
                        print('B在出口右边,不需要反向规划')
                        blank = 0
                        for i in range(self.target_node_coord[0][1], self.cross_exit_coord):
                            if self.path_node[self.target_node_coord[0][0]][i] == 99:
                                blank = blank + 1
                        print('A和出口之间的伪节点数为：', end='')
                        print(blank)
                        tmp = abs(self.target_node_coord[0][1] - self.cross_exit_coord) - blank + \
                              abs(self.target_node_coord[1][1] - self.cross_exit_coord)
                        self.number_of_route_node.append(tmp)
                        self.number_of_route_node.append(99)
                        print('从A到B需要的反向步数为：', end='')
                        print(self.number_of_route_node[1])
                        print('从A到B需要的正向步数为：', end='')
                        print(self.number_of_route_node[0])
                        print('从A到B需要的反向步数为：', end='')
                        print(self.number_of_route_node[1])

                return self.number_of_route_node

    def path_node_realign(self):
        for i in range(self.len_row):
            for j in range(self.len_col - self.target_node_coord[1][1]):
                tmp = self.path_node[i][self.len_col - 1]
                for k in range(self.len_col - 1, 0, -1):
                    self.path_node[i][k] = self.path_node[i][k - 1]
                self.path_node[i][0] = tmp
        print(self.path_node)

    def path_node_restore(self):
        for i in range(self.len_row):
            for j in range(self.len_col):
                self.path_node[i][j] = self.path_node_bak[i][j]

    def path_plan(self):
        coord_row_A = self.target_node_coord[0][0]
        coord_col_A = self.target_node_coord[0][1]
        coord_row_B = self.target_node_coord[1][0]
        coord_col_B = self.target_node_coord[1][1]
        print('岔道口入口坐标：', end='')
        print(self.cross_entry_coord)
        print('岔道口出口坐标：', end='')
        print(self.cross_exit_coord)
        print('A坐标：', end='')
        print(coord_col_A)
        print('B坐标：', end='')
        print(coord_col_B)
        print('路线：', end='')
        print(self.select_path)
        print('正向：', end='')
        print(self.forward)
        print('反向：', end='')
        print(self.backward)

        if coord_row_A == coord_row_B == 0:
            print('A和B都在第一行，规划路线')
            if self.select_path == 0:  # 默认正反两条路线
                if self.forward == 1 and self.backward == 0:
                    print('正向规划')
                    if coord_col_A < coord_col_B:
                        print('A在B左边')
                        blank = 0
                        for i in range(coord_col_A, coord_col_B):
                            if self.path_node[coord_row_A][i] == 99:
                                blank = blank + 1
                        print('A和B之间伪节点数：', end='')
                        print(blank)
                        for i in range(coord_col_A, coord_col_B - blank):
                            self.path_plan_indicate.append('UP')
                        print(self.path_plan_indicate)
                    else:
                        print('A在B右边')
                        blank = 0
                        for i in range(coord_col_A, self.len_col):
                            if self.path_node[coord_row_A][i] == 99:
                                blank = blank + 1
                        for i in range(0, coord_col_B):
                            if self.path_node[coord_row_B][i] == 99:
                                blank = blank + 1
                        print('A和B之间伪节点数：', end='')
                        print(blank)
                        for i in range(0, self.len_col - coord_col_A + coord_col_B - blank):
                            self.path_plan_indicate.append('UP')
                        print(self.path_plan_indicate)
                elif self.forward == 0 and self.backward == 1:
                    print('反向规划')
                    self.path_plan_indicate.append('TURN')
                    if coord_col_A < coord_col_B:
                        print('A在B左边')
                        blank = 0
                        for i in range(0, coord_col_A):
                            if self.path_node[coord_row_A][i] == 99:
                                blank = blank + 1
                        for i in range(coord_col_B, self.len_col):
                            if self.path_node[coord_row_B][i] == 99:
                                blank = blank + 1
                        print('A和B之间伪节点数：', end='')
                        print(blank)
                        for i in range(0, self.len_col - coord_col_B + coord_col_A - blank):
                            self.path_plan_indicate.append('UP')
                        print(self.path_plan_indicate)
                    else:
                        print('A在B右边')
                        blank = 0
                        for i in range(coord_col_B, coord_col_A):
                            if self.path_node[coord_row_A][i] == 99:
                                blank = blank + 1
                        print('A和B之间伪节点数：', end='')
                        print(blank)
                        for i in range(coord_col_B, coord_col_A - blank):
                            self.path_plan_indicate.append('UP')
                        print(self.path_plan_indicate)
            elif self.select_path != 0:
                print('多条路线警告')
                if self.select_path == 1:
                    if self.forward == 1 and self.backward == 0:
                        print('正向规划')
                        if coord_col_A < coord_col_B:
                            print('A在B左边')
                            blank = 0
                            for i in range(coord_col_A, coord_col_B):
                                if self.path_node[coord_row_A][i] == 99:
                                    blank = blank + 1
                            print('A和B之间伪节点数：', end='')
                            print(blank)
                            for i in range(coord_col_A, coord_col_B - blank):
                                self.path_plan_indicate.append('UP')
                            print(self.path_plan_indicate)
                        else:
                            print('A在B右边')
                            blank = 0
                            for i in range(coord_col_A, self.len_col):
                                if self.path_node[coord_row_A][i] == 99:
                                    blank = blank + 1
                            for i in range(0, coord_col_B):
                                if self.path_node[coord_row_B][i] == 99:
                                    blank = blank + 1
                            print('A和B之间伪节点数：', end='')
                            print(blank)
                            for i in range(0, self.len_col - coord_col_A + coord_col_B - blank):
                                self.path_plan_indicate.append('UP')
                            print(self.path_plan_indicate)
                    elif self.forward == 0 and self.backward == 1:
                        print('反向规划')
                        self.path_plan_indicate.append('TURN')
                        if coord_col_A < coord_col_B:
                            print('A在B左边')
                            blank = 0
                            for i in range(0, coord_col_A):
                                if self.path_node[coord_row_A][i] == 99:
                                    blank = blank + 1
                            for i in range(coord_col_B, self.len_col):
                                if self.path_node[coord_row_B][i] == 99:
                                    blank = blank + 1
                            print('A和B之间伪节点数：', end='')
                            print(blank)
                            for i in range(0, self.len_col - coord_col_B + coord_col_A - blank):
                                self.path_plan_indicate.append('UP')
                            print(self.path_plan_indicate)
                        else:
                            print('A在B右边')
                            blank = 0
                            for i in range(coord_col_B, coord_col_A):
                                if self.path_node[coord_row_A][i] == 99:
                                    blank = blank + 1
                            print('A和B之间伪节点数：', end='')
                            print(blank)
                            for i in range(coord_col_B, coord_col_A - blank):
                                self.path_plan_indicate.append('UP')
                            print(self.path_plan_indicate)
                elif self.select_path >= 2:
                    if self.forward == 1 and self.backward == 0:
                        print('正向规划,A一定在B的左边,且A一定在岔道口入口左边')
                        blank = 0
                        for i in range(coord_col_A, self.cross_entry_coord):
                            if self.path_node[coord_row_A][i] == 99:
                                blank = blank + 1
                        print('A和岔道口入口之间伪节点数：', end='')
                        print(blank)
                        for i in range(coord_col_A, self.cross_entry_coord - blank):
                            self.path_plan_indicate.append('UP')
                        for i in range(self.select_path - 1):
                            self.path_plan_indicate.append('RIGHT')
                        blank = 0
                        for i in range(self.cross_entry_coord, self.cross_exit_coord):
                            if self.path_node[self.select_path - 1][i] == 99:
                                blank = blank + 1
                        print('岔道口入口和出口之间伪节点数：', end='')
                        print(blank)
                        for i in range(self.cross_entry_coord, self.cross_exit_coord - blank):
                            self.path_plan_indicate.append('UP')
                        for i in range(self.select_path - 1):
                            self.path_plan_indicate.append('LEFT')
                        blank = 0
                        for i in range(self.cross_exit_coord, coord_col_B):
                            if self.path_node[coord_row_B][i] == 99:
                                blank = blank + 1
                        print('岔道口出口和B之间伪节点数：', end='')
                        print(blank)
                        for i in range(self.cross_exit_coord, coord_col_B - blank):
                            self.path_plan_indicate.append('UP')
                        print(self.path_plan_indicate)
                    elif self.forward == 0 and self.backward == 1:
                        print('反向规划,A一定在B的右边,且A一定在岔道口出口右边')
                        self.path_plan_indicate.append('TURN')
                        blank = 0
                        for i in range(self.cross_exit_coord, coord_col_A):
                            if self.path_node[coord_row_A][i] == 99:
                                blank = blank + 1
                        print('A和岔道口出口之间伪节点数：', end='')
                        print(blank)
                        for i in range(self.cross_exit_coord, coord_col_A - blank):
                            self.path_plan_indicate.append('UP')
                        for i in range(self.select_path - 1):
                            self.path_plan_indicate.append('LEFT')
                        blank = 0
                        for i in range(self.cross_entry_coord, self.cross_exit_coord):
                            if self.path_node[self.select_path - 1][i] == 99:
                                blank = blank + 1
                        print('岔道口入口和出口之间伪节点数：', end='')
                        print(blank)
                        for i in range(self.cross_entry_coord, self.cross_exit_coord - blank):
                            self.path_plan_indicate.append('UP')
                        for i in range(self.select_path - 1):
                            self.path_plan_indicate.append('RIGHT')
                        blank = 0
                        for i in range(coord_col_B, self.cross_entry_coord):
                            if self.path_node[coord_row_B][i] == 99:
                                blank = blank + 1
                        print('岔道口出口和B之间伪节点数：', end='')
                        print(blank)
                        for i in range(coord_col_B, self.cross_entry_coord - blank):
                            self.path_plan_indicate.append('UP')
                        print(self.path_plan_indicate)
        elif coord_row_A != 0 and coord_row_B != 0:
            print('A和B都不在第一行，规划路线')
            if self.target_node_coord[0][0] == self.target_node_coord[1][0]:
                print('A和B在同一条路线上，不用考虑岔道口')
                if self.forward == 1 and self.backward == 0:
                    print('正向规划，这种情况A一定在B的左边')
                    blank = 0
                    for i in range(coord_col_A, coord_col_B):
                        if self.path_node[coord_row_A][i] == 99:
                            blank = blank + 1
                    print('A和B之间伪节点数：', end='')
                    print(blank)
                    for i in range(coord_col_A, coord_col_B - blank):
                        self.path_plan_indicate.append('UP')
                    print(self.path_plan_indicate)
                elif self.forward == 0 and self.backward == 1:
                    self.path_plan_indicate.append('TRUN')
                    print('反向规划，这种情况A一定在B的右边')
                    blank = 0
                    for i in range(coord_col_B, coord_col_A):
                        if self.path_node[coord_row_A][i] == 99:
                            blank = blank + 1
                    print('A和B之间伪节点数：', end='')
                    print(blank)
                    for i in range(coord_col_B, coord_col_A - blank):
                        self.path_plan_indicate.append('UP')
                    print(self.path_plan_indicate)
            else:
                print('A和B不在同一条路线上，需要考虑岔道口的坐标')
                if self.forward == 1 and self.backward == 0:
                    print('选择正向')
                    blank = 0
                    for i in range(coord_col_A, self.cross_exit_coord):
                        if self.path_node[coord_row_A][i] == 99:
                            blank = blank + 1
                    print('A和岔道口出口之间伪节点数：', end='')
                    print(blank)
                    for i in range(coord_col_A, self.cross_exit_coord - blank):
                        self.path_plan_indicate.append('UP')
                    if coord_row_A < coord_row_B:
                        print('A在B上面')
                        for i in range(coord_row_A, coord_row_B):
                            self.path_plan_indicate.append('RIGHT')
                    else:
                        print('A在B下面')
                        for i in range(coord_row_A, coord_row_B):
                            self.path_plan_indicate.append('LEFT')
                    blank = 0
                    for i in range(self.cross_exit_coord, coord_col_B):
                        if self.path_node[coord_row_B][i] == 99:
                            blank = blank + 1
                    print('B和岔道口出口之间伪节点数：', end='')
                    print(blank)
                    for i in range(coord_col_B, self.cross_exit_coord - blank):
                        self.path_plan_indicate.append('UP')
                    print(self.path_plan_indicate)
                elif self.forward == 0 and self.backward == 1:
                    print('选择反向')
                    self.path_plan_indicate.append('TURN')
                    blank = 0
                    for i in range(self.cross_entry_coord, coord_col_A):
                        if self.path_node[coord_row_A][i] == 99:
                            blank = blank + 1
                    print('A和岔道口入口之间伪节点数：', end='')
                    print(blank)
                    for i in range(self.cross_entry_coord, coord_col_A - blank):
                        self.path_plan_indicate.append('UP')
                    if coord_row_A < coord_row_B:
                        print('A在B上面')
                        for i in range(coord_row_A, coord_row_B):
                            self.path_plan_indicate.append('LEFT')
                    else:
                        print('A在B下面')
                        for i in range(coord_row_A, coord_row_B):
                            self.path_plan_indicate.append('RIGHT')
                    blank = 0
                    for i in range(self.cross_entry_coord, coord_col_B):
                        if self.path_node[coord_row_B][i] == 99:
                            blank = blank + 1
                    print('B和岔道口出口之间伪节点数：', end='')
                    print(blank)
                    for i in range(self.cross_entry_coord, coord_col_B - blank):
                        self.path_plan_indicate.append('UP')
                    print(self.path_plan_indicate)
        elif (coord_row_A == 0 and coord_row_B != 0) or (coord_row_B == 0 and coord_row_A != 0):
            print('A和B仅有一个在第一行，规划路线')
            if self.forward == 1 and self.backward == 0:
                print('正向规划')
                if coord_row_A == 0:
                    print('A在第一行')
                    if coord_col_A <= self.cross_entry_coord:
                        print('A在入口左边')
                        blank = 0
                        for i in range(coord_col_A, self.cross_entry_coord):
                            if self.path_node[coord_row_A][i] == 99:
                                blank = blank + 1
                        print('A和入口之间伪节点数：', end='')
                        print(blank)
                        for i in range(coord_col_A, self.cross_entry_coord - blank):
                            self.path_plan_indicate.append('UP')
                        for i in range(coord_row_A, coord_row_B):
                            self.path_plan_indicate.append('RIGHT')
                        blank = 0
                        for i in range(self.cross_entry_coord, coord_col_B):
                            if self.path_node[coord_row_B][i] == 99:
                                blank = blank + 1
                        print('B和入口之间伪节点数：', end='')
                        print(blank)
                        for i in range(self.cross_entry_coord, coord_col_B - blank):
                            self.path_plan_indicate.append('UP')
                        print(self.path_plan_indicate)
                    elif self.cross_entry_coord < coord_col_A <= self.cross_exit_coord:
                        print('A在岔道口入口和出口之间')
                        blank = 0
                        for i in range(coord_col_A, self.cross_exit_coord):
                            if self.path_node[coord_row_A][i] == 99:
                                blank = blank + 1
                        print('A和入口之间伪节点数：', end='')
                        print(blank)
                        for i in range(coord_col_A, self.cross_exit_coord - blank):
                            self.path_plan_indicate.append('UP')
                        for i in range(coord_row_A, coord_row_B):
                            self.path_plan_indicate.append('RIGHT')
                        blank = 0
                        for i in range(coord_col_B, self.cross_exit_coord):
                            if self.path_node[coord_row_B][i] == 99:
                                blank = blank + 1
                        print('B和入口之间伪节点数：', end='')
                        print(blank)
                        for i in range(coord_col_B, self.cross_exit_coord - blank):
                            self.path_plan_indicate.append('UP')
                        print(self.path_plan_indicate)
                    elif self.cross_exit_coord < coord_col_A:
                        print('A在岔道口出口右边')
                        for i in range(coord_col_A, self.len_col):
                            self.path_plan_indicate.append('UP')
                        for i in range(0, self.cross_entry_coord):
                            self.path_plan_indicate.append('UP')
                        for i in range(coord_row_A, coord_row_B):
                            self.path_plan_indicate.append('RIGHT')
                        blank = 0
                        for i in range(self.cross_entry_coord, coord_col_B):
                            if self.path_node[coord_row_B][i] == 99:
                                blank = blank + 1
                        print('B和入口之间伪节点数：', end='')
                        print(blank)
                        for i in range(self.cross_entry_coord, coord_col_B - blank):
                            self.path_plan_indicate.append('UP')
                        print(self.path_plan_indicate)
                elif coord_row_B == 0:
                    print('B在第一行')
                    if coord_col_B < self.cross_exit_coord:
                        print('B在出口左边')
                        blank = 0
                        for i in range(coord_col_A, self.cross_exit_coord):
                            if self.path_node[coord_row_A][i] == 99:
                                blank = blank + 1
                        print('A和出口之间伪节点数：', end='')
                        print(blank)
                        for i in range(coord_col_A, self.cross_exit_coord - blank):
                            self.path_plan_indicate.append('UP')
                        blank = 0
                        for i in range(0, coord_col_B):
                            if self.path_node[coord_row_B][i] == 99:
                                blank = blank + 1
                        tmp_1 = abs(self.cross_exit_coord - self.len_col) + \
                                coord_col_B - blank
                        blank = 0
                        for i in range(coord_col_B, self.cross_exit_coord):
                            if self.path_node[coord_row_B][i] == 99:
                                blank = blank + 1
                        tmp_2 = abs(self.cross_exit_coord - coord_col_B) - blank
                        if tmp_1 <= tmp_2:
                            for i in range(tmp_1):
                                self.path_plan_indicate.append('UP')
                        elif tmp_1 > tmp_2:
                            self.path_plan_indicate.append('TURN')
                            for i in range(tmp_2):
                                self.path_plan_indicate.append('UP')
                        print(self.path_plan_indicate)
                    elif coord_col_B >= self.cross_exit_coord:
                        print('B在出口右边')
                        blank = 0
                        for i in range(coord_col_A, self.cross_exit_coord):
                            if self.path_node[coord_row_A][i] == 99:
                                blank = blank + 1
                        print('A和出口之间伪节点数：', end='')
                        print(blank)
                        for i in range(coord_col_A, self.cross_exit_coord - blank):
                            self.path_plan_indicate.append('UP')
                        for i in range(self.cross_exit_coord, coord_col_B):
                            self.path_plan_indicate.append('UP')
                        print(self.path_plan_indicate)
            elif self.forward == 0 and self.backward == 1:
                print('反向规划')
                self.path_plan_indicate.append('TURN')
                if coord_row_A == 0:
                    print('A在第一行')
                    if coord_col_A <= self.cross_entry_coord:
                        print('A在入口左边')
                        for i in range(0, coord_col_A):
                            self.path_plan_indicate.append('UP')
                        for i in range(self.cross_exit_coord, self.len_col):
                            self.path_plan_indicate.append('UP')
                        for i in range(coord_row_A, coord_row_B):
                            self.path_plan_indicate.append('LEFT')
                        blank = 0
                        for i in range(coord_col_B, self.cross_exit_coord):
                            if self.path_node[coord_row_B][i] == 99:
                                blank = blank + 1
                        for i in range(coord_col_B, self.cross_exit_coord - blank):
                            self.path_plan_indicate.append('UP')
                        print(self.path_plan_indicate)
                    elif self.cross_entry_coord < coord_col_A < self.cross_exit_coord:
                        print('A在入口和出口之间')
                        blank = 0
                        for i in range(self.cross_entry_coord, coord_col_A):
                            if self.path_node[coord_row_A][i] == 99:
                                blank = blank + 1
                        for i in range(self.cross_entry_coord, coord_col_A - blank):
                            self.path_plan_indicate.append('UP')
                        self.path_plan_indicate.append('TURN')
                        for i in range(coord_row_A, coord_row_B):
                            self.path_plan_indicate.append('RIGHT')
                        blank = 0
                        for i in range(self.cross_entry_coord, coord_col_B):
                            if self.path_node[coord_row_B][i] == 99:
                                blank = blank + 1
                        for i in range(self.cross_entry_coord, coord_col_B - blank):
                            self.path_plan_indicate.append('UP')
                        print(self.path_plan_indicate)
                    elif self.cross_exit_coord <= coord_col_A:
                        print('A在出口右边')
                        for i in range(self.cross_exit_coord, coord_col_A):
                            self.path_plan_indicate.append('UP')
                        for i in range(coord_row_A, coord_row_B):
                            self.path_plan_indicate.append('LEFT')
                        blank = 0
                        for i in range(coord_col_B, self.cross_exit_coord):
                            if self.path_node[coord_row_B][i] == 99:
                                blank = blank + 1
                        for i in range(coord_col_B, self.cross_exit_coord - blank):
                            self.path_plan_indicate.append('UP')
                        print(self.path_plan_indicate)
                elif coord_row_B == 0:
                    print('B在第一行')
                    if coord_col_B <= self.cross_entry_coord:
                        print('B在入口左边')
                        blank = 0
                        for i in range(self.cross_entry_coord, coord_col_A):
                            if self.path_node[coord_row_A][i] == 99:
                                blank = blank + 1
                        for i in range(self.cross_entry_coord, coord_col_A - blank):
                            self.path_plan_indicate.append('UP')
                        for i in range(coord_col_B, self.cross_entry_coord):
                            self.path_plan_indicate.append('UP')
                        print(self.path_plan_indicate)
                    elif self.cross_entry_coord < coord_col_B:
                        print('B在入口右边')
                        blank = 0
                        for i in range(self.cross_entry_coord, coord_col_A):
                            if self.path_node[coord_row_A][i] == 99:
                                blank = blank + 1
                        for i in range(self.cross_entry_coord, coord_col_A - blank):
                            self.path_plan_indicate.append('UP')
                        blank = 0
                        for i in range(coord_col_B, self.len_col):
                            if self.path_node[coord_row_B][i] == 99:
                                blank = blank + 1
                        tmp_1 = self.cross_entry_coord + abs(self.len_col - coord_col_B) - blank
                        blank = 0
                        for i in range(self.cross_entry_coord, coord_col_B):
                            if self.path_node[coord_row_B][i] == 99:
                                blank = blank + 1
                        tmp_2 = abs(self.cross_entry_coord - coord_col_B) - blank
                        if tmp_1 <= tmp_2:
                            for i in range(tmp_1):
                                self.path_plan_indicate.append('UP')
                        elif tmp_1 > tmp_2:
                            self.path_plan_indicate.append('TURN')
                            for i in range(tmp_2):
                                self.path_plan_indicate.append('UP')
                        print(self.path_plan_indicate)
        return self.path_plan_indicate

    def print(self):
        for i in range(self.len_row):
            print("[", end='')
            for j in range(self.len_col - 1):
                print(str(self.path_node[i][j]) + ', ', end='')
            print(str(self.path_node[i][self.len_col - 1]), end='')
            print(']')
        print()
