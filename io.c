
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "waveform.h"

WaveformSample *load_csv(const char *filename, int *count) {

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: could not open file %s\n", filename);
        printf("please check the file exists and try again\n");
        return NULL;
    }
    // allocate memory for 1000 samples
    WaveformSample *samples = malloc(1000 * sizeof(WaveformSample));
    if (samples == NULL) {
        printf("error: memory allocation failed\n");
        fclose(fp);
        return NULL;
    }

    char line[512];

    if (fgets(line, sizeof(line), fp) == NULL) {
        printf("error: file is empty\n");
        free(samples);
        fclose(fp);
        return NULL;
    }

    *count = 0;

    // read each row and parse the 8 columns using strtok
    while (fgets(line, sizeof(line), fp) != NULL) {
        WaveformSample s;

        s.timestamp         = atof(strtok(line, ","));
        s.phase_A_voltage   = atof(strtok(NULL, ","));
        s.phase_B_voltage   = atof(strtok(NULL, ","));
        s.phase_C_voltage   = atof(strtok(NULL, ","));
        s.line_current      = atof(strtok(NULL, ","));
        s.frequency         = atof(strtok(NULL, ","));
        s.power_factor      = atof(strtok(NULL, ","));
        s.thd_percent       = atof(strtok(NULL, ","));

        samples[*count] = s;
        (*count)++;
    }
    // check file had actual data rows
    if (*count == 0) {
        printf("error: file has no data rows\n");
        free(samples);
        fclose(fp);
        return NULL;
    }


    fclose(fp);
    return samples;
}

void write_results(const char *filename, WaveformSample *samples, int count,
                   double rms_a, double rms_b, double rms_c,
                   double pp_a, double pp_b, double pp_c,
                   double dc_a, double dc_b, double dc_c,
                   int clipped_a, int clipped_b, int clipped_c,
                   double freq_min, double freq_max,
                   double pf_min, double pf_max,
                   double thd_min, double thd_max) {
    // open file in write mode - creates it if it doesnt exist
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Error: could not open file %s\n", filename);
        return;
    }
    fprintf(fp, "#######################################\n");
    fprintf(fp, "   Power Quality Waveform Analyser\n");
    fprintf(fp, "#########################################\n\n");
    fprintf(fp, "Total samples loaded: %d\n\n", count);

    fprintf(fp, "--- RMS Voltage ---\n");
    fprintf(fp, "Phase A: %.2f V\n", rms_a);
    fprintf(fp, "Phase B: %.2f V\n", rms_b);
    fprintf(fp, "Phase C: %.2f V\n\n", rms_c);

    fprintf(fp, "--- Peak-to-Peak Voltage ---\n");
    fprintf(fp, "Phase A: %.2f V\n", pp_a);
    fprintf(fp, "Phase B: %.2f V\n", pp_b);
    fprintf(fp, "Phase C: %.2f V\n\n", pp_c);

    fprintf(fp, "--- DC Offset ---\n");
    fprintf(fp, "Phase A: %.4f V\n", dc_a);
    fprintf(fp, "Phase B: %.4f V\n", dc_b);
    fprintf(fp, "Phase C: %.4f V\n\n", dc_c);

    fprintf(fp, "--- Clipping Detection ---\n");
    fprintf(fp, "Phase A: %d samples\n", clipped_a);
    fprintf(fp, "Phase B: %d samples\n", clipped_b);
    fprintf(fp, "Phase C: %d samples\n", clipped_c);
    fprintf(fp, "Total: %d samples\n\n", clipped_a + clipped_b + clipped_c);

    fprintf(fp, "=====> EN 50160 Compliance <=====\n");
    fprintf(fp, "Phase A: %s (%.2f V)\n", (rms_a >= 207.0 && rms_a <= 253.0) ? "Compliant" : "Non-Compliant", rms_a);
    fprintf(fp, "Phase B: %s (%.2f V)\n", (rms_b >= 207.0 && rms_b <= 253.0) ? "Compliant" : "Non-Compliant", rms_b);
    fprintf(fp, "Phase C: %s (%.2f V)\n\n", (rms_c >= 207.0 && rms_c <= 253.0) ? "Compliant" : "Non-Compliant", rms_c);

    fprintf(fp, "--- Frequency, Power Factor and THD ---\n");
    fprintf(fp, "Frequency range: %.3f Hz to %.3f Hz\n", freq_min, freq_max);
    fprintf(fp, "Power factor range: %.3f to %.3f\n", pf_min, pf_max);
    fprintf(fp, "THD range: %.2f%% to %.2f%%\n", thd_min, thd_max);

    fprintf(fp, "\n###########################################\n");
    fprintf(fp, "   End of Report\n");
    fprintf(fp, "#############################################\n");

    fclose(fp);
    printf("\nResults written to %s\n", filename);

}