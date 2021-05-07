// Copyright 2021 Luckianchencko Ivan
#include<cmath>
#include<ctime>
#include<vector>
#include<random>
#include "../../../modules/task_3/luckianchencko_i_obolochka/obolochka.h"

mat get_rand_mat(int size) {
    if (size <= 0)
        throw std::invalid_argument("Error size of matrix");
    std::mt19937 gen;
    gen.seed(static_cast<int>(time(0)));
    mat res(size, std::vector<int>(size));
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            res[i][j] = gen() % 2;
        }
    }
    return res;
}
double distance(point a, point b) {
    double res = sqrt(pow(b.x-a.x, 2) + pow(b.y-a.y, 2));
    return res;
}
double cosAngle(point a, point b, point c) {
    double ab = distance(a, b);
    double bc = distance(b, c);
    if ( (ab > 0) && (bc > 0) ) {
        double cos = ((b.x-a.x) * (b.x -c.x) + (b.y - a.y) * (b.y - c.y))
        /(ab*bc);
        return cos;
    } else {
        return 10.0;
    }
}
std::vector<point> get_points(mat img, int size) {
    std::vector<point> res;
    point a;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (img[i][j] == 1) {
               a.x = i;
               a.y = j;
               res.push_back(a);
            }
        }
    }
    return res;
}
std::vector<point> Paral_Jarvis_alg(const mat &img, int size, int thread) {
    std::vector<point> a = get_points(img, size);
    std::vector<point> res;
    int count = a.size();
    int base = 0;
    for (int i = 0; i < count; ++i) {
        if (a[i].x > a[base].x)
            base = i;
        else if ((a[i].x == a[base].x) && (a[i].y < a[base].y))
            base = i;
    }
    res.push_back(a[base]);
    point first = a[base];
    point prev = first;
    prev.y -= 1;
    point cur = first;
    tbb::task_scheduler_init init(thread);
    while (1) {
        count = a.size();
        struct arg {
            double min = 2.0;
            double dist = 0.0;
            int num = -1;
        } glob;
        int grainsize = count / thread;
        glob = tbb::parallel_reduce
            (tbb::blocked_range<int>(0, count, grainsize),
            glob, [&](const tbb::blocked_range<int>& r, arg loc_arg) {
                for (int i = r.begin(); i < r.end(); i++) {
                    if (std::abs(cosAngle(prev, cur, a[i]) -
                        loc_arg.min) < 1e-5) {
                        double loc_curdist = distance(cur, a[i]);
                        if (loc_curdist > loc_arg.dist) {
                            loc_arg.min = cosAngle(prev, cur, a[i]);
                            loc_arg.dist = loc_curdist;
                            loc_arg.num = i;
                        }
                    } else if (cosAngle(prev, cur, a[i]) < loc_arg.min) {
                        loc_arg.min = cosAngle(prev, cur, a[i]);
                        loc_arg.dist = distance(cur, a[i]);
                        loc_arg.num = i;
                    }
                }
                return loc_arg;
            },
        [](arg a, arg b) {
            arg fail;
            if ( std::abs(a.min - b.min) < 1e-5 ) {
                if ( a.dist > b.dist ) {
                    return a;
                } else if ( b.dist > a.dist ) {
                    return b;
                }
            } else if ( a.min < b.min ) {
                return a;
            } else if ( b.min < a.min ) {
                return b;
            }
            return fail;
        });
        res.push_back(a[glob.num]);
        prev = cur;
        cur = a[glob.num];
        a.erase(a.begin()+glob.num);

        if ( (cur.x == first.x) && (cur.y == first.y) ) {
            break;
        }
    }
    return res;
}

std::vector<point> Jarvis_alg(const mat &img, int size) {
    std::vector<point> a = get_points(img, size);
    std::vector<point> res;
    int count = a.size();
    int base = 0;
    for (int i = 0; i < count; ++i) {
        if (a[i].x > a[base].x)
            base = i;
        else if ((a[i].x == a[base].x) && (a[i].y < a[base].y))
            base = i;
    }
    res.push_back(a[base]);
    point first = a[base];
    point prev = first;
    prev.y -= 1;
    point cur = first;
    int num = -1;
    while (1) {
        double min = 2;
        double dist = 0;
        count = a.size();
        for (int i = 0; i < count; i++) {
            if (std::abs(cosAngle(prev, cur, a[i]) - min) < 1e-5) {
                double curdist = distance(cur, a[i]);
                if (curdist > dist) {
                    min = cosAngle(prev, cur, a[i]);
                    dist = curdist;
                    num = i;
                }
            } else if (cosAngle(prev, cur, a[i]) < min) {
                min = cosAngle(prev, cur, a[i]);
                dist = distance(cur, a[i]);
                num = i;
            }
        }
        res.push_back(a[num]);
        prev = cur;
        cur = a[num];
        a.erase(a.begin()+num);
        if ((cur.x == first.x) && (cur.y == first.y)) {
            break;
        }
    }
    return res;
}
