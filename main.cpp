#include <iostream>
#include <cstring>
#include "knapsackProblem.h"
#include "dp.h"
#include "ga.h"
using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("ilegal command line\n");
        return 1;
    }
    cout << atoi(argv[1]) << endl;

    KnapSackProblem problem(atoi(argv[1]));
    problem.WriteInputFile();
    int* ans;
    ans = (int*)malloc(sizeof(int) * problem.n);
    memset(ans, 0, sizeof(int) * problem.n);

    // DP
    ofstream writing_file;
    DP dp(problem);
    int score = dp.solve(ans);
    writing_file.open("ans.txt");
    cout << "score:" << score << endl;
    writing_file << "score:" << score << endl;
    for (int i = 0; i < problem.n; i++) {
        cout << ans[i] << " ";
        writing_file << ans[i] << " ";
    }
    writing_file << endl;
    cout << endl;
    writing_file.close();
    // GA
    GA ga(problem);
    score = ga.solve(ans);
    cout << "score:" << score << endl;
    for (int i = 0; i < problem.n; i++) {
        cout << ans[i] << " ";
    }
    cout << endl;
}