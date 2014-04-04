/*
 * TabellaLatex.h
 *
 *  Created on: Mar 17, 2014
 *      Author: francesco
 */
#include <string>

#ifndef TABELLALATEX_H_
#define TABELLALATEX_H_

//Il mio namespace
namespace mions {

namespace latexlib{

//Classe base
/* I dati sono dei file divisi in colonne, suddivisi per gruppi.
 * Esempio:
 * _________Acciaio__________ | _______Ottone________ |
 * __File1___  ____File2_____ | __File3___ ___File4__ |
 *   12+-34	     34+-5.678        9+-1.1    12.13+-14
 *   etc...
 */
class TabularLatex {
	using std::string;
public:
	//string titolo; //Titolo della tabella, idealmente dovrebbe decidere lei dove e come metterlo
	//string caption; //Casomai dovessimo differenziare
	TabularLatex
	virtual ~TabellaLatex();
private:
};

}

}

#endif /* TABELLALATEX_H_ */
