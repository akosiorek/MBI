MBI
===

Multiple sequence alignment

1. Cel projektu:

Celeme projektu jest stworzenie aplikacji wykorzystującej programowanie dynamiczne i obliczającej najlepsze 

dopasowanie dla trzech podanych sekwencji. Implementacja zostanie zrealizowana w sposób równoległy w 

środowisku CUDA.

2. Wstep:

// TODO - Dopasowanie pary sekwencji - def

Dopasowanie trzech podanych sekwencji jest szczególnym przypadkiem dopasowania wielu sekwencji. Problem

ten jest kategoryzowany jako problem obliczeniowo trudny. Metoda dokładna wykorzystująca programowanie 

dynamiczne obliczenia podobieństwa n sekwencji posiada złożoność wielomianową, gdzie stopień wielomianu 

wynosi n. 

3. Opis algorytmu:

Porównaniu podawane są trzy sekwencje DNA o alfabecie E= {A, C, T, G, _}

Wartości poszczególnych komórek są obliczane zgodnie z zależnością (1).

Funkcja e(si

Miara podobieństwa jest wyrażona równaniem (2) 

),si∈E oznacza miarę podobieństwa elementów si

, ...,s k

Wartości e(si

),e (si

, t

,uk

j

),e (t

Warunki brzegowe algorytmu obliczane są zgodnie z zalęznością (3).

gdzie d - kara za przerwę

Złożoność pamięciowa algorytmu wynosi O(l

stworzonej z porównywanych sekwencji wymagana jest wykonanie 7 porównań opartych na wcześniej 

obliczonych wartościach. 

4. Założenia optymalizacyjne:

• W kroku pierwszym zainicjalizowane zostaną wszystkie wartości brzegowe

• Istnieje możliwość obliczenia w jednym momencie trzech wartości zwartych w obrębie sześcianu (i-1, 

i)x(j-1,j)x(k-1,k) : (i,j, k-1), (i, j-1, k), (i-1,j,k). Przy czym w danym kierunku (i-1 lub j-1 lub k-1) może 

występować w maksymalnie max(floor(i/2), floor(j/2)) lub max(floor(i/2), floor(k/2)) lub max(floor(k/2), 

floor(j/2)) takich sześcianów.

5. Funkcjonalność:

• Porównanie trzech wskazanych sekwencji.

• Odczyt sekwencji zapisanych w plikach tekstowych w formacie plain.

• Możliwość wskazania lokalizacji plików sekwencji. 

• Aplikacja konsolowa.

• Wyniki zapisywane do plików.

6. Narzędzia:

• System kontroli wersji: git

• Kompilator: Visual Studio 2012, 64-bit

• Cuda Framework 6.5
