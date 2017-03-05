#ifndef TONEGRID_H
#define TONEGRID_H

#include <QWidget>
#include <QGridLayout>
#include <QCheckBox>
#include <QLabel>
#include "tonegenerator.h"
#include "audioconstants.h"

class ToneGrid : public QWidget
{
    Q_OBJECT
public:
    explicit ToneGrid(QWidget *parent = 0, quint16 length = 64, const note_t* inkey = C_MAJOR, quint16 insize = C_MAJOR_SIZE);
    ~ToneGrid();

    void setKey(const note_t* inkey, size_t insize);
    QByteArray* generateTrack(quint16 bpm);
    qreal getHighestUsedNote();
    quint16 getUsedTrackCount();
    quint16 getBeatLength();
    void clearGrid();

signals:

public slots:
    void makeDirty();

private:
    QGridLayout* grid;
    const note_t* key;
    quint16 key_len;
    quint16 row_len;
    QVector<QLabel*> labelVec;
    QVector<QCheckBox*> chkBoxVec;
    QVector<ToneGenerator*> instrumentVec;
    QByteArray* buf;
    bool dirty;

    bool isTrackUsed(quint16 index);
    bool isColUsed(quint16 index);
};

#endif // TONEGRID_H
