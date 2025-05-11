#pragma once
#include <time.h>
#include <fstream>
using namespace std;
class KnapSackProblem {
public:
    int n;
    int c_max;
    int *c;
    int *v;
    KnapSackProblem(){}
    KnapSackProblem(int _n) {
        srand((unsigned int)time(NULL));
        n = _n;
        c = (int*)malloc(n * sizeof(int));
        v = (int*)malloc(n * sizeof(int));
        for (int i = 0; i < n; i++) {
            c[i] = rand() % 100 + 1;
            v[i] = rand() % 100 + 1;
        }
        c_max = rand() % (n * 50);
    }

    int ObjectiveFunc(int *solution, int &constraint) {
        int c_sum = 0;
        int v_sum = 0;
        for (int i = 0; i < n; i++) {
            if (solution[i] == 1) {
                c_sum += c[i];
                v_sum += v[i];
            }
        }
        if (c_sum > c_max) constraint = 1;

        return v_sum;
    }

    void WriteInputFile() {
        ofstream writing_file;
        writing_file.open("input.txt");
        writing_file << n << " " << c_max << endl;
        for (int i = 0; i < n; i++) {
            writing_file << c[i] << " " << v[i] << endl;
        }

        writing_file.close();
    }
};