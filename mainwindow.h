#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QAudioOutput>
#include <QBuffer>
#include <QDebug>
#include <QVBoxLayout>
#include "audioconstants.h"
#include "spectrogram.h"
#include "tonegrid.h"
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
    QBuffer* input;
    QAudioOutput* audio;
    Spectrogram* gram;
    quint16 bpm;
    quint16 beats;
    ToneGrid* tonegrid;

private slots:
    void playTone();
    void stopPlaying();
    void handleStateChanged(QAudio::State newState);
    void drawGraph();
};

#endif // MAINWINDOW_H
