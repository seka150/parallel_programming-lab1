#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <string>

int main()
{
    std::ifstream inA("A.txt");
    std::ifstream inB("B.txt");

    if (!inA.is_open() || !inB.is_open())
    {
        std::cerr << "Error: cannot open input files\n";
        return 1;
    }

    int nA, nB;

    if (!(inA >> nA) || !(inB >> nB))
    {
        std::cerr << "Error: cannot read matrix sizes\n";
        return 1;
    }

    if (nA <= 0 || nB <= 0 || nA != nB)
    {
        std::cerr << "Error: invalid matrix sizes\n";
        return 1;
    }

    int n = nA;

    std::vector<int> A(n * n);
    std::vector<int> B(n * n);
    std::vector<long long> C(n * n, 0);

    for (int i = 0; i < n * n; ++i)
    {
        if (!(inA >> A[i]) || !(inB >> B[i]))
        {
            std::cerr << "Error: cannot read matrix elements\n";
            return 1;
        }
    }

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            for (int k = 0; k < n; ++k)
            {
                C[i * n + j] +=
                    static_cast<long long>(A[i * n + k]) * B[k * n + j];
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();

    double elapsed = std::chrono::duration<double, std::milli>(
                         end - start)
                         .count();

    std::ofstream out("C.txt");

    if (!out.is_open())
    {
        std::cerr << "Error: cannot open output file\n";
        return 1;
    }

    out << n << '\n';

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            out << C[i * n + j] << ' ';
        }
        out << '\n';
    }

    std::cout << "Matrix size: " << n << "x" << n << '\n';
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Execution time: " << elapsed << " ms\n";

    return 0;
}