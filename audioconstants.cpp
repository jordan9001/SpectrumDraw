#include "audioconstants.h"

note_t* genNotes(quint16 steps, qint16 startA3offset) {
    note_t* key = new note_t[steps];
    for (int i=0; i<steps; ++i) {
        key[i].freq = A3 * qPow(TWO_ROOT_12, startA3offset + i);
        int oct = ((startA3offset + i) / 12) + 3;
        key[i].name = QString(note_names[(startA3offset + i)%12]);
        key[i].name.append(QString::number(oct));
    }
    return key;
}
