// Copyright 2021 Shagov Maksim
#include <gtest/gtest.h>
#include <vector>
#include "../../../modules/task_4/shagov_m_fox/fox.h"

#include <omp.h>

TEST(Shagov_Maksim_Tbb, Test_4_on_4_Random_Matrix_Mult) {
     Matrix A = {1, 2, 3, 4,
                1, 2, 3, 4,
                1, 2, 3, 4,
                1, 2, 3, 4};
    Matrix B = {4, 3, 2, 1,
                4, 3, 2, 1,
                4, 3, 2, 1,
                4, 3, 2, 1};
    Matrix C = parallelBlockMatrixMultiplication(A, B, 4 * 4, 4);
    Matrix C_my_result = {40, 30, 20, 10,
                          40, 30, 20, 10,
                          40, 30, 20, 10,
                          40, 30, 20, 10};
    ASSERT_EQ(C_my_result, C);
}

TEST(Shagov_Maksim_Tbb, same_result_as_sequential) {
    size_t size = 1000;
    Matrix A = createRandomMatrix(size * size);
    Matrix B = createRandomMatrix(size * size);
    std::pair<double, double> time = {0, 0};
    time.first = omp_get_wtime();
    Matrix C = parallelBlockMatrixMultiplication(A, B, size * size, 1);
    time.second = omp_get_wtime();
    std::cout << "Sequential " << (time.second - time.first) << ' '
              << std::endl;
    time.first = omp_get_wtime();
    Matrix C_1 = parallelBlockMatrixMultiplication(A, B, size * size, 4);
    time.second = omp_get_wtime();
    std::cout << "Parallel " << (time.second - time.first) << ' '
              << std::endl;
    ASSERT_TRUE(isEqualMatrix(C, C_1));
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
