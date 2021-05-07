// Copyright 2021 Luckianchencko Ivan
#include<gtest/gtest.h>
#include <tbb/tick_count.h>
#include<vector>
#include<iostream>
#include "./obolochka.h"

TEST(obolocha, test1) {
    int size = 100;
    mat a(size, std::vector<int>(size));
    a = get_rand_mat(size);
    /*for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            std::cout << a[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;*/
    tbb::tick_count t1, t2;
    t1 = tbb::tick_count::now();
    std::vector<point> res = Jarvis_alg(a, size);
    t2 = tbb::tick_count::now();
    std::cout << " Seq_Time = " << (t2 - t1).seconds() << std::endl;
    t1 = tbb::tick_count::now();
    std::vector<point> par_res = Paral_Jarvis_alg(a, size, 2);
    t2 = tbb::tick_count::now();
    std::cout << " Paral_Time = " << (t2 - t1).seconds() << std::endl;
    int count = res.size();
    for (int i = 0; i < count; i++) {
        std::cout << res[i].x << "," << res[i].y << std::endl;
        ASSERT_EQ(res[i].x, par_res[i].x);
        ASSERT_EQ(res[i].y, par_res[i].y);
    }
}
TEST(obolocha, test2) {
    int size = 1000;
    mat a(size, std::vector<int>(size));
    a = get_rand_mat(size);
    /*for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            std::cout << a[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;*/
    tbb::tick_count t1, t2;
    t1 = tbb::tick_count::now();
    std::vector<point> res = Jarvis_alg(a, size);
    t2 = tbb::tick_count::now();
    std::cout << " Seq_Time = " << (t2 - t1).seconds() << std::endl;
    t1 = tbb::tick_count::now();
    std::vector<point> par_res = Paral_Jarvis_alg(a, size, 2);
    t2 = tbb::tick_count::now();
    std::cout << " Paral_Time = " << (t2 - t1).seconds() << std::endl;
    int count = res.size();
    for (int i = 0; i < count; i++) {
        std::cout << res[i].x << "," << res[i].y << std::endl;
        ASSERT_EQ(res[i].x, par_res[i].x);
        ASSERT_EQ(res[i].y, par_res[i].y);
    }
}
TEST(obolocha, test3) {
    int size = 1000;
    mat a(size, std::vector<int>(size));
    a = get_rand_mat(size);
    /*for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            std::cout << a[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;*/
    tbb::tick_count t1, t2;
    t1 = tbb::tick_count::now();
    std::vector<point> res = Jarvis_alg(a, size);
    t2 = tbb::tick_count::now();
    std::cout << " Seq_Time = " << (t2 - t1).seconds() << std::endl;
    t1 = tbb::tick_count::now();
    std::vector<point> par_res = Paral_Jarvis_alg(a, size, 1);
    t2 = tbb::tick_count::now();
    std::cout << " Paral_Time = " << (t2 - t1).seconds() << std::endl;
    int count = res.size();
    for (int i = 0; i < count; i++) {
        std::cout << res[i].x << "," << res[i].y << std::endl;
        ASSERT_EQ(res[i].x, par_res[i].x);
        ASSERT_EQ(res[i].y, par_res[i].y);
    }
}
TEST(obolocha, test4) {
    int size = 1000;
    mat a(size, std::vector<int>(size));
    a = get_rand_mat(size);
    /*for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            std::cout << a[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;*/
    tbb::tick_count t1, t2;
    t1 = tbb::tick_count::now();
    std::vector<point> res = Jarvis_alg(a, size);
    t2 = tbb::tick_count::now();
    std::cout << " Seq_Time = " << (t2 - t1).seconds() << std::endl;
    t1 = tbb::tick_count::now();
    std::vector<point> par_res = Paral_Jarvis_alg(a, size, 4);
    t2 = tbb::tick_count::now();
    std::cout << " Paral_Time = " << (t2 - t1).seconds() << std::endl;
    int count = res.size();
    for (int i = 0; i < count; i++) {
        std::cout << res[i].x << "," << res[i].y << std::endl;
        ASSERT_EQ(res[i].x, par_res[i].x);
        ASSERT_EQ(res[i].y, par_res[i].y);
    }
}
TEST(obolocha, testsize) {
    int size = 2000;
    mat a(size, std::vector<int>(size));
    a = get_rand_mat(size);
    /*for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            std::cout << a[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;*/
    tbb::tick_count t1, t2;
    t1 = tbb::tick_count::now();
    std::vector<point> res = Jarvis_alg(a, size);
    t2 = tbb::tick_count::now();
    std::cout << " Seq_Time = " << (t2 - t1).seconds() << std::endl;
    t1 = tbb::tick_count::now();
    std::vector<point> par_res = Paral_Jarvis_alg(a, size, 4);
    t2 = tbb::tick_count::now();
    std::cout << " Paral_Time = " << (t2 - t1).seconds() << std::endl;
    int count = res.size();
    for (int i = 0; i < count; i++) {
        std::cout << res[i].x << "," << res[i].y << std::endl;
        ASSERT_EQ(res[i].x, par_res[i].x);
        ASSERT_EQ(res[i].y, par_res[i].y);
    }
}


