/*
 * File_Fdat.h
 *
 *  Created on: Apr 4, 2014
 *      Author: francesco
 */

#ifndef FILE_FDAT_H_
#define FILE_FDAT_H_

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "../utils/NomeDiFile.h"

namespace mions {

//Classi per l'analisi dei dati statistici
namespace dataAnalisi {
using std::vector;


//Aggiunta classe che legge i file in formato fdat
//L'interfaccia è con la notazione "fd[indice]": gli oggetti della classe File_Fdat,
//se l'indice è un numero i, ritornano l'i-esima riga come vettore
//(Naturalmente in questo caso per righe si ignorano commenti e metadati, che iniziano rispettivamente con # e #%).
//Quindi si comportano come matrici (array multidimensionali) indicizzate da numeri, che contengono le colonne di dati.
//Se invece l'indice è una stringa, recupera i metadati associati a quel tag.
template <class T>
class File_Fdat {
	//std::string formato;

	// Leggi una riga alla volta, se è un commento saltala, altrimenti prosegui:
	// 		se è un metadato aggiungilo alla mappa, sennò memorizza la riga di numeri
	/* Esempi di metadati:
	 * #%MATERIALE:2
	 * #%LUNGHEZZA:500
	 *
	 * Inoltre memorizza la matrice dei dati
	 *
	 */

	//Scoprire perchè chiamare il costruttore dall'altro costruttore non funziona

private:
	File_Fdat(std::string nomeFile) {
		using namespace std;
		ifstream file_form;
		string rigamd;

		stringstream sriga;
		stringstream sindice;

		string indice;
		long double dvalore;
		char temp;


		clog << nomeFile << endl;
		clog << "E fin qua...\n";
		file_form.open(nomeFile.c_str());
		if (!(file_form.is_open()))
			throw string("[Errore]: file di dati non aperto") + nomeFile;


		while (getline(file_form,rigamd)) {
			//Controlla se i commenti sono tag
			//Leggi i tag (inizia a (0) e prendi (1) carattere)
			if (rigamd.substr(0,1) == "#") {
				//Se una linea inizia con #% è un metadato, della forma #%(string)INDICE:(long double)DATO
				if (rigamd.substr(0,2) == "#%") {
					sriga << rigamd; //Scrivi nello stringstream la stringa, per estrarci le varie robe
					sriga >> temp >> temp; //Togli #%

					sriga >> temp;
					while ( temp !=  ':' ) {
						sindice << temp;
						//Prendi un nuovo carattere
						sriga >> temp;
					};
					//I caratteri rimanenti sono il valore
					sriga >> dvalore;
					//metti lo stringstream dentro la stringa
					sindice >> indice;
					MetaDatiGenerici[indice] = dvalore;
				} else
					continue;
			//Altrimenti leggi i numeri
			} else {
				sriga << rigamd; //Scrivi nello stringstream la stringa, per estrarci la riga di dati
				#ifdef _MIO_DEBUG_
				clog << rigamd << "\n";
				#endif

//04/04/2014
				vector<T> tempvect;
				T tempnum;
				while (sriga)
				{
					sriga >> tempnum;
					// If perchè sennò, arrivato alla fine, cerca di estrarre un numero, fallisce e quindi pusho
					// di nuovo in tempvect l'ultimo numero
					if (sriga)
						tempvect.push_back(tempnum);
				}

				vColDati.push_back(tempvect);
				//r=r+1;
			}
		sriga.clear();
		sindice.clear();
		}
	}
	std::map<std::string, long double> MetaDatiGenerici;
	vector< vector<T> > vColDati;//Matrice dei dati: un vettore di puntatori a dei vettori
public:

	File_Fdat(mions::utils::NomeDiFile nomeFile) : File_Fdat(nomeFile.pathTotale) { };

	//Se l'indice è un numero n, ritorna l'n-esima riga della matrice di numeri
	inline std::vector<T> operator[](long indice) {
		//At lancia una out_of_range_exception se si mette un indice troppo grande o troppo piccolo
		return vColDati.at(indice);
	}

	//Se l'indice è una stringa, ritorna il metadato associato nella map
	inline long double operator[](std::string stringaindice){


		auto iterat = MetaDatiGenerici.find(stringaindice);
		if (iterat != MetaDatiGenerici.end())
			return iterat->second;
		else
			throw std::string("[Errore]: usato indice non esistente: ") + stringaindice;
	}

};

}//Fine dataAnalisi

}//Fine mions

#endif /* FILE_FDAT_H_ */
