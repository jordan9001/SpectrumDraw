#include "tonegenerator.h"

ToneGenerator::ToneGenerator(QObject *parent) : QObject(parent)
{
    this->freq = 440.0;
}

ToneGenerator::~ToneGenerator()
{

}

QByteArray* ToneGenerator::makeTone(QByteArray* buf, quint16 bpm, quint16 beat_index, quint16 beat_count, quint16 tracks)
{
    // sin(2 Pi Hz t)
    // sin(2 Pi Hz i / sample_rate)
    // sin(TG_FREQ Hz i)
    qreal t;
    quint16 start, end;

    // samples = beats * (min/beat) * (sec/min) * (samples/sec)
    start = beat_index * 60 * SPD_SAMPLE_RATE / bpm;
    end = start + beat_count * 60 * SPD_SAMPLE_RATE / bpm;

    // make sure we have the size
    if (buf->size() < end) {
        buf->resize(end);
    }

    for (quint16 i=start; i<end; i++) {
        t = freq * (qreal)i;
        t = t * SPD_FREQ_CONST;
        t = qSin(t);
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

