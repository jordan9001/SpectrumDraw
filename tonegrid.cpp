#include "tonegrid.h"

ToneGrid::ToneGrid(QWidget *parent, quint16 length, quint16 notesize, qint16 startoffset) : QWidget(parent)
{
    this->dirty = true;
    this->key = genNotes(notesize, startoffset);
    this->key_len = notesize;
    this->row_len = length;

    grid = new QGridLayout(this);
    this->grid->setSpacing(0);
    this->grid->setContentsMargins(0,0,0,0);
    this->grid->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    // add our labels and make our instruments
    labelVec.resize(key_len);
    instrumentVec.resize(key_len);
    for (int i=0; i<key_len; i++) {
        QLabel* l = new QLabel(key[key_len - i - 1].name,this);
        labelVec[i] = l;
        this->grid->addWidget(l, i, 0, Qt::AlignLeft);
        ToneGenerator* tg = new ToneGenerator();
        tg->setNewFreq(key[key_len - i - 1].freq);
        instrumentVec[i] = tg;
    }

    // add our chkboxes
    chkBoxVec.resize(row_len * key_len);
    for (int row=0; row<key_len; row++){
        for (int i=0; i<row_len; i++) {
            QCheckBox* box = new QCheckBox();
            box->setChecked(false);
            connect(box,SIGNAL(clicked()),this,SLOT(makeDirty()));
            this->chkBoxVec[(row*row_len)+i] = box;
            this->grid->addWidget(box, row, i+1, Qt::AlignTop);
        }
    }

    this->setLayout(grid);

    // our waveform buffer
    this->buf = NULL;
}

ToneGrid::~ToneGrid()
{
    //buf
    if (this->buf != NULL) {
        delete this->buf;
    }
    // check boxes
    for (int i=0; i<this->chkBoxVec.size(); i++) {
        delete this->chkBoxVec[i];
    }
    // instruments
    for (int i=0; i<this->instrumentVec.size(); i++) {
        delete this->instrumentVec[i];
    }
    // labels
    for (int i=0; i<this->labelVec.size(); i++) {
        delete this->labelVec[i];
    }
    delete this->grid;
    delete [] this->key;
}

void ToneGrid::setKey(note_t *inkey, size_t insize)
{
    key = inkey;
    key_len = insize;
    // set the instruments
    for (int i=0; i<key_len; i++) {
        instrumentVec[i]->setNewFreq(key[i].freq);
    }
    dirty = true;
}

QByteArray* ToneGrid::generateTrack(quint16 bpm) {
    if (!dirty && buf != NULL) {
        return buf;
    }
    if (buf != NULL) {
        delete buf;
    }
    quint16 beatlen = getBeatLength();
    buf = new QByteArray(beatlen * 60 * SPD_SAMPLE_RATE / bpm, 0);

    quint16 tracks_used = getUsedTrackCount();

    quint16 inst_i;
    qint32 start = -1;
    for (quint16 i=0; i<this->chkBoxVec.size(); i++) {
        // don't handle empty columns
        if ((i%row_len) >= beatlen) {
            continue;
        }
        if (this->chkBoxVec[i]->isChecked()) {
            if (start == -1) {
                // start a new note
                start = i % row_len;
            }
            // if we are at the end of a row
            if (!(((i+1) % row_len)%beatlen)) {
                // play the note
                inst_i = i / row_len;
                instrumentVec[inst_i]->makeTone(buf, bpm, start, ((i%row_len)%beatlen)+1, tracks_used);
                start = -1;
            }
        } else if (start != -1) {
            // finish a note
            inst_i = i / row_len;
            instrumentVec[inst_i]->makeTone(buf, bpm, start, i%row_len, tracks_used);
            start = -1;
        }
    }
    dirty = false;
    return buf;
}

qreal ToneGrid::getHighestUsedNote()
{
    for (quint16 i=0; i<key_len; i++) {
        if (isTrackUsed(i)) {
            return instrumentVec[i]->getFreq();
        }
    }
    return SPD_MIN_FREQ;
}

quint16 ToneGrid::getUsedTrackCount()
{
    quint16 count = 0;
    for (int i=0; i<key_len; i++) {
        if (isTrackUsed(i)) {
            count++;
        }
    }
    return count;
}

quint16 ToneGrid::getBeatLength()
{
    for (qint32 i=row_len-1; i>=0; i--) {
        if (isColUsed(i)) {
            return i+1;
        }
    }
    return 0;
}

void ToneGrid::clearGrid()
{
    this->dirty = true;
    for (int i=0; i<chkBoxVec.size(); i++) {
        chkBoxVec[i]->setChecked(false);
    }
}

void ToneGrid::makeDirty()
{
    this->dirty = true;
}

bool ToneGrid::isTrackUsed(quint16 index)
{
    for (quint16 i=(index*row_len); i<((index+1)*row_len); i++) {
        if (chkBoxVec.at(i)->isChecked()) {
            return true;
        }
    }
    return false;
}

bool ToneGrid::isColUsed(quint16 index)
{
    for (quint16 i=0; i<key_len; i++) {
        quint16 j = (i*row_len) + index;
        if (chkBoxVec.at(j)->isChecked()) {
            return true;
        }
    }
    return false;
}
