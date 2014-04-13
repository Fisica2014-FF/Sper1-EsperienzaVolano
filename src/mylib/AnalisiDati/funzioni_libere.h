/*
 * funzioni_libere.h
 *
 *  Created on: Apr 4, 2014
 *      Author: francesco
 */
#include "VarStat.h"

#ifndef FUNZIONI_LIBERE_H_
#define FUNZIONI_LIBERE_H_

namespace mions {

namespace dataAnalisi {
using std::vector;

//Covarianza (non pesata, qualunque cosa voglia dire)
template <typename T>
T covarianza (VarStat<T> v1, VarStat<T> v2) {

	long long numeroDati = v1.getNumeroDatiEffettivo();
	//TODO!! se le variabili varstat sono sintetiche??????
	// Mmmhhhhh..........
	if (v1.getNumeroDatiEffettivo() != v2.getNumeroDatiEffettivo())
		throw "[Errore]: Errore nella covarianza: insiemi di dati di cardinalità diversa";

	long double cov = (v1.getEnnesimoDato(0) - v1.getMedia()) * (v2.getEnnesimoDato(0) - v2.getMedia());
	for (long long i = 0; i < numeroDati; i++) {
		cov = i*(v1.getEnnesimoDato(i) - v1.getMedia()) * (v2.getEnnesimoDato(i) - v2.getMedia()) / (i+1);
	}

	return cov;
}

}

}

#endif /* FUNZIONI_LIBERE_H_ */
