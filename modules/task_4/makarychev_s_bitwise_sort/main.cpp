// Copyright 2021 Makarychev Sergey
#include <gtest/gtest.h>
#include <chrono>
#include <algorithm>
#include <vector>
#include "./bitwise_sort.h"

TEST(Sequential_Radix_Sort, sort_vector_of_random_value) {
    std::vector<int> vec1 = getRandomVector(78);
    std::vector<int> vec2 = vec1;
    std::sort(vec2.begin(), vec2.begin() + vec2.size());
    signedRadixSort(&vec1);
    ASSERT_EQ(vec1, vec2);
}

TEST(Omp_Radix_Sort, sort_vector_of_litle_positive_value) {
    std::vector<int> vec1 = {1, 3, 7, 4, 0, 7, 0, 9};
    std::vector<int> vec2 = vec1;
    std::sort(vec2.begin(), vec2.begin() + vec2.size());
    signedRadixSortStd(&vec1);
    ASSERT_EQ(vec1, vec2);
}

TEST(Omp_Radix_Sort, sort_vector_of_seq_omp) {
    std::vector<int> vec1 = { 1, 3, 7, 4, 0, 7, 0, 9 };
    std::vector<int> vec2 = vec1;
    signedRadixSortStd(&vec1);
    signedRadixSort(&vec2);
    ASSERT_EQ(vec1, vec2);
}

TEST(Omp_Radix_Sort, sort_vector_of_litle_negative_value) {
    std::vector<int> vec1 = { -1, -8, -4, -10, -1, -6, -6, -6 };
    std::vector<int> vec2 = vec1;
    signedRadixSortStd(&vec1);
    std::sort(vec2.begin(), vec2.begin() + vec2.size());
    ASSERT_EQ(vec1, vec2);
}

TEST(Omp_Radix_Sort, sort_vector_of_zero_value) {
    std::vector<int> vec1 = { 0, 0, 0, 0, 0, 0, 0, 0};
    std::vector<int> vec2 = vec1;
    signedRadixSortStd(&vec1);
    std::sort(vec2.begin(), vec2.begin() + vec2.size());
    ASSERT_EQ(vec1, vec2);
}

TEST(Omp_Radix_Sort, sort_vector_of_mixed_value) {
    std::vector<int> vec1 = { 2, -99947, -9, 87456, -987456, 0 };
    std::vector<int> vec2 = vec1;
    signedRadixSortStd(&vec1);
    std::sort(vec2.begin(), vec2.begin() + vec2.size());
    ASSERT_EQ(vec1, vec2);
}

TEST(Omp_Radix_Sort, sort_just_sorted_vector) {
    std::vector<int> vec1 = { -99999, -1, 0, 8, 9, 15, 10265, 235458 };
    std::vector<int> vec2 = vec1;
    signedRadixSortStd(&vec1);
    std::sort(vec2.begin(), vec2.begin() + vec2.size());
    ASSERT_EQ(vec1, vec2);
}

/*TEST(Omp_Radix_Sort, sort_vector_of_random_value_with_time) {
    auto time1 = std::chrono::duration<double>{};
    auto time2 = std::chrono::duration<double>{};
    std::vector<int> vec1 = getRandomVector(15000000);
    std::vector<int> vec2 = vec1;
    std::cout << std::endl;
    auto const start1 = std::chrono::high_resolution_clock::now();
    signedRadixSortStd(&vec1);
    auto const finish1 = std::chrono::high_resolution_clock::now();
    time1 += std::chrono::duration_cast<std::chrono::duration<double>>(finish1 - start1);
    std::cout << "Par: " << time1.count() << std::endl;
    auto const start2 = std::chrono::high_resolution_clock::now();
    signedRadixSort(&vec2);
    auto const finish2 = std::chrono::high_resolution_clock::now();
    time2 += std::chrono::duration_cast<std::chrono::duration<double>>(finish2 - start2);
    std::cout << "Seq: " << time2.count() << std::endl;
    std::cout << "Acceleration: " << time2.count() / time1.count() << std::endl;
    ASSERT_EQ(vec1, vec2);
}*/
