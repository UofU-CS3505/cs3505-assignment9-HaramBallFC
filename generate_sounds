// HaramBall FC (CS3505 Assignment 9)
// Team: Ebrahim Himayoun, Tom Rasmussen, Diego Galavan,
//       Henish Patel, Reman Adhikari, Ethan Cobler
//
// generate_sounds.cpp = creates chiptune-style sound effects
// and writes them as WAV files in resources/sounds.

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <vector>

using std::string;
using std::vector;

const int SAMPLE_RATE = 44100;
const double PI = 3.14159265358979323846;

// Write mono 16-bit PCM WAV file.
void writeWav(const string& path, const vector<double>& samples)
{
    std::ofstream out(path, std::ios::binary);
    if (!out) {
        std::cerr << "Failed to open " << path << "\n";
        return;
    }

    int32_t dataSize = static_cast<int32_t>(samples.size() * sizeof(int16_t));
    int32_t chunkSize = 36 + dataSize;
    int16_t audioFormat = 1;   // PCM
    int16_t numChannels = 1;   // mono
    int32_t sampleRate = SAMPLE_RATE;
    int16_t bitsPerSample = 16;
    int16_t blockAlign = numChannels * bitsPerSample / 8;
    int32_t byteRate = sampleRate * blockAlign;

    out.write("RIFF", 4);
    out.write(reinterpret_cast<const char*>(&chunkSize), 4);
    out.write("WAVE", 4);

    out.write("fmt ", 4);
    int32_t subchunk1Size = 16;
    out.write(reinterpret_cast<const char*>(&subchunk1Size), 4);
    out.write(reinterpret_cast<const char*>(&audioFormat), 2);
    out.write(reinterpret_cast<const char*>(&numChannels), 2);
    out.write(reinterpret_cast<const char*>(&sampleRate), 4);
    out.write(reinterpret_cast<const char*>(&byteRate), 4);
    out.write(reinterpret_cast<const char*>(&blockAlign), 2);
    out.write(reinterpret_cast<const char*>(&bitsPerSample), 2);

    out.write("data", 4);
    out.write(reinterpret_cast<const char*>(&dataSize), 4);

    for (double s : samples) {
        int v = static_cast<int>(s * 32767.0);
        v = std::max(-32767, std::min(32767, v));
        int16_t sample = static_cast<int16_t>(v);
        out.write(reinterpret_cast<const char*>(&sample), sizeof(sample));
    }
}

// Convert note name + octave to frequency.
double noteFreq(const string& name, int octave = 5)
{
    static const std::map<string, int> table = {
        {"C",0}, {"C#",1}, {"Db",1}, {"D",2}, {"D#",3}, {"Eb",3},
        {"E",4}, {"F",5}, {"F#",6}, {"Gb",6}, {"G",7}, {"G#",8},
        {"Ab",8}, {"A",9}, {"A#",10}, {"Bb",10}, {"B",11}
    };

    return 261.63 * std::pow(2.0, (table.at(name) + (octave - 4) * 12) / 12.0);
}

// Square wave generator.
vector<double> square(double freq, double dur, double vol = 0.5)
{
    int n = static_cast<int>(SAMPLE_RATE * dur);
    vector<double> out;
    out.reserve(n);

    for (int i = 0; i < n; ++i) {
        double value = std::sin(2.0 * PI * freq * i / SAMPLE_RATE) >= 0.0 ? 1.0 : -1.0;
        out.push_back(vol * value);
    }
    return out;
}

// Sine wave generator.
vector<double> sine(double freq, double dur, double vol = 0.5)
{
    int n = static_cast<int>(SAMPLE_RATE * dur);
    vector<double> out;
    out.reserve(n);

    for (int i = 0; i < n; ++i) {
        out.push_back(vol * std::sin(2.0 * PI * freq * i / SAMPLE_RATE));
    }
    return out;
}

// White noise generator.
vector<double> noiseBuf(double dur, double vol = 0.3)
{
    int n = static_cast<int>(SAMPLE_RATE * dur);
    vector<double> out;
    out.reserve(n);

    static std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<double> dist(-1.0, 1.0);

    for (int i = 0; i < n; ++i) {
        out.push_back(vol * dist(rng));
    }
    return out;
}

// Simple ADSR envelope.
vector<double> env(const vector<double>& samples,
                   double attack = 0.01,
                   double decay = 0.05,
                   double sustain = 0.7,
                   double release = 0.1)
{
    int n = static_cast<int>(samples.size());
    int a = std::max(1, static_cast<int>(attack * SAMPLE_RATE));
    int d = std::max(1, static_cast<int>(decay * SAMPLE_RATE));
    int r = std::max(1, static_cast<int>(release * SAMPLE_RATE));
    int s = std::max(0, n - a - d - r);

    vector<double> out;
    out.reserve(n);

    for (int i = 0; i < n; ++i) {
        double m;
        if (i < a) {
            m = static_cast<double>(i) / a;
        } else if (i < a + d) {
            m = 1.0 - (1.0 - sustain) * (i - a) / d;
        } else if (i < a + d + s) {
            m = sustain;
        } else {
            m = sustain * std::max(0, n - i) / static_cast<double>(r);
        }
        out.push_back(samples[i] * m);
    }
    return out;
}

// Silence buffer.
vector<double> sil(double dur)
{
    return vector<double>(static_cast<size_t>(SAMPLE_RATE * dur), 0.0);
}

// Concatenate multiple sample buffers.
vector<double> cat(const std::initializer_list<vector<double>>& parts)
{
    vector<double> out;
    for (const auto& p : parts) {
        out.insert(out.end(), p.begin(), p.end());
    }
    return out;
}

// Pad a buffer with zeros.
vector<double> padTo(const vector<double>& buf, size_t n)
{
    vector<double> out = buf;
    if (out.size() < n) {
        out.resize(n, 0.0);
    }
    return out;
}

// Mix two buffers together with clipping.
vector<double> mixBuffers(const vector<double>& a, const vector<double>& b)
{
    size_t n = std::max(a.size(), b.size());
    vector<double> pa = padTo(a, n);
    vector<double> pb = padTo(b, n);

    vector<double> out;
    out.reserve(n);

    for (size_t i = 0; i < n; ++i) {
        double v = pa[i] + pb[i];
        v = std::max(-0.94, std::min(0.94, v));
        out.push_back(v);
    }
    return out;
}

int main()
{
    std::filesystem::create_directories("resources/sounds");

    // CLICK
    vector<double> click = cat({
        env(square(380, 0.022, 0.52), 0.001, 0.008, 0.50, 0.010),
        env(square(620, 0.038, 0.52), 0.001, 0.012, 0.45, 0.015)
    });
    writeWav("resources/sounds/click.wav", click);

    // CORRECT
    vector<double> correct = cat({
        env(square(noteFreq("C",5), 0.07, 0.58), 0.003, 0.018, 0.65, 0.025),
        env(square(noteFreq("E",5), 0.07, 0.58), 0.003, 0.018, 0.65, 0.025),
        env(square(noteFreq("G",5), 0.07, 0.58), 0.003, 0.018, 0.65, 0.025),
        env(square(noteFreq("C",6), 0.22, 0.62), 0.005, 0.040, 0.72, 0.080)
    });
    writeWav("resources/sounds/correct.wav", correct);

    // WRONG
    vector<double> wrong = cat({
        env(square(260, 0.10, 0.68), 0.003, 0.020, 0.80, 0.040),
        env(square(190, 0.10, 0.68), 0.003, 0.020, 0.80, 0.040),
        env(square(130, 0.16, 0.58), 0.003, 0.040, 0.65, 0.070)
    });
    writeWav("resources/sounds/wrong.wav", wrong);

    // PASS
    vector<double> passSnd = cat({
        env(square(noteFreq("C",5), 0.07, 0.58), 0.003, 0.018, 0.70, 0.025),
        env(square(noteFreq("D",5), 0.07, 0.58), 0.003, 0.018, 0.70, 0.025),
        env(square(noteFreq("E",5), 0.07, 0.58), 0.003, 0.018, 0.70, 0.025),
        env(square(noteFreq("G",5), 0.07, 0.58), 0.003, 0.018, 0.70, 0.025),
        env(square(noteFreq("C",6), 0.07, 0.58), 0.003, 0.018, 0.70, 0.025),
        sil(0.04),
        env(square(noteFreq("C",6), 0.38, 0.65), 0.010, 0.050, 0.75, 0.120)
    });
    writeWav("resources/sounds/pass.wav", passSnd);

    // FAIL
    vector<double> failSnd = cat({
        env(square(noteFreq("G", 5),  0.11, 0.57), 0.005, 0.025, 0.72, 0.040),
        env(square(noteFreq("Gb",5),  0.11, 0.57), 0.005, 0.025, 0.72, 0.040),
        env(square(noteFreq("F", 5),  0.11, 0.57), 0.005, 0.025, 0.72, 0.040),
        env(square(noteFreq("E", 5),  0.30, 0.52), 0.005, 0.050, 0.62, 0.115)
    });
    writeWav("resources/sounds/fail.wav", failSnd);

    // JUGGLE
    vector<double> jugLo = env(sine(85, 0.14, 0.85), 0.001, 0.055, 0.28, 0.060);
    vector<double> jugHi = env(square(720, 0.055, 0.38), 0.001, 0.012, 0.30, 0.022);
    vector<double> juggle = mixBuffers(jugLo, jugHi);
    writeWav("resources/sounds/juggle.wav", juggle);

    // THUD
    vector<double> thudBase  = env(sine(62, 0.26, 0.92), 0.001, 0.060, 0.24, 0.110);
    vector<double> thudCrack = env(noiseBuf(0.055, 0.32), 0.001, 0.018, 0.18, 0.028);
    vector<double> thud = mixBuffers(thudBase, thudCrack);
    writeWav("resources/sounds/thud.wav", thud);

    // CEILING
    vector<double> ceilBase = env(square(1450, 0.13, 0.57), 0.001, 0.020, 0.42, 0.080);
    vector<double> ceilOver = env(sine(2200, 0.07, 0.30), 0.001, 0.010, 0.28, 0.040);
    vector<double> ceiling = mixBuffers(ceilBase, ceilOver);
    writeWav("resources/sounds/ceiling.wav", ceiling);

    std::cout << "Sound files generated:\n";
    for (const string& name : {"click", "correct", "wrong", "pass", "fail", "juggle", "thud", "ceiling"}) {
        string path = "resources/sounds/" + name + ".wav";
        auto size = std::filesystem::file_size(path);
        std::cout << "  " << std::left << std::setw(12) << name
                  << std::right << std::setw(10) << size << " bytes\n";
    }

    return 0;
}
