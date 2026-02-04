# pragma once

# include <cmath>
# include <algorithm>
# include <array>

class OverdriveDSP
{
    public:
        // constructor
        OverdriveDSP();

        // prepare the DSP with the given sample rate
        void prepare(float sampleRate);

        // audio processing loop
        void process(float* buffer, int numSamples, float drive, float tone, float level);

        // reset the DSP state
        void reset();

    private:
        // DSP state variables
        float sampleRate = 44100.0f;
        float previousTone = 0.0f;
        float inputHistory1 = 0.0f;   // x[n-1]
        float inputHistory2 = 0.0f;   // x[n-2]
        float outputHistory1 = 0.0f;  // y[n-1]
        float outputHistory2 = 0.0f;  // y[n-2]
        
        // Peaking EQ coefficients
        float b0 = 0.0f, b1 = 0.0f, b2 = 0.0f;
        float a1 = 0.0f, a2 = 0.0f;
        
        // Helper to calculate peaking EQ coefficients
        void updateToneCoefficients(float tone);

        // helper function for soft clipping
        float softClip(float input);
};
