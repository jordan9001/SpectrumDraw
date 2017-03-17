#ifndef AUDIOCONSTANTS
#define AUDIOCONSTANTS

#define SPD_SAMPLE_RATE     8000
#define SPD_FREQ_CONST      ((2.0 * M_PI) / (qreal)SPD_SAMPLE_RATE)
#define SPD_SAMPLE_SIZE     8
#define SPD_CODEC           "audio/pcm"
#define SPD_MAX_VAL         126
#define SPD_MAX_SPECTRUM    800
#define SPD_MIN_FREQ        200
#define SPD_NUM_CHAN        1

#define TWO_ROOT_12         1.059463094359295
#define A3                  220.00

#include <QObject>
#include <QtMath>

const QString note_names[] = {
    "A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#",
};

typedef struct note_t {
    qreal freq;
    QString name;
}note_t;

note_t* genNotes(quint16 steps, qint16 startA3offset);

#endif // AUDIOCONSTANTS

