// Copyright 2021 Makarov Alexander

#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include <random>

#include "tbb/tick_count.h"
#include "./image_convex_hull.h"

const int prm_size = 15;
std::vector<std::vector<int> > primitives = {
    {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    },
    {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    },
    {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1,
        1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1,
        1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1,
        1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1,
        1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1,
        1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1,
        1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1,
        1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1,
        1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1,
        1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1,
        1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    },
    {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
        1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
        1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
        1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
        1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    },
    {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1,
        1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1,
        1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1,
        1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1,
        1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1,
        1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1,
        1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1,
        1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1,
        1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    },
};

std::vector<std::list <std::pair<int, int> > > primitives_convex_hulls = {
    {
        {1, 6}, {1, 7}, {1, 8}, {6, 13}, {7, 13}, {8, 13}, {13, 8}, {13, 6},
        {8, 1}, {6, 1}
    },
    {
        {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6}, {1, 7}, {1, 8}, {1, 9},
        {1, 10}, {1, 11}, {1, 12}, {1, 13}, {2, 13}, {3, 13}, {4, 13}, {5, 13},
        {6, 13}, {7, 13}, {8, 13}, {9, 13}, {10, 13}, {11, 13}, {12, 13},
        {13, 13}
    },
    {
        {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6}, {1, 7}, {1, 8}, {1, 9},
        {1, 10}, {1, 11}, {1, 12}, {1, 13}, {2, 13}, {3, 13}, {4, 13}, {5, 13},
        {6, 13}, {7, 13}, {8, 13}, {9, 13}, {10, 13}, {11, 13}, {12, 13},
        {13, 13}, {13, 1}
    },
    {
        {1, 4}, {1, 5}, {1, 6}, {1, 7}, {1, 8}, {1, 9}, {1, 10}, {2, 11},
        {3, 12}, {4, 13}, {5, 13}, {6, 13}, {7, 13}, {8, 13}, {9, 13},
        {10, 13}, {13, 10}, {13, 4}, {10, 1}, {4, 1}, {3, 2}, {2, 3}
    },
    {
        {1, 1}, {1, 2}, {1, 12}, {1, 13}, {2, 13}, {12, 13}, {13, 13}, {13, 1}
    },
};

TEST(Components, Test_Vert_Cross) {
    const std::vector<int> image = primitives[0];
    int h = 15, w = 15;
    std::vector<int> marked_image = mark_components(image, w, h);
    /*for (int i = 0; i < h; i++){
        for (int j = 0; j < w; j++)
            std::cout << marked_image[i * w + j] << " ";
        std::cout << std::endl;
    }*/

    std::vector<std::list <std::pair<int, int> > > convex_hulls =
                             get_convex_hulls(marked_image, w, h);
    /*for (int i = 0; i < convex_hulls.size(); i++) {
        std::cout << i + 2 << ": ";
        for (auto point : convex_hulls[i]) {
            std::cout << "(" << point.first << ";" << point.second << ") ";
        }
        std::cout << std::endl;
    }*/
    std::vector<std::list <std::pair<int, int> > > expected_hulls;
    expected_hulls.push_back(primitives_convex_hulls[0]);
    ASSERT_EQ(convex_hulls, expected_hulls);
}

TEST(Components, Test_Triangle) {
    const std::vector<int> image = primitives[1];
    int h = 15, w = 15;
    std::vector<int> marked_image = mark_components(image, w, h);
    std::vector<std::list <std::pair<int, int> > > convex_hulls =
                             get_convex_hulls(marked_image, w, h);
    std::vector<std::list <std::pair<int, int> > > expected_hulls;
    expected_hulls.push_back(primitives_convex_hulls[1]);
    ASSERT_EQ(convex_hulls, expected_hulls);
}

TEST(Components, Test_Perimeter) {
    const std::vector<int> image = primitives[2];
    int h = 15, w = 15;
    std::vector<int> marked_image = mark_components(image, w, h);
    std::vector<std::list <std::pair<int, int> > > convex_hulls =
                             get_convex_hulls(marked_image, w, h);
    std::vector<std::list <std::pair<int, int> > > expected_hulls;
    expected_hulls.push_back(primitives_convex_hulls[2]);
    ASSERT_EQ(convex_hulls, expected_hulls);
}

TEST(Components, Test_Sqr_Without_Angles) {
    const std::vector<int> image = primitives[3];
    int h = 15, w = 15;
    std::vector<int> marked_image = mark_components(image, w, h);
    std::vector<std::list <std::pair<int, int> > > convex_hulls =
                             get_convex_hulls(marked_image, w, h);
    std::vector<std::list <std::pair<int, int> > > expected_hulls;
    expected_hulls.push_back(primitives_convex_hulls[3]);
    ASSERT_EQ(convex_hulls, expected_hulls);
}

TEST(Components, Test_Diag_Cross) {
    const std::vector<int> image = primitives[4];
    int h = 15, w = 15;
    std::vector<int> marked_image = mark_components(image, w, h);
    std::vector<std::list <std::pair<int, int> > > convex_hulls =
                            get_convex_hulls(marked_image, w, h);
    std::vector<std::list <std::pair<int, int> > > expected_hulls;
    expected_hulls.push_back(primitives_convex_hulls[4]);
    ASSERT_EQ(convex_hulls, expected_hulls);
}

TEST(Components, Test_100x100_prim_image) {
    int h = 100, w = 100;
    int size = w * h;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<int> image(size * prm_size * prm_size);
    std::vector<std::list <std::pair<int, int> > > expected_hulls(size);
    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++) {
            int prm_num = static_cast<int>(gen() % primitives.size());
            expected_hulls[i * w + j] = primitives_convex_hulls[prm_num];
            for (std::pair<int, int>& point : expected_hulls[i * w + j]) {
                point.first += j * prm_size;
                point.second += i * prm_size;
            }
            for (int k = 0; k < prm_size; k++)
                for (int q = 0; q < prm_size; q++) {
                    int idx = (i * w * prm_size + j) * prm_size +
                           k * w * prm_size + q;
                    image[idx] = primitives[prm_num][k * prm_size + q];
                }
        }
    std::vector<int> marked_image = mark_components(image, w * prm_size,
                                                           h * prm_size);
    std::vector<std::list <std::pair<int, int> > > convex_hulls_seq,
                                                   convex_hulls_par;

    tbb::tick_count start_time, end_time;
    tbb::tick_count::interval_t seq_time, par_time;

    start_time = tbb::tick_count::now();
    convex_hulls_seq = get_convex_hulls_seq(marked_image, w * prm_size,
                                                          h * prm_size);
    end_time = tbb::tick_count::now();
    seq_time = end_time - start_time;

    start_time = tbb::tick_count::now();
    convex_hulls_par = get_convex_hulls(marked_image, w * prm_size,
                                                      h * prm_size);
    end_time = tbb::tick_count::now();
    par_time = end_time - start_time;

    std::cout << "Seq time: " << seq_time.seconds() << " s" << std::endl;
    std::cout << "Par time: " << par_time.seconds() << " s" << std::endl;

    ASSERT_EQ(convex_hulls_par, convex_hulls_seq);
    ASSERT_EQ(convex_hulls_par, expected_hulls);
}


TEST(Components, Test_200x200_prim_image) {
    int h = 200, w = 200;
    int size = w * h;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<int> image(size * prm_size * prm_size);
    std::vector<std::list <std::pair<int, int> > > expected_hulls(size);
    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++) {
            int prm_num = static_cast<int>(gen() % primitives.size());
            expected_hulls[i * w + j] = primitives_convex_hulls[prm_num];
            for (std::pair<int, int>& point : expected_hulls[i * w + j]) {
                point.first += j * prm_size;
                point.second += i * prm_size;
            }
            for (int k = 0; k < prm_size; k++)
                for (int q = 0; q < prm_size; q++) {
                    int idx = (i * w * prm_size + j) * prm_size +
                           k * w * prm_size + q;
                    image[idx] = primitives[prm_num][k * prm_size + q];
                }
        }
    std::vector<int> marked_image = mark_components(image, w * prm_size,
                                                           h * prm_size);
    std::vector<std::list <std::pair<int, int> > > convex_hulls_seq,
                                                   convex_hulls_par;

    tbb::tick_count start_time, end_time;
    tbb::tick_count::interval_t seq_time, par_time;

    start_time = tbb::tick_count::now();
    convex_hulls_seq = get_convex_hulls_seq(marked_image, w * prm_size,
                                                          h * prm_size);
    end_time = tbb::tick_count::now();
    seq_time = end_time - start_time;

    start_time = tbb::tick_count::now();
    convex_hulls_par = get_convex_hulls(marked_image, w * prm_size,
                                                      h * prm_size);
    end_time = tbb::tick_count::now();
    par_time = end_time - start_time;

    std::cout << "Seq time: " << seq_time.seconds() << " s" << std::endl;
    std::cout << "Par time: " << par_time.seconds() << " s" << std::endl;

    ASSERT_EQ(convex_hulls_par, convex_hulls_seq);
    ASSERT_EQ(convex_hulls_par, expected_hulls);
}

TEST(Components, Test_700x700_worst) {
    int h = 700, w = 700;
    int size = w * h;
    std::vector<int> image(size, 0);
    std::vector<std::list <std::pair<int, int> > > expected_hulls(1);
    for (int i = 0; i < h; i++) {
        expected_hulls[0].push_back(std::pair<int, int>(0, i));
    }
    for (int i = 1; i < w; i++) {
        expected_hulls[0].push_back(std::pair<int, int>(i, h - 1));
    }
    expected_hulls[0].push_back(std::pair<int, int>(w - 1, 0));
    std::vector<int> marked_image = mark_components(image, w, h);
    std::vector<std::list <std::pair<int, int> > > convex_hulls_seq,
                                                   convex_hulls_par;

    tbb::tick_count start_time, end_time;
    tbb::tick_count::interval_t seq_time, par_time;

    start_time = tbb::tick_count::now();
    convex_hulls_seq = get_convex_hulls_seq(marked_image, w, h);
    end_time = tbb::tick_count::now();
    seq_time = end_time - start_time;

    start_time = tbb::tick_count::now();
    convex_hulls_par = get_convex_hulls(marked_image, w, h);
    end_time = tbb::tick_count::now();
    par_time = end_time - start_time;

    std::cout << "Seq time: " << seq_time.seconds() << " s" << std::endl;
    std::cout << "Par time: " << par_time.seconds() << " s" << std::endl;

    ASSERT_EQ(convex_hulls_par, convex_hulls_seq);
    ASSERT_EQ(convex_hulls_par, expected_hulls);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
