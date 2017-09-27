#include <common.hpp>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <complex>
#include <vector>
#include <map>
#include <math.h>

EMOLIB_BEGIN_NAMESPACE

struct wavHeader
{
    /* RIFF Chunk Descriptor */
    uint8_t         RIFF[4];        // RIFF Header Magic header
    uint32_t        ChunkSize;      // RIFF Chunk Size
    uint8_t         WAVE[4];        // WAVE Header
    /* "fmt" sub-chunk */
    uint8_t         fmt[4];         // FMT header
    uint32_t        Subchunk1Size;  // Size of the fmt chunk
    uint16_t        AudioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw,257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
    uint16_t        NumOfChan;      // Number of channels 1=Mono 2=Stereo
    uint32_t        SamplesPerSec;  // Sampling Frequency in Hz
    uint32_t        bytesPerSec;    // bytes per second
    uint16_t        blockAlign;     // 2=16-bit mono, 4=16-bit stereo
    uint16_t        bitsPerSample;  // Number of bits per sample
    /* "data" sub-chunk */
    uint8_t         Subchunk2ID[4]; // "data"  string
    uint32_t        Subchunk2Size;  // Sampled data length
};


typedef std::vector<double> v_d;
typedef std::complex<double> c_d;
typedef std::vector<v_d> m_d;
typedef std::vector<c_d> v_c_d;
typedef std::map<int,std::map<int,c_d> > twmap;

class MFCC
{
private:
    const double PI = 4*atan(1.0);
    int fs;
    twmap twiddle;
    size_t winLengthSamples, frameShiftSamples, numCepstra, numFFT, numFFTBins, numFilters;
    double preEmphCoef, lowFreq, highFreq;
    v_d frame, powerSpectralCoef, lmfbCoef, hamming, mfcc, prevsamples;
    m_d fbank, dct;
    
public:
    MFCC(int sampFreq=16000, int nCep=12, int winLength=25, int frameShift=10, int numFilt=40, double lf=50, double hf=6500);
    v_d process_frame_buffer(int16_t* samples, size_t N);
    std::string process_frame(int16_t* samples, size_t N);
    int process (std::ifstream &wavFp, std::ofstream &mfcFp);
    std::vector<v_d> process_buffer(std::ifstream &wavFp);
    std::vector<v_d> process_buffer(int16_t* samples, uint64_t count);
    
private:
    // Hertz to Mel conversion
    inline double hz2mel (double f) {
        return 2595*std::log10 (1+f/700);
    }
    
    // Mel to Hertz conversion
    inline double mel2hz (double m) {
        return 700*(std::pow(10,m/2595)-1);
    }
    
    void compTwiddle();
    v_c_d fft(v_c_d x);
    void preEmphHam();
    void computePowerSpec();
    void applyLMFB();
    void applyDct();
    void initHamDct();
    void initFilterbank();
    std::string v_d_to_string (v_d vec);
};


EMOLIB_END_NAMESPACE
