#ifndef TONEROW_H
#define TONEROW_H

#include <QWidget>
#include <QHBoxLayout>
#include <QComboBox>
#include <QCheckBox>
#include "tonegenerator.h"

#define DEFAULT_HIGHEST 200;

class ToneRow : public QWidget
{
    Q_OBJECT
public:
    explicit ToneRow(QWidget* parent = 0, quint16 length = 64);
    ~ToneRow();

    QByteArray* generateTrack(QByteArray* buf, quint16 bpm, quint16 tracks);
    qreal getFreq();

signals:

public slots:
    void changeNote(int);

private:
    QHBoxLayout* rowlayout;
    QComboBox* noteCombo;
    QVector<QCheckBox*> chkBoxVec;
    ToneGenerator* instrument;
};

#endif // TONEROW_H
