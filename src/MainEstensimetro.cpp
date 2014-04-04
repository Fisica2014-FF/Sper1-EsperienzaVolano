/*
 * MainViscosimetro.cpp
 *
 *  Created on: Mar 26, 2014
 *      Author: Andrea Piccinin
 */


#include <iostream>
#include <cmath>

struct dato {
	double media;
	double errore;
};

dato formula(double, double, double );

int main(int numparam, char* args[]) {
	using namespace std;

	double diametri[] = {
		0.0015,
		0.0016,
		0.002,
		0.0024,
		0.0032,
		0.004,
		0.0048,
		0.0056,
		0.0064,
		0.0072
	};

	double velocita[] = {
		0.00207282,
		0.002431,
		0.00376976,
		0.00553234,
		0.0101183,
		0.016432,
		0.0251473,
		0.0387838,
		0.0580847,
		0.0943861,
	};

	double E_velocita[] = {
		6.67E-007,
		8.87E-007,
		1.36E-006,
		8.16E-006,
		2.05E-005,
		5.07E-005,
		4.47E-005,
		6.33E-005,
		0.000422872,
		0.000318812,
	};

	cout << "test: \n";
	for (int i = 0; i < 10; ++i) {
		dato e1 = formula(velocita[i],E_velocita[i],diametri[i]);
		cout << e1.media << " +- " << e1.errore << endl;
	}

	cout << endl;
	for (int i = 0; i < 10; ++i) {
		dato e1 = formula(velocita[i],E_velocita[i],diametri[i]);
		cout << e1.media << "	" << e1.errore << endl;
	}

}

//d = diametro
dato formula(double velocita, double E_velocita, double d ) {
	using std::sqrt;
	using std::pow;
	double delRho = 6838;
	double E_rho = 6;
	dato risultato;
	double g = 9.806;
	double E_g = 0.001;

	double E_diam = 0.00001;
	risultato.media = pow(d,2) * g * delRho /  (18 * velocita) ;
	risultato.errore = sqrt(
			pow( (2*d*g*delRho   / (18*velocita))           * E_diam ,2) +
			pow( (2*d*d*delRho   / (18*velocita))           * E_g,2) +
			pow( (2*d*d*g        / (18*velocita))           * E_rho ,2) +
			pow( (2*d*d*g*delRho / (18*velocita*velocita))  * E_velocita ,2)
	);

	return risultato;
}
