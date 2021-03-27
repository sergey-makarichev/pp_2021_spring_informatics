// Copyright 2021 Solovev Aleksandr
#include "../../../modules/task_1/solovev_a_radix_sort/radix_sort.h"
#include <stdio.h>
#include <cstring>
#include <utility>
#include <iostream>

int generateRandomArray(std::vector<int> &buffer, double min, double max) {
    if ((buffer.size() <= 0) || (buffer.data() == nullptr))
        return -1;
    if (min >= max) {
        double temp = min;
        min = max;
        max = temp;
    }
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::uniform_real_distribution<double> buf(min, max);
    for (size_t i = 0; i < buffer.size(); i++) {
        buffer[i] = static_cast<int>(buf(gen));
    }
    return 0;
}

int MergeArrays(std::vector<int> &buffer1, std::vector<int> &buffer2,  std::vector<int> &result) {
    result.resize( buffer1.size() + buffer2.size() );
    result.clear();
    for (int i = 0; i < buffer1.size(); i++)
    {
        result.push_back(buffer1[i]);
    }
    for (int i = 0 ; i < buffer2.size(); i++)
    {
        result.push_back(buffer2[i]);
    }
    
    return 0;
}

int SortingCheck(std::vector<int> &buffer) {
    for (int i = 0; i < buffer.size() - 1; i++) {
        if (buffer[i] <= buffer[i + 1]) {
            i++;
        } else {
            return -112;
        }
    }
    return 0;
}

void CountingSort(std::vector<int> &input, std::vector<int> &output, int valbyte) {
    unsigned char *buffer = (unsigned char *)input.data();
    int counter[256] = {0};
    size_t length_counter = 256;
    int value = 0;
    
    for (size_t i = 0; i < input.size(); i++) {
        counter[buffer[4 * i + valbyte]]++;
    }
    
    for (size_t j = 0; j < length_counter; j++) {
        int tmp = counter[j];
        counter[j] = value;
        value += tmp;
    }

    for (size_t i = 0; i < input.size(); i++) {
        output[counter[buffer[4 * i + valbyte]]++] = input[i];
    }
}

int RadixSortUnsigned(std::vector<int> &buffer) {
    if (buffer.size() == 0) {
        return -1;
    }
    if ((buffer.size() < 0) || (buffer.data() == nullptr)) {
        return -1;
    }
    std::vector<int> outbuf(buffer.size());
    if (outbuf.data() == nullptr) {
        return -1;
    }
    for (int i = 0; i < 2; i++) {
        CountingSort(buffer, outbuf, 2 * i);
        CountingSort(outbuf, buffer, 2 * i + 1);
    }
    return 0;
}

int RadixSort(std::vector<int> &buffer) {
    if (buffer.size() < 1)
        return -1;
    if (buffer.data() == nullptr)
        return -1;

    int positive_length = 0, negative_length = 0;
    int status = 0;
    
    for (size_t i = 0; i < buffer.size(); i++) {
        if (buffer[i] >= 0) {
            positive_length++;
        } else {
            negative_length++;
        }
    }
    std::vector<int> positive_numbers(positive_length);
    std::vector<int> negative_numbers(negative_length);
    positive_length = 0;
    negative_length = 0;
    for (int i = 0; i < buffer.size(); i++)
    {
        if (buffer[i] >= 0)
        {
            positive_numbers[positive_length] = buffer[i];
            positive_length++;
        } else {
            negative_numbers[negative_length] = buffer[i];
            negative_length++;
        }
    }
 
    status = RadixSortUnsigned(positive_numbers);
    status = RadixSortUnsigned(negative_numbers);

    status = MergeArrays(negative_numbers,  positive_numbers,  buffer);
    return status;
}
