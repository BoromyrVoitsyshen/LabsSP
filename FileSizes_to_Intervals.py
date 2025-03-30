from itertools import groupby

qt = []

with open("result.txt", "r", encoding="utf-16") as file:
    v = [int(line.strip()) for line in file if line.strip() != "-1"]

if v:  
    for key, group in groupby(v):
        qt.append((key, len(list(group))))

    with open("output.txt", "w") as output_file:
        for value, count in qt:
            output_file.write(f"{value} {count}\n")
