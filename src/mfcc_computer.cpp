#include <mfcc_computer.hpp>
#include <memory_stream.hpp>

EMOLIB_BEGIN_NAMESPACE

// Twiddle factor computation
void MFCC::compTwiddle(void) {
    const c_d J(0,1);      // Imaginary number 'j'
    for (int N=2; N<=numFFT; N*=2)
        for (int k=0; k<=N/2-1; k++)
            twiddle[N][k] = exp(-2*PI*k/N*J);
}

// Cooley-Tukey DIT-FFT recursive function
v_c_d MFCC::fft(v_c_d x) {
    int N = x.size();
    if (N==1)
        return x;
    
    v_c_d xe(N/2,0), xo(N/2,0), Xjo, Xjo2;
    int i;
    
    // Construct arrays from even and odd indices
    for (i=0; i<N; i+=2)
        xe[i/2] = x[i];
    for (i=1; i<N; i+=2)
        xo[(i-1)/2] = x[i];
    
    // Compute N/2-point FFT
    Xjo = fft(xe);
    Xjo2 = fft(xo);
    Xjo.insert (Xjo.end(), Xjo2.begin(), Xjo2.end());
    
    // Butterfly computations
    for (i=0; i<=N/2-1; i++) {
        c_d t = Xjo[i], tw = twiddle[N][i];
        Xjo[i] = t + tw * Xjo[i+N/2];
        Xjo[i+N/2] = t - tw * Xjo[i+N/2];
    }
    return Xjo;
}

//// Frame processing routines
// Pre-emphasis and Hamming window
void MFCC::preEmphHam(void) {
    v_d procFrame(frame.size(), hamming[0]*frame[0]);
    for (int i=1; i<frame.size(); i++)
        procFrame[i] = hamming[i] * (frame[i] - preEmphCoef * frame[i-1]);
    frame = procFrame;
}

// Power spectrum computation
void MFCC::computePowerSpec(void) {
    frame.resize(numFFT); // Pads zeros
    v_c_d framec (frame.begin(), frame.end()); // Complex frame
    v_c_d fftc = fft(framec);
    
    for (int i=0; i<numFFTBins; i++)
        powerSpectralCoef[i] = pow(abs(fftc[i]),2);
}

// Applying log Mel filterbank (LMFB)
void MFCC::applyLMFB(void) {
    lmfbCoef.assign(numFilters,0);
    
    for (int i=0; i<numFilters; i++) {
        // Multiply the filterbank matrix
        for (int j=0; j<fbank[i].size(); j++)
            lmfbCoef[i] += fbank[i][j] * powerSpectralCoef[j];
        // Apply Mel-flooring
        if (lmfbCoef[i] < 1.0)
            lmfbCoef[i] = 1.0;
    }
    
    // Applying log on amplitude
    for (int i=0; i<numFilters; i++)
        lmfbCoef[i] = std::log (lmfbCoef[i]);
}

// Computing discrete cosine transform
void MFCC::applyDct(void) {
    mfcc.assign(numCepstra+1,0);
    for (int i=0; i<=numCepstra; i++) {
        for (int j=0; j<numFilters; j++)
            mfcc[i] += dct[i][j] * lmfbCoef[j];
    }
}

// Initialisation routines
// Pre-computing Hamming window and dct matrix
void MFCC::initHamDct(void) {
    int i, j;
    
    hamming.assign(winLengthSamples,0);
    for (i=0; i<winLengthSamples; i++)
        hamming[i] = 0.54 - 0.46 * cos(2 * PI * i / (winLengthSamples-1));
    
    v_d v1(numCepstra+1,0), v2(numFilters,0);
    for (i=0; i <= numCepstra; i++)
        v1[i] = i;
    for (i=0; i < numFilters; i++)
        v2[i] = i + 0.5;
    
    dct.reserve (numFilters*(numCepstra+1));
    double c = sqrt(2.0/numFilters);
    for (i=0; i<=numCepstra; i++) {
        v_d dtemp;
        for (j=0; j<numFilters; j++)
            dtemp.push_back (c * cos(PI / numFilters * v1[i] * v2[j]));
        dct.push_back(dtemp);
    }
}

// Precompute filterbank
void MFCC::initFilterbank () {
    // Convert low and high frequencies to Mel scale
    double lowFreqMel = hz2mel(lowFreq);
    double highFreqMel = hz2mel (highFreq);
    
    // Calculate filter centre-frequencies
    v_d filterCentreFreq;
    filterCentreFreq.reserve (numFilters+2);
    for (int i=0; i<numFilters+2; i++)
        filterCentreFreq.push_back (mel2hz(lowFreqMel + (highFreqMel-lowFreqMel)/(numFilters+1)*i));
    
    // Calculate FFT bin frequencies
    v_d fftBinFreq;
    fftBinFreq.reserve(numFFTBins);
    for (int i=0; i<numFFTBins; i++)
        fftBinFreq.push_back (fs/2.0/(numFFTBins-1)*i);
    
    // Filterbank: Allocate memory
    fbank.reserve (numFilters*numFFTBins);
    
    // Populate the fbank matrix
    for (int filt=1; filt<=numFilters; filt++) {
        v_d ftemp;
        for (int bin=0; bin<numFFTBins; bin++) {
            double weight;
            if (fftBinFreq[bin] < filterCentreFreq[filt-1])
                weight = 0;
            else if (fftBinFreq[bin] <= filterCentreFreq[filt])
                weight = (fftBinFreq[bin] - filterCentreFreq[filt-1]) / (filterCentreFreq[filt] - filterCentreFreq[filt-1]);
            else if (fftBinFreq[bin] <= filterCentreFreq[filt+1])
                weight = (filterCentreFreq[filt+1] - fftBinFreq[bin]) / (filterCentreFreq[filt+1] - filterCentreFreq[filt]);
            else
                weight = 0;
            ftemp.push_back (weight);
        }
        fbank.push_back(ftemp);
    }
}

// Convert vector of double to string (for writing MFCC file output)
std::string MFCC::v_d_to_string (v_d vec) {
    std::stringstream vecStream;
    for (int i=0; i<vec.size()-1; i++) {
        vecStream << std::scientific << vec[i];
        vecStream << ", ";
    }
    vecStream << std::scientific << vec.back();
    vecStream << "\n";
    return vecStream.str();
}

MFCC::MFCC(int sampFreq, int nCep, int winLength, int frameShift, int numFilt, double lf, double hf) {
    fs          = sampFreq;     // Sampling frequency
    numCepstra  = nCep;         // Number of cepstra
    numFFT      = 512;          // FFT size
    numFilters  = numFilt;      // Number of Mel warped filters
    preEmphCoef = 0.97;         // Pre-emphasis coefficient
    lowFreq     = lf;           // Filterbank low frequency cutoff in Hertz
    highFreq    = hf;         // Filterbank high frequency cutoff in Hertz
    winLengthSamples   = winLength * fs / 1e3;  // winLength in milliseconds
    frameShiftSamples  = frameShift * fs / 1e3; // frameShift in milliseconds
    
    numFFTBins = numFFT/2 + 1;
    powerSpectralCoef.assign (numFFTBins, 0);
    prevsamples.assign (winLengthSamples-frameShiftSamples, 0);
    
    initFilterbank();
    initHamDct();
    compTwiddle();
}

// Process each frame and extract MFCC
v_d MFCC::process_frame_buffer(int16_t* samples, size_t N)
{
    // Add samples from the previous frame that overlap with the current frame
    // to the current samples and create the frame.
    frame = prevsamples;
    for (int i=0; i<N; i++)
        frame.push_back(samples[i]);
    prevsamples.assign(frame.begin()+frameShiftSamples, frame.end());
    
    preEmphHam();
    computePowerSpec();
    applyLMFB();
    applyDct();
    
    return mfcc;
}

std::string MFCC::process_frame(int16_t* samples, size_t N)
{
    return v_d_to_string (process_frame_buffer(samples, N));
}

// Read input file stream, extract MFCCs and write to output file stream
int MFCC::process(std::ifstream &wavFp, std::ofstream &mfcFp)
{
    // Read the wav header
    wavHeader hdr;
    int headerSize = sizeof(wavHeader);
    wavFp.read((char *) &hdr, headerSize);
    
    // Check audio format
    if (hdr.AudioFormat != 1 || hdr.bitsPerSample != 16) {
        std::cerr << "Unsupported audio format, use 16 bit PCM Wave" << std::endl;
        return 1;
    }
    // Check sampling rate
    if (hdr.SamplesPerSec != fs) {
        std::cerr << "Sampling rate mismatch: Found " << hdr.SamplesPerSec << " instead of " << fs <<std::endl;
        return 1;
    }
    
    // Initialise buffer
    uint16_t bufferLength = winLengthSamples-frameShiftSamples;
    int16_t* buffer = new int16_t[bufferLength];
    int bufferBPS = (sizeof buffer[0]);
    
    // Read and set the initial samples
    wavFp.read((char *) buffer, bufferLength*bufferBPS);
    for (int i=0; i<bufferLength; i++)
        prevsamples[i] = buffer[i];
    delete [] buffer;
    
    // Recalculate buffer size
    bufferLength = frameShiftSamples;
    buffer = new int16_t[bufferLength];
    
    // Read data and process each frame
    wavFp.read((char *) buffer, bufferLength*bufferBPS);
    while (wavFp.gcount() == bufferLength*bufferBPS && !wavFp.eof())
    {
        mfcFp << process_frame(buffer, bufferLength);
        wavFp.read((char *) buffer, bufferLength*bufferBPS);
    }
    
    delete [] buffer;
    buffer = nullptr;
    return 0;
}

std::vector<v_d> MFCC::process_buffer(std::ifstream &wavFp)
{
    std::vector<v_d> mfccs;
    
    // Read the wav header
    wavHeader hdr;
    
    int headerSize = sizeof(wavHeader);
    wavFp.read((char *) &hdr, headerSize);
    
    // Check audio format
    if (hdr.AudioFormat != 1 || hdr.bitsPerSample != 16) {
        std::cerr << "Unsupported audio format, use 16 bit PCM Wave" << std::endl;
        return mfccs;
    }
    // Check sampling rate
    if (hdr.SamplesPerSec != fs) {
        std::cerr << "Sampling rate mismatch: Found " << hdr.SamplesPerSec << " instead of " << fs <<std::endl;
        return mfccs;
    }
    
    // Initialise buffer
    uint16_t bufferLength = winLengthSamples-frameShiftSamples;
    int16_t* buffer = new int16_t[bufferLength];
    int bufferBPS = (sizeof buffer[0]);
    
    // Read and set the initial samples
    wavFp.read((char *) buffer, bufferLength*bufferBPS);
    for (int i=0; i<bufferLength; i++)
        prevsamples[i] = buffer[i];
    delete [] buffer;
    
    // Recalculate buffer size
    bufferLength = frameShiftSamples;
    buffer = new int16_t[bufferLength];
    
    // Read data and process each frame
    wavFp.read((char *) buffer, bufferLength*bufferBPS);
    while (wavFp.gcount() == bufferLength*bufferBPS && !wavFp.eof())
    {
        mfccs.push_back(process_frame_buffer(buffer, bufferLength));
        wavFp.read((char *) buffer, bufferLength*bufferBPS);
    }
    
    delete [] buffer;
    buffer = nullptr;
    return mfccs;
}

std::vector<v_d> MFCC::process_buffer(int16_t* samples, uint64_t count)
{
    std::vector<v_d> mfccs;
    
    MemoryStream stream((uint8_t*)samples, count * sizeof(int16_t));
    
    // Initialise buffer
    uint16_t bufferLength = winLengthSamples-frameShiftSamples;
    int16_t* buffer = new int16_t[bufferLength];
    int bufferBPS = (sizeof buffer[0]);
    
    // Read and set the initial samples
    stream.read((char *) buffer, bufferLength*bufferBPS);
    for (int i=0; i<bufferLength; i++)
        prevsamples[i] = buffer[i];
    delete [] buffer;
    
    // Recalculate buffer size
    bufferLength = frameShiftSamples;
    buffer = new int16_t[bufferLength];
    
    // Read data and process each frame
    stream.read((char *) buffer, bufferLength*bufferBPS);
    while (stream.gcount() == bufferLength*bufferBPS && !stream.eof())
    {
        mfccs.push_back(process_frame_buffer(buffer, bufferLength));
        stream.read((char *) buffer, bufferLength*bufferBPS);
    }
    
    delete [] buffer;
    buffer = nullptr;
    return mfccs;
}

EMOLIB_END_NAMESPACE

