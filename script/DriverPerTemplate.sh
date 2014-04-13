#!/bin/bash
# Script che da n file .fdat e m file di template, produce n*m file generati
# Primo argomento: cartella dei dati
# secondo argomento: cartella dei template
# Terzo argomento. cartella dei risultati
# Ambedue senza / alla fine!! ./ciao/ = SBAGLIATO, ./ciao = GIUSTO!!!

# Fallo solo per i file della prima pallina, che poi ci includiamo gli altri dello stesso gruppo
for filedati in ../DatiFormattati/d0{1..7}.fdat
do
	for template in ../gnuplot/*.templatescript
	do
		echo "filedati: $filedati"
		echo "template: $template"
		echo '' | ./InterfacciaFitGnuplot.pl $filedati $template | gnuplot -p
	done
done
exit 0
