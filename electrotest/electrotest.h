#ifndef _ELECTROTEST_H_
#define _ELECTROTEST_H_

float calc_power_r(float volt, float resistance);

float calc_resistance(int count, char conn, float *array);

int e_resistance(float orig_resistance, float *res_array);

#endif
