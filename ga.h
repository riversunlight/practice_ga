
#include "knapsackProblem.h"
using namespace std;

class GA
{
public:
    KnapSackProblem problem;
    int **population;
    int *population_score;
    int **offspring;
    int *offspring_score;
    int population_size;
    int offspring_size;
    int generation_cnt;
    int max_generation = 100;

    GA(KnapSackProblem _problem)
    {
        problem = _problem;
        population_size = 20;
        population = (int **)malloc(sizeof(int *) * population_size);
        population_score = (int *)malloc(sizeof(int) * population_size);
    }

    void init()
    {
        for (int i = 0; i < population_size; i++)
        {
            population[i] = (int *)malloc(sizeof(int) * problem.n);

            for (int j = 0; j < problem.n; j++)
                population[i][j] = rand() % 2;
            int constraint = 0;

            int *individual_hypo;
            individual_hypo = (int *)malloc(sizeof(int) * problem.n);
            decoder(population[i], individual_hypo);
            population_score[i] = problem.ObjectiveFunc(individual_hypo, constraint);
            if (constraint)
                population_score[i] = 0;
        }
    }

    void recombine(int p1, int p2)
    {
        int r = rand() % problem.n;

        offspring = (int **)malloc(sizeof(int *) * 2);
        for (int i = 0; i < 2; i++)
        {
            offspring[i] = (int *)malloc(sizeof(int) * problem.n);
        }
        cout << "p1 and p2 genotype:" << endl;
        for (int i = 0; i < problem.n; i++)
            cout << population[p1][i] << " ";
        cout << endl;
        for (int i = 0; i < problem.n; i++)
            cout << population[p2][i] << " ";
        cout << endl;

        for (int i = 0; i < problem.n; i++)
        {
            if (i < r)
            {
                offspring[0][i] = population[p1][i];
                offspring[1][i] = population[p2][i];
            }
            else
            {
                offspring[0][i] = population[p2][i];
                offspring[1][i] = population[p1][i];
            }
        }

        cout << "offsprings: r=" << r << endl;
        for (int i = 0; i < problem.n; i++)
            cout << offspring[0][i] << " ";
        cout << endl;
        for (int i = 0; i < problem.n; i++)
            cout << offspring[1][i] << " ";
        cout << endl;
    }

    // stochastic change offspring
    void mutate()
    {
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < problem.n; j++)
            {
                int r = rand() % 100;
                if (r < 1)
                    offspring[i][j] ^= 1;
            }
        }
    }

    // stochastic
    void select_parents(int &p1, int &p2)
    {
        for (int i = 0; i < 2; i++)
        {
            int *histogram = (int *)malloc(sizeof(int) * (population_size + 1));
            histogram[0] = 0;
            for (int j = 0; j < population_size; j++)
            {
                if (j == p1)
                    continue;
                if (j == p1 + 1)
                {
                    histogram[j + 1] = histogram[j - 1] + population_score[j] + 1;
                }
                histogram[j + 1] = histogram[j] + population_score[j] + 1;
            }
            int sum = histogram[population_size];
            if (histogram[population_size] == 0)
                sum = histogram[population_size - 1];
            cout << "sum:" << sum << endl;
            int r = rand() % sum;
            for (int j = 0; j < population_size; j++)
            {
                if (j == p1)
                    continue;
                if (r < histogram[j + 1])
                {
                    if (i == 0)
                        p1 = j;
                    else
                    {
                        p2 = j;
                    }
                    break;
                }
            }
        }
    }

    void decoder(int genotype[], int phenotype[])
    {
        int c_sum = 0;
        for (int i = 0; i < problem.n; i++)
        {
            if (genotype[i] == 1 && c_sum + problem.c[i] <= problem.c_max)
            {
                phenotype[i] = 1;
                c_sum += problem.c[i];
            }
            else
            {
                phenotype[i] = 0;
            }
        }
    }

    void eval_offspring()
    {
        offspring_score = (int *)malloc(sizeof(int) * 2);
        for (int i = 0; i < 2; i++)
        {
            int constraint = 0;
            int *off_hypo;
            off_hypo = (int *)malloc(sizeof(int) * problem.n);
            decoder(offspring[i], off_hypo);
            offspring_score[i] = problem.ObjectiveFunc(off_hypo, constraint);
            if (constraint)
                offspring_score[i] = 0;
        }
    }

    // deterministic
    void select_next_generation()
    {
        cout << "ga:" << endl;
        int l1 = -1, l2 = -1; // worst individual of population
        for (int i = 0; i < population_size; i++)
        {
            int idx = i;
            if (l1 == -1)
                l1 = idx;
            else
            {
                if (population_score[idx] < population_score[l1])
                {
                    int tmp = l1;
                    l1 = idx;
                    idx = tmp;
                }
                if (l2 == -1)
                    l2 = idx;
                else if (population_score[idx] < population_score[l2])
                {
                    int tmp = l2;
                    l2 = idx;
                    idx = tmp;
                }
            }
        }
        cout << "l1, l2:" << l1 << "," << l2 << endl;
        cout << offspring_score[0] << " " << offspring_score[1] << endl;
        cout << population_score[l1] << " " << population_score[l2] << endl;
        int best_offspring = 0;
        if (offspring_score[0] > offspring_score[1])
        {
            best_offspring = 1;
        }
        if (offspring_score[best_offspring] > population_score[l2])
        {
            population[l1] = population[l2];
            population_score[l1] = population_score[l2];
            population[l2] = offspring[best_offspring];
            population_score[l2] = offspring_score[best_offspring];
            l1 = l2;
            best_offspring ^= 1;
        }
        if (offspring_score[best_offspring] > population_score[l1])
        {
            population[l1] = offspring[best_offspring];
            population_score[l1] = offspring_score[best_offspring];
        }
    }

    int termination_condition()
    {
        if (generation_cnt >= max_generation)
            return 1;
        return 0;
    }

    int solve(int *ans)
    {
        init();
        generation_cnt = 0;

        int best_idx = -1;
        int best_score = 0;
        int *score_log = (int *)malloc(sizeof(int) * (max_generation + 10));
        while (!termination_condition())
        {
            cout << "generation_cnt: " << generation_cnt << endl;
            int p1 = -1, p2 = -1;
            select_parents(p1, p2);

            cout << "p1: " << p1 << " " << p2 << endl;
            recombine(p1, p2);
            mutate();
            eval_offspring();
            select_next_generation();
            best_idx = -1;
            best_score = -100000;

            for (int i = 0; i < population_size; i++)
            {
                if (population_score[i] > best_score)
                {
                    best_idx = i;
                    best_score = population_score[i];
                }
            }

            cout << "best_score:" << best_score << endl;
            for (int i = 0; i < problem.n; i++)
            {
                cout << population[best_idx][i] << " ";
            }
            cout << endl;
            score_log[generation_cnt] = best_score;
            generation_cnt++;
        }
        int *best_solution;
        best_solution = (int *)malloc(sizeof(int) * problem.n);
        decoder(population[best_idx], best_solution);
        for (int i = 0; i < problem.n; i++)
        {
            ans[i] = best_solution[i];
        }

        ofstream writing_file;

        writing_file.open("score_generation.txt");
        for (int i = 0; i < max_generation; i++)
        {
            writing_file << score_log[i] << endl;
        }
        writing_file.close();

        return best_score;
    }
};