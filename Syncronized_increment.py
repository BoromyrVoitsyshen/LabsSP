import threading

ITERATIONS = 1000
shared_var = 0

event1_ready = threading.Event()
event2_ready = threading.Event()
lock = threading.Lock()

def thread1():
    global shared_var
    for _ in range(ITERATIONS):
        with lock:
            shared_var += 1
        event1_ready.set() 
        event2_ready.wait() 
        event2_ready.clear()

def thread2():
    for _ in range(ITERATIONS):
        event1_ready.wait() 
        event1_ready.clear()
        event2_ready.set() 
t1 = threading.Thread(target=thread1)
t2 = threading.Thread(target=thread2)

t1.start()
t2.start()

t1.join()
t2.join()

print("Final value synchronized:", shared_var)
