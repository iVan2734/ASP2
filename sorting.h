#ifndef ASP2_DZ3_SORTING_H
#define ASP2_DZ3_SORTING_H

#include "image.h"
#include <vector>
#include <array>
#include <queue>
#include <algorithm>
#include <iostream>
#include <utility>

using namespace std;

enum SortingDirection {
    ASCENDING,
    DESCENDING
};

class SortingAlgorithm {
public:
    virtual ~SortingAlgorithm() = default;

    virtual void sort(Image *image, SortingDirection direction) = 0;
};
class ExampleSort final : public SortingAlgorithm {
public:
    void sort(Image *image, SortingDirection direction) override {
        const int count = image->getElementCount();
        for (int i = 0; i < count; i++) {
            for (int j = i + 1; j < count; j++) {
                const int el1 = image->getElement(i);
                const int el2 = image->getElement(j);
                //cout<<el1<<" "<<el2<<endl;

                if (direction == ASCENDING && el1 > el2 || direction == DESCENDING && el1 < el2)
                    image->swapElements(i, j);
            }
        }
    }
};
class CppSort final : public SortingAlgorithm {
public:
    void sort(Image *image, SortingDirection direction) override {
        int n = image->getElementCount();
        vector<int> arr(n);
        for (int i = 0; i < n; i++) {
            arr[i] = image->getElement(i);
        }

        if (direction == ASCENDING) {
            std::sort(arr.begin(), arr.end());
        } else {
            std::sort(arr.begin(), arr.end(), greater<int>());
        }
    }
};
class AnastasijaSort final : public SortingAlgorithm {
public:
    AnastasijaSort(int k, double c): k(k), c(c) {
    }

    void quickSort(Image *image, SortingDirection direction) {
        int left = 0;
        int right = image->getElementCount() - 1;
        pair<int, int> data;
        array<int, 3> arr;
        queue<array<int, 3> > que;
        arr[0] = left;
        arr[1] = right;
        arr[2] = 0;
        que.push(arr);
        while (!que.empty()) {
            arr = que.front();
            que.pop();
            if (arr[0] >= arr[1]) continue;
            data = partition(image, direction, arr[0], arr[1], arr[2]);
            if (data.second >= this->k) {
                bubbleSort(image, direction, arr[0], arr[1]);
            } else {
                int temp = arr[1];
                //arr[0]=arr[0]; //left bounds
                arr[1] = data.first - 1; // pivot element(new right bound)
                arr[2] = data.second; //number of consecutive elements bigger then ...
                que.push(arr);

                arr[0] = data.first + 1;
                arr[1] = temp;
                //arr[2]=data.second;
                que.push(arr);
            }
        }
    }

    pair<int, int> partition(Image *image, SortingDirection direction, int left, int right, int count) {
        int i = left;
        int j = right;
        int pivot = image->getElement(i);
        while (i < j) {
            if (direction == ASCENDING) {
                while (image->getElement(i) <= pivot && i < j) {
                    i++;
                }
                while (image->getElement(j) > pivot) {
                    j--;
                }
            } else {
                while (image->getElement(i) >= pivot && i < j) {
                    i++;
                }
                while (image->getElement(j) < pivot) {
                    j--;
                }
            }
            if (i < j) {
                image->swapElements(i, j);
            }
        }
        image->swapElements(left, j);
        pair<int, int> p;
        p.first = j;

        ///heurisitka za anastasiju
        double x = min(j - left, right - left - j) / (right - left);
        if (x <= this->c) {
            p.second = count + 1;
            return p;
        } else {
            p.second = 0;
            return p;
        }
    }

    void bubbleSort(Image *image, SortingDirection direction, int left, int right) {
        if (left >= right) return;
        int position = right;
        while (position != left) {
            int bound = position;
            position = left;
            for (int i = left; i < bound; i++) {
                if (image->getElement(i) > image->getElement(i + 1) && direction == ASCENDING) {
                    image->swapElements(i, i + 1);
                    position = i;
                }
                if (image->getElement(i) < image->getElement(i + 1) && direction == DESCENDING) {
                    image->swapElements(i, i + 1);
                    position = i;
                }
            }
        }
    }

    void sort(Image *image, SortingDirection direction) override {
        quickSort(image, direction);
    }

private:
    double c;
    int k;
};
class MatijaSort final : public SortingAlgorithm {
public:
    MatijaSort(int k): k(k) {
    }

    void mergeSort(Image *image, SortingDirection direction) {
        int size = image->getElementCount();
        int blockSize = size;
        //trazim velicinu bkloka pri polovljenu
        while (blockSize / 2 > this->k) {
            blockSize /= 2;
        }
        for (int i = 0; i < size; i += blockSize) {
            int left = i;
            int right = min(i + blockSize, size);
            insertionSort(image, direction, left, right);
        }
        int currSize = blockSize;
        while (currSize < size) {
            //currSize*2 jer spajam sve polovine
            for (int left = 0; left < size; left += 2 * currSize) {
                int right = min(left + 2 * currSize, size);
                merge(image, direction, left, right);
            }
            currSize *= 2;
        }
    }

    void merge(Image *image, SortingDirection direction, int left, int right) {
        int len = right - left;
        vector<int> indices(len);
        vector<int> data(len);
        vector<int> position(len);

        int mid = (right + left) / 2;
        int i = left;
        int j = mid;
        int count = 0;

        while (i<mid && j<right) {
            if (image->getElement(i)<=image->getElement(j) && direction == ASCENDING) {
                indices[count] = i - left;
                count++;
                i++;
            }
            if (image->getElement(i) > image->getElement(j) && direction == ASCENDING) {
                indices[count] = j - left;
                count++;
                j++;
            }
            if (image->getElement(i) >= image->getElement(j) && direction == DESCENDING) {
                indices[count] = i - left;
                count++;
                i++;
            }
            if (image->getElement(i) < image->getElement(j) && direction == DESCENDING) {
                indices[count] = j - left;
                count++;
                j++;
            }
        }
        for (int m = i; m < mid; m++, count++) {
            indices[count] = m - left;
        }
        for (int m = j; m < right; m++, count++) {
            indices[count] = m - left;
        }
        for (int l = 0; l < len; l++) {
            position[l] = l;
        }
        for (int l = 0; l < len; l++) {
            data[l] = l;
        }
        for (int l = 0; l < len; l++) {
            int index1 = indices[l];
            int index2 = data[l];
            int curr = position[index1];
            //menjam podatka
            data[l] = index1;
            data[curr] = index2;

            //upadtujem pozcije
            //position[index1]=curr;
            position[index1] = l;
            position[index2] = curr;
            image->swapElements(left + l, left + curr);
        }
    }

    void insertionSort(Image *image, SortingDirection direction, int left, int right) {
        for (int i = left + 1; i < right; i++) {
            int key = image->getElement(i);
            int j = i - 1;
            while (j >= left && image->getElement(j) > key && direction == ASCENDING || j >= left && image->
                   getElement(j) < key && direction == DESCENDING) {
                image->swapElements(j + 1, j);
                j--;
            }
        }
    }

    void sort(Image *image, SortingDirection direction) override {
        mergeSort(image, direction);
    }

private:
    int k;
};


#endif //ASP2_DZ3_SORTING_H
