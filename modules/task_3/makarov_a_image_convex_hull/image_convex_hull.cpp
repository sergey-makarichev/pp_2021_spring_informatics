// Copyright 2021 Makarov Alexander
#include "../../../modules/task_2/makarov_a_image_convex_hull/image_convex_hull.h"

#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"

#include <utility>
#include <stack>
#include <iostream>
#include <list>
#include <algorithm>

std::vector<int> mark_components(const std::vector<int>& bin_image,
                                 int w, int h) {
    int components_count = 1;  // plus background
    std::vector<int> result(bin_image);
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (result[i * w + j] == 0) {
                components_count++;
                std::pair<int, int> burn_point(j, i);
                result[i * w + j] = components_count;
                std::stack<std::pair<int, int> > burn_stack;
                burn_stack.push(burn_point);
                while (!burn_stack.empty()) {
                    std::pair<int, int> curr_point = burn_stack.top();
                    int x = curr_point.first;
                    int y = curr_point.second;
                    burn_stack.pop();
                    if (y > 0 &&
                        result[(y - 1) * w + x] == 0) {
                        result[(y - 1) * w + x] = components_count;
                        burn_stack.push(std::pair<int, int>(x, y - 1));
                    }
                    if (y + 1 < h &&
                        result[(y + 1) * w + x] == 0) {
                        result[(y + 1) * w + x] = components_count;
                        burn_stack.push(std::pair<int, int>(x, y + 1));
                    }
                    if (x > 0 &&
                        result[y * w + x - 1] == 0) {
                        result[y * w + x - 1] = components_count;
                        burn_stack.push(std::pair<int, int>(x - 1, y));
                    }
                    if (x + 1 < w &&
                        result[y * w + x + 1] == 0) {
                        result[y * w + x + 1] = components_count;
                        burn_stack.push(std::pair<int, int>(x + 1, y));
                    }
                }
            }
        }
    }
    return result;
}

int orientation(std::pair<int, int> c, std::pair<int, int> a,
                std::pair<int, int> b) {
    int vec_mlp = (a.first - c.first) * (b.second - c.second) -
                  (a.second - c.second) * (b.first - c.first);
    if (vec_mlp == 0) return 0;  // colinear
    return (vec_mlp > 0) ? 1: 2;
    // 1 - b on left side ca, 2 - b on rigth side ca
}

class JarvisAlgorithm {
    const std::vector<std::list<std::pair<int, int> > > &components;
    std::vector<std::list<std::pair<int, int> > >& result;
public:
    JarvisAlgorithm(const std::vector<std::list<std::pair<int, int> > > &i_components,
        std::vector<std::list<std::pair<int, int> > >& i_result): components(i_components), result(i_result) {}
        
    void operator() (const tbb::blocked_range<int>& r) const {
        int begin = r.begin(), end = r.end();
        for (int comp_num = begin; comp_num != end; comp_num++) {
            std::list<std::pair<int, int> > component_list = components[comp_num];
            if (component_list.size() < 3) {
                result[comp_num] = component_list;
            } else {
                std::pair<int, int> start(component_list.front());
                int start_idx = 0;
                int n = component_list.size();
                std::vector<std::pair<int, int> > component(n);
                int counter_1 = 0;
                for (auto point : component_list) {
                    component[counter_1] = point;
                    if (point.first < start.first) {
                        start = point;
                        start_idx = counter_1;
                    }
                    counter_1++;
                }
                int curr = start_idx;
                int next;
                do {
                    result[comp_num].push_back(component[curr]);
                    next = (curr + 1) % n;
                    for (int i = 0; i < n; i++) {
                        int orient = orientation(component[curr], component[next],
                                                                     component[i]);
                        if (orient == 1)
                            next = i;
                    }
                    curr = next;
                } while (curr != start_idx);
            }
        }
    }
};

std::vector<std::list <std::pair<int, int> > > get_convex_hulls(
                          const std::vector<int>& marked_image, int w, int h) {
    int comp_count = *std::max_element(marked_image.begin(),
                                       marked_image.end()) - 1;
    std::vector<std::list<std::pair<int, int> > > components(comp_count);
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (marked_image[i * w + j] == 1) {
                continue;
            } else {
                std::pair<int, int> point(j, i);
                components[marked_image[i * w + j] - 2].push_back(point);
            }
        }
    }
    std::vector<std::list<std::pair<int, int> > > result(comp_count);

    tbb::parallel_for(tbb::blocked_range<int>(0, static_cast<int>(components.size())), 
        JarvisAlgorithm(components, result));
        
    /*for (int comp_num = 0; comp_num < static_cast<int>(components.size());
                                                              comp_num++) {
        std::list<std::pair<int, int> > component_list =
                                                components[comp_num];
        if (component_list.size() < 3) {
            result[comp_num] = component_list;
        } else {
            std::pair<int, int> start(w, h);
            int start_idx = 0;
            int n = component_list.size();
            std::vector<std::pair<int, int> > component(n);
            int counter_1 = 0;
            for (auto point : component_list) {
                component[counter_1] = point;
                if (point.first < start.first) {
                    start = point;
                    start_idx = counter_1;
                }
                counter_1++;
            }
            int curr = start_idx;
            int next;
            do {
                result[comp_num].push_back(component[curr]);
                next = (curr + 1) % n;
                std::vector<int> next_array(threads_count, next);
                #pragma omp parallel private(thread_num)
                {
                    thread_num = omp_get_thread_num();
                    #pragma omp for
                    for (int i = 0; i < n; i++) {
                        int orient = orientation(component[curr],
                          component[next_array[thread_num]], component[i]);
                        if (orient == 1)
                            next_array[thread_num] = i;
                    }
                }
                next = next_array[0];
                for (int i = 0; i < static_cast<int>(next_array.size());
                                                                     i++) {
                    int orient = orientation(component[curr],
                                component[next], component[next_array[i]]);
                    if (orient == 1)
                        next = next_array[i];
                }
                curr = next;
            } while (curr != start_idx);
        }
    }*/
    
    return result;
}

std::vector<std::list <std::pair<int, int> > > get_convex_hulls_seq(
                          const std::vector<int>& marked_image, int w, int h) {
    int comp_count = *std::max_element(marked_image.begin(),
                                       marked_image.end()) - 1;
    std::vector<std::list<std::pair<int, int> > > components(comp_count);
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (marked_image[i * w + j] == 1) {
                continue;
            } else {
                std::pair<int, int> point(j, i);
                components[marked_image[i * w + j] - 2].push_back(point);
            }
        }
    }
    std::vector<std::list<std::pair<int, int> > > result(comp_count);
    for (int comp_num = 0; comp_num < static_cast<int>(components.size());
                                                                  comp_num++) {
        std::list<std::pair<int, int> > component_list = components[comp_num];
        if (component_list.size() < 3) {
            result[comp_num] = component_list;
        } else {
            std::pair<int, int> start(w, h);
            int start_idx = 0;
            int n = component_list.size();
            std::vector<std::pair<int, int> > component(n);
            int counter_1 = 0;
            for (auto point : component_list) {
                component[counter_1] = point;
                if (point.first < start.first) {
                    start = point;
                    start_idx = counter_1;
                }
                counter_1++;
            }
            int curr = start_idx;
            int next;
            do {
                result[comp_num].push_back(component[curr]);
                next = (curr + 1) % n;
                for (int i = 0; i < n; i++) {
                    int orient = orientation(component[curr], component[next],
                                                                 component[i]);
                    if (orient == 1)
                        next = i;
                }
                curr = next;
            } while (curr != start_idx);
        }
    }
    return result;
}
