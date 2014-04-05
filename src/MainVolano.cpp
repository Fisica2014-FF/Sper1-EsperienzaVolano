/*
 * MainVolano.cpp
 *
 *  Created on: Apr 4, 2014
 *      Author: francesco
 */
#include <iostream>

// se definita, la macro sotto disabilita gli assert. Forse è meglio spostarla
//#define NDEBUG

int main(int argc, char **argv) {

#ifdef _MIO_DEBUG_
	std::clog << "_MIO_DEBUG_ definito: debug attivo" << std::endl;
#else

#endif



	return 0;
}
