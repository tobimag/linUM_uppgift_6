/* libcomponent.c */

#include <math.h>
#include <stdio.h>

#include "libcomponent.h"

#if DEBUG
#define DEBUG_PRINT(f_, ...) printf((f_), __VA_ARGS__)
#else
#define DEBUG_PRINT
#endif

const int e12_resistor_table_size = 12;
const float e12_resistor_table[12] = {1, 1.2, 1.5, 1.8, 2.2, 2.7, 3.3, 3.9, 4.7, 5.6, 6.8, 8.2};
const float tolerance = 0.1;

static float calculate_scaling(const float resistance)
{
  const float exponent = floorf(log10f(resistance));
  const float scale = powf(10.0, exponent);
  DEBUG_PRINT("Scale is %f\n", scale);
  return scale;
}

static float get_e12_resistor_value(const int index, const float scale)
{
  if (index >= 0 && index < e12_resistor_table_size) // Normal case
    {
      return e12_resistor_table[index]*scale;
    }
  else if(index < 0) // Negative index, get last resistor in previous decade
    {
      return e12_resistor_table[e12_resistor_table_size-1]*(scale/10);
    }
  else // Index too big, get first resistor in next decade
    {
      return e12_resistor_table[0]*scale*10;
    }
}

static int find_candidates(const float resistance, float* left_candidate, float* right_candidate)
{
  const float scale = calculate_scaling(resistance);
  float e12_resistor = 0, relative_error = 0;
  int i;
  for(i = 0; i < e12_resistor_table_size+1; ++i)
    {
      e12_resistor = get_e12_resistor_value(i, scale);
      relative_error = (e12_resistor - resistance)/resistance;
      DEBUG_PRINT("Resistor: %f | Error: %f\n", e12_resistor, relative_error);
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
  float total_resistance = 0;
  int node;
  for(node = 0; node < nb_of_resistors; ++node)
    {
      DEBUG_PRINT("Adding %f to total resistance\n", e12_values[node]);
      total_resistance += e12_values[node];
    }
  DEBUG_PRINT("Total resistance in tree: %f\n", total_resistance);
  return total_resistance;
}

static void set_other_candidates_to_zero(float *e12_values, const int nb_of_resistors)
{
  int node;
  for(node = 0; node < nb_of_resistors; ++node)
    {
      e12_values[node] = 0;
    }
}

static int calc_e12_values(const float resistance, float *e12_values, const int nb_of_resistors)
{
  DEBUG_PRINT("Resistance: %f, Reistors to use: %d\n", resistance, nb_of_resistors);
  const float scale = calculate_scaling(resistance);

  float left_candidate = 0, right_candidate = 0;
  const int result = find_candidates(resistance, &left_candidate, &right_candidate);
  DEBUG_PRINT("Left candidate: %f, Right candidate: %f\n", left_candidate, right_candidate);
  
  // Seems as if we hit an exact match
  if(result == 0)
    {
      e12_values[0] = left_candidate;
      DEBUG_PRINT("Exact match! Chose left candidate: %f\n", e12_values[0]);
      return 1;
    }
  // Still some work to do
  else if (result == 1)
    {
      int used_resistors = 0;
      // Do we want to continue in a new tree?
      if(nb_of_resistors - 1 > 0)
	{
	  const float remaining_resistance = fabs(resistance - left_candidate);
	  DEBUG_PRINT("Creating new tree with start resistance: %f \n", remaining_resistance);
	  // Create new tree with rest of array and remaining resistance
	  used_resistors = calc_e12_values(remaining_resistance, &(e12_values[1]), nb_of_resistors-1);
	}

      // Sum up resistance in left tree include this candidate
      const float total_resistance_left_tree = left_candidate + sum_resistance(&(e12_values[1]), nb_of_resistors-1);
      DEBUG_PRINT("Total resistance of left tree is: %f \n", total_resistance_left_tree);
     
      // Choose candidate
      const float left_tree_diff = fabs(total_resistance_left_tree - resistance);
      const float right_tree_diff = fabs(right_candidate - resistance);
      DEBUG_PRINT("Left tree diff: %f | Right tree diff: %f\n", left_tree_diff, right_tree_diff);
      if(left_tree_diff < right_tree_diff)
	{
	  e12_values[0] = left_candidate;
	  DEBUG_PRINT("Chose left candidate: %f\n", left_candidate);
	  used_resistors = used_resistors + 1;
	}
      else
	{
	  e12_values[0] = right_candidate;
	  DEBUG_PRINT("Chose right candidate: %f\n", right_candidate);
	  set_other_candidates_to_zero(&(e12_values[1]), nb_of_resistors-1);
	  used_resistors = 1;
	}
      return used_resistors;
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
