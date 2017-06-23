/*! \file libcomponent.c */

#include <math.h>
#include <stdio.h>

#include "libcomponent.h"

#if DEBUG
#define DEBUG_PRINT(f_, ...) printf((f_), __VA_ARGS__)
#else
#define DEBUG_PRINT
#endif

#define E12_RESISTOR_TABLE_SIZE 12
#define TOLERANCE 0.1

static const int e12_resistor_table_size = 12;
static const float e12_resistor_table[E12_RESISTOR_TABLE_SIZE] = {1, 1.2, 1.5, 1.8, 2.2, 2.7, 3.3, 3.9, 4.7, 5.6, 6.8, 8.2};

/*! calculate_scaling 
 *  \brief Calculates the tenfold closest to the supplied resistance value
 *
 *  \param[in] resistance is the value of the resistance
 *  \return the tenfold, i.e. the scaling 
 */
static float calculate_scaling(const float resistance)
{
  const float exponent = floorf(log10f(resistance));
  const float scale = powf(10.0, exponent);
  DEBUG_PRINT("Scale is %f\n", scale);
  return scale;
}

/*! \brief Returns the value of the E12 resistor given by the index and the scale 
 *
 *  Turns the straight table of E12 resistor values into a circular, covering all decades.
 *  The scale chooses which decade of E12 resistors that shall be used while the index
 *  chooses the resistor in that decade. If the index is negative, the last resistor in
 *  the previous decade is chosen. If the index is bigger than straight table, the first
 *  resistor in the next decade is chosen. 
 *
 *  \param[in] index is the selector of a resistor, ignoring decade
 *  \param[in] scale is the selector of decade
 *  \return the value of the chosen resistory
 */
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

/*! \brief Find one or two E12 resistors to replace the actual resistance
 *
 *  Given a resistance, find either one or two E12 resistors which are candidates
 *  to replace the resistance. The first candidate will be in the left part of the
 *  the tree and will be less than than the resistance. The second candidate will
 *  be in the right part of three and wil lbe greater than the resistance. If a
 *  match is found, i.e. a resistor exactly matching the resistance, this will be
 *  the first (left) candidate and the right_candidate will be untouched. 
 *
 *  \param resistance is the actual resistance to replace
 *  \param left_candidate is the E12 resistor in the left tree
 *  \param right_candidate is the E12 resistor in the right tree
 *  \return 0 if only candidate found, 1 if two candidates found, -1 if no canidates were found
 */
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
      if(fabs(relative_error) < TOLERANCE)
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

/*! \brief Sums the value of a number of resistors
 *
 * \param e12_values are the resistor values to sum
 * \param nb_of_resistors is how many values to sum
 * \return the sum of the resistor values 
 */
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

/*! \brief Sets all candidates to zero
 *
 * \param candidates is the candidates that shall be set to zero
 * \param nb_of_resistors is the number of candidates
 */
static void set_other_candidates_to_zero(float *candidates, const int nb_of_resistors)
{
  int node;
  for(node = 0; node < nb_of_resistors; ++node)
    {
      candidates[node] = 0;
    }
}

/*! \brief Realize resistance with resistors from E12-series
 *
 *  Finds up given number of resistors from E12-series which closest replace
 *  the original resistance.
 *
 *  \param[in] resistance is the resistance which should be replaced
 *  \param[out] e12_values are the chosen resistors from E12-series
 *  \param [in] nb_of_resistors is the maximum nubmer of resistors to use
 *  \return the number of resistors used
 */
static int calc_e12_values(const float resistance, float *e12_values, const int nb_of_resistors)
{
  DEBUG_PRINT("Resistance: %f, Reistors to use: %d\n", resistance, nb_of_resistors);

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
