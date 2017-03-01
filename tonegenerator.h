#ifndef TONEGENERATOR_H
#define TONEGENERATOR_H

#include <QObject>
#include <QtMath>
#include "audioconstants.h"

class ToneGenerator : public QObject
{
    Q_OBJECT
public:
    explicit ToneGenerator(QObject *parent = 0);
    ~ToneGenerator();

    void setNewFreq(qreal newfreq);
    QByteArray* makeTone(QByteArray* buf, quint16 bpm, quint16 beat_index, quint16 beat_count, quint16 tracks);

private:
    qreal freq;

signals:

public slots:
};

#endif // TONEGENERATOR_H
