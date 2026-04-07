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
    double rms_a = compute_rms(samples, count, 0);
    double rms_b = compute_rms(samples, count, 1);
    double rms_c = compute_rms(samples, count, 2);

    printf("Phase A RMS: %.2f V\n", rms_a);
    printf("Phase B RMS: %.2f V\n", rms_b);
    printf("Phase C RMS: %.2f V\n", rms_c);

    double pp_a = compute_peak_to_peak(samples, count, 0);
    double pp_b = compute_peak_to_peak(samples, count, 1);
    double pp_c = compute_peak_to_peak(samples, count, 2);

    printf("Phase A Peak-to-Peak: %.2f V\n", pp_a);
    printf("Phase B Peak-to-Peak: %.2f V\n", pp_b);
    printf("Phase C Peak-to-Peak: %.2f V\n", pp_c);

    double dc_a = compute_dc_offset(samples, count, 0);
    double dc_b = compute_dc_offset(samples, count, 1);
    double dc_c = compute_dc_offset(samples, count, 2);

    printf("Phase A DC Offset: %.4f V\n", dc_a);
    printf("Phase B DC Offset: %.4f V\n", dc_b);
    printf("Phase C DC Offset: %.4f V\n", dc_c);

    int clipped_a = count_clipped(samples, count, 0);
    int clipped_b = count_clipped(samples, count, 1);
    int clipped_c = count_clipped(samples, count, 2);

    printf("Phase A clipped: %d samples\n", clipped_a);
    printf("Phase B clipped: %d samples\n", clipped_b);
    printf("Phase C clipped: %d samples\n", clipped_c);
    printf("total clipped: %d samples\n", clipped_a + clipped_b + clipped_c);
    /*  TO BE NOTED ==  Brief stated that its 20 total ,
     * but actual  data shows 20 per phase = 60 total
     * my result is correct based on the actual CSV data*/
    printf("\n----- Voltage Compliance Check (EN 50160) ------\n");
    printf("Acceptable range: 207V to 253V\n\n");
    check_compliance(rms_a, 0);
    check_compliance(rms_b, 1);
    check_compliance(rms_c, 2);
    free(samples);
    return 0;
}