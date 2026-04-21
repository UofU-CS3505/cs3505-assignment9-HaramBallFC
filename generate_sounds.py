"""
Generate HaramBall FC chiptune sound effects.
Run from repo root:  python3 generate_sounds.py
No external dependencies — uses only Python stdlib.
Background music (background.mp3) is downloaded separately via yt-dlp.
"""
import wave, struct, math, os, random

SAMPLE_RATE = 44100

def write_wav(path, samples):
    with wave.open(path, 'w') as f:
        f.setnchannels(1)
        f.setsampwidth(2)
        f.setframerate(SAMPLE_RATE)
        buf = bytearray()
        for s in samples:
            v = max(-32767, min(32767, int(s * 32767)))
            buf += struct.pack('<h', v)
        f.writeframes(bytes(buf))

def note_freq(name, octave=5):
    table = {'C':0,'C#':1,'Db':1,'D':2,'D#':3,'Eb':3,'E':4,
             'F':5,'F#':6,'Gb':6,'G':7,'G#':8,'Ab':8,'A':9,'A#':10,'Bb':10,'B':11}
    return 261.63 * (2 ** ((table[name] + (octave - 4) * 12) / 12.0))

def square(freq, dur, vol=0.5):
    n = int(SAMPLE_RATE * dur)
    return [vol * (1.0 if math.sin(2*math.pi*freq*i/SAMPLE_RATE) >= 0 else -1.0)
            for i in range(n)]

def sine(freq, dur, vol=0.5):
    n = int(SAMPLE_RATE * dur)
    return [vol * math.sin(2*math.pi*freq*i/SAMPLE_RATE) for i in range(n)]

def noise_buf(dur, vol=0.3):
    return [vol * (random.random()*2-1) for _ in range(int(SAMPLE_RATE * dur))]

def env(samples, attack=0.01, decay=0.05, sustain=0.7, release=0.1):
    n = len(samples)
    a = max(1, int(attack  * SAMPLE_RATE))
    d = max(1, int(decay   * SAMPLE_RATE))
    r = max(1, int(release * SAMPLE_RATE))
    s = max(0, n - a - d - r)
    out = []
    for i, v in enumerate(samples):
        if   i < a:       m = i / a
        elif i < a+d:     m = 1.0 - (1.0-sustain)*(i-a)/d
        elif i < a+d+s:   m = sustain
        else:             m = sustain * max(0, n-i) / r
        out.append(v * m)
    return out

def sil(dur):
    return [0.0] * int(SAMPLE_RATE * dur)

def cat(*parts):
    out = []
    for p in parts: out.extend(p)
    return out

def pad_to(buf, n):
    return buf + [0.0] * max(0, n - len(buf))

os.makedirs('resources/sounds', exist_ok=True)

# CLICK
click = cat(
    env(square(380, 0.022, 0.52), 0.001, 0.008, 0.50, 0.010),
    env(square(620, 0.038, 0.52), 0.001, 0.012, 0.45, 0.015),
)
write_wav('resources/sounds/click.wav', click)

# CORRECT
correct = cat(
    env(square(note_freq('C',5), 0.07, 0.58), 0.003, 0.018, 0.65, 0.025),
    env(square(note_freq('E',5), 0.07, 0.58), 0.003, 0.018, 0.65, 0.025),
    env(square(note_freq('G',5), 0.07, 0.58), 0.003, 0.018, 0.65, 0.025),
    env(square(note_freq('C',6), 0.22, 0.62), 0.005, 0.040, 0.72, 0.080),
)
write_wav('resources/sounds/correct.wav', correct)

# WRONG
wrong = cat(
    env(square(260, 0.10, 0.68), 0.003, 0.020, 0.80, 0.040),
    env(square(190, 0.10, 0.68), 0.003, 0.020, 0.80, 0.040),
    env(square(130, 0.16, 0.58), 0.003, 0.040, 0.65, 0.070),
)
write_wav('resources/sounds/wrong.wav', wrong)

# PASS
pass_snd = cat(
    env(square(note_freq('C',5), 0.07, 0.58), 0.003, 0.018, 0.70, 0.025),
    env(square(note_freq('D',5), 0.07, 0.58), 0.003, 0.018, 0.70, 0.025),
    env(square(note_freq('E',5), 0.07, 0.58), 0.003, 0.018, 0.70, 0.025),
    env(square(note_freq('G',5), 0.07, 0.58), 0.003, 0.018, 0.70, 0.025),
    env(square(note_freq('C',6), 0.07, 0.58), 0.003, 0.018, 0.70, 0.025),
    sil(0.04),
    env(square(note_freq('C',6), 0.38, 0.65), 0.010, 0.050, 0.75, 0.120),
)
write_wav('resources/sounds/pass.wav', pass_snd)

# FAIL
fail_snd = cat(
    env(square(note_freq('G', 5),  0.11, 0.57), 0.005, 0.025, 0.72, 0.040),
    env(square(note_freq('Gb',5),  0.11, 0.57), 0.005, 0.025, 0.72, 0.040),
    env(square(note_freq('F', 5),  0.11, 0.57), 0.005, 0.025, 0.72, 0.040),
    env(square(note_freq('E', 5),  0.30, 0.52), 0.005, 0.050, 0.62, 0.115),
)
write_wav('resources/sounds/fail.wav', fail_snd)

# JUGGLE
jug_lo = env(sine(85, 0.14, 0.85), 0.001, 0.055, 0.28, 0.060)
jug_hi = env(square(720, 0.055, 0.38), 0.001, 0.012, 0.30, 0.022)
n = max(len(jug_lo), len(jug_hi))
juggle = [min(0.94, max(-0.94, a+b)) for a, b in zip(pad_to(jug_lo,n), pad_to(jug_hi,n))]
write_wav('resources/sounds/juggle.wav', juggle)

# THUD
thud_base  = env(sine(62, 0.26, 0.92), 0.001, 0.060, 0.24, 0.110)
thud_crack = env(noise_buf(0.055, 0.32), 0.001, 0.018, 0.18, 0.028)
n = max(len(thud_base), len(thud_crack))
thud = [min(0.94, max(-0.94, a+b)) for a, b in zip(pad_to(thud_base,n), pad_to(thud_crack,n))]
write_wav('resources/sounds/thud.wav', thud)

# CEILING
ceil_base = env(square(1450, 0.13, 0.57), 0.001, 0.020, 0.42, 0.080)
ceil_over = env(sine(2200,   0.07, 0.30), 0.001, 0.010, 0.28, 0.040)
n = max(len(ceil_base), len(ceil_over))
ceiling = [min(0.94, max(-0.94, a+b)) for a, b in zip(pad_to(ceil_base,n), pad_to(ceil_over,n))]
write_wav('resources/sounds/ceiling.wav', ceiling)

print("Sound files generated:")
for name in ['click','correct','wrong','pass','fail','juggle','thud','ceiling']:
    p = f'resources/sounds/{name}.wav'
    print(f"  {name:12s}  {os.path.getsize(p):>8,} bytes")
