#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "waveform.h"


// calculate RMS voltage for a given phase
// we use phase 0=A, 1=B, 2=C to avoid writing 3 separate functions
double compute_rms(WaveformSample *samples, int count, int phase) {
    double sum_sq = 0.0;
    for (int i = 0; i < count; i++) {
        double voltage;

        if (phase == 0) voltage = samples[i].phase_A_voltage;
        else if (phase == 1) voltage = samples[i].phase_B_voltage;
        else voltage = samples[i].phase_C_voltage;
        sum_sq += voltage*voltage;
    }
    return sqrt(sum_sq/count);

}
// peak to peak = highest sample minus lowest sample
double compute_peak_to_peak(WaveformSample *samples, int count, int phase) {
    double max = -9999999.0;
    double min =  9999999.0;

    for (int i = 0; i < count; i++) {
        double voltage;
        if (phase == 0) voltage = samples[i].phase_A_voltage;
        else if (phase == 1) voltage = samples[i].phase_B_voltage;
        else voltage = samples[i].phase_C_voltage;

        if (voltage > max) max = voltage;
        if (voltage < min) min = voltage;
    }
    return max - min;
}
//DC offset = mean voltage, should be ~0V for clean AC
double compute_dc_offset(WaveformSample *samples, int count, int phase) {
    double sum = 0.0;
    for (int i = 0; i < count; i++) {
        if (phase == 0) sum += samples[i].phase_A_voltage;
        else if (phase == 1) sum += samples[i].phase_B_voltage;
        else sum += samples[i].phase_C_voltage;
    }

    return sum/count;
}
//flag any sample where voltage hits sensor limit of 325.0V
int count_clipped(WaveformSample *samples, int count, int phase) {
    int clipped = 0;
    for (int i = 0; i < count; i++) {
        double voltage;
        if (phase == 0) voltage = samples[i].phase_A_voltage;
        else if (phase == 1) voltage = samples[i].phase_B_voltage;
        else voltage = samples[i].phase_C_voltage;
        if (voltage >= 324.9 || voltage <= -324.9) {
            clipped++;
        }
    }

    return clipped;

}
//EN 50160 standard - acceptable range 207V to 253V (230V +/- 10%)
void check_compliance(double rms, int phase) {
    char phase_name;

    if (phase == 0) phase_name = 'A';
    else if (phase == 1) phase_name = 'B';
    else phase_name = 'C';

    if (rms >= 207.0 && rms <= 253.0) {
        printf("phase %c: Compliant (%.2f V within 207-253V band)\n", phase_name, rms);
    } else {
        printf("Phase %c: Non-Compliant (%.2f V outside 207-253V band)\n", phase_name, rms);
    }
}
// standard deviation - two pass algorithm
// pass 1: calculate mean
// pass 2 : calculate how far each value is from mean

double compute_std_dev(WaveformSample *samples, int count, int phase) {
    double sum = 0.0;
    // pass 1 : calculate mean
    for (int i = 0; i < count; i++) {
        if (phase == 0) sum += samples[i].phase_A_voltage;
        else if (phase == 1) sum += samples[i].phase_B_voltage;
        else sum += samples[i].phase_C_voltage;
    }
    double mean = sum / count;
    //pass 2 - calculate variance
    double sum_sq = 0.0;
    for (int i = 0; i < count; i++) {
        double voltage;
        if (phase == 0) voltage = samples[i].phase_A_voltage;
        else if (phase == 1) voltage = samples[i].phase_B_voltage;
        else voltage = samples[i].phase_C_voltage;

        double diff = voltage - mean;
        sum_sq += diff * diff;
    }
    return sqrt(sum_sq / count);
}
// variance = standard deviation squared
double compute_variance(WaveformSample *samples, int count, int phase) {
    double std_dev = compute_std_dev(samples, count, phase);
    return std_dev * std_dev;
}
/* status flags for phase using bitwise operations
 bit 0 = clipping detected
 bit 1 = out of EN 50160 tolerance
 bit 2 = high THD (total harmonic distortion { > 5%} )
 bit 3 = low power factor (hoe efficient electricity is being used {< 0.95})
*/
uint8_t compute_status_flags(double rms, int clipped,
                             double thd, double pf) {
    uint8_t flag = 0;
    // bit 0 - clipping
    if (clipped > 0) {
        flag |= (1 << 0);
    }
    // bit 1 = out of EN 50160 tolerance
    if (rms < 207.0 || rms > 253.0) {
        flag |= (1 << 1);
    }
    // bit 2 - high thd
    if (thd > 5.0) {
        flag |= (1 << 2);
    }
    // bit 3 = low power factor
    if (pf < 0.95) {
        flag |= (1 << 3);
    }

    return flag;
}
/* sort samples by phase A voltage magnitude using bubble sort
   uses a  pointer based swapping - no qsort allowed as per brief */
void sort_by_voltage(WaveformSample *samples, int count) {
    int swapped;
    for (int i = 0; i < count - 1; i++) {
        swapped = 0;

        for (int j = 0; j < count - i - 1; j++) {
            // compare absolute voltage values
            double curr = samples[j].phase_A_voltage;
            double next = samples[j + 1].phase_A_voltage;

            if (curr > next) {
                // swap using a temp struct
                WaveformSample temp = samples[j];
                samples[j] = samples[j + 1];
                samples[j + 1] = temp;
                swapped = 1;
            }
        }

        // if no swaps happened - already sorted
        if (swapped == 0) break;
    }
}
