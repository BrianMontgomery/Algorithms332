//
//  shell_sort.h
//  stacks_queues_bags
//
//  Created by William McCarthy on 10/8/20.
//  Copyright © 2020 William McCarthy. All rights reserved.
//

#ifndef Merge_Sort_h
#define Merge_Sort_h

#include <cassert>
#include <algorithm>
#include "Utils.h"
#include "Insertion_Sort.h"

template <typename T>
class merge_sort {
	public:
		static void sort(T* arr, size_t n, const comparator<T>& comp = fwd_comparator<T>())
		{
			T *aux = new T[n];
			sort(arr, aux, 0, n - 1, comp);
			delete[] aux;
		}

private:
		static const int CUTOFF = 7;

		static void sort(T* arr, T* aux, size_t low, size_t high, const comparator<T>& comp)
		{
			//check that the size is not 0 or 1
			if (high <= low + CUTOFF) {
				insertion_sort<std::string> ins_sort;
				ins_sort.sort(arr, low, high, comp);
				return;
			}

			size_t mid = low + (high - low) / 2;
			sort(arr, aux, low, mid, comp); // Sort left half.
			sort(arr, aux, mid + 1, high, comp); // Sort right half.
			merge(arr, aux, low, mid, high, comp);
		}

		static void merge(T* arr, T* aux, size_t low, size_t mid, size_t high, const comparator<T>& comp)
		{
			int i = low; 
			int j = mid + 1;

			for (int k = low; k <= high; k++)
			{
				// Copy a[lo..hi] to aux[lo..hi].
				aux[k] = arr[k];
			}

			for (int k = low; k <= high; k++)
			{
				// Merge back to a[lo..hi].
				if (i > mid) {
					arr[k] = aux[j++];
				}
				else if (j > high) 
				{
					arr[k] = aux[i++];
				}
				else if (less(aux[j], aux[i], comp))
				{
					arr[k] = aux[j++];
				}
				else {
					arr[k] = aux[i++];
				}
			}
		}
};

template <typename T>
class merge_bu_sort {
	public:
		// merge_bu_sort() = delete;

		static void sort(T* arr, size_t n, const comparator<T>& comp = fwd_comparator<T>())
		{
			// Do lg N passes of pairwise merges.
			T *aux = new T[n];
			for (size_t sz = 1; sz < n; sz = sz + sz) // sz: subarray size
			{
				for (size_t lo = 0; lo < n - sz; lo += sz + sz) // lo: subarray index
				{
					merge(arr, aux, lo, lo + sz - 1, std::min<size_t>(lo + 2 * sz - 1, n - 1), comp);
				}
			}
		}

	private:
		static void merge(T* arr, T* aux, size_t low, size_t mid, size_t high, const comparator<T>& comp)
		{
			int i = low;
			int j = mid + 1;

			for (int k = low; k <= high; k++)
			{
				// Copy a[lo..hi] to aux[lo..hi].
				aux[k] = arr[k];
			}

			for (int k = low; k <= high; k++)
			{
				// Merge back to a[lo..hi].
				if (i > mid) {
					arr[k] = aux[j++];
				}
				else if (j > high)
				{
					arr[k] = aux[i++];
				}
				else if (less(aux[j], aux[i], comp))
				{
					arr[k] = aux[j++];
				}
				else {
					arr[k] = aux[i++];
				}
			}
		}
};

#endif /* Merge_Sort_h */