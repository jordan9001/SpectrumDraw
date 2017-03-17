#include "tonegenerator.h"

ToneGenerator::ToneGenerator(QObject *parent) : QObject(parent)
{
    freq = 440.0;
    // set the default sound
    mixka = 0.2;
    mixkb = 0.8;
    soundFuncA = ToneGenerator::sinInst;
    soundFuncB = ToneGenerator::triangleInst;
}

ToneGenerator::~ToneGenerator()
{

}

QByteArray* ToneGenerator::makeTone(QByteArray* buf, quint16 bpm, quint16 beat_index, quint16 beat_end, quint16 tracks)
{
    // sin(2 Pi Hz t)
    // sin(2 Pi Hz i / sample_rate)
    // sin(TG_FREQ Hz i)
    qreal t;
    uint start, end;

    // samples = beats * (min/beat) * (sec/min) * (samples/sec)
    start = ((uint)beat_index) * 60 * SPD_SAMPLE_RATE / (uint)bpm;
    end = ((uint)beat_end) * 60 * SPD_SAMPLE_RATE / (uint)bpm;

    // make sure we have the size
    if ((uint)buf->size() < end) {
        buf->resize(end);
    }

    for (uint i=start; i<end; i++) {
        t = freq * (qreal)i;
        t = t * SPD_FREQ_CONST;
        t = ToneGenerator::mixInst(t, mixka, mixkb, soundFuncA, soundFuncB);
        // now we normalize t
        t *= SPD_MAX_VAL / tracks;
        qint16 mix = (qint16)(t) + (qint16)(*buf)[i];
        if (mix > SPD_MAX_VAL) {
            mix = SPD_MAX_VAL;
        } else if (mix < -SPD_MAX_VAL) {
            mix = -SPD_MAX_VAL;
        }
        (*buf)[i] = (qint8)mix;
    }
    return buf;
}

void ToneGenerator::setNewFreq(qreal newfreq) {
    this->freq = newfreq;
}

qreal ToneGenerator::getFreq()
{
    return this->freq;
}

void ToneGenerator::setInstrument(waveform a, waveform b, qreal mix)
{
    switch (a) {
    case wvsquare:
        soundFuncA = ToneGenerator::squareInst;
    case wvtriangle:
        soundFuncA = ToneGenerator::triangleInst;
    case wvsawtooth:
        soundFuncA = ToneGenerator::sawtoothInst;
    default:
        soundFuncA = ToneGenerator::sinInst;
    }
    switch (b) {
    case wvsquare:
        soundFuncB = ToneGenerator::squareInst;
    case wvtriangle:
        soundFuncB = ToneGenerator::triangleInst;
    case wvsawtooth:
        soundFuncB = ToneGenerator::sawtoothInst;
    default:
        soundFuncB = ToneGenerator::sinInst;
    }
    mixka = mix;
    mixkb = 1.0 - mix;
}

qreal ToneGenerator::sinInst(qreal x)
{
    return qSin(x);
}

qreal ToneGenerator::squareInst(qreal x)
{
    return (fmod(x, 2*M_PI) > M_PI) ? 1.0 : -1.0;
}

qreal ToneGenerator::triangleInst(qreal x)
{
    qreal ret = fmod(x, M_PI) / (M_PI/2);
    if (fmod(x, 2*M_PI) > M_PI) {
        ret = -ret + 1;
    } else {
        ret = ret - 1;
    }
    return ret;
}

qreal ToneGenerator::sawtoothInst(qreal x)
{
    return (fmod(x, 2*M_PI) / M_PI) - 1;
}

qreal ToneGenerator::mixInst(qreal x, qreal ka, qreal kb, qreal (*af)(qreal), qreal (*bf)(qreal))
{
    if (bf == NULL) {
        return af(x);
    }

    qreal reta = af(x);
    qreal retb = bf(x);
    return (ka * reta) + (kb * retb);
}
