# include "OverdriveDSP.h"

// constructor
OverdriveDSP::OverdriveDSP()
{
    sampleRate = 44100.0f;
    previousTone = 0.0f;
    inputHistory1 = 0.0f;
    inputHistory2 = 0.0f;
    outputHistory1 = 0.0f;
    outputHistory2 = 0.0f;
}

// prepare the DSP with the given sample rate
void OverdriveDSP::prepare(float newSampleRate)
{
    sampleRate = newSampleRate;
    reset();
    updateHPFCoefficients();
    updateToneCoefficients(3000.0f);
}

// reset the DSP state
void OverdriveDSP::reset()
{
    inputHistory1 = 0.0f;
    inputHistory2 = 0.0f;
    outputHistory1 = 0.0f;
    outputHistory2 = 0.0f;
    previousTone = 0.0f;
    hp_a1 = 0.0f;
    hp_a2 = 0.0f;
    hp_b0 = 0.0f;
    hp_b1 = 0.0f;
    hp_b2 = 0.0f;
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

// helper to calculate peaking EQ coefficients
void OverdriveDSP::updateToneCoefficients(float tone)
{
    // map tone parameter (200-8000 Hz) to peak frequency
    float peakFreq = 200.0f + (tone - 200.0f) * 1.5f;  // shift toward midrange
    if (peakFreq > 8000.0f) peakFreq = 8000.0f;
    
    // peaking EQ with moderate Q (sharpness) and gain (boost amount)
    float Q = 1.0f;  // Q factor (sharpness of peak)
    float peakGain = 6.0f;  // dB boost amount (Tube Screamer boosts mids)
    
    // biquad cookbook formulas
    float A = std::pow(10.0f, peakGain / 40.0f);  // convert dB to linear
    float w0 = 2.0f * 3.14159265f * peakFreq / sampleRate;
    float sinW0 = std::sin(w0);
    float cosW0 = std::cos(w0);
    float alpha = sinW0 / (2.0f * Q);
    
    // peaking EQ coefficients
    b0 = 1.0f + alpha * A;
    b1 = -2.0f * cosW0;
    b2 = 1.0f - alpha * A;
    a1 = -2.0f * cosW0;
    a2 = 1.0f - alpha / A;
    
    // normalize (divide by a0)
    float a0 = 1.0f + alpha / A;
    b0 /= a0;
    b1 /= a0;
    b2 /= a0;
    a1 /= a0;
    a2 /= a0;
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

        if (tone != previousTone) {
            updateToneCoefficients(tone);
            previousTone = tone;
        }

        // biquad formula
        float toneSample = b0 * clippedSample + b1 * inputHistory1 + b2 * inputHistory2 
                           - a1 * outputHistory1 - a2 * outputHistory2;

        // update state variables
        inputHistory2 = inputHistory1;
        inputHistory1 = clippedSample;
        outputHistory2 = outputHistory1;
        outputHistory1 = toneSample;

        // apply output level
        buffer[i] = toneSample * levelLinear;
    }
}
