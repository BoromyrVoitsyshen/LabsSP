import threading
import time

shared_var = 0
lock = threading.Lock()
ITERATIONS = 10**6

def increment_with_lock():
    global shared_var
    for _ in range(ITERATIONS):
        with lock:
            shared_var += 1

shared_var = 0
ITERATIONS = 10**9

def increment_without_lock():
    global shared_var
    for _ in range(ITERATIONS):
        shared_var += 1

def main():
    global shared_var
    shared_var = 0

    t1 = threading.Thread(target=increment_without_lock)
    t2 = threading.Thread(target=increment_without_lock)

    start_time = time.time()

    t1.start()
    t2.start()

    t1.join()
    t2.join()

    end_time = time.time()

    print(f"Final value with lock: {shared_var}")
    print(f"Time taken with lock: {end_time - start_time:.6f} seconds")

if __name__ == "__main__":
    main()
