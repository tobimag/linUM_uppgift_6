/* component.c */

#include <math.h>
#include <stdio.h>

#include "component.h"

const int e12_resistor_table_size = 12;
const float e12_resistor_table[12] = {1, 1.2, 1.5, 1.8, 2.2, 2.7, 3.3, 3.9, 4.7, 5.6, 6.8, 8.2};
const float tolerance = 0.1;

static int calculate_scaling(const float resistance)
{
  const float exponent = floorf(log10f(resistance));
  const float scale = powf(10.0, exponent);
  return scale;
}

static int get_e12_resistor_value(const int index, const int scale)
{
  if (index >= 0 && index < e12_resistor_table_size) // Normal case
    return e12_resistor_table[index]*scale;
  else if(index < 0) // Negative index, get last resistor in previous decade
    return e12_resistor_table[e12_resistor_table_size-1]*(scale/10);
  else // Index too big, get first resistor in next decade
    return e12_resistor_table[0]*scale*10;
}

static int find_candidates(const float resistance, float* left_candidate, float* right_candidate)
{
  
  const float scale = calculate_scaling(resistance);
  int i;
  float e12_resistor, relative_error;
  for(i = 0; i < e12_resistor_table_size; ++i)
    {
      e12_resistor = get_e12_resistor_value(i, scale);
      relative_error = (e12_resistor - resistance)/resistance;
      printf("Resistor: %f | Error: %f\n", e12_resistor, relative_error);

      // We found something close
      if(fabs(relative_error) < tolerance)
	{
	  // We hit the motherload!
	  if(relative_error == 0)
	    {
	      *left_candidate = e12_resistor;
	      return 0;
	    }
	  // The current resistor is on the right side of the tree
	  if(relative_error > 0)
	    {
	      // The left candidate is the previous resistor
	      *left_candidate = get_e12_resistor_value(i-1, scale);
	      *right_candidate = e12_resistor;
	      return 1;
	    }
	  // The current resistor is on left side of the tree
	  else
	    {
	      *left_candidate = e12_resistor;
	      // The right candidate is the next resistor
	      *right_candidate = get_e12_resistor_value(i+1, scale);
	      return 1;
	    }
	} //if
    } // for
  return -1;
}

static float sum_resistance(const float *e12_values, const int nb_of_resistors)
{
  float total_resistance;
  int node;
  for(node = 0; node < nb_of_resistors; ++node)
    {
      total_resistance += e12_values[node];
    }
  return total_resistance;
}

static int calc_e12_values(const float resistance, float *e12_values, const int nb_of_resistors)
{
  const float scale = calculate_scaling(resistance);

  float left_candidate, right_candidate;
  int result;
  result = find_candidates(resistance, &left_candidate, &right_candidate);
  
  // Seems as if we hit an exact match
  if(result == 0)
    {
      e12_values[0] = left_candidate;
      return 1;
    }
  // Still some work to do
  else if (result == 1)
    {
      int used_resistors;
      if(nb_of_resistors - 1 > 0)
	{
	  const float remaining_resistance = fabs(resistance - left_candidate);
	  printf("Creating new tree with start resistance: %f \n", remaining_resistance);
	  used_resistors = calc_e12_values(remaining_resistance, &(e12_values[1]), nb_of_resistors-1);
	}

      float total_resistance_left_tree = left_candidate + sum_resistance(&(e12_values[1]), nb_of_resistors-1);
      printf("Total resistance of left tree is: %f \n", total_resistance_left_tree);

      if(total_resistance_left_tree < right_candidate)
	{
	  e12_values[0] = left_candidate;
	}
      else
	{
	  e12_values[0] = right_candidate;
	}
      return used_resistors + 1;
    }

  return -1;
}

int e_resistance(const float orig_resistance, float *res_array)
{
  res_array[0] = 0;
  res_array[1] = 0;
  res_array[2] = 0;

  return calc_e12_values(orig_resistance, res_array, 3);

}
