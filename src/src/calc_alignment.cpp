/*
 * sample.cu
 *
 *  Created on: Jan 17, 2015
 *      Author: Adam Kosiorek
 */

#include "calc_alignment.h"
#include <map>
#include <algorithm>

// kara za przerwę
const int penalty = -1;

// mapa konwertująca litery słownika DNA na indeksy w macierzy podobieństwa
std::map<char, int> ctoi;




// macierz podobieństwa między literami słownik w kolejności A,G,C,T,-
const int similarity_matrix[5][5] = { { 1, -1, -1, -1, -2 },
		{ -1, 1, -1, -1, -2 }, { -1, -1, 1, -1, -2 }, { -1, -1, -1, 1, -2 }, {
				-2, -2, -2, -2, 0 } };

int e(char a, char b) {
	return similarity_matrix[ctoi[a]][ctoi[b]];
}


/**
 * Inicjalizacja mapy konwertującej litery słownika DNA na indeksy w macierzy podobieństwa
 */
void init_char_to_int_map(std::map<char, int>& ctoi) {
	ctoi['A'] = 0;
	ctoi['G'] = 1;
	ctoi['C'] = 2;
	ctoi['T'] = 3;
	ctoi['-'] = 4;
}

/**
 * Alokacja pamięci dla wczytanych sekwencji.
 * Obliczenie wartości brzegowych 3D tablicy podobieństwa.
 */
void f_start(std::vector<std::vector<std::vector<int> > >& tab, uint size1,
		uint size2, uint size3) {

	// Alokacja pamięci
	tab.resize(size1);

	for (uint i = 0; i < size1; ++i) {
		tab[i].resize(size2);
		for (uint j = 0; j < size2; ++j)
			tab[i][j].resize(size3);
	}

	// Obliczanie wartości brzegowych
	tab[0][0][0] = 0;

	for (uint k = 1; k < size3; ++k)
		tab[0][0][k] = k * penalty;

	for (uint j = 1; j < size2; ++j)
		tab[0][j][0] = j * penalty;

	for (uint i = 1; i < size1; ++i)
		tab[i][0][0] = i * penalty;
}


Cube calc_alingment_CPU(const Sequence& seq) {
	Cube tab;
	// init tab;

	init_char_to_int_map(ctoi);

	f_start(tab, seq[0].size(), seq[1].size(), seq[2].size());

	//Obliczenie podobieństw między sekwencjami
	for (uint i = 1; i < seq[0].size(); ++i) {
		for (uint j = 1; j < seq[1].size(); ++j) {
			for (uint k = 1; k < seq[2].size(); ++k) {
				std::vector<int> f(7);

				f[0] = tab[i - 1][j - 1][k - 1] + e(seq[0][i], seq[1][j])
						+ e(seq[0][i], seq[2][k]) + e(seq[1][j], seq[2][k]);

				f[1] = tab[i - 1][j - 1][k] + e(seq[0][i], seq[1][j])
						+ e(seq[0][i], '-') + e(seq[1][j], '-');
				f[2] = tab[i - 1][j][k - 1] + e(seq[0][i], '-')
						+ e(seq[0][i], seq[2][k]) + e('-', seq[2][k]);
				f[3] = tab[i][j - 1][k - 1] + e('-', seq[1][j])
						+ e('-', seq[2][k]) + e(seq[1][j], seq[2][k]);

				f[4] = tab[i - 1][j][k] + e(seq[0][i], '-') + e(seq[0][i], '-')
						+ e('-', '-');
				f[5] = tab[i][j - 1][k] + e('-', seq[1][j]) + e('-', '-')
						+ e(seq[1][j], '-');
				f[6] = tab[i][j][k - 1] + e('-', '-') + e('-', seq[2][k])
						+ e('-', seq[2][k]);

				// Znalezienie największej wartości
				tab[i][j][k] = *std::max_element(f.begin(), f.end());
			}
		}
	}

	return tab;
}
