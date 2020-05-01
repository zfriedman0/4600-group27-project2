# Sources:
# https://cmdlinetips.com/2018/01/3-ways-to-read-a-file-and-skip-initial-comments-in-python/
# https://www.geeksforgeeks.org/python-converting-all-strings-in-list-to-integers/
def parse_processes():
    temp = 'num_processes='
    with open('test.txt') as f:
        for line in f:
            if temp in line: return int(line[line.find(temp) + len(temp):])

def parse_resources():
    temp = 'num_resources='
    with open('test.txt') as f:
        for line in f:
            if temp in line: return int(line[line.find(temp) + len(temp):])

# find units per resource
def parse_units(num_resources):
    with open('test.txt') as f:
        for line in f:
            if line.startswith("%") or not line.strip(): continue # skips comments
            #if not line.strip(): continue # skips blank lines
            if line.startswith("n"): continue # skips process/resource assignment

            current_line = line.strip().split(',')
            if len(current_line) == num_resources:
                array = list(map(int, current_line))
                return array

def print_M(M):
    print("Adjacency Matrix:")
    for x in M:
        print(*x)

def main():
    num_processes = parse_processes()
    num_resources = parse_resources()
    num_units = parse_units(num_resources) # units per resource
    print("Number of processes:", num_processes)
    print("Number of resources:", num_resources)
    print("Units per resource: ", num_units)

    # skip to matrix values and assign adjacency matrix per process/resource
    Pcount = 0
    Rcount = 0
    M = [] # The adjacency matrix
    with open('test.txt') as f:
        for line in f:
            if line.startswith("%") or not line.strip(): continue # skips comments
            if line.startswith("n"): continue # skips process/resource assignment
            #if : continue # skips blank lines

            current_line = list(map(int, line.strip().split(',')))
            if len(current_line) == num_resources: continue

            if current_line[0] == 0 or 1:
                if Pcount < num_processes:
                    M.append(current_line)
                    Pcount += 1
                    continue
                if Rcount < num_resources:
                    M.append(current_line)
                    Rcount += 1
                    continue

    print_M(M)

    #end main()

main()
