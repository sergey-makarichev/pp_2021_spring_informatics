// Copyright 2021 Makarychev Sergey
#include <omp.h>
#include <random>
#include <algorithm>
#include <vector>
#include "../../../modules/task_2/makarychev_s_bitwise_sort/bitwise_sort.h"

std::vector<int> getRandomVector(int sizeVec) {
    if (sizeVec < 0)
        throw "Wrong vector size";
    std::vector<int> rVec(sizeVec);
    std::random_device rd;
    std::mt19937 mersenne(rd());
    std::uniform_int_distribution<> uid(-1000, 2000);
    for (int i = 0; i < sizeVec; i++)
        rVec[i] = uid(mersenne);
    return rVec;
}

void createCounters(int* sortVec, int* counters, int sizeVec) {
  unsigned char* bytePointer = (unsigned char*)sortVec;
  unsigned char* dataEnd = (unsigned char*)(sortVec + sizeVec);
  int s = static_cast<int>(sizeof(int));
  while (bytePointer != dataEnd) {
    for (int i = 0; i < s; i++)
      counters[256 * i + *bytePointer++]++;
  }
}

void mergeOrderVec(int* vec1, int size1,  int* vec2, int size2) {
    int* resVec = new int[size1 + size2];
    int i = 0, s = 0, j = 0;
    while (i < size1 && j < size2) {
        if (vec1[i] < vec2[j])
            resVec[s++] = vec1[i++];
        else
            resVec[s++] = vec2[j++];
    }
    while (i < size1)
        resVec[s++] = vec1[i++];
    while (j < size2)
        resVec[s++] = vec2[j++];
    i = s = 0;
    while (i < size1 + size2)
        vec1[i++] = resVec[s++];
    delete[] resVec;
}

void signedRadix(int byteNumber, int sizeVec, int* sourceVec,
int* destVec, int* count) {
  int sum = 0;
  int* countPointer;
  if (byteNumber == sizeof(int) - 1) {
    int numNegative = 0;
    for (int i = 128; i < 256; i++)
      numNegative += count[i];

    sum = numNegative;
    countPointer = count;
    int tmp;
    for (int i = 0; i < 128; ++i, ++countPointer) {
      tmp = *countPointer;
      *countPointer = sum;
      sum += tmp;
    }
    sum = 0;
    countPointer = count + 128;
    for (int i = 0; i < 128; ++i, ++countPointer) {
      tmp = *countPointer;
      *countPointer = sum;
      sum += tmp;
    }
  } else {
    countPointer = count;
    int tmp;
    for (int i = 256; i > 0; --i, ++countPointer) {
      tmp = *countPointer;
      *countPointer = sum;
      sum += tmp;
    }
  }

  unsigned char* bytePointer = (unsigned char*)sourceVec + byteNumber;
  int* sourceVecPointer = sourceVec;
  for (int i = sizeVec; i > 0; --i, bytePointer += sizeof(int), ++sourceVecPointer) {
    countPointer = count + *bytePointer;
    destVec[*countPointer] = *sourceVecPointer;
    ++(*countPointer);
  }
}

void signedRadixSort(int* sortVec, int sizeVec) {
  int* out = new int[sizeVec];
  std::vector<int> counters(sizeof(int) * 256);
  int* count;
  createCounters(sortVec, counters.data(), sizeVec);
  int s = static_cast<int>(sizeof(int));
  for (int i = 0; i < s; i++) {
    count = counters.data() + 256 * i;
    signedRadix(i, sizeVec, sortVec, out, count);
    std::swap(sortVec, out);
  }
  delete[] out;
}

void signedRadixSortOmp(int* sortVec, int sizeVec) {
    int sizePartVec = 0, flag = 0;
    int s = static_cast<int>(sizeof(int));
#pragma omp parallel shared(sortVec, sizeVec)
    {
        int sizePartVec = 0;
        int numberThreads = omp_get_num_threads();
        sizePartVec = sizeVec / numberThreads;
        int remainder = sizeVec % numberThreads;
        int threadId = omp_get_thread_num();

#pragma omp master
        {
            sizePartVec += remainder;
            remainder = 0;
        }

        int* localOutVec = new int[sizePartVec];
        int* localInVec = new int[sizePartVec];
        int j = 0;
        for (int i = threadId * sizePartVec + remainder;
        i < (threadId + 1) * sizePartVec + remainder; i++, j++) {
            localInVec[j] = sortVec[i];
        }

        std::vector<int> counters(sizeof(int) * 256);
        createCounters(localInVec, counters.data(), sizePartVec);
        int* count;
        for (int i = 0; i < s; i++) {
            count = counters.data() + 256 * i;
            signedRadix(i, sizePartVec, localInVec, localOutVec, count);
            std::swap(localInVec, localOutVec);
        }
        delete[] localOutVec;

#pragma omp master
        {
            for (int i = 0; i < sizePartVec; i++)
                sortVec[i] = localInVec[i];
            flag++;
        }
#pragma omp barrier
#pragma omp critical
        if (threadId != 0) {
            mergeOrderVec(sortVec, flag * sizePartVec + remainder,
        localInVec, sizePartVec);
            flag++;
        }
        delete[] localInVec;
    }
}
