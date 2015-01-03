#include <map>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <iterator>

// macierz podobieństwa między literami słownik w kolejności A,G,C,T,-
const int similarity_matrix[5][5] =   {{1,-1,-1,-1,-2},
									   {-1,1,-1,-1,-2},
									   {-1,-1,1,-1,-2},
									   {-1,-1,-1,1,-2},
									   {-2,-2,-2,-2,0}};
// kara za przerwę
const int penalty = -1;

// mapa konwertująca litery słownika DNA na indeksy w macierzy podobieństwa
std::map<char,int> ctoi;

/**
 * Inicjalizacja mapy konwertującej litery słownika DNA na indeksy w macierzy podobieństwa
 */
void init_char_to_int_map(std::map<char,int>& ctoi)
{
	ctoi['A'] = 0;
	ctoi['G'] = 1;
	ctoi['C'] = 2;
	ctoi['T'] = 3;
	ctoi['-'] = 4;
}

/**
 * Inicjalizacja tablicy modyfikacji indeksów.
 * Modyfikacja typu (0,0,1) oznacza:
 * x = x - 0; y = y - 0; z = z - 1;
 */
void init_lut(std::vector<std::vector<int> >& lut)
{
	lut[0][0] = 0;lut[0][1] = 0;lut[0][2] = 1;
	lut[1][0] = 0;lut[1][1] = 1;lut[1][2] = 0;
	lut[2][0] = 0;lut[2][1] = 1;lut[2][2] = 1;
	lut[3][0] = 1;lut[3][1] = 0;lut[3][2] = 0;
	lut[4][0] = 1;lut[4][1] = 0;lut[4][2] = 1;
	lut[5][0] = 1;lut[5][1] = 1;lut[5][2] = 0;
	lut[6][0] = 1;lut[6][1] = 1;lut[6][2] = 1;
}

/*
 * Wydruk tablicy 3D
 */
void print_result(std::vector<std::vector<std::vector<int> > >& tab)
{
	std::cout << "\nResult\n";
	for (uint i = 0; i < tab.size(); ++i)
	{
		for (uint j = 0; j < tab[1].size(); ++j)
		{
			std::copy(tab[i][j].begin(),tab[i][j].end(),std::ostream_iterator<int>(std::cout, ","));
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
}

/**
 * Wczytanie sekwencji
 */
int read_sequence(char* file_name, std::vector<char>& seq)
{
	std::ifstream file(file_name);
	// Alokacja pamięci na znaki zapisane w pliku
	seq.resize(file.tellg());
	// Przypisanie pliku do wektora
	seq.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	// Usunięcie znaku końca linii
	seq.pop_back();

	// [DEBUG] Wydruk do strumienia wejściowego;
	std::cout << std::endl;

	std::copy(seq.begin(),seq.end(),std::ostream_iterator<char>(std::cout, ","));

	return seq.size();
}

/**
 * Zapis wyniku porównania sekwencji globalnych
 */
void write_sequence(std::string file_name, std::vector<char>& seq)
{
	std::string new_file = file_name + "result";
	// [DEBUG] Wydruk do strumienia wejściowego;
	std::copy(seq.begin(),seq.end(),std::ostream_iterator<char>(std::cout, ","));
	std::cout << std::endl;
	// Otwarcie pliku
	std::ofstream file(new_file.c_str(), std::ios::binary);

	// Zapis do pliku
	std::copy(seq.begin(),seq.end(),std::ostreambuf_iterator<char>(file));
}

/**
 * Alokacja pamięci dla wczytanych sekwencji.
 * Obliczenie wartości brzegowych 3D tablicy podobieństwa.
 */
void f_start(std::vector<std::vector<std::vector<int> > >& tab,
		uint size1, uint size2, uint size3)
{

	// Alokacja pamięci
	tab.resize(size1);

	for (uint i=0; i< size1; ++i)
	{
		tab[i].resize(size2);
		for(uint j=0; j < size2; ++j)
			tab[i][j].resize(size3);
	}

	// Obliczanie wartości brzegowych
	tab[0][0][0] = 0;

	for (uint k = 1; k < size3; ++k)
		tab[0][0][k] = k*penalty;

	for (uint j = 1; j < size2; ++j)
		tab[0][j][0] = j*penalty;

	for (uint i = 1; i < size1; ++i)
		tab[i][0][0] = i*penalty;
}

/**
 * Odczytanie wartości podobieństwa dwóch liter z alfabetu DNA
 */
int e(char a, char b)
{
	return similarity_matrix[ctoi[a]][ctoi[b]];
}

void calc_alingment(std::vector<std::vector<std::vector<int> > >& tab,
		std::vector<std::vector<char> >& seq)
{
	init_char_to_int_map(ctoi);

	f_start(tab, seq[0].size(), seq[1].size(), seq[2].size());

	//Obliczenie podobieństw między sekwencjami
	for (uint i = 1; i < seq[0].size(); ++i) {
		for (uint j = 1; j < seq[1].size(); ++j) {
			for (uint k = 1; k < seq[2].size(); ++k)
			{
				std::vector<int> f(7);

				f[0] = tab[i-1][j-1][k-1] + e(seq[0][i],seq[1][j]) + e(seq[0][i],seq[2][k]) + e(seq[1][j], seq[2][k]);

				f[1] = tab[i-1][j-1][k] + e(seq[0][i],seq[1][j]) + e(seq[0][i],'-') + e(seq[1][j], '-');
				f[2] = tab[i-1][j][k-1] + e(seq[0][i],'-') + e(seq[0][i],seq[2][k]) + e('-', seq[2][k]);
				f[3] = tab[i][j-1][k-1] + e('-',seq[1][j]) + e('-',seq[2][k]) + e(seq[1][j], seq[2][k]);

				f[4] = tab[i-1][j][k] + e(seq[0][i],'-') + e(seq[0][i],'-') + e('-','-');
				f[5] = tab[i][j-1][k] + e('-',seq[1][j]) + e('-','-') + e(seq[1][j], '-');
				f[6] = tab[i][j][k-1] + e('-','-') + e('-',seq[2][k]) + e('-', seq[2][k]);

				// Znalezienie największej wartości
				tab[i][j][k] = *std::max_element(f.begin(),f.end());
			}
		}
	}
}

void calc_global_alingment(std::vector<std::vector<char> >& new_seq,
		std::vector<std::vector<std::vector<int> > >& tab,
		std::vector<std::vector<char> >& seq)
{
	std::cout << "Global alignment path\n";

	// tablica przechowująca rodzaj modyfikacji na indeksach
	// wykorzystywana do wyznaczania podobieństwa globalnego
	std::vector<std::vector<int> > lut(7,std::vector<int>(3));
	init_lut(lut);

	// Znalezienie najdłuższej długości sekwencji.
	uint max = std::max(seq[0].size(), seq[1].size());
		 max = std::max(max, uint(seq[2].size()));

	// Alokacja pamięci na sekwencje o długości maksymalnej sekwencji.
	for(uint i=0; i < new_seq.size(); ++i)
		new_seq[i].resize(max);

	// Aktualne indexy
	std::vector<uint> index(3);

	// Inicjalizacja indexów na ostatni element tablicy podobieństw
	for(uint i=0; i < seq.size(); ++i)
	{
		index[i] = seq[i].size()-1;
		new_seq[i][max-1] = seq[i][index[i]];

		// [DEBUG] Wydruk
		std::cout << "i: " << index[i] << "\t";
	}

	// [DEBUG] Wydruk
	std::cout << "\t max: " << tab[index[0]][index[1]][index[2]] << std::endl;

	// W każdej iteracji szukany jest największy następny element różniący się
	// w odległości od poprzedniego o max 1 w każdym kierunku

	for(int a=max-1; a>0;)
	{
		// Początkowo zakładamy, że największym elementem jest element leżący na skosie.
		std::vector<int> tmp_index(3);
		for(uint i=0; i < index.size(); ++i)
			tmp_index[i] = (index[i]-1) >= 0 ? (index[i]-1) : 0;

		int elem = tab[tmp_index[0]][tmp_index[1]][tmp_index[2]];

		// Wartość skośnego przekształcenia w tablicy lut
		int p = 6;

		for(uint h=0; h < (lut.size()-1); ++h)
		{
			for(uint i=0; i < index.size(); ++i)
				tmp_index[i] = index[i]-lut[h][i];

			// porównanie kolejnych elementów
			if(tmp_index[0] > -1 && tmp_index[1] > -1 && tmp_index[2] > -1
			   && tab[tmp_index[0]][tmp_index[1]][tmp_index[2]] > elem)
			{
				elem = tab[tmp_index[0]][tmp_index[1]][tmp_index[2]];
				p = h;
			}
		}
		--a;

		for(uint i=0; i < index.size(); ++i)
		{
			// Aktualizacja indexów
			index[i] -=lut[p][i];

			// wstawienie przerwy lub przepisanie orginalnych wartości
			if (!lut[p][i] || index[i] < 0) new_seq[i][a] = '-';
			else new_seq[i][a] = seq[i][a];

			// [DEBUG] Wydruk
			std::cout << "i: " << index[i] << "\t";
		}
		// [DEBUG] Wydruk
		std::cout << "\t max: " << elem << " p: " << p << " a: " << a << std::endl;
	}
}


int main(int argc, char** argv)
{
	if (argc < 4)
	{
		std::cerr << "Not enough sequence to compare\n";
		return 1;
	}

	// oryginalne sekwencje
	std::vector<std::vector<char> > seq(3);

	// tablica 3D z wartościami podobieństwa
	std::vector<std::vector<std::vector<int> > > tab;

	// [DEBUG] Wydruk
	std::cout << "Read sequences\n";

	for(uint i=0; i < seq.size(); ++i)
		std::cout << "\t Length. seq = " << read_sequence(argv[i+1], seq[i]);

	// Obliczenie dopasowania
	calc_alingment(tab, seq);

	// [DEBUG] Wydruk wyników.
	print_result(tab);

	// Stworzenie nowych sekwencji, w których zapisany jest wynik porównania.
	std::vector<std::vector<char> > new_seq(3);

	// Obliczenie dopasowania globalnego
	calc_global_alingment(new_seq,tab,seq);

	// Zapis do plików
	// [DEBUG] Wydruk
	std::cout << "\nSequences after global alignment\n";

	for(uint i=0; i < new_seq.size(); ++i)
		write_sequence(std::string(argv[i+1]),new_seq[i]);

	return 0;
}
