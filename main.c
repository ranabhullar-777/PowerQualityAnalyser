#include <stdio.h>
#include <stdlib.h>
#include "waveform.h"

WaveformSample *load_csv(const char *filename, int *count);

int main(void) {

    int count = 0;
    WaveformSample *samples = load_csv("power_quality_log.csv", &count);

    if (samples == NULL) {
        return 1;
    }

    printf("===>>>>> Power Quality Analyser <<<<<===\n");
    printf("Loaded %d samples\n\n", count);
    for (int i = 0; i < 5; i++) {
        printf("Time: %.4f | PhaseA: %.2f | PhaseB: %.2f | PhaseC: %.2f\n",
               samples[i].timestamp,
               samples[i].phase_A_voltage,
               samples[i].phase_B_voltage,
               samples[i].phase_C_voltage);
    }

    free(samples);
    return 0;
}