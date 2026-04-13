#ifndef IO_H
#define IO_H
#include "waveform.h"
WaveformSample *load_csv(const char *filename, int *count);
void write_results(const char *filename, WaveformSample *samples, int count,
                   double rms_a, double rms_b, double rms_c,
                   double pp_a, double pp_b, double pp_c,
                   double dc_a, double dc_b, double dc_c,
                   int clipped_a, int clipped_b, int clipped_c,
                   double freq_min, double freq_max,
                   double pf_min, double pf_max,
                   double thd_min, double thd_max,
                   double std_a, double std_b, double std_c,
                 double var_a, double var_b, double var_c);
#endif