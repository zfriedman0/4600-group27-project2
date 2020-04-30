# Sources:
# https://cmdlinetips.com/2018/01/3-ways-to-read-a-file-and-skip-initial-comments-in-python/
# https://www.geeksforgeeks.org/python-converting-all-strings-in-list-to-integers/
# file = open("bar.txt", "r")

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
            if line.startswith("%"): continue
            if line.startswith("n"): continue
            if not line.strip(): continue

            current_line = line.strip().split(',')
            if len(current_line) == num_resources:
                array = list(map(int, current_line))
                return array

def main():
    num_processes = parse_processes()
    num_resources = parse_resources()
    num_units = parse_units(num_resources)
    print("Number of processes:", num_processes)
    print("Number of resources:", num_resources)
    print("Units per resource: ", num_units)

main()
