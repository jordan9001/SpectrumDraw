#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QAudioOutput>
#include <QBuffer>
#include <QDebug>
#include "tonegenerator.h"

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
    ToneGenerator* tg;
    QByteArray* gen;
    QBuffer* input;
    QAudioOutput* audio;

private slots:
    void playTone();
    void stopPlaying();
    void handleStateChanged(QAudio::State newState);
};

#endif // MAINWINDOW_H
