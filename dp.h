#pragma once
#include "knapsackProblem.h"
using namespace std;
class DP
{
public:
    KnapSackProblem problem;
    DP(KnapSackProblem _problem)
    {
        problem = _problem;
    }

    // Return: fitness
    int solve(int ans[])
    {
        int **dp;
        dp = (int **)malloc(sizeof(int *) * (problem.n + 1));
        for (int i = 0; i < problem.n + 1; i++)
        {
            dp[i] = (int *)malloc(sizeof(int) * (problem.c_max + 1));
            for (int j = 0; j < problem.c_max + 1; j++)
            {
                dp[i][j] = 0;
            }
        }

        for (int i = 0; i < problem.n; i++)
        {
            for (int j = problem.c_max; j >= 0; j--)
            {
                if (dp[i][j] > dp[i + 1][j])
                {
                    dp[i + 1][j] = dp[i][j];
                }
                if (j + problem.c[i] <= problem.c_max)
                {
                    if (dp[i][j] + problem.v[i] > dp[i + 1][j + problem.c[i]])
                    {
                        dp[i + 1][j + problem.c[i]] = dp[i][j] + problem.v[i];
                    }
                }
            }
        }

        int tmp = problem.c_max;
        for (int i = problem.n - 1; i >= 0; i--)
        {
            if (tmp - problem.c[i] >= 0 && dp[i + 1][tmp] == dp[i][tmp - problem.c[i]] + problem.v[i])
            {
                ans[i] = 1;
                tmp -= problem.c[i];
            }
        }
        for (int i = 0; i < problem.n; i++)
        {
            for (int j = 0; j < problem.c_max; j++)
            {
                cout << dp[i][j] << " ";
            }
            cout << endl;
        }

        int constraint = 0;
        int eval = problem.ObjectiveFunc(ans, constraint);
        if (constraint)
        {
            cout << "DP's ans is ilegal." << endl;
        }
        return eval;
    }
};