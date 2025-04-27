import threading
import random
import time

n, m, k = 16, 16, 16

A = [[random.randint(0, 9) for _ in range(m)] for _ in range(n)]
B = [[random.randint(0, 9) for _ in range(k)] for _ in range(m)]
C = [[0 for _ in range(k)] for _ in range(n)]

print_lock = threading.Lock()

def multiply_element(row, col):
    global A, B, C
    sum_value = sum(A[row][i] * B[i][col] for i in range(m))
    C[row][col] = sum_value

    with print_lock:
        print(f"[{row},{col}] = {sum_value}")

def main():
    threads = []
    start_time = time.time()

    for i in range(n):
        for j in range(k):
            t = threading.Thread(target=multiply_element, args=(i, j))
            t.start()
            threads.append(t)

    for t in threads:
        t.join()

    end_time = time.time()

    print("\nMatrix C:")
    for row in C:
        print(row)

    print(f"\nTime taken: {end_time - start_time:.6f} seconds")

if __name__ == "__main__":
    main()
