import re

def parse_string(string):
    return re.findall(r'\S+', string)

def file_reader(fileName):
    # line_file    --> the line for all file
    # x AND y      --> two vector that rapresent city x[0] and y[0]
    #                  --> are coordinate of city 1
    # flag         --> if true that means the line_file rapresent point not descriptions
    # best         --> solution

    line_file = ""
    x = []
    y = []
    best = 0.0
    flag = False

    # START read File
    with open(fileName) as file:
        Problem = []
        n = 0
        r = 0
        for line in file:
            line_file = line.strip()
            # Stop read because file is end
            if line_file == "EOF":
                break

            parsedString = parse_string(line_file)

            if len(parsedString) == 1:
                n = int(parsedString[0])
                mat = [[0] * n for _ in range(n)]
                Problem = mat
                #print(f"The problem has length {len(Problem)} x {len(Problem)}")

            else:
                for i in range(n):
                    value_tmp = int(parsedString[i])
                    if value_tmp == 0:
                        Problem[r][i] = -1
                    else:
                        Problem[r][i] = value_tmp
                r += 1

    return Problem

def file_reader_test(fileName):
    # line_file    --> the line for all file
    # x AND y      --> two vector that rapresent city x[0] and y[0]
    #                  --> are coordinate of city 1
    # flag         --> if true that means the line_file rapresent point not descriptions
    # best         --> solution

    line_file = ""
    x = []
    y = []
    best = 0.0
    flag = False

    # START read File
    with open(fileName) as file:
        Problem = []
        n = 0
        r = 0
        for line in file:
            line_file = line.strip()
            # Stop read because file is end
            if line_file == "EOF":
                break

            parsedString = parse_string(line_file)

            if len(parsedString) == 1:
                n = int(parsedString[0])
                mat = [[0] * n for _ in range(n)]
                Problem = mat
                #print(f"The problem has length {len(Problem)} x {len(Problem)}")

            else:
                for i in range(n):
                    value_tmp = int(parsedString[i])
                    Problem[r][i] = value_tmp
                r += 1

    return Problem
