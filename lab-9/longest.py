import numpy as np


def longest_sequence(arr):
    arr = np.array(arr)
    changes = np.diff(arr)
    change_indices = np.where(changes != 0)[0] + 1 
    indices = np.concatenate(([0], change_indices, [len(arr)]))
    lengths = indices[1:] - indices[:-1]
    
    return np.max(lengths)

# Пример использования
arr = [1, 1, 2, 2, 2, 3, 3, 1, 1, 1, 1, 4, 4, 4, 4, 4, 4]
result = longest_sequence(arr)
print("Длина самой длинной последовательности одинаковых элементов:", result)