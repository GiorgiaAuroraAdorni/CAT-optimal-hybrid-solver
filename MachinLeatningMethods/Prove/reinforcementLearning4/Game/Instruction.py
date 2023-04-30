import itertools


TOT_istructions_test = [
[0, 0, 1],
[0, 1, 0],
[0, -1, 1],
[0, -1, -1],
[2, 0, 1, -1, 0, 0, -1],
]



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




TOT_istructions_2 = [
[0, 0, 1],
[0, 1, 0],
[0, -1, 1],
[0, -1, -1],
[2, 0, 1, -1, 0, 0, -1],
[2, -1, 0, -1, 0, 0, -1, 0, -1],
[2, -1, 0, -1, 0, 0, 1, 0, 1],
[2, 0, -1, 0, -1, -1, 0, -1, 0],
[2, 0, 1, 0, 1, -1, 0, -1, 0],
[0, -1, 1, 1, 1],
[0, 1, 1, -1, 1],
[1, 1, 1, 1, -1],
[1, 1, -1, 1, 1]
]

def generate_combinations(n):
    result = []
    # TODO: cambiare qui se si vuole avere mosse con pattern piu grandi di 4
    for length in range(1, 5):
        for combination in itertools.product(range(1, n+1), repeat=length):
            result.append(list(combination))
    return result
