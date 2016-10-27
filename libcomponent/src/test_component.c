/* test_component.c */

#include <stdio.h>
#include "component.h"

const int NB_OF_RESISTORS = 3;

void print_result(const int count, const float *res_array)
{
  printf("Used %d resistors: ", count);
  int i;
  for(i = 0; i < count; ++i)
    {
      printf("%.1f", res_array[i]);
      if(i < count - 1)
	printf(", ");
    }
  printf("\n");
}

int main(void)
{
  float res_array[NB_OF_RESISTORS];
  const float orig_resistance = 1380;
  int count = 0;

  count = e_resistance(orig_resistance, res_array);

  print_result(count, res_array);

  return 0;
}
