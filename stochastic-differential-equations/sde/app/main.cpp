#include "sde.hpp"
#include <iostream>
#include <fstream>
#include <util.hpp>

template<typename T>
void to_csv(const char* str, std::vector<T>& v) {
    std::ofstream fout(str, std::ios_base::app);

    for(T elem: v) {
        fout << elem << ",";
    }
    fout << "\n";
    fout.close();
}

template<typename T>
void to_csv_row(const char* str, std::vector<T>& v) {
    std::ofstream fout(str, std::ios_base::app);

    for(T elem: v) {
        fout << elem << "\n";
    }
    fout << "\n";
    fout.close();
}

void clean_file(const char* str) {
    std::ofstream fout(str, std::ios_base::trunc);
    fout.close();
}

// f = x'(t) | g = x(t)
void task_1() {
    int n = 100000;
    int seed = 777;
    double h = 0.01;
    double a = 0.5;
    double x_start = 0.0;
    double D = 1;
    int n_iter = 1;

    SDE<double> sde_euler(seed, x_start, h, n, a, n_iter, D);
    std::vector<double> x(n);
    std::vector<double> xt(n);
    std::vector<double> prob(n);
    sde_euler.heun(x.data(), xt.data(), prob.data());

    clean_file("x.csv");
    clean_file("xt.csv");

    to_csv_row("x.csv", x);
    to_csv_row("xt.csv", xt);

    printf("X(t):\n");
    for(double val : x)
        printf("%f ", val);
    printf("\n=======\n");

    printf("X'(t):\n");
    for(double val : xt)
        printf("%f ", val);
    printf("\n=======\n");
}

// f = P(t)
void task_2_1() {
    int n = 1000;
    int seed = 777;
    double h = 0.1;
    double a = 0.5;
    double x_start = 0.0;
    double D = 1;
    int n_iter = 1000;

    SDE<double> sde_euler(seed, x_start, h, n, a, n_iter, D);

    std::vector<double> x(n);
    std::vector<double> xt(n);
    std::vector<double> prob(n);

    sde_euler.heun(x.data(), xt.data(), prob.data());

    to_csv("prob.csv", prob);

    printf("P(t)\n");
    for(size_t i = 0; i < prob.size(); i++) {
        printf("%f ", prob[i]);
    }
    printf("\n=======\n");
}

// f = tao(t)
void task_2_2() {

    int n = 1000;
    int seed = 777;
    double h = 0.1;
    double a = 0.5;
    double x_start = 0.0;

    int n_iter = 1000;

    std::vector<double> v;
    double dc = 0;
    while(dc <= 5) {
        v.push_back(dc);
        dc += 0.01;
    }

    std::vector<double> taos;
    std::vector<double> Ds;

    printf("TAO:\n");
    for(double D : v) {
        std::vector<double> x(n);
        std::vector<double> xt(n);
        std::vector<double> prob(n);
        SDE<double> sde_euler(seed, x_start, h, n, a, n_iter, D);

        sde_euler.heun(x.data(), xt.data(), prob.data());

        double integral = 0;
        double step = h;
        int k = 0;
        while(k < prob.size() && prob[k] > 0.98) k++;

        for(int i = 0; i < k-1; i++) {
            double sum = (prob[i] + prob[i + 1]) / 2;
            integral += sum * step;
        }
        taos.push_back(integral);
        Ds.push_back(D);
        printf("%f ", integral);
    }

    to_csv("tao.csv", taos);
    to_csv("tao.csv", Ds);

    printf("\n=======\n");
}

void task_perf() {
    int n = 20;
    int seed = 777;
    double h = 0.1;
    double a = 0.5;
    double x_start = 0.0;
    double D = 1;
    int n_iter = 1000;


    std::vector<double> x(n);
    std::vector<double> xt(n);
    std::vector<double> prob(n);

    int threads[] = {1,2,3,4};
    for(int nt: threads) {
        SDE<double> sde(seed, x_start, h, n, a, n_iter, D, nt);
        omp_set_num_threads(nt);
        double t1 = omp_get_wtime();
        sde.heun(x.data(), xt.data(), prob.data());
        PrintArray(prob.data(), n);
        double t2 = omp_get_wtime();
        printf("%d threads = %f ms\n", nt, (t2-t1)*1000);
    }
}

int main() {
    // task_1();


    task_perf();

    // task_2_1();
    // task_2_2();
}