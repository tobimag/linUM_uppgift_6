/* test_libcomponent.c */

#include <stdio.h>
#include "libcomponent.h"

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

int main(int argc, char* argv[])
{
  if(argc != 2)
    {
      printf("Usage: %s <Resistance>\n", argv[0]);
      return 1;
    }
  float res_array[NB_OF_RESISTORS];
  float orig_resistance;
  sscanf(argv[1],"%f", &orig_resistance);
  int count = 0;

  count = e_resistance(orig_resistance, res_array);

  print_result(count, res_array);

  return 0;
}
