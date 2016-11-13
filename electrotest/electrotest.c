#include <stdio.h>
#include <ctype.h>

#include "electrotest.h"

int main(){
  float volt;
  printf("Ange spänningskälla i V: ");
  scanf("%f", &volt);

  char conn;
  do{
    printf("Ange koppling[S|P]: ");
    scanf(" %c", &conn);
    conn= toupper(conn);
  }while(conn!= 'S' && conn!= 'P');

  int count;
  printf("Antal komponenter: ");
  scanf("%d", &count);
  
  float array[count];
  for(int i= 0; i< count; i++){
    printf("Komponent %d i ohm: ", i+1);
    scanf("%f", &array[i]);
  } 

  float orig_resistance= calc_resistance(count, conn, array);
  printf("Ersättningsresistans:\n%.1f ohm\n", orig_resistance);

  printf("Effekt:\n%.2f W\n", calc_power_r(volt, orig_resistance ));

  printf("Ersättningsresistanser i E12-serien kopplade i serie:\n");
  float res_array[3];
  e_resistance(orig_resistance, res_array);
  for(int i= 0; i< 3; i++)
    if(res_array[i] != 0)
	printf("%.0f\n", res_array[i]);

  return 0;
}


