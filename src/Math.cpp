#include <cmath>

namespace Math {

	float fast_sigmoid(float x) {
		return x / (1 + abs(x));
	}

}
