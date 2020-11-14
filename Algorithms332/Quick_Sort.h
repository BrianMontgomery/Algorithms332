#include "Random.h"

template <typename T>
class quick_sort
{
public:
	static void sort(T* arr, size_t n, const comparator<T>& comp = fwd_comparator<T>())
	{
		std_random::shuffle(arr, n);
		sort(arr, 0, int(n - 1), comp);
		assert(is_sorted(arr, n));
	}

private:
	static void sort(T* arr, int low, int high, const comparator<T>& comp)
	{
		if (high <= low) return;
		int j = partition(arr, low, high, comp); // Partition (see page 291).
		sort(arr, low, j - 1, comp); // Sort left part a[lo .. j-1].
		sort(arr, j + 1, high, comp); // Sort right part a[j+1 .. hi].
	}

	static int partition(T* arr, int low, int high, const comparator<T>& comp)
	{ // Partition into a[lo..i-1], a[i], a[i+1..hi].
		int i = low, j = high + 1; // left and right scan indices
		T v = arr[low]; // partitioning item

		while (true)
		{ // Scan right, scan left, check for scan complete, and exchange.
			while (less(arr[++i], v, comp))
			{
				if (i == high) {
					break;
				}
			}
			while (less(v, arr[--j], comp))
			{
				if (j == low) {
					break;
				}
			}

			if (i >= j)
			{
				break;
			}

			exch(arr, i, j);
		}
		exch(arr, low, j); // Put v = a[j] into position
		return j; // with a[lo..j-1] <= a[j] <= a[j+1..hi].
	}

	static void exch(T* arr, int i, int j)
	{
		T temp = arr[i];
		arr[i] = arr[j];
		arr[j] = temp;
	}
};

template <typename T>
class quick_sort_3way {
public:
	static void sort(T* arr, size_t n, const comparator<T>& comp = fwd_comparator<T>())
	{
		std_random<T>::shuffle(arr, n);
		sort(arr, 0, int(n - 1), comp);
		assert(is_sorted(arr, n));
	}

private:
	static void sort(T* arr, int low, int high, const comparator<T>& comp)
	{ // See page 289 for public sort() that calls this method.
		if (high <= low) return;
		int lt = low, i = low, gt = high;
		T* v = &arr[low];

		while (i <= gt)
		{
			if (less(arr[i], *v, comp))
			{
				bool changeV = false;
				if (*v == arr[lt])
				{
					changeV = true;
				}

				exch(arr, lt++, i++);

				if (changeV)
				{
					v = &arr[i - 1];
				}
			} 
			else if(less(*v, arr[i], comp))
			{
				exch(arr, i, gt--);
			}
			else 
			{
				i++;
			}

		} // Now a[lo..lt-1] < v = a[lt..gt] < a[gt+1..hi].

		sort(arr, low, lt - 1, comp);
		sort(arr, gt + 1, high, comp);
	}

	static void exch(T* arr, int i, int j)
	{
		T temp = arr[i];
		arr[i] = arr[j];
		arr[j] = temp;
	}
};