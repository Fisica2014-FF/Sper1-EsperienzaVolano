/*
 * errore_reciproco.cxx
 *
 *  Created on: Mar 31, 2014
 *      Author: andrea
 */
#include <iostream>

using namespace std;

int main()
{
	double err = 0.1;
	double val = 0;
	double err_rec = 0;
	int i = 1;
	while (cin >> val) {
		err_rec = i * 2 *3.14 * err / (val * val);
		cout << err_rec << endl;
		i++;
	}
	return 0;
}



