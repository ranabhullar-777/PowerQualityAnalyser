#include <stdio.h>
#include <stdlib.h>
#include "waveform.h"
#include "io.h"

int main(int argc, char *argv[]) {

    // check that the user has actually given  us a filename to work with
    if (argc < 2) {
        printf("Error: no file provided!\n");
        printf("Usage: ./PowerQualityAnalyser power_quality_log.csv\n");
        return 1;
    }


    // load all 1000 rows from the CSV file into memory
    int count = 0;
    WaveformSample *samples = load_csv(argv[1], &count);

    if (samples == NULL) {
        return 1;
    }

    printf("=== Power Quality Analyser ===\n\n");
    printf("Loaded %d samples\n\n", count);

    // Print first 5 rows to verify if the csv loaded correctly
    for (int i = 0; i < 5; i++) {
        printf("Time: %.4f | PhaseA: %.2f | PhaseB: %.2f | PhaseC: %.2f\n",
               samples[i].timestamp,
               samples[i].phase_A_voltage,
               samples[i].phase_B_voltage,
               samples[i].phase_C_voltage);
    }

    // calculate RMS volatge for all 3 phases
    double rms_a = compute_rms(samples, count, 0);
    double rms_b = compute_rms(samples, count, 1);
    double rms_c = compute_rms(samples, count, 2);

    printf("Phase A RMS: %.2f V\n", rms_a);
    printf("Phase B RMS: %.2f V\n", rms_b);
    printf("Phase C RMS: %.2f V\n", rms_c);

    // calculate peak to peak voltage
    double pp_a = compute_peak_to_peak(samples, count, 0);
    double pp_b = compute_peak_to_peak(samples, count, 1);
    double pp_c = compute_peak_to_peak(samples, count, 2);

    printf("Phase A Peak-to-Peak: %.2f V\n", pp_a);
    printf("Phase B Peak-to-Peak: %.2f V\n", pp_b);
    printf("Phase C Peak-to-Peak: %.2f V\n", pp_c);


    // calculate dc offset
    double dc_a = compute_dc_offset(samples, count, 0);
    double dc_b = compute_dc_offset(samples, count, 1);
    double dc_c = compute_dc_offset(samples, count, 2);

    printf("Phase A DC Offset: %.4f V\n", dc_a);
    printf("Phase B DC Offset: %.4f V\n", dc_b);
    printf("Phase C DC Offset: %.4f V\n", dc_c);


    // count clipped samples
    int clipped_a = count_clipped(samples, count, 0);
    int clipped_b = count_clipped(samples, count, 1);
    int clipped_c = count_clipped(samples, count, 2);

    printf("Phase A clipped: %d samples\n", clipped_a);
    printf("Phase B clipped: %d samples\n", clipped_b);
    printf("Phase C clipped: %d samples\n", clipped_c);
    printf("total clipped: %d samples\n", clipped_a + clipped_b + clipped_c);
    /*  TO BE NOTED == 60 individual clipped values across all phases
   20 per phase, 1 phase clips per row = 60 total
   Brief refers to 20 unique clipping events */


    // check if each phase is within EN 50160 tolerance band 207-253V
    printf("\n----- Voltage Compliance Check (EN 50160) ------\n");
    printf("Acceptable range: 207V to 253V\n\n");
    check_compliance(rms_a, 0);
    check_compliance(rms_b, 1);
    check_compliance(rms_c, 2);


/*  to find min and max for frequency , power factor and THD
 *  these are pre computed in the CSV so no calculations
 *  needed just scan all samples to find the range
 */
    double freq_min = samples[0].frequency;
    double freq_max = samples[0].frequency;
    double pf_min = samples[0].power_factor;
    double pf_max = samples[0].power_factor;
    double thd_min = samples[0].thd_percent;
    double thd_max = samples[0].thd_percent;

    for (int i = 1; i < count; i++) {
        if (samples[i].frequency < freq_min) freq_min = samples[i].frequency;
        if (samples[i].frequency > freq_max) freq_max = samples[i].frequency;
        if (samples[i].power_factor < pf_min) pf_min = samples[i].power_factor;
        if (samples[i].power_factor > pf_max) pf_max = samples[i].power_factor;
        if (samples[i].thd_percent < thd_min) thd_min = samples[i].thd_percent;
        if (samples[i].thd_percent > thd_max) thd_max = samples[i].thd_percent;
    }
    printf("\n====> Frequency, Power Factor and THD <=====\n");
    printf("Frequency range: %.3f Hz to %.3f Hz\n", freq_min, freq_max);
    printf("Power factor range: %.3f to %.3f\n", pf_min, pf_max);
    printf("THD range: %.2f%% to %.2f%%\n", thd_min, thd_max);

    // calculate standard deviation and variance for each phase
    double std_a = compute_std_dev(samples, count, 0);
    double std_b = compute_std_dev(samples, count, 1);
    double std_c = compute_std_dev(samples, count, 2);

    double var_a = compute_variance(samples, count, 0);
    double var_b = compute_variance(samples, count, 1);
    double var_c = compute_variance(samples, count, 2);

    printf("\n====> Standard Deviation <====\n");
    printf("Phase A: %.4f V\n", std_a);
    printf("Phase B: %.4f V\n", std_b);
    printf("Phase C: %.4f V\n", std_c);

    printf("\n====>Variance <====\n");
    printf("Phase A: %.4f V\n", var_a);
    printf("Phase B: %.4f V\n", var_b);
    printf("Phase C: %.4f V\n", var_c);


    // write all results to results.txt
    write_results("results.txt", samples, count,
              rms_a, rms_b, rms_c,
              pp_a, pp_b, pp_c,
              dc_a, dc_b, dc_c,
              clipped_a, clipped_b, clipped_c,
              freq_min, freq_max,
              pf_min, pf_max,
              thd_min, thd_max,
              std_a, std_b, std_c,
              var_a, var_b, var_c);



    free(samples);
    return 0;
}