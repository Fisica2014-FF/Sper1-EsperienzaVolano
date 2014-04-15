#!/bin/bash
# Script che da n file .fdat e m file di template, produce n*m file generati
# Però modificato per il volano, qua. Ha più senso modificarlo ogni volta alla fine

# Fallo per l'andata
for filedati in ../dati_formattati/v{0..9}.fdat
do
	for template in ../gnuplot/*_acc.templatescript
	do
		echo "filedati: $filedati"
		echo "template: $template"
		echo '' | ./IstanziaFileDiTemplate.pl $filedati $template | gnuplot -p
	done
done

# Fallo per il ritorno
for filedati in ../dati_formattati/v{0..9}r.fdat
do
	for template in ../gnuplot/*_dec.templatescript
	do
		echo "filedati: $filedati"
		echo "template: $template"
		echo '' | ./IstanziaFileDiTemplate.pl $filedati $template | gnuplot -p
	done
done
exit 0
