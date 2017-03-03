#ifndef SPECTROGRAM_H
#define SPECTROGRAM_H

#include <QWidget>
#include <QPainter>
#include <QVector>
#include <QtMath>
#include "audioconstants.h"

#define WIN_A0          0.42
#define WIN_A1          0.50
#define WIN_A2          0.08
#define WIN_C1          (2.0 * M_PI)
#define WIN_C2          (4.0 * M_PI)

class Spectrogram : public QWidget
{
    Q_OBJECT
public:
    explicit Spectrogram(QWidget* parent = 0);
    ~Spectrogram();

    void drawGram(const QByteArray* buf, quint16 bpm);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);

private:
    QVector<qreal>* dft(const QByteArray* buf, int start, int spectrum_size);
    void cleanGram();

    QBrush brush;
    QPainter painter;
    QVector<QVector<qreal>*> gram;
};

#endif // SPECTROGRAM_H
