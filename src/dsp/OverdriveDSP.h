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
        float fixedGain = 2.0f;
        float driveExponent = 1.5f;

        // POST LPF
        float postLPF_inputHistory1 = 0.0f;   // POST LPF x[n-1]
        float postLPF_inputHistory2 = 0.0f;   // POST LPF x[n-2]
        float postLPF_outputHistory1 = 0.0f;  // POST LPF y[n-1]
        float postLPF_outputHistory2 = 0.0f;  // POST LPF y[n-2]

        float post_b0 = 0.0f, post_b1 = 0.0f, post_b2 = 0.0f;
        float post_a1 = 0.0f, post_a2 = 0.0f;

        float applyPostLPF(float input);

        void updatePostLPFCoefficients();

        // LPF
        float lp_inputHistory1 = 0.0f;   // LPF x[n-1]
        float lp_inputHistory2 = 0.0f;   // LPF x[n-2]
        float lp_outputHistory1 = 0.0f;  // LPF y[n-1]
        float lp_outputHistory2 = 0.0f;  // LPF y[n-2]
        
        float lp_b0 = 0.0f, lp_b1 = 0.0f, lp_b2 = 0.0f;
        float lp_a1 = 0.0f, lp_a2 = 0.0f;

        float applyLPF(float input);

        void updateLPFCoefficients(float tone);

        // HPF
        float hp_inputHistory1 = 0.0f;   // HPF x[n-1]
        float hp_inputHistory2 = 0.0f;   // HPF x[n-2]
        float hp_outputHistory1 = 0.0f;  // HPF y[n-1]
        float hp_outputHistory2 = 0.0f;  // HPF y[n-2]

        float hp_b0 = 0.0f, hp_b1 = 0.0f, hp_b2 = 0.0f;
        float hp_a1 = 0.0f, hp_a2 = 0.0f;

        float applyHPF(float input);

        void updateHPFCoefficients();

        // tanh clip
        float tanhClip(float input);
};
