#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QAudioOutput>
#include <QBuffer>
#include <QDebug>
#include <QVBoxLayout>
#include "tonerow.h"
#include "audioconstants.h"
#include "spectrogram.h"
#include "styles.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QByteArray* gen;
    QBuffer* input;
    QAudioOutput* audio;
    QVector<ToneRow*> controlrows;
    QVBoxLayout* rowarea;
    Spectrogram* gram;
    quint16 bpm;
    quint16 beats;
    quint16 tracks;

    void addRows(quint16 rows);
    void deleteRows();
    void compileSound();
    qreal highestNote();

private slots:
    void playTone();
    void stopPlaying();
    void handleStateChanged(QAudio::State newState);
    void drawGraph();
};

#endif // MAINWINDOW_H
