/*! \file test_component.c */

#include <stdio.h>
#include "libcomponent.h"

/*! \brief Prints the result, i.e. the array of resistor values
 *
 *  \param[in] count is the size of the array, i.e. the number resistors to print
 *  \param[in] res_array are the resistor values to print
 */
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
  
  const unsigned int MAX_NUMBER_OF_RESISTORS = 3;
  float res_array[MAX_NUMBER_OF_RESISTORS];

  float orig_resistance;
  sscanf(argv[1],"%f", &orig_resistance);
  int count = 0;

  count = e_resistance(orig_resistance, res_array);

  print_result(count, res_array);

  return 0;
}
