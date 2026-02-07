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
        float hp_inputHistory1 = 0.0f;   // HPF x[n-1]
        float hp_inputHistory2 = 0.0f;   // HPF x[n-2]
        float hp_outputHistory1 = 0.0f;  // HPF y[n-1]
        float hp_outputHistory2 = 0.0f;  // HPF y[n-2]

        // peaking EQ coefficients
        float b0 = 0.0f, b1 = 0.0f, b2 = 0.0f;
        float a1 = 0.0f, a2 = 0.0f;
        
        // high-pass filter coefficients
        float hp_b0 = 0.0f, hp_b1 = 0.0f, hp_b2 = 0.0f;
        float hp_a1 = 0.0f, hp_a2 = 0.0f;

        // HPF
        float applyHPF(float input);

        // high pass helper
        void updateHPFCoefficients();

        // helper to calculate peaking EQ coefficients
        void updateToneCoefficients(float tone);

        // helper function for soft clipping
        float softClip(float input);
};
