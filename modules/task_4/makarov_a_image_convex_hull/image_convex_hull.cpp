// Copyright 2021 Makarov Alexander
#include "../../../modules/task_4/makarov_a_image_convex_hull/image_convex_hull.h"

#include <utility>
#include <stack>
#include <iostream>
#include <list>
#include <algorithm>
#include <thread>

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

void jarvis_algorithm(const std::list<std::pair<int, int> >* components, const int chunk_size, std::list<std::pair<int, int> >* const result) {
    for (int comp_num = 0; comp_num < chunk_size; comp_num++) {
        std::list<std::pair<int, int> > component_list = components[comp_num];
        if (component_list.size() < 3) {
            result[comp_num] = component_list;
        } else {
            std::pair<int, int> start(component_list.front());
            int start_idx = 0;
            int n = component_list.size();
            std::vector<std::pair<int, int> > component(n);
            std::pair<int, int>* comp_ptr = component.data();
            int counter_1 = 0;
            for (auto point : component_list) {
                comp_ptr[counter_1] = point;
                if (point.first < start.first) {
                    start = point;
                    start_idx = counter_1;
                }
                counter_1++;
            }
            int curr = start_idx;
            int next;
            do {
                std::pair<int, int> curr_point = comp_ptr[curr];
                result[comp_num].push_back(comp_ptr[curr]);
                next = (curr + 1) % n;
                std::pair<int, int> next_point = comp_ptr[next];
                for (int i = 0; i < n; i++) {
                    int orient = orientation(curr_point, next_point,
                                                                 comp_ptr[i]);
                    if (orient == 1) {
                        next_point = comp_ptr[i];
                        next = i;
                    }
                }
                curr = next;
            } while (curr != start_idx);
        }
    } 
}

void find_left(const std::pair<int, int>* component, std::pair<int, int> curr_point, int start, int end, int * const result) {
    int next = *result;
    std::pair<int, int> next_point(component[next]);
    for (int i = start; i < end; i++) {
        int orient = orientation(curr_point, next_point,
                                                     component[i]);
        if (orient == 1) {
            next_point = component[i];
            next = i;
        }
    }
    *result = next;
}

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

    int cores_count = std::thread::hardware_concurrency();
    int components_size = static_cast<int>(components.size());
    
    if (cores_count == 1) {
        jarvis_algorithm(components.data(), components_size, result.data());
    } else if (cores_count < components_size) {
        int sub_chunk_size = components_size / (cores_count - 1);
        int main_chunk_size = components_size % (cores_count - 1);
        std::vector<std::thread> thrds(cores_count - 1);
        for (int i = 0; i < cores_count - 1; i++) {
            thrds[i] = std::thread(jarvis_algorithm, components.data() + i * sub_chunk_size, sub_chunk_size, result.data() + i * sub_chunk_size);
        }
        jarvis_algorithm(components.data() + (cores_count - 1) * sub_chunk_size, main_chunk_size, result.data() + (cores_count - 1) * sub_chunk_size);
        for (int i = 0; i < cores_count - 1; i++) {
            thrds[i].join();
        }
    } else {
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
                int sub_chunk_size = n / (cores_count - 1);
                int main_chunk_size = n % (cores_count - 1);
                do {
                    std::pair<int, int> curr_point(component[curr]);
                    result[comp_num].push_back(curr_point);
                    next = (curr + 1) % n;
                    std::vector<int> next_array(cores_count, next);
                    /*std::vector<std::thread> thrds(cores_count - 1);
                    for (int i = 0; i < cores_count; i++) {
                        thrds[i] = std::thread(find_left, component.data(), curr_point, i * );
                    }*/
                    find_left(component.data(), curr_point, 0, n, next_array.data() + (cores_count - 1));
                    /*for (int i = 0; i < cores_count; i++) {
                        thrds[i].join();
                    }*/
                    //next = next_array[cores_count - 1];
                    for (int i = 0; i < cores_count; i++) {
                        int orient = orientation(curr_point, component[next], component[next_array[i]]);
                        if (orient == 1) {
                            next = next_array[i];
                        }
                    }
                    curr = next;
                } while (curr != start_idx);
            }
        }
    }
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
            std::pair<int, int>* comp_ptr = component.data();
            int counter_1 = 0;
            for (auto point : component_list) {
                comp_ptr[counter_1] = point;
                if (point.first < start.first) {
                    start = point;
                    start_idx = counter_1;
                }
                counter_1++;
            }
            int curr = start_idx;
            int next;
            do {
                std::pair<int, int> curr_point(comp_ptr[curr]);
                result[comp_num].push_back(curr_point);
                next = (curr + 1) % n;
                std::pair<int, int> next_point(comp_ptr[next]);
                for (int i = 0; i < n; i++) {
                    int orient = orientation(curr_point, next_point,
                                                                 comp_ptr[i]);
                    if (orient == 1) {
                        next_point = comp_ptr[i];
                        next = i;
                    }
                }
                curr = next;
            } while (curr != start_idx);
        }
    }
    return result;
}
