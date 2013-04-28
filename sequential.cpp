#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <climits>
using namespace std;

int **resurse, **preturi, **bugete;
int Pmin;

typedef struct{
		int colA;
		int maxA;
		int colB;
		int maxB;
}Rezultat;

Rezultat *rezultat;

typedef struct{
		int cost;
		int costRes;
}Costuri;

/* Adaugare rezultat la un anumit pas */
void adaugareRezultat(int index, int n){
	int colA = 0, colB = 0, maxA = 0, maxB = 0;
	
	for(int i = 0; i < n; i++)
		for(int j = 0; j < n; j++){
			colB += resurse[i][j];
			if(resurse[i][j] == 0){
				if(preturi[i][j] > maxA)
					maxA = preturi[i][j];
			}
			else if(resurse[i][j] == 1) 
						if(preturi[i][j] > maxB)
							maxB = preturi[i][j];
		}
	colA = n*n - colB;
	
	rezultat[index].colA = colA;
	rezultat[index].maxA = maxA;
	rezultat[index].colB = colB;
	rezultat[index].maxB = maxB;
}

/* Afisare rezultat */
void afisareRezultat(int T, ofstream& out){
	for(int i = 0; i < T; i++){
		out << rezultat[i].colA << " " << rezultat[i].maxA << " " << 
		rezultat[i].colB << " "	<< rezultat[i].maxB << endl;
	}

}

/* Distanta Manhattan */
int distanta(int i, int j, int i1, int j1){
		return abs(i - i1) + abs(j - j1);
}

/* Calculeaza Costij si CostResij pentru anul curent */
 Costuri Cost(int i, int j, int n){
	  int cost, costRes;
	  Costuri c;
	  c.cost = INT_MAX;
	  c.costRes = INT_MAX;
	  
	  for(int i1 = 0; i1 < n; i1++)
			for(int j1 = 0; j1 < n; j1++){
				if(resurse[i][j] == (1 - resurse[i1][j1])){
					cost = preturi[i1][j1] + distanta(i, j, i1, j1);
					if(cost < c.cost)
						c.cost = cost;
				}
				else{
					costRes = preturi[i1][j1] + distanta(i, j, i1, j1);
					if(costRes < c.costRes)
						c.costRes = costRes;
				}
			}
		return c;	
}


int main(int argc, char* argv[]){
		int Pmax, n, T, cost, costRes;
		ifstream in(argv[2]);
		ofstream out(argv[3]);
		
		T = atoi(argv[1]);
		rezultat = (Rezultat*)malloc(T * sizeof(Rezultat));
		
		/*
		 * Citirea datelor din fisier
		 * */
		if(in.is_open())
			in >> Pmin >> Pmax >> n;
		else
			cerr << "Unable to open the file!" << endl;
			
			resurse = (int**)malloc(n * sizeof(int*));
			preturi = (int**)malloc(n * sizeof(int*));
			bugete = (int**)malloc(n * sizeof(int*));
			
			for(int i = 0; i < n; i++){
				resurse[i]  = (int*)malloc(n * sizeof(int));
				preturi[i]  = (int*)malloc(n * sizeof(int));
				bugete[i]   = (int*)malloc(n * sizeof(int));
			}
		
		/* Matricea de resurse */
		for(int i = 0; i < n; i++)
			for(int j = 0; j < n; j++)
				in >> resurse[i][j];
	
		/* Matricea de preturi */
		for(int i = 0; i < n; i++)
			for(int j = 0; j < n; j++)
				in >> preturi[i][j];
					
		/* Matricea de bugete */
		for(int i = 0; i < n; i++)
			for(int j = 0; j < n; j++)
				in >> bugete[i][j];			
					
		in.close();			
		Costuri costMat[n][n];
		Costuri c;
			
		/* Evolutia pietei in fiecare an */
		for(int k = 0; k < T; k++){      
			
			for(int l = 0; l < n; l++)
				for(int h = 0; h < n; h++){
					c = Cost(l, h, n);
					costMat[l][h].cost = c.cost;
					costMat[l][h].costRes = c.costRes;
				}
			    
			for(int i = 0; i < n; i++)
				for(int j = 0; j < n; j++){
					cost 		= costMat[i][j].cost;
					costRes = costMat[i][j].costRes;
					
					/* 2.Daca costul resursei complementare depaseste bugetul alocat */
					if(cost > bugete[i][j]){
						preturi[i][j] = preturi[i][j] + cost - bugete[i][j];
						bugete[i][j] = cost;
					}
	
					/* 3.Daca costul resursei complementare este sub bugetul alocat */
					else if(cost < bugete[i][j]){
						preturi[i][j] = preturi[i][j] + (cost - bugete[i][j])/2;
						bugete[i][j]  = cost;
					}
					
					/* 4.Daca costul resursei complementare este egal cu bugetul alocat */
					else if(cost == bugete[i][j]){
						preturi[i][j] = costRes + 1;
						bugete[i][j] = cost;
					}
						
					/* 5.Daca pretul devine mai mic decat pretul minim admis */
					if(preturi[i][j] < Pmin)
						preturi[i][j] = Pmin;
					
					/* 6.Daca pretul devine mai mare decat pretul maxim admis */
					if(preturi[i][j] > Pmax){
						resurse[i][j] = 1 - resurse[i][j];
						bugete[i][j] = Pmax;
						preturi[i][j] = (Pmin + Pmax)/2;
					}		
				}
				adaugareRezultat(k, n);
		}
		
		afisareRezultat(T, out);
		/* Afisare ultima matrice */
		for(int i = 0; i < n; i++){
			for(int j = 0; j < n; j++)
				out << "(" << resurse[i][j] << "," << preturi[i][j] << "," << bugete[i][j] << ") ";
			out << endl;	
		}
		
		
		out.close();	
		return 0;

}

