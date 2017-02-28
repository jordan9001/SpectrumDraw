#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->tg = new ToneGenerator();
    this->gen = new QByteArray();

    connect(ui->PlayButton,SIGNAL(clicked()),this,SLOT(playTone()));
    connect(ui->StopButton,SIGNAL(clicked()),this,SLOT(stopPlaying()));
}

MainWindow::~MainWindow()
{
    delete tg;
    delete ui;
}

void MainWindow::playTone()
{
    // get the QByte array from tone generator
    this->tg->makeTone(this->gen, 440, 0, 5);
    input = new QBuffer(this->gen);

    // get our Audio Format from tone generator also

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if (!info.isFormatSupported(this->tg->toneformat)) {
       qWarning() << "Raw audio format not supported by backend, cannot play audio.";
       return;
    }

    audio = new QAudioOutput(this->tg->toneformat, this);
    connect(audio, SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleStateChanged(QAudio::State)));

    input->open(QIODevice::ReadOnly);
    audio->start(input);
}

void MainWindow::stopPlaying()
{
    if (audio) {
        audio->stop();
    }
}

void MainWindow::handleStateChanged(QAudio::State newState)
{
    switch (newState) {
        case QAudio::IdleState:
            // Finished playing (no more data)
            qDebug() << "Done playing";
            audio->stop();
            break;

        case QAudio::StoppedState:
            // Stopped
            qWarning() << "Stopped";
            if (audio->error() != QAudio::NoError) {
                // Error handling
                qWarning() << "Stopped with error";
            }
            delete input;
            delete audio;
            break;

        default:
            // ... other cases as appropriate
            break;
    }
}
