/*
 * sample.cu
 *
 *  Created on: Jan 17, 2015
 *      Author: Adam Kosiorek
 */

#include <vector>

void someKernel(std::vector<int>& vec) {

	vec[0] = vec[1] + vec[2];
}
