#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->audio = NULL;
    this->bpm = 256;
    this->beats = 64;

    // add out tone grid
    this->tonegrid = new ToneGrid(this, this->beats);
    ui->ToneScrollArea->setWidget(this->tonegrid);
    ui->ToneScrollArea->setStyleSheet(TONE_AREA_STYLE);

    // add our spectrogram
    gram = new Spectrogram(0);

    connect(ui->PlayButton,SIGNAL(clicked()),this,SLOT(playTone()));
    connect(ui->StopButton,SIGNAL(clicked()),this,SLOT(stopPlaying()));
    connect(ui->GraphButton,SIGNAL(clicked()),this,SLOT(drawGraph()));
}

MainWindow::~MainWindow()
{
    delete this->gram;
    delete this->tonegrid;
    delete ui;
}

void MainWindow::playTone()
{
    // get the combined output
    QByteArray* gen = this->tonegrid->generateTrack(this->bpm);
    // get the QByte array from tone generator
    input = new QBuffer();
    input->setData(*gen);

    QAudioFormat format;
    format.setSampleRate(SPD_SAMPLE_RATE);
    format.setChannelCount(1);
    format.setSampleSize(SPD_SAMPLE_SIZE);
    format.setCodec(SPD_CODEC);
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if (!info.isFormatSupported(format)) {
       qWarning() << "Raw audio format not supported by backend, cannot play audio.";
       return;
    }

    audio = new QAudioOutput(format, this);
    connect(audio, SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleStateChanged(QAudio::State)));

    input->open(QIODevice::ReadOnly);
    audio->start(input);
}

void MainWindow::drawGraph()
{
    QByteArray* gen = this->tonegrid->generateTrack(this->bpm);
    this->gram->drawGram(gen, this->bpm, this->tonegrid->getHighestUsedNote());
}

void MainWindow::stopPlaying()
{
    if (audio != NULL) {
        audio->stop();
    }
}

void MainWindow::handleStateChanged(QAudio::State newState)
{
    switch (newState) {
        case QAudio::IdleState:
            // Finished playing (no more data)
            qDebug() << "Done playing";
            if (audio) {
                audio->stop();
            }
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
            audio = NULL;
            break;

        default:
            // ... other cases as appropriate
            break;
    }
}
