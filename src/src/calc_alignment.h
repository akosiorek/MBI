/*
 * calc_alignment.h
 *
 *  Created on: Jan 17, 2015
 *      Author: Adam Kosiorek
 */

#ifndef SRC_CALC_ALIGNMENT_H_
#define SRC_CALC_ALIGNMENT_H_

#include <vector>

typedef std::vector<std::vector<std::vector<int> > > Cube;
typedef std::vector<std::vector<char> > Sequence;

Cube calc_alignment_CPU(const Sequence& seq);
Cube calc_alignment_GPU(const Sequence& seq);


#endif /* SRC_CALC_ALIGNMENT_H_ */
