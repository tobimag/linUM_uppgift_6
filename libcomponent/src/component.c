/* component.c */

#include <math.h>
#include <stdio.h>

#include "component.h"

const int NB_OF_E12_VALUES = 12;
const float e12_resistor_table[12] = {1, 1.2, 1.5, 1.8, 2.2, 2.7, 3.3, 3.9, 4.7, 5.6, 6.8, 8.2};
const float tolerance = 0.1;

int calc_e12_values(const float resistance, float *e12_values, const int nb_of_e12_values)
{
  const float exponent = floorf(log10f(resistance));
  const float scale = powf(10.0, exponent);

  float e12_resistor, relative_error, left_candidate, right_candidate;
  int i;

  for(i = 0; i < NB_OF_E12_VALUES; ++i)
    {
      e12_resistor = e12_resistor_table[i] * scale;
      relative_error = (e12_resistor - resistance)/resistance;
      printf("Resistor: %f | Error: %f\n", e12_resistor, relative_error);
      if(fabs(relative_error) < tolerance)
	{
	  if(relative_error > 0)
	    {
	      left_candidate = e12_resistor_table[i-1]*scale;
	      right_candidate = e12_resistor;
	    }
	  else
	    {
	       left_candidate = e12_resistor;
	       right_candidate = e12_resistor_table[i+1]*scale;
	    }
	  printf("Found candidates: %f and %f\n", left_candidate, right_candidate);
	  break;
	} //if
    } // for
  
  if(nb_of_e12_values - 1 > 0)
    {
      const float remaining_resistance = fabs(resistance - left_candidate);
      printf("Creating new tree with start resistance: %f \n", remaining_resistance);
      calc_e12_values(remaining_resistance, &(e12_values[1]), nb_of_e12_values-1);
    }

  int node;
  float total_resistance_left_tree = left_candidate;
  for(node = 1; node < nb_of_e12_values; ++node)
    {
      total_resistance_left_tree += e12_values[node];
    }
  printf("Total resistance of left tree is: %f \n", total_resistance_left_tree);

  if(total_resistance_left_tree < right_candidate)
    {
      e12_values[0] = left_candidate;
    }
  else
    {
      e12_values[0] = right_candidate;
    }

  return 0;
}

int e_resistance(const float orig_resistance, float *res_array)
{
  res_array[0] = 0;
  res_array[1] = 0;
  res_array[2] = 0;

  calc_e12_values(orig_resistance, res_array, 3);

  return 0;
}
