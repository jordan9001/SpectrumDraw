#ifndef SPECTROGRAM_H
#define SPECTROGRAM_H

#include <QWidget>
#include <QVector>
#include <QtMath>
#include "audioconstants.h"

#define SPECTRUM_SIZE   2000

#define WIN_A0          0.42
#define WIN_A1          0.50
#define WIN_A2          0.08
#define WIN_C1          (2.0 * M_PI)
#define WIN_C2          (4.0 * M_PI)

class Spectrogram : public QWidget
{
    Q_OBJECT
public:
    explicit Spectrogram(QWidget *parent = 0);
    ~Spectrogram();

signals:

public slots:

private:
    QVector<qreal>* dft(const QByteArray* buf, int start, int spectrum_size);
};

#endif // SPECTROGRAM_H
