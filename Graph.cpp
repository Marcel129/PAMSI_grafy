#include "Graph.h"

void graph::load(std::string sciezka, int opcja) {
	if (opcja == 0) {
		load_as_list(sciezka);
		czy_lista = 1;
	}
	else if (opcja == 1) {
		load_as_matrix(sciezka);
		czy_lista = 0;
	}
	else {
		std::cerr << "Nie rozpoznano opcji" << std::endl;
		exit(1);
	}
}



void graph::load_as_list(std::string sciezka) {
	/*
	zak³adam, ¿e graf jest w pliku zapisany w postaci:
	ilosc_krawedzi  ilosc_wierzcho³ków  wierzcho³ek_startowy
	W kolejnych liniach znajduja sie definicje krawedzi grafu (trójka danych):
	wierzcho³ek_poczatkowy  wierzcho³ek_koncowy  waga
	*/
	std::fstream plik;
	plik.open(sciezka, std::ios::in);
	if (plik.good()) {
		plik >> liczba_krawedzi >> liczba_wierzcholkow >> wierzcholek_startowy;//wczytaj ilosc_krawedzi ilosc_wierzcho³ków oraz wierzcho³ek_startowy
		wierzcholek_z_pliku tmp_wierz;
		vector<wierzcholek_z_pliku> lista_wierzcholkow_z_pliku;
		while (!plik.eof()) {
			plik >> tmp_wierz.w1 >> tmp_wierz.w2 >> tmp_wierz.waga;//wczytaj jeden wierzcholek
			if (tmp_wierz.w1 == tmp_wierz.w2)continue;
			lista_wierzcholkow_z_pliku.push_back(tmp_wierz);//wrzuc go na liste
		}

		for (const auto& wierz : lista_wierzcholkow_z_pliku) {
			dodaj_wierzcholek_do_listy(lista_wierzcholkow_z_pliku, wierz.w1);
			dodaj_wierzcholek_do_listy(lista_wierzcholkow_z_pliku, wierz.w2);
		}
	}
	else {
		std::cerr << "Blad otwarcia pliku" << std::endl;
		exit(1);
	}
}


void graph::dodaj_wierzcholek_do_listy(const vector<wierzcholek_z_pliku>& lista_wierzcholkow_z_pliku, const int& wierz) {
	bool czy_dodany = false;

	for (const auto& elem : lista_sasiedztwa) {
		if (czy_dodany = elem.wierzcholek == wierz) break;//wyjdz z petli, jesli znajdziesz wierzcholek na liscie sasiedztwa
	}

	//jesli nie ma wierzcholka na liscie, dodajemy go
	if (!czy_dodany) {
		sasiedzi_wierzcholka sas;
		pojedynczy_sasiad_wierzcholka p_sas;
		sas.wierzcholek = wierz;

		for (const auto& elem : lista_wierzcholkow_z_pliku) {
			if (elem.w1 == wierz) {//jesli znajdziesz pasujacy wierzcholek
				p_sas.sasiad = elem.w2;//odczytaj jego sasiada
				p_sas.waga = elem.waga;//i wage polaczenia
				sas.sasiedzi.push_back(p_sas);//i dodaj je na liste sasiedztwa
			}
			else if (elem.w2 == wierz) {//analogicznie dla drugiego wierzcholka z pary
				p_sas.sasiad = elem.w1;
				p_sas.waga = elem.waga;
				sas.sasiedzi.push_back(p_sas);
			}
		}
		lista_sasiedztwa.push_back(sas);
	}
}



void graph::load_as_matrix(std::string sciezka) {
	/*
	zak³adam, ¿e graf jest w pliku zapisany w postaci:
	ilosc_krawedzi  ilosc_wierzcho³ków  wierzcho³ek_startowy
	W kolejnych liniach znajduja sie definicje krawedzi grafu (trójka danych):
	wierzcho³ek_poczatkowy  wierzcho³ek_koncowy  waga
	*/
	std::fstream plik;
	plik.open(sciezka, std::ios::in);//otwieramy plik z grafem do wczytania
	if (!plik.good()) {//jesli nie uda sie otworzyc pliku zakoncz program
		std::cerr << "Blad otwarcia pliku" << std::endl;
		exit(1);
	}
	plik >> liczba_krawedzi >> liczba_wierzcholkow >> wierzcholek_startowy;//wczytaj ilosc_krawedzi ilosc_wierzcho³ków oraz wierzcho³ek_startowy

	wierzcholek_z_pliku tmp_wierz;//zmienne pomocnicze do wczytania danych z pliku
	vector<wierzcholek_z_pliku> lista_krawedzi_z_pliku;
	int j = 0;
	while (!plik.eof()) {//dopóki w pliku s¹ jakieœ wierzcholki
		plik >> tmp_wierz.w1 >> tmp_wierz.w2 >> tmp_wierz.waga;//wczytaj jeden wierzcholek
		if (tmp_wierz.w1 == tmp_wierz.w2) {//pomiñ krawêdzie ³¹cz¹ce wierzcholek z samym soba, nie biora udzia³u w dojsciu do innnego wierzcholka
			++j;//policz, ze pominales krawedz
			continue; 
		}
		lista_krawedzi_z_pliku.push_back(tmp_wierz);//wrzuc go na liste
	}
	liczba_krawedzi = liczba_krawedzi - j;
	vector<shared_ptr<element_listy_wierzcholkow>> lista_wierzcholkow;
	vector<shared_ptr<element_listy_krawedzi>> lista_krawedzi;

	bool czy_pierwszy_jest_na_liscie = false, czy_drugi_jest_na_liscie = false;//dwie zmienne do pamiêtania, czy dany wierzcholek jest juz na liscie wierzcholkow

	for (const auto& elem : lista_krawedzi_z_pliku) {

		czy_pierwszy_jest_na_liscie = false, czy_drugi_jest_na_liscie = false;//zerujemy zmienne

		for (const auto& elem2 : lista_wierzcholkow) {
			//sprawdzamy, czy ktorys z wierzcholkow jest juz wpisany na liste
			if (elem.w1 == elem2->wierzcholek) {//czy wierzcholek pobrany z pliku jest na liscie wierzcholkow
				czy_pierwszy_jest_na_liscie = true;//jesli jest, oznacz, ¿e tak
				break;
			}
		}
		for (const auto& elem2 : lista_wierzcholkow) {
			if (elem.w2 == elem2->wierzcholek) {//czy wierzcholek pobrany z pliku jest na liscie wierzcholkow
				czy_drugi_jest_na_liscie = true;//jesli jest, oznacz, ¿e tak
				break;
			}
		}

		//dodanie brakujacy wierzcholkow na liste wierzcholkow
		if (!czy_pierwszy_jest_na_liscie) {//jezeli pierwszego wierzcholka nie ma na liscie
			element_listy_wierzcholkow tmp_w;
			tmp_w.wierzcholek = elem.w1;//zczytaj dane tego wierzcholka i umiesc go na liscie
			tmp_w.indeks = lista_wierzcholkow.size();
			//std::cout << "indeks " << tmp_w.indeks<<std::endl;
			lista_wierzcholkow.push_back(make_shared<element_listy_wierzcholkow>(tmp_w));
			//lista_wierzcholkow.back()->wskaznik_na_siebie_na_liscie = lista_wierzcholkow.back();
		}
		if (!czy_drugi_jest_na_liscie) {//jezeli drugiego wierzcholka nie ma na liscie
			element_listy_wierzcholkow tmp_w;
			tmp_w.wierzcholek = elem.w2;//zczytaj dane tego wierzcholka i umiesc go na liscie
			tmp_w.indeks = lista_wierzcholkow.size();
			lista_wierzcholkow.push_back(make_shared<element_listy_wierzcholkow>(tmp_w));
		}
	}

	//tworzymy liste krawedzi
	for (const auto& elem : lista_krawedzi_z_pliku) {//przechodzimy po liscie krawedzi wczytanej z pliku
		element_listy_krawedzi tmp_elem;
		for (auto& elem3 : lista_wierzcholkow) {
			if (elem3->wierzcholek == elem.w1) {
				tmp_elem.pierwszy_wierzolek = elem3;
			}
			else if (elem3->wierzcholek == elem.w2) {
				tmp_elem.drugi_wierzcholek = elem3;
			}
		}
		tmp_elem.waga_krawedzi = elem.waga;
		lista_krawedzi.push_back(make_shared<element_listy_krawedzi>(tmp_elem));
	}

	shared_ptr<element_listy_krawedzi> tttt;
	vector<shared_ptr<element_listy_krawedzi>> wek;
	for (int j = 0; j < liczba_wierzcholkow; ++j)
		wek.push_back(nullptr);
	for (int j = 0; j < liczba_wierzcholkow; ++j)
		macierz_sasiedztwa.push_back(wek);
	
	for (auto& elem : lista_krawedzi) {
		macierz_sasiedztwa[elem->pierwszy_wierzolek->indeks][elem->drugi_wierzcholek->indeks] = elem;//wpisz znane krawedzie grafu do macierzy sasiedztwa
		macierz_sasiedztwa[elem->drugi_wierzcholek->indeks][elem->pierwszy_wierzolek->indeks] = elem;
	}
}



void graph::dijkstra(bool czy_wypisac = false) {
	czy_wypisac_liste = czy_wypisac;
	if (czy_lista == 1) dijkstra_lista();//czy_lista = 1 =>graf zaimplementowano jako listê s¹siedztwa
	else if (czy_lista == 0) dijkstra_macierz();//czy_lista = 0 =>graf zaimplementowano jako macierz s¹siedztwa
	else {
		std::cerr << "Nie rozpoznano opcji" << std::endl;
		exit(1);
	}
}



void graph::dijkstra_lista() const {
	vector<najkrotsza_sciezka_do_wierzcholka> lista_najkrotszych_sciezek;
	najkrotsza_sciezka_do_wierzcholka tmp_sc;//zmienna pomocnicza do zainicjowania listy naktorszych sciezek

	std::priority_queue <pojedynczy_sasiad_wierzcholka, vector<pojedynczy_sasiad_wierzcholka>, porownanie_kosztu_wierzcholkow > wierzcholki_do_sprawdzenia;

	for (const auto& elem : lista_sasiedztwa) {
		tmp_sc.koszt_calkowity = INF;//przypisz nieodwiedzonym wierzcholkom wartosc sciezki rowna nieskonczonosc
		tmp_sc.wierzcholek = elem.wierzcholek;//zczytaj nazwe wierzcholka

		if (elem.wierzcholek == wierzcholek_startowy) {//dla wierzcholka starowego koszt=0
			pojedynczy_sasiad_wierzcholka sas;
			tmp_sc.koszt_calkowity = 0;//startowy koszt dojscia ustaw jako 0
			sas.sasiad = tmp_sc.wierzcholek;
			sas.waga = tmp_sc.koszt_calkowity;
			wierzcholki_do_sprawdzenia.push(sas);//wrzuc wierzcholek startowy do kolejki
		}
		lista_najkrotszych_sciezek.push_back(tmp_sc);//dodaj wierzcholek do listy
	}

	pojedynczy_sasiad_wierzcholka zdjety_wierzcholek;
	vector<pojedynczy_sasiad_wierzcholka> sasiedzi_sprawdzanego_wierzcholka;
	vector<int> lista_odwiedzonych_wierzcholkow;
	bool odwiedzony = false;

	while (!wierzcholki_do_sprawdzenia.empty()) {

		zdjety_wierzcholek = wierzcholki_do_sprawdzenia.top();//odczytaj wartosc wierzcholka o najnizszej wadze
		wierzcholki_do_sprawdzenia.pop();//wyrzuc go z kolejki

		for (const int& elem : lista_odwiedzonych_wierzcholkow) {
			if (odwiedzony = elem == zdjety_wierzcholek.sasiad) break;//sprawdz, czy zdjety wierzcholek zostal odwiedzony
		}

		if (!odwiedzony) {//sprawdzamy, czy zdjety wierzcholek nie zostal juz odwiedzony
			for (const auto& elem : lista_sasiedztwa) {
				if (elem.wierzcholek == zdjety_wierzcholek.sasiad) {//odczytujemy sasiadow sprawdzanego wierzcholka z listy sasiedztwa 
					sasiedzi_sprawdzanego_wierzcholka = elem.sasiedzi;//zapamietujemy je w zmiennej "sasiedzi_sprawdzanego_wierzcholka"
					break;//po znalezieniu wierzcholka wychodzimy z petli
				}
			}

			for (const auto& elem2 : sasiedzi_sprawdzanego_wierzcholka) {//przechodzimy po liscie sasiadow sprawdzanego wierzcholka
				for (auto& elem : lista_najkrotszych_sciezek) {//szukamy ich na liscie z najkrotszymi siezkami do wierzcholkow 
					if (elem.wierzcholek == elem2.sasiad) {//jezeli go znajdziemy 
						if (elem.koszt_calkowity > elem2.waga + zdjety_wierzcholek.waga) {//jesli aktualny koszt dojscia > 
							//koszt dojscia do aktualnie sprawdzanego wierzcholka + waga jego polaczenia z aktualnie sprawdzanym wierzcholkiem

							elem.koszt_calkowity = elem2.waga + zdjety_wierzcholek.waga;//poprawiamy koszt dojscia
							elem.wierzcholki_posrednie.push_back(zdjety_wierzcholek.sasiad);//zapamietujemy wierzcholek poprzedzacy wierzcholek, do ktorego sciezke wlasnie poprawilismy

							pojedynczy_sasiad_wierzcholka tmp_s;//kopiuje dane sprawdzanego wierzcholka
							tmp_s.waga = elem.koszt_calkowity;
							tmp_s.sasiad = elem.wierzcholek;
							wierzcholki_do_sprawdzenia.push(tmp_s);//wrzucam je na liste wierzcholkow do sprawdzenia
						}
					}
				}
			}
			lista_odwiedzonych_wierzcholkow.push_back(zdjety_wierzcholek.sasiad);//wrzuc zdjety wierzcholek na liste odwiedzonych wierzcholkow
			odwiedzony = false;//reset zmiennej sprawdzajcej, czy wierzcholek zostal odwiedzony
		}
	}
	if (czy_wypisac_liste) {//wypisanie do pliku listy najkrotszych sciezek
		std::fstream lista_sc;
		lista_sc.open("D:\\Studia AiR\\Sem 4\\PAMSI\\PAMSI_P2\\najkrotsze_sciezki.txt", std::ios::app);
		lista_sc << "LISTA SASIEDZTWA\n"<<"Wierzcholek startowy: " << wierzcholek_startowy << "\n";
		for (const auto& elem : lista_najkrotszych_sciezek) {
			lista_sc << "Do: " << elem.wierzcholek << "  Koszt: " << elem.koszt_calkowity << "   Wierzcholki posrednie: ";
			for (const auto& elem2 : elem.wierzcholki_posrednie)
				lista_sc << elem2 << " ";
			lista_sc<<"\n";
		}
	}
}



void graph::dijkstra_macierz()const {
	vector<najkrotsza_sciezka_do_wierzcholka> lista_najkrotszych_sciezek;
	najkrotsza_sciezka_do_wierzcholka tmp_sc1, tmp_sc2;//zmienna pomocnicza do zainicjowania listy naktorszych sciezek
	bool czy_pierwszy_na_liscie = false, czy_drugi_na_liscie = false;

	std::priority_queue <pojedynczy_sasiad_wierzcholka, vector<pojedynczy_sasiad_wierzcholka>, porownanie_kosztu_wierzcholkow > wierzcholki_do_sprawdzenia;

	for (const auto& elem2 : macierz_sasiedztwa) {
		for (const auto& elem : elem2) {
			if (elem == nullptr) continue;//jesli polaczenie nie istnieje (wiec nie mozna dostac sie do wierzcholka) pomin krok
			
			tmp_sc1.koszt_calkowity = INF;//przypisz nieodwiedzonym wierzcholkom wartosc sciezki rowna nieskonczonosc
			tmp_sc2.koszt_calkowity = INF;//przypisz nieodwiedzonym wierzcholkom wartosc sciezki rowna nieskonczonosc

			czy_pierwszy_na_liscie = false;//reset obecnosci na liscie
			czy_drugi_na_liscie = false;//reset obecnosci na liscie

			for (const auto& elem3 : lista_najkrotszych_sciezek) {//przejdz po liscie najktorszych sciezek
				if (elem3.wierzcholek == elem->pierwszy_wierzolek->wierzcholek && !czy_pierwszy_na_liscie) //sprawdz, czy sprawdzany wierzcholek jest juz tam jest
					czy_pierwszy_na_liscie = true;//jesli tak, zapamietaj to
				
				if (elem3.wierzcholek == elem->drugi_wierzcholek->wierzcholek && !czy_drugi_na_liscie) //analogicznie dla drugiego wierzcholka
					czy_drugi_na_liscie = true;
				
				if (czy_drugi_na_liscie && czy_pierwszy_na_liscie) break;//jesli ktorys z wierzcholkow jest na liscie wyjdz z petli
			}

			if (!czy_pierwszy_na_liscie) {//jesli pierwszego wierzcholka nie ma na liscie
				tmp_sc1.wierzcholek = elem->pierwszy_wierzolek->wierzcholek;//zczytaj nazwe wierzcholka
				//jesli mozna dostac sie do danego wierzcholka i nie ma go jeszcze na liscie
				if (elem->pierwszy_wierzolek->wierzcholek == wierzcholek_startowy) {//dla wierzcholka starowego koszt=0
					pojedynczy_sasiad_wierzcholka sas;
					tmp_sc1.koszt_calkowity = 0;
					sas.sasiad = tmp_sc1.wierzcholek;
					sas.waga = tmp_sc1.koszt_calkowity;
					wierzcholki_do_sprawdzenia.push(sas);//wrzuc wierzcholek startowy do kolejki
				}
				lista_najkrotszych_sciezek.push_back(tmp_sc1);//dodaj wierzcholek do listy
			}

			if (!czy_drugi_na_liscie) {//jesli drugiego wierzcholka nie ma na liscie
				tmp_sc2.wierzcholek = elem->drugi_wierzcholek->wierzcholek;//zczytaj nazwe wierzcholka
				//jesli mozna dostac sie do danego wierzcholka i nie ma go jeszcze na liscie
				if (elem->drugi_wierzcholek->wierzcholek == wierzcholek_startowy) {//dla wierzcholka starowego koszt=0
					pojedynczy_sasiad_wierzcholka sas;
					tmp_sc2.koszt_calkowity = 0;
					sas.sasiad = tmp_sc2.wierzcholek;
					sas.waga = tmp_sc2.koszt_calkowity;
					wierzcholki_do_sprawdzenia.push(sas);//wrzuc wierzcholek startowy do kolejki
				}
				lista_najkrotszych_sciezek.push_back(tmp_sc2);//dodaj wierzcholek do listy
			}
		}
	}

	pojedynczy_sasiad_wierzcholka zdjety_wierzcholek;
	vector<pojedynczy_sasiad_wierzcholka> sasiedzi_sprawdzanego_wierzcholka;
	vector<int> lista_odwiedzonych_wierzcholkow;
	bool odwiedzony = false;

	while (!wierzcholki_do_sprawdzenia.empty()) {

		zdjety_wierzcholek = wierzcholki_do_sprawdzenia.top();//odczytaj wartosc wierzcholka o najnizszej wadze
		wierzcholki_do_sprawdzenia.pop();//wyrzuc go z kolejki

		for (const int& elem : lista_odwiedzonych_wierzcholkow) {
			if (odwiedzony = elem == zdjety_wierzcholek.sasiad) break;//sprawdz, czy zdjety wierzcholek zostal odwiedzony
		}

		if (!odwiedzony) {//jesli zdjety wierzcholek nie zostal juz odwiedzony
			
			for (const auto& el : macierz_sasiedztwa) {
				for (const auto& elem : el) {//przechodzimy po znalezionej kolumnnie macierzy sasiedztwa
					if (elem != nullptr) {
						if (elem->pierwszy_wierzolek->wierzcholek == zdjety_wierzcholek.sasiad) {
							pojedynczy_sasiad_wierzcholka tmp_sas;
							tmp_sas.sasiad = elem->drugi_wierzcholek->wierzcholek;
							tmp_sas.waga = elem->waga_krawedzi;
							sasiedzi_sprawdzanego_wierzcholka.push_back(tmp_sas);//zapamietujemy je w zmiennej "sasiedzi_sprawdzanego_wierzcholka"
						}
						if (elem->drugi_wierzcholek->wierzcholek == zdjety_wierzcholek.sasiad) {
							pojedynczy_sasiad_wierzcholka tmp_sas;
							tmp_sas.sasiad = elem->pierwszy_wierzolek->wierzcholek;
							tmp_sas.waga = elem->waga_krawedzi;
							sasiedzi_sprawdzanego_wierzcholka.push_back(tmp_sas);//zapamietujemy je w zmiennej "sasiedzi_sprawdzanego_wierzcholka"
						}
					}
				}
			}
			for (const auto& elem2 : sasiedzi_sprawdzanego_wierzcholka) {//przechodzimy po liscie sasiadow sprawdzanego wierzcholka
				for (auto& elem : lista_najkrotszych_sciezek) {//szukamy ich na liscie z najkrotszymi siezkami do wierzcholkow 
					if (elem.wierzcholek == elem2.sasiad) {//jezeli go znajdziemy 
						if (elem.koszt_calkowity > elem2.waga + zdjety_wierzcholek.waga) {//jesli aktualny koszt dojscia > 
							//koszt dojscia do aktualnie sprawdzanego wierzcholka + waga jego polaczenia z aktualnie sprawdzanym wierzcholkiem

							elem.koszt_calkowity = elem2.waga + zdjety_wierzcholek.waga;//poprawiamy koszt dojscia
							elem.wierzcholki_posrednie.push_back(zdjety_wierzcholek.sasiad);//zapamietujemy wierzcholek poprzedzacy wierzcholek, do ktorego sciezke wlasnie poprawilismy

							pojedynczy_sasiad_wierzcholka tmp_s;//kopiuje dane sprawdzanego wierzcholka
							tmp_s.waga = elem.koszt_calkowity;
							tmp_s.sasiad = elem.wierzcholek;
							wierzcholki_do_sprawdzenia.push(tmp_s);//wrzucam je na liste wierzcholkow do sprawdzenia
						}
					}
				}
			}
			lista_odwiedzonych_wierzcholkow.push_back(zdjety_wierzcholek.sasiad);//wrzuc zdjety wierzcholek na liste odwiedzonych wierzcholkow
			odwiedzony = false;//reset zmiennej sprawdzajcej, czy wierzcholek zostal odwiedzony
		}
	}
	
	if (czy_wypisac_liste) {//wypisanie do pliku listy najkrotszych sciezek
		std::fstream lista_sc;
		lista_sc.open("D:\\Studia AiR\\Sem 4\\PAMSI\\PAMSI_P2\\najkrotsze_sciezki.txt", std::ios::app);
		lista_sc <<"MACIERZ SASIEDZTWA:\n" <<"Wierzcholek startowy: " << wierzcholek_startowy << "\n";
		for (const auto& elem : lista_najkrotszych_sciezek) {
			lista_sc << "Do: " << elem.wierzcholek << "  Koszt: " << elem.koszt_calkowity << "   Wierzcholki posrednie: ";
			for (const auto& elem2 : elem.wierzcholki_posrednie)
				lista_sc << elem2 << " ";
			lista_sc << "\n";
		}
	}
	
}






void graph::display()const {//wyswietla graf w postaci listy sasiedztwa
	if (czy_lista == 1) {
		std::cout << "Lista sasiedztwa:" << std::endl;
		for (const auto& elem : lista_sasiedztwa) {
			std::cout << elem.wierzcholek << ":";
			for (const auto& elem2 : elem.sasiedzi) {
				std::cout << elem2.sasiad << "->" << elem2.waga << ",";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
	else if (czy_lista == 0) {

		std::cout << "Macierz sasiedztwa" << std::endl;
		for (const auto& elem : macierz_sasiedztwa) {
			for (const auto& elem2 : elem) {
				elem2 == nullptr ? std::cout << "0" : std::cout << elem2->waga_krawedzi;
				std::cout << " ";
			}
			std::cout << std::endl;
		}
	}
	else {
		std::cout << "nie rozpoznano opcji" << std::endl;
		exit(1);
	}

}