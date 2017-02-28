#ifndef TONEGENERATOR_H
#define TONEGENERATOR_H

#include <QObject>
#include <QAudioFormat>
#include <QtMath>

#define TG_SAMPLE_RATE  8000
#define TG_FREQ_CONST   ((2.0 * M_PI) / 8000.0)
#define TG_SAMPLE_SIZE  8
#define TG_CODEC        "audio/pcm"
#define TG_MAX_VAL      126

class ToneGenerator : public QObject
{
    Q_OBJECT
public:
    explicit ToneGenerator(QObject *parent = 0);
    ~ToneGenerator();

    QAudioFormat toneformat;

    QByteArray* makeTone(QByteArray* buf, quint16 freq, quint16 start_t, quint16 end_t);

signals:

public slots:
};

#endif // TONEGENERATOR_H
