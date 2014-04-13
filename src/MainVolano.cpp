/*
 * MainVolano.cpp
 *
 *  Created on: Apr 4, 2014
 *      Author: francesco
 */
#include <iostream>
#include "mylib/AnalisiDati/VarStat.h"

// se definita, la macro sotto disabilita gli assert. Forse è meglio spostarla
//#define NDEBUG

int main(int argc, char **argv) {
	using namespace std;
#ifdef _MIO_DEBUG_
	std::clog << "_MIO_DEBUG_ definito: debug attivo" << std::endl;
#endif
	using vs = mions::dataAnalisi::VarStat<long double>;
	vs test = vs(12);
	cout << test.getDerivata() << "\n";
	cout << test.getEnnesimoDato(0) << "\n";
	cout << test.getNumeroDatiEffettivo() << "\n";

	return 0;
}
