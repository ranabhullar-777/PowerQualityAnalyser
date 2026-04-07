#include <stdio.h>
#include <stdlib.h>
#include "waveform.h"
#include "io.h"

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("Usage: ./PowerQualityAnalyser power_quality_log.csv\n");
        return 1;
    }

    int count = 0;
    WaveformSample *samples = load_csv(argv[1], &count);

    if (samples == NULL) {
        return 1;
    }

    printf("=== Power Quality Analyser ===\n\n");
    printf("Loaded %d samples\n\n", count);

    // Print first 5 rows to verify
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