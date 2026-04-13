

# Power Quality Waveform Analyser
### Programming for Engineers — UGMFGT-15-1
**University of the West of England | 2025-26**

---

## Overview
A command-line C program that reads a real-world 3-phase 
power quality sensor log, computes key electrical metrics 
for each phase, detects anomalies, and writes a structured 
report to results.txt.

---

## Dataset
- File: power_quality_log.csv
- 1000 samples at 5000 Hz over 200ms
- 10 complete cycles of a 50Hz UK supply
- 8 columns: timestamp, phase voltages (A/B/C), 
  line current, frequency, power factor, THD

---

## What the program computes
- RMS voltage per phase (checked against EN 50160 standard)
- Peak-to-peak voltage per phase
- DC offset per phase
- Clipping detection (sensor limit ±325.0V)
- Tolerance compliance (207V - 253V band)
- Standard deviation per phase (merit extension)
- Frequency, power factor and THD reporting

---

## How to compile (CLion)
1. Open project in CLion
2. Make sure power_quality_log.csv is in the project folder
3. Set Working Directory to $ProjectFileDir$
4. Click Run

---

## How to compile (command line)
gcc main.c io.c waveform.c -o PowerQualityAnalyser -lm

## How to run (command line)
./PowerQualityAnalyser power_quality_log.csv

---

## Project structure
- main.c        — entry point, handles command-line arguments
- io.c / io.h   — CSV file reader and results writer
- waveform.c / waveform.h — all analysis functions
- results.txt   — generated output report
- logbook.docx  — development logbook

---

## Expected output
- Phase A RMS: ~229.7V (COMPLIANT)
- Peak-to-peak: ~650.0V
- DC offset: ~0.00V
- Clipped samples: 60 total (20 per phase)
- Frequency: 50.000 - 50.048 Hz
- Power factor: 0.950 - 0.962
- THD: 2.00% - 2.18%

---

## GitHub
https://github.com/ranabhullar-777/PowerQualityAnalyser
