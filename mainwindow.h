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

#define TONE_AREA_STYLE ""

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
    quint16 bpm;
    quint16 beats;
    quint16 tracks;

    void addRows(quint16 rows);
    void deleteRows();
    void compileSound();

private slots:
    void playTone();
    void stopPlaying();
    void handleStateChanged(QAudio::State newState);
};

#endif // MAINWINDOW_H
