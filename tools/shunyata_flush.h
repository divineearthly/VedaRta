#pragma once
#include <cstdio>
#include <cmath>

struct BirdCLEFVeda;

struct ShunyataFlush {
    BirdCLEFVeda* clef;
    float before_correlation;
    float after_correlation;
    float noise_released_pct;
    
    ShunyataFlush() : clef(nullptr), before_correlation(0), 
        after_correlation(0), noise_released_pct(0) {}
    
    void attach(BirdCLEFVeda* c);
    void flush();
    void report();
};
