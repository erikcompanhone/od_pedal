# include "OverdriveDSP.h"

// constructor
OverdriveDSP::OverdriveDSP()
{
    sampleRate = 44100.0f;
    previousTone = 0.0f;
    lp_inputHistory1 = 0.0f;
    lp_inputHistory2 = 0.0f;
    lp_outputHistory1 = 0.0f;
    lp_outputHistory2 = 0.0f;
}

// prepare the DSP with the given sample rate
void OverdriveDSP::prepare(float newSampleRate)
{
    sampleRate = newSampleRate;
    reset();
    updateHPFCoefficients();
    updatePostLPFCoefficients();
    updateLPFCoefficients(800.0f);
}

// reset the DSP state
void OverdriveDSP::reset()
{
    // HPF history
    hp_inputHistory1 = 0.0f;
    hp_inputHistory2 = 0.0f;
    hp_outputHistory1 = 0.0f;
    hp_outputHistory2 = 0.0f;

    // Post-LPF history
    postLPF_inputHistory1 = 0.0f;
    postLPF_inputHistory2 = 0.0f;
    postLPF_outputHistory1 = 0.0f;
    postLPF_outputHistory2 = 0.0f;

    // LPF history
    lp_inputHistory1 = 0.0f;
    lp_inputHistory2 = 0.0f;
    lp_outputHistory1 = 0.0f;
    lp_outputHistory2 = 0.0f;

    previousTone = 0.0f;
}

// helper function for soft clipping
float OverdriveDSP::softClip(float input)
{
    if (input > 1.0f)
        return (2.0f / 3.0f);
    else if (input < -1.0f)
        return -(2.0f / 3.0f);
    else
        return input - (input * input * input) / 3.0f;
}

float OverdriveDSP::applyLPF(float input)
{
    float output = lp_b0 * input + lp_b1 * lp_inputHistory1 + lp_b2 * lp_inputHistory2
                   - lp_a1 * lp_outputHistory1 - lp_a2 * lp_outputHistory2;
    
    // update history
    lp_inputHistory2 = lp_inputHistory1;
    lp_inputHistory1 = input;
    lp_outputHistory2 = lp_outputHistory1;
    lp_outputHistory1 = output;

    return output;
}

// POST LPF
float OverdriveDSP::applyPostLPF(float input)
{
    float output = post_b0 * input + post_b1 * postLPF_inputHistory1 + post_b2 * postLPF_inputHistory2
                   - post_a1 * postLPF_outputHistory1 - post_a2 * postLPF_outputHistory2;
    
    // update history
    postLPF_inputHistory2 = postLPF_inputHistory1;
    postLPF_inputHistory1 = input;
    postLPF_outputHistory2 = postLPF_outputHistory1;
    postLPF_outputHistory1 = output;

    return output;
}

// helper to update post low-pass filter coefficients
void OverdriveDSP::updatePostLPFCoefficients()
{
    float cutoffFreq = 7000.0f;  // Fixed at 7 kHz
    float Q = 0.707f;
    
    float w0 = 2.0f * 3.14159265f * cutoffFreq / sampleRate;
    float sinW0 = std::sin(w0);
    float cosW0 = std::cos(w0);
    float alpha = sinW0 / (2.0f * Q);
    
    // Apply LPF formulas
    post_b0 = (1.0f - cosW0) / 2.0f;
    post_b1 = 1.0f - cosW0;
    post_b2 = (1.0f - cosW0) / 2.0f;
    post_a1 = -2.0f * cosW0;
    post_a2 = 1.0f - alpha;
    
    // Normalize
    float a0 = 1.0f + alpha;
    post_b0 /= a0;
    post_b1 /= a0;
    post_b2 /= a0;
    post_a1 /= a0;
    post_a2 /= a0;
}

// helper to update low-pass filter coefficients
void OverdriveDSP::updateLPFCoefficients(float tone)
{
    float cutoffFreq = tone;  // Hz
    float Q = 0.707f;          // Standard rolloff
    
    // biquad cookbook formulas
    float w0 = 2.0f * 3.14159265f * cutoffFreq / sampleRate;
    float sinW0 = std::sin(w0);
    float cosW0 = std::cos(w0);
    float alpha = sinW0 / (2.0f * Q);
        
    // Apply LPF formulas:
    lp_b0 = (1.0f - cosW0) / 2.0f;
    lp_b1 = 1.0f - cosW0;
    lp_b2 = (1.0f - cosW0) / 2.0f;
    lp_a1 = -2.0f * cosW0;
    lp_a2 = 1.0f - alpha;
    
    // normalize (divide by a0)
    float a0 = 1.0f + alpha;
    lp_b0 /= a0;
    lp_b1 /= a0;
    lp_b2 /= a0;
    lp_a1 /= a0;
    lp_a2 /= a0;
}

// HPF
float OverdriveDSP::applyHPF(float input)
{
    float output = hp_b0 * input + hp_b1 * hp_inputHistory1 + hp_b2 * hp_inputHistory2
                   - hp_a1 * hp_outputHistory1 - hp_a2 * hp_outputHistory2;

    // update history
    hp_inputHistory2 = hp_inputHistory1;
    hp_inputHistory1 = input;
    hp_outputHistory2 = hp_outputHistory1;
    hp_outputHistory1 = output;

    return output;
}

// helper to update high-pass filter coefficients
void OverdriveDSP::updateHPFCoefficients()
{
    float cutOffFreq = 720.0f; // Hz
    float Q = 0.707f;

    float w0 = 2.0f * 3.14159265f * cutOffFreq / sampleRate;
    float sinW0 = std::sin(w0);
    float cosW0 = std::cos(w0);
    float alpha = sinW0 / (2.0f * Q);

    // HPF formulas
    hp_b0 = (1.0f + cosW0) / 2.0f;
    hp_b1 = -(1.0f + cosW0);
    hp_b2 = (1.0f + cosW0) / 2.0f;
    hp_a1 = -2.0f * cosW0;
    hp_a2 = 1.0f - alpha;

    // normalize
    float a0 = 1.0f + alpha;
    hp_b0 /= a0;
    hp_b1 /= a0;
    hp_b2 /= a0;
    hp_a1 /= a0;
    hp_a2 /= a0;
}

// audio processing loop
void OverdriveDSP::process(float* buffer, int numSamples, float drive, float tone, float level)
{
    // convert dB parameters to linear
    float driveLinear = std::pow(10.0f, drive / 20.0f);
    float levelLinear = std::pow(10.0f, level / 20.0f);
    
    for (int i = 0; i < numSamples; ++i)
    {
        // apply drive
        float inputSample = buffer[i] * driveLinear;

        // Apply HPF
        float hpfSample = applyHPF(inputSample);

        // soft clipping
        float clippedSample = softClip(hpfSample);

        // post LPF
        float postLPFSample = applyPostLPF(clippedSample);

        if (tone != previousTone) {
            updateLPFCoefficients(tone);
            previousTone = tone;
        }

        // apply LPF
        float toneSample = applyLPF(postLPFSample);

        // apply output level
        buffer[i] = toneSample * levelLinear;
    }
}
