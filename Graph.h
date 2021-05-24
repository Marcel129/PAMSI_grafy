#pragma once
#ifndef GRAPH_H
#define GRAPH_H

#define INF 4294967295//symulator nieskonczonosci, maksymalna wartosc unsigned int

#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <array>
#include <memory>

using std::vector;
using std::shared_ptr;
using std::make_shared;

struct wierzcholek_z_pliku {//struktura potrzebna jedynie do wczytania danych z pliku i stworzenia listy lub macierzy sasiedztwa
	int w1, w2;
	unsigned int waga;
};

struct pojedynczy_sasiad_wierzcholka {
	int sasiad;
	unsigned int waga;
};

struct sasiedzi_wierzcholka {
	int wierzcholek;
	vector<pojedynczy_sasiad_wierzcholka> sasiedzi;
};

struct najkrotsza_sciezka_do_wierzcholka{
	int wierzcholek;
	unsigned int koszt_calkowity;
	vector<int> wierzcholki_posrednie;
};

struct element_listy_wierzcholkow {
	//shared_ptr<element_listy_wierzcholkow> wskaznik_na_siebie_na_liscie;
	unsigned int indeks;
	int wierzcholek;
};

struct element_listy_krawedzi {
	shared_ptr<element_listy_wierzcholkow> pierwszy_wierzolek;
	shared_ptr<element_listy_wierzcholkow> drugi_wierzcholek;
	//shared_ptr<element_listy_krawedzi> wskaznik_na_siebie_na_liscie;
	int waga_krawedzi;

};


struct porownanie_kosztu_wierzcholkow{
	bool operator()(const pojedynczy_sasiad_wierzcholka &w1, const pojedynczy_sasiad_wierzcholka &w2) {	return w1.waga > w2.waga;}
};

class graph {

	int czy_lista;//jesli 1 graf jest w formie listy sasiedztwa, jesli 0 - macierzy sasiedztwa
	bool czy_wypisac_liste;
	unsigned int liczba_wierzcholkow;
	unsigned int liczba_krawedzi;
	int wierzcholek_startowy;
	vector<sasiedzi_wierzcholka> lista_sasiedztwa;
	vector<vector<shared_ptr<element_listy_krawedzi>>> macierz_sasiedztwa;
	vector<najkrotsza_sciezka_do_wierzcholka> lista_najkrotszych_sciezek_2;

	void dodaj_wierzcholek_do_listy(const vector<wierzcholek_z_pliku> &,const int&);
	void dijkstra_lista()const;
	void dijkstra_macierz()const;
	void load_as_list(std::string);
	void load_as_matrix(std::string);
public:
	void load(std::string, int);//³aduje graf z pliku ze wskazanej sciezki
	void display() const;
	void dijkstra(bool) ;
};


#endif // !GRAPH_H

