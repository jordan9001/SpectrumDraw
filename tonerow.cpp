#include "tonerow.h"

ToneRow::ToneRow(QWidget* parent, quint16 length) : QWidget(parent)
{
    // make our layout
    this->rowlayout = new QHBoxLayout();
    // add the combobox
    this->noteCombo = new QComboBox();
    this->noteCombo->addItem("A3", 220.00);
    this->noteCombo->addItem("C4", 261.63);
    this->noteCombo->addItem("E4", 329.63);
    this->noteCombo->addItem("G4", 392.00);
    this->noteCombo->addItem("A4", 440.00);
    this->noteCombo->addItem("C5", 523.25);
    this->rowlayout->addWidget(this->noteCombo);

    // add the length of qcheckboxes
    this->chkBoxVec = QVector<QCheckBox*>(length);
    for (int i=0; i<length; i++) {
        QCheckBox* box = new QCheckBox();
        box->setChecked(false);
        this->chkBoxVec[i] = box;
        this->rowlayout->addWidget(box);
    }

    this->setLayout(this->rowlayout);

    this->instrument = new ToneGenerator();

    connect(this->noteCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(changeNote(int)));
}

ToneRow::~ToneRow()
{
    delete this->instrument;
    for (int i=0; i<this->chkBoxVec.size(); i++) {
        delete this->chkBoxVec[i];
    }
    delete this->noteCombo;
    delete this->rowlayout;
}

QByteArray* ToneRow::generateTrack(QByteArray *buf, quint16 bpm, quint16 tracks)
{
    int start = 0;
    //loop through the on areas
    for (int i=0; i<this->chkBoxVec.size(); i++) {
        if (!this->chkBoxVec[i]->isChecked() && start != -1) {
            this->instrument->makeTone(buf, bpm, start, i - start, tracks);
            start = -1;
        }
        if (this->chkBoxVec[i]->isChecked() && start == -1) {
            start = i;
        }
    }
    return buf;
}

void ToneRow::changeNote(int)
{
    qreal value = this->noteCombo->currentData().toReal();
    this->instrument->setNewFreq(value);
}

