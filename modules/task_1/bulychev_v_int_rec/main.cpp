// Copyright 2021 Bulychev Vladislav
#include <gtest/gtest.h>
#include <cmath>
#include <vector>
#include "./int_rec.h"

double f(std::vector<double> t) {
    double x = t[0];
    double y = t[1];
    return x * x * x - y * y;
}

double f1(std::vector<double> t) {
    double x = t[0];
    double y = t[1];
    double z = t[2];
    return (z / (3 * x * x + y * y));
}

double f2(std::vector<double> t) {
    double x = t[0];
    return x * x - sin(x);
}

double f3(std::vector<double> t) {
    double x = t[0];
    double y = t[1];
    return x + y + 1;
}

double f4(std::vector<double> t) {
    double x = t[0];
    double y = t[1];
    double z = t[2];
    return x * x - 2 * y - cos(z) + 2;
}

TEST(Integrate_rectangle, Test_int_rec_1) {
    int s = 2;
    std::vector<double> a(s);
    std::vector<double> b(s);

    a[0] = 5;
    b[0] = 17;
    a[1] = 21;
    b[1] = 53;

    double ans = 104704;

    double ans1 = Calculation(a, b, 50, f);

    ASSERT_NEAR(ans, ans1, 100);
}

TEST(Integrate_rectangle, Test_int_rec_2) {
    int s = 3;
    std::vector<double> a(s);
    std::vector<double> b(s);

    a[0] = 4;
    b[0] = 10;
    a[1] = 15;
    b[1] = 20;
    a[2] = 1;
    b[2] = 10;

    double ans = 3.3218;

    double ans1 = Calculation(a, b, 10, f1);

    ASSERT_NEAR(ans, ans1, 100);
}

TEST(Integrate_rectangle, Test_int_rec_3) {
    int s = 1;
    std::vector<double> a(s);
    std::vector<double> b(s);

    a[0] = -32;
    b[0] = 134;

    double ans = 812956.0353;

    double ans1 = Calculation(a, b, 50, f2);

    ASSERT_NEAR(ans, ans1, 200);
}

TEST(Integrate_rectangle, Test_int_rec_4) {
    int s = 2;
    std::vector<double> a(s);
    std::vector<double> b(s);

    a[0] = 5;
    b[0] = 14;
    a[1] = 3;
    b[1] = 21;

    ASSERT_NO_THROW(Calculation(a, b, 5, f3));
}

TEST(Integrate_rectangle, Test_int_rec_5) {
    int s = 3;
    std::vector<double> a(s);
    std::vector<double> b(s);

    a[0] = 15;
    b[0] = 25;
    a[1] = 30;
    b[1] = 45;
    a[2] = -1;
    b[2] = 0.5;

    double ans = 75251.8655;

    double ans1 = Calculation(a, b, 10, f4);

    ASSERT_NEAR(ans, ans1, 2000);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
