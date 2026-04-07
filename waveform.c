#include <stdio.h>
#include <math.h>
#include "waveform.h"
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
double compute_dc_offset(WaveformSample *samples, int count, int phase) {
    double sum = 0.0;
    for (int i = 0; i < count; i++) {
        if (phase == 0) sum += samples[i].phase_A_voltage;
        else if (phase == 1) sum += samples[i].phase_B_voltage;
        else sum += samples[i].phase_C_voltage;
    }

    return sum/count;
}
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