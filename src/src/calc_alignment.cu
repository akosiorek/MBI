/*
 * sample.cu
 *
 *  Created on: Jan 17, 2015
 *      Author: Adam Kosiorek
 */

#include "calc_alignment.h"

__global__ void calc_alignment_GPU_impl(int* x, int* y, int* z, int nX, int nY,
		int nZ, int* cube) {

}

Cube calc_alignment_GPU(const Sequence& seq) {

	std::vector<int*> data(seq.size());
	int totalSize = 1;
	for (int i = 0; i < data.size(); ++i) {
		cudaMalloc((void**) data[i], seq[i].size());
		cudaMemcpy(data[i], &seq[i][0], seq[i].size() * sizeof(int),
				cudaMemcpyHostToDevice);
		totalSize *= seq[i].size();
	}
	int* cube = NULL;
	cudaMalloc((void**) cube, totalSize);

	calc_alignment_GPU_impl<<<1, 1>>>(data[0], data[1], data[2], seq[0].size(), seq[1].size(), seq[2].size(), cube);

	Cube output(seq[0].size(),
			std::vector<std::vector<int> >(seq[1].size(),
					std::vector<int>(seq[2].size())));

	for (int x = 0; x < seq[0].size(); ++x) {
		for (int y = 0; y < seq[1].size(); ++y) {
			cudaMemcpy(&output[x][y][0], cube, seq[2].size() * sizeof(int),
					cudaMemcpyDeviceToHost);
			cube += seq[2].size();
		}
	}

	return output;
}
