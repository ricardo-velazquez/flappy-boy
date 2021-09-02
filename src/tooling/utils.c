#include <gb/gb.h>
#include <rand.h>

INT8 clamp (INT8 n, INT8 min, INT8 max) {
	if (n < min) return min;
	if (n > max) return max;
	return n;
}

INT8 max (INT8 x, INT8 y) {
	return x > y ? x : y;
}

INT8 min (INT8 x, INT8 y) {
	return x < y ? x : y;
}

INT8 random (INT8 value) {
	return ((UINT8)rand() * value) >> 8;
}

INT8 random_range (INT8 min, INT8 max) {
	return min + random((max - min));
}

INT8 norm (INT8 value, INT8 min, INT8 max) {
	return ((value  - min ) << 8 / (max  - min ) << 8) >> 8;
}

INT8 lerp (INT8 norm, INT8 value) {
	return value * norm / 10;
}

INT8 lerp_range(INT8 norm, INT8 min, INT8 max) {
	return lerp(norm, (max - min)) + min;
}