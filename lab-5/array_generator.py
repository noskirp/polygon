import csv
import os
import random


def create_csv_files(n_max, m, output_dir="output"): #n_max - максимальный допустимый элемент; m - количество файлов которые нужно создать
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    for i in range(1, m + 1):
        filename = os.path.join(output_dir, f"{n_max}_{i}.csv")
        with open(filename, 'w', newline='') as csvfile:
            writer = csv.writer(csvfile)
            mas = [x for x in range(1, n_max + 1)]
            random.shuffle(mas)
            for j in mas:
                writer.writerow([j])

create_csv_files(10, 10)

