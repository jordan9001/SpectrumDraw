#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->audio = NULL;
    this->gen = new QByteArray();
    this->bpm = 256;
    this->beats = 64;
    this->tracks = 8;

    // add tonerows to the ui, making our grid
    this->rowarea = new QVBoxLayout();
    this->rowarea->setSpacing(0);
    this->rowarea->setMargin(0);
    this->addRows(this->tracks);
    ui->ToneRowArea->setLayout(this->rowarea);

    // set the style on the area
    ui->ToneRowArea->setStyleSheet(TONE_AREA_STYLE);

    connect(ui->PlayButton,SIGNAL(clicked()),this,SLOT(playTone()));
    connect(ui->StopButton,SIGNAL(clicked()),this,SLOT(stopPlaying()));
}

MainWindow::~MainWindow()
{
    this->deleteRows();
    delete this->rowarea;
    delete ui;
}

void MainWindow::playTone()
{
    // get the combined output
    this->compileSound();
    // get the QByte array from tone generator
    input = new QBuffer(this->gen);

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
            audio = NULL;
            break;

        default:
            // ... other cases as appropriate
            break;
    }
}

void MainWindow::addRows(quint16 rows)
{
    this->controlrows = QVector<ToneRow*>(rows);
    for (quint16 i=0; i<rows; i++) {
        ToneRow* t = new ToneRow(this, this->beats);
        this->controlrows[i] = t;
        this->rowarea->addWidget(t);
    }
}

void MainWindow::deleteRows()
{
    for (int i=0; i<this->controlrows.size(); i++) {
        delete this->controlrows[i];
    }
}

void MainWindow::compileSound()
{
    // zero gen
    gen->resize(this->beats * 60 * SPD_SAMPLE_RATE / this->bpm);
    gen->fill(0);

    //loop over rows, adding our amount to gen
    for (int i=0; i<this->controlrows.size(); i++) {
        controlrows[i]->generateTrack(gen, this->bpm, this->tracks);
    }
}
