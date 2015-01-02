#include <map>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <iterator>

int d = -1; // kara za przerwę

std::map<char,int> ctoi;

int l;
int m;
int n;

std::vector<std::vector<char> > seq;

std::vector<std::vector<std::vector<int> > > tab;

std::vector<std::vector<int> > lut(7,std::vector<int>(3));

const int tab_e[5][5] =   {{1,-1,-1,-1,-2},
						   {-1,1,-1,-1,-2},
						   {-1,-1,1,-1,-2},
						   {-1,-1,-1,1,-2},
						   {-2,-2,-2,-2,0}};


void init_tab()
{
	ctoi['A'] = 0;
	ctoi['G'] = 1;
	ctoi['C'] = 2;
	ctoi['T'] = 3;
	ctoi['-'] = 4;
}

void init_lut()
{
	lut[0][0] = 0;lut[0][1] = 0;lut[0][2] = 1;
	lut[1][0] = 0;lut[1][1] = 1;lut[1][2] = 0;
	lut[2][0] = 0;lut[2][1] = 1;lut[2][2] = 1;
	lut[3][0] = 1;lut[3][1] = 0;lut[3][2] = 0;
	lut[4][0] = 1;lut[4][1] = 0;lut[4][2] = 1;
	lut[5][0] = 1;lut[5][1] = 1;lut[5][2] = 0;
	lut[6][0] = 1;lut[6][1] = 1;lut[6][2] = 1;

}

void f_start()
{

	tab.resize(n);

	for (int i=0; i<n; ++i)
	{
		tab[i].resize(m);
		for(int j=0; j<m; ++j)
			tab[i][j].resize(l);
	}

	tab[0][0][0] = 0;

	for (int k = 1; k < l; ++k)
		tab[0][0][k] = k*d;

	for (int j = 1; j < m; ++j)
		tab[0][j][0] = j*d;

	for (int i = 1; i < n; ++i)
		tab[i][0][0] = i*d;
}


int e(char a, char b)
{
	return tab_e[ctoi[a]][ctoi[b]];
}


int read_sequence(char* file_name)
{
	std::ifstream file(file_name);
	std::cout << file_name << std::endl;
	std::vector<char> t(file.tellg());
	t.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	t.pop_back();
	std::copy(t.begin(),t.end(),std::ostream_iterator<char>(std::cout, ","));
	std::cout << std::endl;

	seq.push_back(t);
	return t.size();
}

void write_sequence(std::string file_name, std::vector<char>& seq)
{
	std::string new_file = file_name + "re";
	std::copy(seq.begin(),seq.end(),std::ostream_iterator<char>(std::cout, ","));

	std::ofstream file(new_file.c_str(), std::ios::binary);
	std::copy(seq.begin(),seq.end(),std::ostreambuf_iterator<char>(file));
}

int main(int argc, char** argv)
{
	if (argc < 4)
	{
		std::cerr << "Not enough sequence to compare\n";
		return 1;
	}

	l = read_sequence(argv[1]);
	m = read_sequence(argv[2]);
	n = read_sequence(argv[3]);

	std::cout << "After read l: " << l << " m: " << m << " n: " << n << std::endl;

	init_tab();
	init_lut();

	f_start();

	int min = 0;

	for (int i = 1; i < l; ++i)
	for (int j = 1; j < m; ++j)
	for (int k = 1; k < n; ++k)
	{
		std::vector<int> f(7);

		f[0] = tab[i-1][j-1][k-1] + e(seq[0][i],seq[1][j]) + e(seq[0][i],seq[2][k]) + e(seq[1][j], seq[2][k]);

		f[1] = tab[i-1][j-1][k] + e(seq[0][i],seq[1][j]) + e(seq[0][i],'-') + e(seq[1][j], '-');
		f[2] = tab[i-1][j][k-1] + e(seq[0][i],'-') + e(seq[0][i],seq[2][k]) + e('-', seq[2][k]);
		f[3] = tab[i][j-1][k-1] + e('-',seq[1][j]) + e('-',seq[2][k]) + e(seq[1][j], seq[2][k]);

		f[4] = tab[i-1][j][k] + e(seq[0][i],'-') + e(seq[0][i],'-') + e('-','-');
		f[5] = tab[i][j-1][k] + e('-',seq[1][j]) + e('-','-') + e(seq[1][j], '-');
		f[6] = tab[i][j][k-1] + e('-','-') + e('-',seq[2][k]) + e('-', seq[2][k]);

		std::vector<int>::iterator it = std::max_element(f.begin(),f.end());
		tab[i][j][k] = *it;
		min = min > (*it) ? (*it) : min;
	}

	for (int i = 0; i < l; ++i)
	{
		for (int j = 0; j < m; ++j)
		{
			std::copy(tab[i][j].begin(),tab[i][j].end(),std::ostream_iterator<int>(std::cout, ","));
			std::cout << std::endl;
		}
		std::cout << std::endl;

	}

	int x,y,z;

	int max = m > n ? m:n;

	max = max > l ? max:l;

	x = l-1; y = m-1; z = n-1;

	std::cout << "max: " << tab[x][y][z] << " \t x: " << x << " y: " << y << " z: " << z << std::endl;

	std::vector<std::vector<char> > new_seq(3,std::vector<char>(max));

	new_seq[0][max-1] = seq[0][x];
	new_seq[1][max-1] = seq[1][y];
	new_seq[2][max-1] = seq[2][z];

	for(int a=max-1; a>0;)
	{
		int i = (x-1) >= 0 ? (x-1) : 0;
		int j = (y-1) >= 0 ? (y-1) : 0;
		int k = (z-1) >= 0 ? (z-1) : 0;

		int elem = tab[i][j][k];
		int p = 6;

		for(int h=0; h<6; ++h)
		{
			int xi = x-lut[h][0];
			int yi = y-lut[h][1];
			int zi = z-lut[h][2];

			if(xi > -1 && yi > -1 && zi > -1 && tab[xi][yi][zi] > elem)
			{
				elem = tab[xi][yi][zi];
				p = h;
			}
		}

		x -= lut[p][0];
		y -= lut[p][1];
		z -= lut[p][2];

		std::cout << "max: " << elem << " \t x: " << x << " y: " << y << " z: " << z << " p: " << p << " a: " << a << std::endl;

		--a;

		if (!lut[p][0] || x < 0) new_seq[0][a] = '-';
		else new_seq[0][a] = seq[0][a];

		if (!lut[p][1] || y < 0) new_seq[1][a] = '-';
		else new_seq[1][a] = seq[1][a];

		if (!lut[p][2] || z < 0) new_seq[2][a] = '-';
		else new_seq[2][a] = seq[2][a];
	}

	write_sequence(std::string(argv[1]),new_seq[0]);
	std::cout << std::endl;

	write_sequence(std::string(argv[2]),new_seq[1]);
	std::cout << std::endl;

	write_sequence(std::string(argv[3]),new_seq[2]);
	std::cout << std::endl;

	return 0;
}
