#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <Windows.h>
#include <ctime>
#include <string>
#include "Graph.h"
using namespace std;

__int64 currentTimeMillis()
{
	static const __int64 magic = 116444736000000000; // 1970/1/1
	SYSTEMTIME st;
	GetSystemTime(&st);
	FILETIME   ft;
	SystemTimeToFileTime(&st, &ft); // in 100-nanosecs...
	__int64 t;
	memcpy(&t, &ft, sizeof t);
	return (t - magic) / 10; // scale to millis., if 10 - micros
}

int suma_liczb(int n)
{
	if (n == 0) return 1;
	else return n + suma_liczb(n - 1);
}

void generuj_graf(int liczba_wierz, int gestosc, string plik_wyj) {

	int max_gest = suma_liczb(liczba_wierz)-1;
	int liczba_kraw = (gestosc *max_gest)/100;
	vector<int> wierzcholki;

	for (int i = 0; i < liczba_wierz; ++i) 
		wierzcholki.push_back(i);
	
	std::vector<wierzcholek_z_pliku> w_do_pliku;
	wierzcholek_z_pliku pojedynczy_wierzcholek;
	bool czy_krawedz_istnieje = false;
	int j = 1;
	for (int i = 0; i < liczba_kraw; ++i) {
		++j;
		czy_krawedz_istnieje = false;
		pojedynczy_wierzcholek.w1 = j % liczba_wierz;
		pojedynczy_wierzcholek.w2 = ((std::rand()+ wierzcholki.size()) % wierzcholki.size());
		pojedynczy_wierzcholek.waga = (std::rand()+501) % 500;

		if (pojedynczy_wierzcholek.w1 != pojedynczy_wierzcholek.w2) {
			for (const auto& elem : w_do_pliku) {
				if ((elem.w1 == pojedynczy_wierzcholek.w1 && elem.w2 == pojedynczy_wierzcholek.w2) || (elem.w1 == pojedynczy_wierzcholek.w2 && elem.w2 == pojedynczy_wierzcholek.w1)) {
					--i;
					czy_krawedz_istnieje = true;
					srand(currentTimeMillis());
					//std::cout << i <<" "<< pojedynczy_wierzcholek.w1 << " "<< pojedynczy_wierzcholek.w2 << std::endl;
					break;
				}
			}
			if (!czy_krawedz_istnieje) {
				//std::cout << "dodaje krawedz" << std::endl;
				w_do_pliku.push_back(pojedynczy_wierzcholek);
			}
		}
		else
		{
			--i;
			srand(currentTimeMillis());
		}
	}
	std::fstream plik;
	plik.open(plik_wyj, std::ios::out);//otwieramy plik z grafem do wczytania
	if (!plik.good()) {//jesli nie uda sie otworzyc pliku zakoncz program
		std::cerr << "Blad otwarcia pliku" << std::endl;
		exit(1);
	}
	plik << liczba_kraw <<" "<< liczba_wierz <<" "<< w_do_pliku[0].w1<<"\n\n";
	for (const auto& elem : w_do_pliku)
		plik << elem.w1 << " " << elem.w2 << " " << elem.waga << "\n";
}

int main() {
	/*
	// generowanie grafów
	srand(currentTimeMillis());
	vector<int> liczba_wierzch = { 5, 10, 15, 20, 25 };
	vector<int> gestosci = { 25,50,75,100 };
	string sciezka_baza = "D:\\Studia AiR\\Sem 4\\PAMSI\\PAMSI_P2\\grafy_do_badan\\graf", sciezka;
	for (const auto& elem:liczba_wierzch) {
		for (const auto& elem2 : gestosci) {
			for (int i = 1; i <= 100; ++i) {
				sciezka = sciezka_baza + "_" + to_string(elem) + "_" + to_string(elem2) + "_" + to_string(i) + ".txt";
				generuj_graf(elem, elem2, sciezka);
			}
		}
	}
	*/

    
	string sciezka_wyniki = "D:\\Studia AiR\\Sem 4\\PAMSI\\PAMSI_P2\\wyniki_pomiarow.txt";
	string sciezka_baza = "D:\\Studia AiR\\Sem 4\\PAMSI\\PAMSI_P2\\grafy_do_badan\\graf", sciezka;
	vector<int> liczba_wierzch{ 5, 10, 15, 20, 25 };
	vector<int> gestosci{ 25,50,75,100 };
	std::fstream plik_wyj;
	plik_wyj.open(sciezka_wyniki, std::ios::app);//otwieramy plik z wynikami

	if (plik_wyj.good()) {//jesli nie uda sie otworzyc pliku zakoncz dzialanie programu
		plik_wyj << "Ilosc wierzcholkow  wypelnienie  lista  macierz \n";
		plik_wyj.close();
	}
	else {
		std::cerr << "Blad otwarcia pliku" << std::endl;
		exit(1);
	}
	__int64 czas_start_lista, czas_dzialania_lista=0, czas_start_macierz, czas_dzialania_macierz=0;

	for (const auto& elem : liczba_wierzch) {
		for (const auto& elem2 : gestosci) {
			for (int i = 1; i <= 100; ++i) {
				graph graf, graf2;
				sciezka = sciezka_baza + "_" + to_string(elem) + "_" + to_string(elem2) + "_" + to_string(i) + ".txt";//z³o¿enie sciezki do nastepnego pliku
				
				graf.load(sciezka, 0);//zaladuj graf jako liste sasiedztwa
				
				czas_start_lista = currentTimeMillis();//poczatek pomiaru czasu
				graf.dijkstra(true);//wywolanie algorytmu dikstry dla grafu jako listy
				czas_dzialania_lista += currentTimeMillis() - czas_start_lista;//dodaj czas przetwarzania pojedynczego grafu

				graf2.load(sciezka, 1);//zaladuj graf jako macierz sasiedztwa
				

				czas_start_macierz = currentTimeMillis();
				graf2.dijkstra(true);//wywolanie algorytmu dikstry dla grafu jako macierzy
				czas_dzialania_macierz += currentTimeMillis() - czas_start_macierz;	

			}
			
			plik_wyj.open(sciezka_wyniki, std::ios::app);//otwieramy plik z wynikami

			if (!plik_wyj.good()) {//jesli nie uda sie otworzyc pliku zakoncz dzialanie programu
				std::cerr << "Blad otwarcia pliku" << std::endl;
				exit(1);
			}
			//zapisz wyniki do pliku
			plik_wyj  << elem << " " << elem2 << " " << czas_dzialania_lista / 100
				<< " " << czas_dzialania_macierz / 100 << "\n";
			plik_wyj.close();//zamknij plik
		}
	}
}

