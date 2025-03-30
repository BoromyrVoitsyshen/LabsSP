import sys

def process(L, R, v, res):
    """Розширює інтервал L-R, включаючи всі однакові значення."""
    N = len(v)
    while L > 0 and v[L - 1] == v[L]:
        L -= 1
    while R < N - 1 and v[R + 1] == v[R]:
        R += 1
    if L <= R:
        res.append((L, R))

def main():
    try:
        with open("result.txt", "r", encoding="utf-16") as file:
            v = [int(line.strip()) for line in file if int(line.strip()) != -1]
    except FileNotFoundError:
        print("Can't open the file")
        sys.exit(1)
    
    v.sort()  
    N = len(v)

    while True:
        try:
            per = float(input("Enter the % of all files you want to include (0;100): "))
            if 0 < per < 100:
                break
            else:
                print("The % should be in the range (0;100)!")
        except ValueError:
            print("Invalid input. Please enter a real number.")
    
    M = int((per / 100) * N)
    res = []
    cur = 0

    while cur < (N - M):
        process(cur, cur + M - 1, v, res)  
        while cur < N - 1 and v[cur + 1] == v[cur]:
            cur += 1
        cur += 1
    
    process(N - M, N - 1, v, res) 

    res.sort(key=lambda x: v[x[0]])

    min_dif_ind = max_dens_ind = None
    min_diff = float('inf')
    max_density = float('-inf')

    for i, (L, R) in enumerate(res):
        interval_diff = v[R] - v[L]
        files_count = (R - L + 1) * 100 / N

        if interval_diff > 0:
            density = (R - L) / interval_diff
        else:
            density = 0  

        print(f"Interval: {v[L]}-{v[R]} bytes, interval difference: {interval_diff} bytes, "
              f"files count: {files_count:.2f}% of all, "
              f"average density: {density:.2f}")

        if 0 < interval_diff < min_diff:
            min_dif_ind = i
            min_diff = interval_diff
        
        if density > max_density:
            max_dens_ind = i
            max_density = density

    if min_dif_ind is not None:
        L, R = res[min_dif_ind]
        print(f"\nShortest interval: {v[L]}-{v[R]} bytes, interval difference: {v[R] - v[L]} bytes, "
              f"files count: {(R - L + 1) * 100 / N:.2f}% of all")

    if max_dens_ind is not None:
        L, R = res[max_dens_ind]
        print(f"Densest interval: {v[L]}-{v[R]} bytes, "
              f"files count: {(R - L + 1) * 100 / N:.2f}% of all, "
              f"average density: {max_density:.2f}")

if __name__ == "__main__":
    main()
