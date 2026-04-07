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