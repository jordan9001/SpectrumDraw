#include "tonegenerator.h"

ToneGenerator::ToneGenerator(QObject *parent) : QObject(parent)
{
    // setup the defaultformat for this one
    this->toneformat.setSampleRate(TG_SAMPLE_RATE);
    this->toneformat.setChannelCount(1);
    this->toneformat.setSampleSize(TG_SAMPLE_SIZE);
    this->toneformat.setCodec(TG_CODEC);
    this->toneformat.setByteOrder(QAudioFormat::LittleEndian);
    this->toneformat.setSampleType(QAudioFormat::SignedInt);
}

ToneGenerator::~ToneGenerator()
{

}

QByteArray* ToneGenerator::makeTone(QByteArray* buf, quint16 freq, quint16 start_t, quint16 end_t)
{
    // sin(2 Pi Hz t)
    // sin(2 Pi Hz i / sample_rate)
    // sin(TG_FREQ Hz i)
    qreal t;

    // make sure we have the size
    if (buf->size() < end_t * TG_SAMPLE_RATE) {
        buf->resize(end_t * TG_SAMPLE_RATE);
    }

    for (quint16 i=start_t * TG_SAMPLE_RATE; i<(end_t * TG_SAMPLE_RATE); i++) {
        t = (qreal)(freq * i);
        t = t * TG_FREQ_CONST;
        t = qSin(t);
        // now we normalize t
        t *= TG_MAX_VAL;
        (*buf)[i] = (quint8)t;
    }
    return buf;
}
