#include <stddef.h>

typedef int (*is_before_t)(const void *new_data, 
						   const void *src_data, 
						   void *param);

void BubbleSort(int *arr, size_t size);

void InsertionSort(int *arr, size_t size);

void SelectionSort(int *arr, size_t size);

int CountingSort(const int *arr,
				 size_t arr_size,
				 int min_val,
				 int max_val,
				 int *result);
				 
int RadixSort(int *arr, size_t arr_size);

int MergeSort(int *arr, size_t size);

void HeapSort(void *arr, 
			  size_t arr_size, 
			  size_t element_size, 
			  is_before_t func, 
			  void *param);
