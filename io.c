
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "waveform.h"

WaveformSample *load_csv(const char *filename, int *count) {

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: could not open file %s\n", filename);
        return NULL;
    }

    WaveformSample *samples = malloc(1000 * sizeof(WaveformSample));
    if (samples == NULL) {
        printf("Error: memory allocation failed\n");
        fclose(fp);
        return NULL;
    }

    char line[512];


    fgets(line, sizeof(line), fp);

    *count = 0;


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

    fclose(fp);
    return samples;
}