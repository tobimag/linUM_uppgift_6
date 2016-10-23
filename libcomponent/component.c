/* component.c */

#include <math.h>
#include <stdio.h>

#include "component.h"

const int NB_OF_E12_VALUES = 12;
const float e12_values[12] = {1, 1.2, 1.5, 1.8, 2.2, 2.7, 3.3, 3.9, 4.7, 5.6, 6.8, 8.2};
const float tolerance = 0.1;

float calc_e12_value(float resistance)
{
  const float exponent = floorf(log10f(resistance));
  const float scale = powf(10.0, exponent);
  float e12_resistor, error, previous_e12_resistor;
  int i;
  for(i = 0; i < NB_OF_E12_VALUES; ++i)
    {
      e12_resistor = e12_values[i]*scale;
      error = fabs(e12_resistor - resistance)/resistance;
      printf("Resistor: %f | Error: %f\n", e12_resistor, error);
      if(error < tolerance)
	return e12_resistor;
    }

  return -1;
}

int e_resistance(float orig_resistance, float *res_array)
{
  res_array[0] = 0;
  res_array[1] = 0;
  res_array[2] = 0;

  res_array[0] = calc_e12_value(orig_resistance);

  return 0;
}
