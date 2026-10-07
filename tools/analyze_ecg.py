"""Offline ECG CSV inspection helper.

Input CSV columns: timestamp_ms, raw_adc
Optional: filtered, heart_rate_bpm, quality
"""

import csv
import statistics
import sys

def main(path: str) -> int:
    with open(path, newline="", encoding="utf-8") as f:
        rows = list(csv.DictReader(f))
    if not rows:
        print("No ECG samples found.")
        return 1
    raw = [float(r["raw_adc"]) for r in rows]
    print(f"samples={len(raw)}")
    print(f"raw_mean={statistics.fmean(raw):.2f}")
    print(f"raw_min={min(raw):.2f}")
    print(f"raw_max={max(raw):.2f}")
    if "heart_rate_bpm" in rows[0]:
        hr = [float(r["heart_rate_bpm"]) for r in rows if float(r["heart_rate_bpm"]) > 0]
        if hr:
            print(f"mean_heart_rate_bpm={statistics.fmean(hr):.2f}")
    return 0

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("usage: python3 tools/analyze_ecg.py samples.csv")
        raise SystemExit(2)
    raise SystemExit(main(sys.argv[1]))
