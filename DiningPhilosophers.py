import threading
import time
import random

# Розробити та запрограмувати алгоритм вирішеня задачі про філософів, що обідають – 
# https://en.wikipedia.org/wiki/Dining_philosophers_problem .
# Рішення має враховувати:
# ексклюзивність ресурсів (виделку як ресурс може взяти лише один (з двох adjacent neighboring) філософ у кожний момент часу, 
# і тільки якщо вона вільна)
# deadlock-free (щоб система не могла стати в тупик) 
# livelock-free (щоб філософи не померли від голодування, коли їм довго не вдається взяти виделки, 
# бо вони якимось чином блокуються сусідами), ситуація нескінченного очікування
# Перевага надається рішенням, які:
# реалізують однаковий алгоритм дій усіх філософів (не залежать від номера філософа, тощо)
# не мають централізованої сутності (офіціанта, оракула, тощо), який керує розподілом ресурсів-виделок
# не передбачають комунікацію між філософами (щоб вони домовились про виделки чи інші спільні дії)
# є масштабованим на будь-яку кількість філософів (і виделок відповідно) і не мають очевидних bottlenecks
# можна довести формально їх коректність і виконуваність всіх вказаних вимог
# Всі програми-філософи мають діяти за однаковим алгоритмом. Всі вони рівноправні (і мають бути взаємозамінні), 
# жоден не має пріоритету над іншими.

class Philosopher(threading.Thread):
    def __init__(self, left_fork, right_fork):
        super().__init__()
        self.left_fork = left_fork
        self.right_fork = right_fork
    
    def think(self):
        print(f"{self.name} is thinking.")
        time.sleep(random.uniform(1, 3))
    
    def eat(self):
        print(f"{self.name} is eating.")
        time.sleep(random.uniform(1, 2))
    
    def run(self):
        while True:
            self.think()
            while True:
                acquired_first = self.left_fork.acquire(timeout=random.uniform(0.1, 0.5))
                if acquired_first:
                    print(f"{self.name} picked up the left fork.")
                    acquired_second = self.right_fork.acquire(timeout=random.uniform(0.1, 0.5))
                    if acquired_second:
                        print(f"{self.name} picked up the right fork and is ready to eat.")
                        self.eat()
                        self.right_fork.release()
                        print(f"{self.name} put down the right fork.")
                    self.left_fork.release()
                    print(f"{self.name} put down the left fork.")
                    if acquired_second:
                        break  
                time.sleep(random.uniform(0.1, 0.5))  

num_philosophers = 5
forks = [threading.Lock() for _ in range(num_philosophers)]
philosophers = [Philosopher(forks[i], forks[(i + 1) % num_philosophers]) for i in range(num_philosophers)]

for p in philosophers:
    p.start()

for p in philosophers:
    p.join()
