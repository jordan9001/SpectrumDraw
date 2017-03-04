#include "tonerow.h"

ToneRow::ToneRow(QWidget* parent, quint16 length, QString note, qreal frequency) : QWidget(parent)
{
    // make our layout
    this->rowlayout = new QHBoxLayout();
    this->frequency = frequency;
    // add the label
    this->note = new QLabel(note);
    this->rowlayout->addWidget(this->note);

    // add the length of qcheckboxes
    this->chkBoxVec = QVector<QCheckBox*>(length);
    for (int i=0; i<length; i++) {
        QCheckBox* box = new QCheckBox();
        box->setChecked(false);
        this->chkBoxVec[i] = box;
        this->rowlayout->addWidget(box);
    }

    this->rowlayout->setSpacing(4);
    this->rowlayout->setMargin(0);
    this->rowlayout->setContentsMargins(0,0,0,0);
    this->rowlayout->setAlignment(Qt::AlignLeft);
    this->setLayout(this->rowlayout);

    this->instrument = new ToneGenerator();
}

ToneRow::~ToneRow()
{
    delete this->instrument;
    for (int i=0; i<this->chkBoxVec.size(); i++) {
        delete this->chkBoxVec[i];
    }
    delete this->note;
    delete this->rowlayout;
}

QByteArray* ToneRow::generateTrack(QByteArray *buf, quint16 bpm, quint16 tracks)
{
    int start = 0;
    int len = this->chkBoxVec.size();
    //loop through the on areas
    for (int i=0; i<len; i++) {
        if (this->chkBoxVec[i]->isChecked() && start == -1) {
            start = i;
        }
        if (start != -1 && (!this->chkBoxVec[i]->isChecked() || i==len-1)) {
            int end = i - start;
            if (i==len-1 && this->chkBoxVec[i]->isChecked()) {
                end = len - start;
            }
            this->instrument->makeTone(buf, bpm, start, end, tracks);
            start = -1;
        }
    }
    return buf;
}

qreal ToneRow::getFreq()
{
    return this->frequency;
}

