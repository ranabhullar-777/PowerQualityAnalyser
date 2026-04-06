#ifndef WAVEFORM_H
#define WAVEFORM_H

// struct to hold onw row of data from csv file
typedef struct {
    double timestamp; // time in seconds
    double phase_A_voltage;// voltage in volts
    double phase_B_voltage;// voltage in volts
    double phase_C_voltage;//  voltage in volts
    double line_current;// current in amps
    double frequency;// in hz
    double power_factor; // from 0 to 1
    double thd_percent;// in percentage
} WaveformSample;

#endif
