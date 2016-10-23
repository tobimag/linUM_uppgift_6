#include <stdlib.h>
#include <stdio.h>
#include "libresistance.h"

/*
Funktionen beräknar den totala resistansen för olika kopplade  resistanser. Resistanserna ska kunna kopplas parallellt eller i serie. 
    count: Antal komponenter.
    conn: Seriellt eller parallellt kopplade komponeter [ P | S ].
    *array: En pekare på en array av komponentvärden som är lika stor som count.
    Värdet 0 skall returneras om något motstånd är noll vid parallellkoppling, dvs  R1||R2=0, om R1 eller R2 är 0 Ohm.
    Biblioteket får inte krascha om en "nollpekare" skickas till funktionen, dvs om array=0.
    Om argumenten är felaktiga skall funktionen returnera -1.

Returvärdet är den resulterande resistansen.*/

// Beräknar resistansen vid seriekoppling
float res_s(int count, float *array) 
{
	float res = 0;
	for (int i = 0; i < count; i++) {
		res += array[i];
	}
	return res;
}

// Beräknar resistansen vid parallellkoppling
float res_p(int count, float *array) 
{
	float res = 0;
	for (int i = 0; i < count; i++) {
		if (array[i] == 0) { // En parallellkopplad resistans är noll
			printf("En parallellkopplad resistans är noll\n");
			return 0;
		}
		res += 1/array[i];
	}
	
	return 1/res;
}

float calc_resistance(int count, char conn, float *array)
{
	
	// Felaktig indata till parametern conn
	if ((conn != 'P' && conn != 'S')) {
		printf("Parametern 'conn' måste vara S eller P\n");
		return -1;
	} 
	
	// Inskickad array är en nullpekare
	if (array == 0) {
		printf("Parametern 'array' är en nullpekare\n");
		return -1;
	}
	
	// Felaktig indata till parametern count
	if (count <= 0) {
		printf("Parametern 'count' måste vara ett heltal > 0\n");
		return -1;
	}
	
	// Beräkning av ersättningsresistansen
	if (conn == 'S') {             // Seriekoppling
		return res_s(count, array);
	} else {        // (conn == 'P')  Parallellkoppling
		return res_p(count, array);
	}
}
