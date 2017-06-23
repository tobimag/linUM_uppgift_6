/*! \file libcomponent.h */

#ifndef _LIBCOMPONENT_H_
#define _LIBCOMPONENT_H_

/*! \brief Realize resistance with resistors from E12-series
 *
 *  Finds up to three resistors from E12-series which closest replace
 *  the original resistance.
 *
 *  \param[in] orig_resistance is the resistance which should be replaced
 *  \param[out] res_array are the chosen resistors from E12-series
 *  \return the number of resistors used
 */
int e_resistance(const float orig_resistance, float *res_array);

#endif
