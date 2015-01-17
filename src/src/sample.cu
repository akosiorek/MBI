/*
 * sample.cu
 *
 *  Created on: Jan 17, 2015
 *      Author: Adam Kosiorek
 */

#include <vector>

__global__ void someKernel(int* vec, int N) {

	vec[0] = vec[1] + vec[2];
}
