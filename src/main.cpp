#include <map>
#include <algorithm>

const int l=100;
const int m=90;
const int n=80;

char A[l];
char B[m];
char C[n];

int tab[l][m][n];

int d = -1; // kara za przerwę

std::map<char,int> char_to_int;


int tab_e[4][4] = {{1,-1,-1,-1},
				   {-1,1,-1,-1},
				   {-1,-1,1,-1},
				   {-1,-1,-1,1}};

void init_tab()
{
	char_to_int['A'] = 0;
	char_to_int['G'] = 1;
	char_to_int['C'] = 2;
	char_to_int['T'] = 3;
}

void f_start()
{
	tab[0][0][0] = 0;

	for (int k = 1; k < l; ++k)
		tab[0][0][k] = k*d;

	for (int j = 1; j < m; ++j)
		tab[0][j][0] = j*d;

	for (int i = 1; i < n; ++i)
		tab[i][0][0] = i*d;
}

int eAB(int i, int j)
{
	if (i && j)
		return tab_e[char_to_int[A[i]]][char_to_int[B[i]]];
	return d;
}

int eAC(int i, int j)
{
	if (i && j)
		return tab_e[char_to_int[A[i]]][char_to_int[C[i]]];
	return d;
}

int eBC(int i, int j)
{
	if (i && j)
		return tab_e[char_to_int[B[i]]][char_to_int[C[i]]];
	return d;
}

int e(int i, int j, int k)
{
	return eAB(i,j) + eAC(i,k) + eBC(j,k);
}

int main(int argc, char** argv)
{
	for (int i = 1; i < n; ++i)
		for (int j = 1; j < m; ++j)
			for (int k = 1; k < l; ++k)
			{
				int fijk[7];
				fijk[0] = tab[i-1][j-1][k-1] + e(i,j,k);
				fijk[1] = tab[i-1][j-1][k] + e(i,j,0);
				fijk[2] = tab[i-1][j][k-1] + e(i,0,k);
				fijk[3] = tab[i][j-1][k-1] + e(0,j,k);
				fijk[4] = tab[i-1][j][k] + e(i,0,0);
				fijk[5] = tab[i][j-1][1] + e(0,j,0);
				fijk[6] = tab[i][j][k-1] + e(0,0,k);

				tab[i][j][k] = std::max_element(fijk,fijk+7);
			}
}
