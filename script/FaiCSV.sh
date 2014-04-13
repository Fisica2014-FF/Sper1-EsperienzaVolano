#!/bin/bash

# Prepara le tabelle per i csv per calc
for i in {1..7}
do
	#Elimina metadati e commenti dai file e ppoi incolla le colonne di numeri
	cd ../DatiFormattati 
	grep -Pv "#" ./d0${i}.fdat > ~/tmp/relazione/gd0${i}.fdat
	grep -Pv "#" ./d0${i}_2.fdat > ~/tmp/relazione/gd0${i}_2.fdat
	grep -Pv "#" ./d0${i}_3.fdat > ~/tmp/relazione/gd0${i}_3.fdat
	echo -ne "Copiati i file"
done
exit 0
