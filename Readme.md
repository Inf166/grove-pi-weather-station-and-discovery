# Tutorial zum GrovePi

1. Aufsetzen des GrovePi auf einen Raspberry Pi (an GPIO anschließen)
2. Verbinden eines Sensors mit einem digital - / analog Port
3. Herunterladen der GrovePi-Daten:
*sudo curl -kL dexterindustries.com/update_grovepi | bash*
4. herunterladen / zur Verfügung stellen der .c / .h Dateien:
[GrovePi C-Dateien](https://github.com/DexterInd/GrovePi/tree/0e53b166bcb82dad6028b9c7e7ed362fbc9d9654/Software/C)
5. schreiben der [programm.c] Datei (inkludiert grovepi.h)
6. ausführen —> *gcc ../grovepi.c [programm.c]*
7. ausführen —> *./a.out*