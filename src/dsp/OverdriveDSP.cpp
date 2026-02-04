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

void OverdriveDSP::updateToneCoefficients(float tone)
{
    // Map tone parameter (200-8000 Hz) to peak frequency
    float peakFreq = 200.0f + (tone - 200.0f) * 1.5f;  // Shift toward midrange
    if (peakFreq > 8000.0f) peakFreq = 8000.0f;
    
    // Peaking EQ with moderate Q (sharpness) and gain (boost amount)
    float Q = 1.0f;  // Q factor (sharpness of peak)
    float peakGain = 6.0f;  // dB boost amount (Tube Screamer boosts mids)
    
    // Biquad cookbook formulas
    float A = std::pow(10.0f, peakGain / 40.0f);  // Convert dB to linear
    float w0 = 2.0f * 3.14159265f * peakFreq / sampleRate;
    float sinW0 = std::sin(w0);
    float cosW0 = std::cos(w0);
    float alpha = sinW0 / (2.0f * Q);
    
    // Peaking EQ coefficients
    b0 = 1.0f + alpha * A;
    b1 = -2.0f * cosW0;
    b2 = 1.0f - alpha * A;
    a1 = -2.0f * cosW0;
    a2 = 1.0f - alpha / A;
    
    // Normalize (divide by a0)
    float a0 = 1.0f + alpha / A;
    b0 /= a0;
    b1 /= a0;
    b2 /= a0;
    a1 /= a0;
    a2 /= a0;
}

// audio processing loop
void OverdriveDSP::process(float* buffer, int numSamples, float drive, float tone, float level)
{
    // Convert dB parameters to linear
    float driveLinear = std::pow(10.0f, drive / 20.0f);
    float levelLinear = std::pow(10.0f, level / 20.0f);
    
    for (int i = 0; i < numSamples; ++i)
    {
        // Now use the pre-calculated values
        // Apply drive
        float inputSample = buffer[i] * driveLinear;

        // Soft clipping
        float clippedSample = softClip(inputSample);

        if (tone != previousTone) {
            updateToneCoefficients(tone);
            previousTone = tone;
        }

        // Correct biquad formula
        float toneSample = b0 * clippedSample + b1 * inputHistory1 + b2 * inputHistory2 
                           - a1 * outputHistory1 - a2 * outputHistory2;

        // Update state variables
        inputHistory2 = inputHistory1;
        inputHistory1 = clippedSample;
        outputHistory2 = outputHistory1;
        outputHistory1 = toneSample;

        // Apply output level
        buffer[i] = toneSample * levelLinear;
    }
}

