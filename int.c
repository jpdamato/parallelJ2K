#include "int.h"

int int_min(int a, int b) {
	return a < b ? a : b;
}
/**
Get the maximum of two integers
@return Returns a if a > b else b
*/
int int_max(int a, int b) {
	return (a > b) ? a : b;
}
/**
Clamp an integer inside an interval
@return
<ul>
<li>Returns a if (min < a < max)
<li>Returns max if (a > max)
<li>Returns min if (a < min) 
</ul>
*/
int int_clamp(int a, int min, int max) {
	if (a < min)
		return min;
	if (a > max)
		return max;
	return a;
}
/**
@return Get absolute value of integer
*/
int int_abs(int a) {
	return a < 0 ? -a : a;
}
/**
Divide an integer and round upwards
@return Returns a divided by b
*/
int int_ceildiv(int a, int b) {
	return (a + b - 1) / b;
}
/**
Divide an integer by a power of 2 and round upwards
@return Returns a divided by 2^b
*/
int int_ceildivpow2(int a, int b)
{
	return (a + (1 << b) - 1) >> b;
}
/**
Divide an integer by a power of 2 and round downwards
@return Returns a divided by 2^b
*/
int int_floordivpow2(int a, int b) {
	return a >> b;
}
/**
Get logarithm of an integer and round downwards
@return Returns log2(a)
*/
int int_floorlog2(int a) 
{
	int l;
	for (l = 0; a > 1; l++) {
		a >>= 1;
	}
	return l;
}
/* ----------------------------------------------------------------------- */
/**
Multiply two fixed-precision rational numbers.
@param a
@param b
@return Returns a * b
*/
int fix_mul(int a, int b)
{
    __int64 temp = (__int64) a * (__int64) b ;
    temp += temp & 4096;
    return (int) (temp >> 13) ;
}
/*@}*/

/*@}*/
