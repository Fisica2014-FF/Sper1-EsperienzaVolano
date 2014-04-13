#!/bin/bash

# Questo file traduce tutti i file .initialvalue di gnuplot in file .fdat con il traduttore

for initialvalue in ../gnuplot/*.initialvalue
do
		echo "file initialvalue: $initialvalue"
		
		# ./a/b/ciao.templatescript --> ciao // grep esclude i file senza estensione (usabili magari come ./readme o simili)
		nomeinitialvalue=$(echo "$initialvalue" | grep --perl-regexp --only-matching '(.+)\.(.*)' | perl -pe 's/(.*\/)(.+)\.(.+)/\2/g')

		echo "file fdat risultante: ${nomeinitialvalue}.fdat"
		cat $initialvalue | ./traduciInitialValueInFdat.sh >../gnuplot/fit/${nomeinitialvalue}.fdat
done

exit 0
