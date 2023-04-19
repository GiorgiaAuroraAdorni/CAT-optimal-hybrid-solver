import itertools


TOT_istructions = [
[0, 0, 1],
[0, 1, 0],
[0, -1, 1],
[0, -1, -1],
#[2, 0, 1, -1, 0, 0, -1],
#[2, -1, 0, -1, 0, 0, -1, 0, -1],
#[2, -1, 0, -1, 0, 0, 1, 0, 1],
#[2, 0, -1, 0, -1, -1, 0, -1, 0],
#[2, 0, 1, 0, 1, -1, 0, -1, 0],
[0, -1, 1, 1, 1],
[0, 1, 1, -1, 1],
[1, 1, 1, 1, -1],
[1, 1, -1, 1, 1]
]

def generate_combinations(n):
    result = []
    for length in range(1, 5):
        for combination in itertools.product(range(1, n+1), repeat=length):
            result.append(list(combination))
    return result
