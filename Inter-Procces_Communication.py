import multiprocessing
import time

def f(x):
    if x % 2 == 0:
        return 1
    time.sleep(1000)
    return 0

def g(x):
    if x % 3 == 0:
        time.sleep(1000)
        return 1
    return 0

def f_process(x, write_conn):
    result = f(x)
    if result is not None:
        write_conn.send(result)

def g_process(x, write_conn):
    result = g(x)
    write_conn.send(result)

def main():
    x = int(input("Enter x: "))

    f_parent_recv, f_child_send = multiprocessing.Pipe(duplex=False)
    g_parent_recv, g_child_send = multiprocessing.Pipe(duplex=False)

    f_proc = multiprocessing.Process(target=f_process, args=(x, f_child_send))
    g_proc = multiprocessing.Process(target=g_process, args=(x, g_child_send))
    f_proc.start()
    g_proc.start()

    f_result = None
    g_result = None

    continue_without_prompt = False

    while True:
        if f_parent_recv.poll(0.5) and f_result is None:
            try:
                f_result = f_parent_recv.recv()
                print(f"f(x) = {f_result}")
            except EOFError:
                pass

        if g_parent_recv.poll(0.5) and g_result is None:
            try:
                g_result = g_parent_recv.recv()
                print(f"g(x) = {g_result}")
            except EOFError:
                pass

        if f_result == 0 or g_result == 0:
            print("Result is 0")
            break

        if f_result == 1 and g_result == 1:
            print("Both functions returned 1: result is 1")
            break

        if f_result is not None and g_result is not None:
            print(f"f = {f_result}, g = {g_result}, result = {f_result and g_result}")
            break

        print("Still waiting...")
        for _ in range(10):
            time.sleep(1)
            if f_result is not None and g_result is not None:
                break
        else:
            if not continue_without_prompt:
                print("\n1) Продовжити обчислення")
                print("2) Припинити")
                print("3) Продовжити, не перепитуючи більше")
                answer = input("Ваш вибір (1/2/3): ").strip()

                if answer == "2":
                    print("Програма зупинена.")
                    break
                elif answer == "3":
                    continue_without_prompt = True
                    print("Продовжуємо без подальших запитів.")
                else:
                    print("Продовжуємо обчислення...")

        if f_result is not None and g_result is not None:
            print("Результати готові!")
            break

    f_proc.terminate()
    g_proc.terminate()
    f_proc.join()
    g_proc.join()

if __name__ == '__main__':
    multiprocessing.freeze_support()
    main()
