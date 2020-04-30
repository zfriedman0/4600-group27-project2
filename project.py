# Sources:
# https://cmdlinetips.com/2018/01/3-ways-to-read-a-file-and-skip-initial-comments-in-python/
# file = open("bar.txt", "r")
import re

def parse_processes():
    temp = 'num_processes='
    with open('test.txt') as f:
        for line in f:
            # if line.startswith("%"):
            #     continue
            if temp in line: return int(line[line.find(temp) + len(temp):])
                #num_processes = int(line[line.find(temp1) + len(temp1):])
                #print("Number of processes:", num_processes)
                #continue

def parse_resources():
    temp = 'num_resources='
    with open('test.txt') as f:
        for line in f:
            # if line.startswith("%"):
            #     continue
            if temp in line: return int(line[line.find(temp) + len(temp):])
        #num_resources = int(line[line.find(temp2) + len(temp2):])
        #print("Number of resources:", num_resources)
        #continue

def parse_units(num_resources):
    with open('test.txt') as f:
        for line in f:
            if line.startswith("%"): continue # skips comments
            if line.startswith("n"): continue # skips number resource/process definition
            if not line.strip(): continue # skips blank lines

            #current_line = line.split(",") # delineates commas
            current_line = re.split(r'[,|\n]', line)
            print(current_line)
            # if len(current_line) == num_resources:
            #     x = 0
            #     num_units = [None] * num_resources
            #     while x < (num_resources + 1):
            #         num_units[x] = current_line[x]
            #     #print(num_units)
            #     return

                # for x in current_line:
                #     array[x] = current_line[x]
                # print(current_line)
                # return
            # for x in current_line:
            #     if x > 1

def main():
    num_processes = parse_processes()
    num_resources = parse_resources()
    num_units = parse_units(num_resources)
    print("Number of processes:", num_processes)
    print("Number of resources:", num_resources)

main()
