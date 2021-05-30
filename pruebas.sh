#!/bin/bash

echo "compilando TSP"
make
# Incialmente se ha utilizao ls -rS ./pruebas<algoritmo> pero en hendrix no va
PRUEBAS_FB="prueba3.tsp  prueba4.tsp  prueba5.tsp  prueba6.tsp  prueba7.tsp  prueba8.tsp  prueba9.tsp  prueba10.tsp  prueba11.tsp  prueba12.tsp"
PRUEBAS_AV="prueba5.tsp  prueba10.tsp  prueba20.tsp  prueba30.tsp  prueba40.tsp  prueba50.tsp  prueba60.tsp  prueba70.tsp  prueba80.tsp  prueba90.tsp  prueba100.tsp  prueba110.tsp  prueba120.tsp  prueba130.tsp  prueba140.tsp  prueba150.tsp"
PRUEBAS_PD="prueba5.tsp  prueba10.tsp  prueba11.tsp  prueba12.tsp  prueba13.tsp  prueba14.tsp  prueba15.tsp  prueba16.tsp  prueba17.tsp  prueba18.tsp  prueba19.tsp  prueba20.tsp  prueba21.tsp  prueba22.tsp"
PRUEBAS_RP="prueba5.tsp  prueba10.tsp  prueba11.tsp  prueba12.tsp  prueba13.tsp  prueba14.tsp  prueba15.tsp  prueba16.tsp  prueba17.tsp  prueba18.tsp  prueba19.tsp  prueba20.tsp  prueba21.tsp  prueba22.tsp  prueba23.tsp  prueba24.tsp  prueba25.tsp  prueba26.tsp  prueba27.tsp  prueba29.tsp  prueba30.tsp  prueba40.tsp"

for DATA in $PRUEBAS_FB 
do
    RUTA="./pruebasFB/"$DATA
    echo "Probando con fuerza bruta el ficheo: "$DATA
    ./tsp -fb $RUTA
done 

for DATA in $PRUEBAS_AV 
do
    RUTA="./pruebasAV/"$DATA
    echo "Probando con el algoritmo voraz el ficheo: "$DATA
    ./tsp -av $RUTA
done 

for DATA in $PRUEBAS_PD 
do
    RUTA="./pruebasPD/"$DATA
    echo "Probando con programacion dinamica el ficheo: "$DATA
    ./tsp -pd $RUTA
done 

for DATA in $PRUEBAS_RP 
do
    RUTA="./pruebasRP/"$DATA
    echo "Probando con ramificacion y poda el ficheo: "$DATA
    ./tsp -rp $RUTA
done 

echo "Elimiando el ejecutable"
make clean
echo "Pruebas finalizadas correctamente"