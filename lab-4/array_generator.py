import random
import csv
import os

def gen(l, number):
    length = l
    mas = [random.randint(-20, 20) for _ in range(length)]

    output_dir = os.path.expanduser("~/Desktop/itmo/ITMO/ALGOS/polygon/lab-4/output")
    os.makedirs(output_dir, exist_ok=True)
    filepath = os.path.join(output_dir, f"{length}_{number}.csv")
    print(filepath)

    with open(filepath, mode='w', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(mas)

for i in range(1, 11):
    gen(25, i)
