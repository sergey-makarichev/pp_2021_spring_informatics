// Copyright 2021 Molotkova Svetlana
#ifndef MODULES_TASK_4_MOLOTKOVA_S_TR_VERT_GAUSS_H_
#define MODULES_TASK_4_MOLOTKOVA_S_TR_VERT_GAUSS_H_

#include <vector>
#include <random>
#include <ctime>

int clamp(int value, int max, int min);
std::vector<double> matrixCreation(int rows, int cols);
std::vector<double> transpose(const std::vector<double>& matrix, int rows, int cols);
std::vector<double> gaussKernel(int radius, int sigma);
std::vector<double> gaussFilter(const std::vector<double>& matrix, int rows, int cols, int radius, int sigma);
std::vector<double> gaussFilter_par(const std::vector<double>& matrix, int rows, int cols, int radius, int sigma);
#endif  // MODULES_TASK_4_MOLOTKOVA_S_TR_VERT_GAUSS_H_
