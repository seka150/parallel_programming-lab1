import csv
import re
import subprocess
import sys
from pathlib import Path

import matplotlib.pyplot as plt

ROOT = Path(__file__).resolve().parent
PROGRAM = ROOT / "build" / "matrix"
GENERATOR = ROOT / "gen.py"

SIZES = [50, 100, 200, 300, 500]
REPEATS = 3

if not PROGRAM.exists():
    print("Error: program not found. Run: cmake --build build")
    sys.exit(1)

results = []

for n in SIZES:
    times = []

    for _ in range(REPEATS):
        subprocess.run(
            [sys.executable, str(GENERATOR), str(n)],
            cwd=ROOT,
            check=True
        )

        process = subprocess.run(
            [str(PROGRAM)],
            cwd=ROOT,
            capture_output=True,
            text=True,
            check=True
        )

        match = re.search(
            r"Execution time:\s*([\d.]+)\s*ms",
            process.stdout
        )

        if match is None:
            print("Error: execution time not found")
            print(process.stdout)
            sys.exit(1)

        times.append(float(match.group(1)))

    average = sum(times) / len(times)
    results.append((n, average))

    print(f"Size: {n}x{n}, average time: {average:.3f} ms")

with open(ROOT / "results.csv", "w", newline="") as file:
    writer = csv.writer(file)
    writer.writerow(["size", "average_time_ms"])
    writer.writerows(results)

sizes = [item[0] for item in results]
times = [item[1] for item in results]

plt.plot(sizes, times, marker="o")
plt.xlabel("Matrix size (N x N)")
plt.ylabel("Average execution time (ms)")
plt.title("Matrix multiplication performance")
plt.grid(True)
plt.tight_layout()
plt.savefig(ROOT / "performance.png", dpi=150)
plt.show()

print("Saved results.csv and performance.png")