#ifndef TONEGENERATOR_H
#define TONEGENERATOR_H

#include <QObject>
#include <QtMath>
#include <cmath>
#include "audioconstants.h"

typedef enum waveform {
    wvsin, wvsquare, wvtriangle, wvsawtooth
} waveform;

class ToneGenerator : public QObject
{
    Q_OBJECT
public:
    explicit ToneGenerator(QObject *parent = 0);
    ~ToneGenerator();

    void setNewFreq(qreal newfreq);
    QByteArray* makeTone(QByteArray* buf, quint16 bpm, quint16 beat_index, quint16 beat_end, quint16 tracks);
    qreal getFreq();
    void setInstrument(waveform a, waveform b, qreal mix);

private:
    qreal freq;
    qreal mixka;
    qreal mixkb;
    qreal (*soundFuncA)(qreal x);
    qreal (*soundFuncB)(qreal x);

    static qreal sinInst(qreal x);
    static qreal squareInst(qreal x);
    static qreal triangleInst(qreal x);
    static qreal sawtoothInst(qreal x);
    static qreal mixInst(qreal x, qreal ka, qreal kb, qreal (*af)(qreal x), qreal (*bf)(qreal x));

signals:

public slots:
};

#endif // TONEGENERATOR_H
