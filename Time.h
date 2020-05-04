#pragma once

#include <omp.h>

class Time {
public:
    Time() {
        this->O1 = omp_get_wtime();
    }

    void start() {
        this->O1 = omp_get_wtime();
    }

    void stop() {
        this->O2 = omp_get_wtime();
    }

    double get() {
        return (double)(this->O2 - this->O1);
    }

private:
    double O1, O2;
};