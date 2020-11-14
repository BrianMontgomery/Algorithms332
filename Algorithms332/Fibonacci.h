#include <iostream>

#include "Random.h"

class box {
	class pair {
		float x;
		float y;
	};
private:
	int N = 25;
public:
	void getClosest() {
		pair arr[25];
		std_random::shuffle(arr, n);
			if (N < 2) { arr[N] == N; }
			arr[N] = arr[N - 1] + arr[N - 2];
			std::cout << N << " " << arr[N] << std::endl;
		}
	}
};