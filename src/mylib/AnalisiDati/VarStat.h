
#pragma once//L'equivalente delle Include guards

#include <cmath>
#include <vector>
#include <algorithm>
#include <memory>//Shared_ptr
#include <limits>
#include <string>
#include <map>
#include <cmath>
#include <unordered_map>
//#include <iostream>
#include <fstream>
#include <sstream>

#include <cassert>

#include "../utils/NomeDiFile.h"
#include "./File_Fdat.h"
#include "./funzioni_libere.h" //Covarianza

#ifdef _MIO_DEBUG_

#include <iostream>//Per cerr

//Stampa il nome della variabile
#define VNAME(x) #x
#define VDUMP(x) std::clog << #x << " " << x << std::endl

#endif


//Il mio namespace
namespace mions {

//Classi per l'analisi dei dati statistici
namespace dataAnalisi {
using std::vector;
//using std::shared_ptr;

///////////////////////
//					 //
//	  VERSIONE 3.0	 //
//					 //
///////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
/*
 * Forward declarations
 */
template <class> class VarStat;//Forward declaration da usare nella funzione operator<<

template <class T> const VarStat<T> operator*(const long double& , const VarStat<T> );
template <class T> const VarStat<T> operator*(const VarStat<T> , const long double& );
template <class T> const VarStat<T> potcombo(const VarStat<T>& , const long double , const VarStat<T>& , const long double );

///////////////////////////////////////////////////////////////////////////////////////////////////
//Versione base, per stampare rapidamente
template <typename U>
std::ostream& operator <<(std::ostream& os, const VarStat<U>& rhs) {
	using namespace std;

	//Eclipse dà problemi con endl, modifichiamolo temporaneamente
	//Qui non serve
	//#define endl "\n"
	os << "Media +- sigmaMedia:               " << rhs.getMedia() << " +- " << rhs.getErroreMedia() << endl;
	//#undef endl
	return os;
}

//Moltiplicazione a destra per uno scalare
template <typename U>
inline const VarStat<U> operator*(const VarStat<U> lhs, const long double& rhs) {
	VarStat<U> result = lhs; // Copia il primo oggettp
	result *= rhs;            // Aggiungici dentro l'altro
	return result;              // Ritorna il risultato
}

//Moltiplicazione a sinistra per uno scalare
template <typename U>
inline const VarStat<U> operator*(const long double& lhs, const VarStat<U> rhs) {
	return (rhs*lhs);
}






///////////////////////////////////////////////////////////////////////////////////////////////////
// Classe per l'analisi di UNA variabile statistica offline, completammente riscritta.
template <class T>
class VarStat {



//Campi
private:

	long double dMedia = -INFINITY;
	//long double dDeviazioneStandardPopo = -INFINITY;
	long double dErroreMedia = -INFINITY;
	long iNumero_dati = 0;
	vector<T> dati;
	vector<T> errori;
	long double derivata;



public:
	//vector<T> vectDati;
	//Funzioni overloaded
	friend std::ostream& operator<<<T>(std::ostream& , const VarStat<T>& );
	friend const VarStat<T> operator*<T>(const VarStat<T> , const long double& );
	friend const VarStat<T> operator*<T>(const long double& , const VarStat<T> );
	//TOD potcombo
	//friend const VarStat<T> potcombo(const VarStat<T>& , const long double , const VarStat<T>& , const long double );

	//
	VarStat(T valore) {
		iNumero_dati = 1;
		dMedia = (long double)valore;
		//long double dDeviazioneStandardPopo = 0;
		dErroreMedia = 0;

		//Serve svuotarli?
		if ( !errori.empty() )
			errori.clear();// Svuota l'array degli errori
		if ( !dati.empty() )
			dati.clear();// Svuota l'array dei dati

		//Metti un dato solo com
		dati.push_back(valore);
		errori.push_back(0.0);

		//La derivata di un valore puro è come quella di una funzione costante
		derivata = 0;
	}

	//ATTENZIONE! Se numDati è più d'uno, ricordarsi di passare l'errore dei dati e non quello della media
	VarStat(T valoreMedio, long double DevStdPop, long numDati = 1) {
		using std::sqrt;
		if (DevStdPop < 1e-14)
			throw "Errore: usato errore nullo";
		iNumero_dati = numDati;
		dMedia = (long double)valoreMedio;
		//dDeviazioneStandardPopo = DevStdPop;
		dErroreMedia = DevStdPop / sqrt(numDati);
		if ( !errori.empty() )
			errori.clear();// Svuota l'array degli errori
		if ( !dati.empty() )
			dati.clear();// Svuota l'array dei dati

		//Riempi i dati con copie del valore medio con errore
		for (int i = 0; i < numDati; ++i) {
			dati.push_back(valoreMedio);
			errori.push_back(DevStdPop);
		}


		//La derivata di un valore solo ma con errore è una variabile x con derivata (d/dx)x = 1
		derivata = 1;
	}

	//TODO: Pensarci su
//	VarStat(mions::utils::NomeDiFile nomeDiFile) {
//		if (nomeDiFile.estensione != "fdat") {
//			throw "[Errore]: Usato un file non fdat, l'estensione non è corretta";
//		}
//		VarStat(dati, true);
//	}


	//Costruttore
	VarStat(const vector<T>& aDati, bool eliminaTreSigma = true) {
		using std::vector;
		using std::abs;
		using std::sqrt;
		//dati = {1,2,3};//La classe ha una copia del vector! Non dei dati! Copiare un vector non è troppo impegnativo. O no? NOOO!!!
		//long numDatiIniziale = aDati.size();
		//Salva i dati
		dati = aDati;
		//vector<long> ListaDatifuori3Sigma; //0.003 = 100% - 99.7% = percentuale atttesa di fuori sigma, più spazio a caso


		//Costruiamo una variabile x come una funzione f(x)=x con erivata (d/dx)x = 1
		derivata = 1;


		if ( !errori.empty() )
			errori.clear();// Svuota l'array degli errori
		errori.reserve(dati.size());

		T dVarianzaCampione; 		// Sample variance
		T dVarianzaPopolazione; 	// Population variance
		//T dDeviazioneStandardCamp;	//
		T dDeviazioneStandardPopo;  //Population variance

		//Se il vettore è vuoto la random variable è 0 +- 0 Buona idea?
		if (dati.size() == 0) {

			#ifdef _MIO_DEBUG_
			std::clog << "Vettore vuoto, metto la variabile a zero+-zero";
			#endif


			iNumero_dati = 0;
			dMedia = 0;
			dDeviazioneStandardPopo = 0;

			dErroreMedia = 0;
			return;
		}

		//Calcola la media standard (non pesata cioè)
		dMedia=(long double)dati[0];
		for(unsigned long i=0; i < dati.size(); i++){
			//Media
			dMedia=(i*dMedia+(long double)dati[i])/(i+1);
		}

		//Calcola la deviazione del campione
		dVarianzaCampione=pow(((long double)dati[0]-dMedia),2);
		for(unsigned long i=0; i < dati.size(); i++){
			//Varianza
			dVarianzaCampione=(i*dVarianzaCampione+pow(((long double)dati[i]-dMedia),2)) /
					(i+1);
		}


		//se sigma2c=S/N e sigma2p=S/(N-1), allora, sostituendo S e risolvendo, sigma2p=sigma2c*N/(N-1)
		dVarianzaPopolazione = dVarianzaCampione*(long double)dati.size()/((long double)dati.size()-1);

		dDeviazioneStandardPopo = sqrt(dVarianzaPopolazione);
		//dDeviazioneStandardCamp = sqrt(dVarianzaCampione);
		iNumero_dati = dati.size();
		////////////////////////////////////////////////////////////////////////////////////////////////////
		//Se eliminaTreSigma è true, rifai i conti togliendo i dati inaccettabili
		unsigned long numCancellazioni = 0;
		if (eliminaTreSigma){
			std::clog << "Elimino i dati oltre 3 sigma...\n" ;
			for (typename vector<T>::iterator pDato = dati.begin();
					pDato != dati.end();)
			{
				if (abs(dMedia - *pDato ) >= 3*dDeviazioneStandardPopo) {
					/* Cancelliamo dal Vector i dati inaccettabili. Operazione costosa perchè i dati successivi vengono traslati
					 * indietro, ma è meglio un Vector di una LinkedList perchè i dati possono essere messi nella cache e occuma meno memoria.
					 * erase richiede un iterator, quindi siamo "costretti" a usarlo
					 */
					std::cout << "Eliminato dato: " << *pDato << "\n";

					//Erase restituisce l'iteratore dell'elemento successivo a quelli appena cancellati
					pDato = dati.erase(pDato);

					numCancellazioni = numCancellazioni + 1;
				} else {
					//Passiamo all'elemento successivo
					pDato++;
				}
			}
			std::clog << "Cancellati " << numCancellazioni << " dati\n\n";
		}//EndIf
		////////////////////////////////////////////////////////////////////////////////////////////////////

		if ( numCancellazioni > 0) {
			//Rifacciamo i conti
			dMedia=(long double)dati[0];
			for(unsigned long i=0; i < dati.size(); i++) {
					dMedia=(i*dMedia+(long double)dati[i])/(i+1);

			}

			dVarianzaCampione=pow(((long double)dati[0]-dMedia),2);
			for(unsigned long i=0; i < dati.size(); i++) {
				//Varianza
				dVarianzaCampione=(i*dVarianzaCampione+pow(((long double)dati[i]-dMedia),2)) /
									(i+1);
			}
			//dDeviazioneStandardCamp = sqrt(dVarianzaCampione);

			//se sigma2c=S/N e sigma2p=S/(N-1), allora, sostituendo S e risolvendo, sigma2p=sigma2c*N/(N-1)
			dVarianzaPopolazione = dVarianzaCampione*(long double)dati.size()/((long double)dati.size()-1);
		}//EndIf del ricalcolo
		////////////////////////////////////////////////////////////////////////////////////////////////////

		//Deviazione standard popolazione
		dDeviazioneStandardPopo=sqrt(dVarianzaPopolazione);
		iNumero_dati = dati.size();
		dErroreMedia = dDeviazioneStandardPopo / sqrt(iNumero_dati);
		for (unsigned long i = 0; i < dati.size(); i++)
			errori.push_back(dDeviazioneStandardPopo);

	}//Fine costruttore




	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Media pesata
	//Costruttore con la media pesata
	//TODO: Finire!!!!
	VarStat(const vector<VarStat<T>>& aDatiConErrore) {
		using std::vector;
		using std::abs;
		//La classe ha una copia del vector
		//long numDatiIniziale = aDati.size();
		//Salva i dati
		for (unsigned long i = 0; i < dati.size(); i++) {
			dati.push_back(aDatiConErrore.at(i).getMedia());
			errori.push_back(aDatiConErrore.at(i));
		}


		//Costruiamo una variabile x come una funzione f(x)=x con erivata (d/dx)x = 1
		derivata = 1;


		T dVarianzaCampione; 		//Sample variance
		//T dVarianzaPopolazione; 	//Population variance
		//T dDeviazioneStandardCamp;  //Non la uso ma lasciamola
		//T dDeviazioneStandardPopo;  //Aggiunta, si puo ricavare dall'errore della media

		//Se il vettore è vuoto la random variable è 0 +- 0 Buona idea?
		if (dati.size() == 0) {

			#ifdef _MIO_DEBUG_
			std::clog << "Vettore vuoto, metto la variabile a zero+-zero";
			#endif

			iNumero_dati = 0;
			dMedia = 0;
			//dDeviazioneStandardPopo = 0;

			dErroreMedia = 0;
			return;
		}

		//Calcola la media standard (non pesata cioè)
		long double sommaMedia = 0;
		long double somma_sigma2 = 0;
		for(unsigned long i=0; i < dati.size(); i++){
			//Media
			sommaMedia += dati[i] / pow(errori[i],2);
			somma_sigma2 += 1 / pow(errori[i],2);
		}
		dMedia = sommaMedia / somma_sigma2;

		//Calcola la deviazione del campione
		iNumero_dati = dati.size();
		dErroreMedia = sqrt(1 / somma_sigma2);


	}//Fine costruttore

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Fine Media pesata

	//Distruttore
	virtual ~VarStat() = default;//Virtual perchè devono ereditare da questa. Lecito il default? Bè compila

	//Getters
	inline long double getMedia() const {return dMedia;};
	//Scarto Quadratico Medio (N)
	inline long double getDeviazioneStandardPop() const {return dErroreMedia*std::sqrt(iNumero_dati);};
	inline long double getVarianzaPopolazione() const {return dErroreMedia*dErroreMedia / (long double)iNumero_dati;};
	inline long double getDeviazioneStandardCamp() const {return getDeviazioneStandardPop()*sqrt(((long double)iNumero_dati-1)/iNumero_dati);};
	inline long double getVarianzaCampione() const {return getDeviazioneStandardCamp()*getDeviazioneStandardCamp();};
	// Errore della media
	inline long double getErroreMedia() const {return dErroreMedia;}
	inline long double getDerivata() const {return derivata;}
	inline long double getEnnesimoDato(long i) const {return dati.at(i);}
	inline long double getEnnesimoErrore(long i) const {return errori.at(i);}
	inline long int    getNumeroDatiEffettivo() const {
		//Controlla che iNumero_dati sia sempre coerente
		assert(iNumero_dati == dati.size() and iNumero_dati == errori.size());
		return iNumero_dati;
	}
	//Range della variabile

	//Stampa dei valori
	void stampaTutto(std::ostream& os) {
		using namespace std;

		//Eclipse dà problemi con endl, modifichiamolo temporaneamente
		#define endl "\n"
		os << "Numero dati:                       " << getNumeroDatiEffettivo() 	<< endl;
		os << "Media:                             " << getMedia() 					<< endl;
		os << "Errore della media:                " << getErroreMedia() 			<< endl;
		os << "Deviazione standard popolazione:   " << getDeviazioneStandardPop() 	<< endl;
		os << "Varianza della popolazione:        " << getVarianzaPopolazione() 	<< endl;
		os << "Varianza del campione:             " << getVarianzaCampione() 		<< endl;
		os << "Deviazione standard campione:      " << getDeviazioneStandardCamp()  << endl;
		os << "Derivata:                          "	<< getDerivata() 				<< endl;
		#undef endl

	}


	//Stampa in formato fdat, usando nomeDato come postfisso dei nomi delle variabili
	// Es diamtr.stampaFdat(cout,"Diametro") stampa:
	// #%Numero_Dati_Diametro:15
	// #%Media_Diametro:11.42
	// etc...
	void stampaFdat(std::ostream& os, std::string nomeDato) {
		using namespace std;

		//Eclipse dà problemi con endl, modifichiamolo temporaneamente
		#define endl "\n"
		std::string p = "#%"; // Prefisso delle righe dei metadati nel formato fdat
		os << p << "Numero_Dati_"                     << nomeDato << ":" << getNumeroDatiEffettivo()    << endl;
		os << p << "Media_"                           << nomeDato << ":" << getMedia()                  << endl;
		os << p << "Errore_Media_"                    << nomeDato << ":" << getErroreMedia()            << endl;
		os << p << "Deviazione_Standard_Popolazione_" << nomeDato << ":" << getDeviazioneStandardPop()  << endl;
		os << p << "Varianza_Popolazione_"            << nomeDato << ":" << getVarianzaPopolazione()    << endl;
		os << p << "Varianza_Campione_"               << nomeDato << ":" << getVarianzaCampione()       << endl;
		os << p << "Deviazione_Standard_Campione_"    << nomeDato << ":" << getDeviazioneStandardCamp() << endl;
		os << p << "Derivata_"                        << nomeDato << ":" << getDerivata()               << endl;
		#undef endl

	}


	//Operatori
	//Somma una variabile statistica a un'altra e memorizzala nella prima. Vedi commento su -=, sotto
	inline VarStat<T>& operator+=(const VarStat<T>& rhs) {
		using std::sqrt;


		dMedia = dMedia + rhs.dMedia;

		// d(x+y)/dx = x', d(x+y)/dy = y'
		//							  x'          sigmaMx                    y'             sigmaMy
		//                         vvvvvvvv     vvvvvvvvvv             vvvvvvvvvvvv    vvvvvvvvvvvvvvv
		dErroreMedia = sqrt( pow( (derivata) * dErroreMedia,2) + pow( (rhs.derivata) * rhs.dErroreMedia,2) /* -2*derivata*rhs.derivata*cov(x,y) ? */ );
		//Derivata: derivata della somma è somma delle derivate
		derivata = derivata + rhs.derivata;

		// metti un dato solo, col suo errore
		iNumero_dati = 1;
		errori.clear();// Svuota l'array degli errori
		dati.clear();// Svuota l'array dei dati
		dati.push_back(dMedia);
		errori.push_back(getDeviazioneStandardPop());


		return *this;
	}

	//Sottrai una variabile statistica a un'altra e memorizzala nella prima. v1 -= v2 è come v.operator-=(v2), quindi le funzioni get, etc qui dentro si riferiscono a v1!!! E rhs.get... a v2

	inline VarStat<T>& operator-=(const VarStat<T>& rhs) {
		using std::sqrt;

		//Aggiungi il secondo membro moltiplicato per -1
		this->operator +=( VarStat<T>(-1) * rhs);
		return *this;
	}

	//Operatori
	//moltiplica una variabile statistica a un'altra e memorizzala nella prima
	inline VarStat<T>& operator*=(const VarStat<T>& rhs) {
		using std::sqrt;

		//Salviamo temporaneamente la media di lhs, che ci servirà dopo nelle derivate
		long double tMedia = dMedia;
		dMedia = dMedia * rhs.dMedia;
		// d(x*f(x))/dx = x'*f'(x), d(x*y)/dy = xy'
		// d(x*y)/dx = x'y, d(x*y)/dy = xy'
		//							        x'y               sigmaMx                         xy'                 sigmaMy
		//                         vvvvvvvvvvvvvvvvvvvvv     vvvvvvvvvv              vvvvvvvvvvvvvvvvvvvvv    vvvvvvvvvvvvvvv
		dErroreMedia = sqrt( pow( (derivata * rhs.dMedia) * dErroreMedia ,2) + pow( (tMedia * rhs.derivata) * rhs.dErroreMedia ,2) /* -2*...*cov(x,y) ? */ );
		//Derivata: derivata del prodotto x*y è x'y + xy'
		derivata = derivata * rhs.dMedia + tMedia * rhs.derivata;

		// metti un dato solo, col suo errore
		iNumero_dati = 1;
		errori.clear();// Svuota l'array degli errori
		dati.clear();// Svuota l'array dei dati
		dati.push_back(dMedia);
		errori.push_back(getDeviazioneStandardPop());

		return *this;
	}

	// X / Y
	inline VarStat<T>& operator/=(const VarStat<T>& rhs) {
		using std::sqrt;

		//Salviamo temporaneamente la media di lhs, che ci servirà dopo nelle derivate
		long double tMedia = dMedia;
		dMedia = dMedia / rhs.dMedia;
		// d(x/y)/dx = x'/y, d(x/y)/dy = xy' / y^2
		dErroreMedia = sqrt( pow( (derivata / rhs.dMedia) * dErroreMedia ,2) + pow( (tMedia * rhs.derivata / pow(rhs.dMedia,2)) * rhs.dErroreMedia ,2) /* -2*...*cov(x,y) ? */ );
		//Derivata: derivata della divisione x/y è (x'y - xy') / y^2
		derivata = (derivata*rhs.dMedia - tMedia * rhs.derivata) / pow(rhs.dMedia,2);

		//metti un dato solo, col suo errore
		iNumero_dati = 1;
		errori.clear();// Svuota l'array degli errori
		dati.clear();// Svuota l'array dei dati
		dati.push_back(dMedia);
		errori.push_back(getDeviazioneStandardPop());

		return *this;
	}




	//TODO: Lo tengo o metto esplicito che la moltiplicazione per scalare è moltiplicazione per costante (con derivata 0)?
	//Moltiplicazione per scalare, compound assignment. v *= d è come v.operator*=(d), quindi le funzioni get, etc qui dentro si riferiscono a v
	inline VarStat<T>& operator*=(const long double& rhs) {
		using std::sqrt;

		//Eleviamo rhs a un numero duale, siccome è una costante la derivata è 0. Quindi (-1,0)
		this->operator *=( VarStat<T>(rhs) );
		return *this;
	}

	//Somma di due VarStat v1 + v2
	//Trucchetto per riutilizzare il lavoro svolto con +=
	const VarStat<T> operator+(const VarStat<T> &other) const {
		VarStat<T> result = *this; // Copia il primo oggetto
		result += other;            // Aggiungici dentro l'altro
		return result;              // Ritorna il risultato
	}

	//Sottrazione di due VarStat:  v1 - v2
	//Trucchetto per riutilizzare il lavoro svolto con -=
	const VarStat<T> operator-(const VarStat<T>& other) const {
		VarStat<T> result = *this; // Copia il primo oggetto
		result -= other;            // Aggiungici dentro l'altro
		return result;              // Ritorna il risultato
	}

	//Meno unario: -v1
	const VarStat<T> operator-() const {
		VarStat<T> result = *this;    // Copia il primo oggetto
		//Eleviamo -1 a un numero duale, siccome è una costante la derivata è 0. Quindi (c,c') = (-1,0)
		return VarStat(-1) * result;  // Ritorna il risultato
	}

	//Moltiplicazione di due varstat: v1 * v2
	const VarStat<T> operator*(const VarStat<T>& other) const {
		VarStat<T> result = *this; // Copia il primo oggetto
		result *= other;            // Aggiungici dentro l'altro
		return result;              // Ritorna il risultato
	}

	const VarStat<T> operator/(const VarStat<T>& other) const {
		VarStat<T> result = *this; // Copia il primo oggetto
		result /= other;            // Aggiungici dentro l'altro
		return result;              // Ritorna il risultato
	}



	//Moltiplicazione per uno scalare
	//Trucchetto per riutilizzare il lavoro svolto con *= double,
//	const VarStat<T> operator*(const double rhs) const {
//		VarStat<T> result = *this; // Copia il primo oggettp
//		result *= rhs;            // Aggiungici dentro l'altro
//		return result;              // Ritorna il risultato
//	}

	//double dModa=0;

};


}//Fine DataAnalisi

}//Fine del mio namespace
