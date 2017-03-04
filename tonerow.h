#ifndef TONEROW_H
#define TONEROW_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include "tonegenerator.h"

#define DEFAULT_HIGHEST 200;

class ToneRow : public QWidget
{
    Q_OBJECT
public:
    explicit ToneRow(QWidget* parent = 0, quint16 length = 64, QString note = "A3", qreal frequency = 220.00);
    ~ToneRow();

    QByteArray* generateTrack(QByteArray* buf, quint16 bpm, quint16 tracks);
    qreal getFreq();

signals:

public slots:

private:
    QHBoxLayout* rowlayout;
    QLabel* note;
    qreal frequency;
    QVector<QCheckBox*> chkBoxVec;
    ToneGenerator* instrument;
};

#endif // TONEROW_H
