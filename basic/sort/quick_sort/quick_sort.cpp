#include <algorithm>
#include <assert.h>
#include <utility>

// partition on range: [start, end) of array A
// returns "mid" such that
// index of range: [start, mid) <= A[mid]
// index of range: [mid, end) > A[mid]
static int partition(int A[], int start, int end) {
  if (start + 1 >= end)
    return start;

  // <= pivot: [start, mid)
  // > pivot: [mid, tail)
  int pivot = start, tail = start + 1, mid = start + 1;

  for (; tail != end; tail++) {
    // for each iteration we tring to add A[tail] in
    if (A[tail] <= A[pivot]) {
      std::swap(A[mid], A[tail]);
      mid++;
    }
  }
  std::swap(A[pivot], A[mid - 1]);
  return mid;
}

static void quick_sort(int A[], int start, int end) {
  if (start + 1 >= end)
    return;
  int mid = partition(A, start, end);
  quick_sort(A, start, mid);
  quick_sort(A, mid, end);
}

const int TEST_ARRAY_SIZE = 1000000;

static int test_array[TEST_ARRAY_SIZE];

int main() {
  for (int i = 0; i < TEST_ARRAY_SIZE; i++) {
    test_array[i] = i;
  }
  for (int i = 0; i < 20; i++) {
    std::random_shuffle(test_array, test_array + TEST_ARRAY_SIZE);
    quick_sort(test_array, 0, TEST_ARRAY_SIZE);
    for (int i = 0; i < TEST_ARRAY_SIZE; i++) {
      assert(test_array[i] == i);
    }
  }
}