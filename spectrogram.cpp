#include "spectrogram.h"

Spectrogram::Spectrogram(QWidget* parent) : QWidget(parent)
{
    // set up our brushes
    brush = QBrush(QColor(0,0,0,255));
}

Spectrogram::~Spectrogram()
{
    cleanGram();
}

void Spectrogram::drawGram(const QByteArray* buf, quint16 bpm)
{
    cleanGram();
    // split the thing up based on bpm and frequency
    // fill gram with the dfts
    int size = 60 * SPD_SAMPLE_RATE / bpm;
    int length = buf->size() / size;
    for (int i=0; i < length; i++) {
        QVector<qreal>* ft = dft(buf, i*size, size);
        gram.push_back(ft);
    }
    // call update, to get a paint event
    this->update();
}

void Spectrogram::paintEvent(QPaintEvent *event)
{
    // Set up the painter
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    // paint the spectrogram
    painter.setBrush(brush);

    painter.fillRect(QRectF(0,0,10,20),brush);

    painter.end();
}

QVector<qreal>* Spectrogram::dft(const QByteArray *buf, int start, int spectrum_size)
{
    // create a qByteArray for our output
    QVector<qreal>* dataOut = new QVector<qreal>(spectrum_size);
    QVector<qreal> rePart;
    rePart.resize(spectrum_size);
    QVector<qreal> imPart;
    imPart.resize(spectrum_size);
    // take in the area of the buffer
    // filter it with a window function (blackman)

    for (int i = 0; i < spectrum_size; ++i) {
        //blackman window function
        qreal multiplier = WIN_A0 - (WIN_A1 * qCos(WIN_C1 * i / (spectrum_size-1))) + (WIN_A2 * qCos(WIN_C2 * i / (spectrum_size-1)));
        (*dataOut)[i] = multiplier * (qreal)(*buf)[i+start];
    }


    // do the dft

    // this is super slow, if needs be we can change it to a real fft
    // but for now we will brute force the dft like so
    for (int k=0; k<spectrum_size; ++k) {
        // Real part
        rePart[k] = 0;
        for (int n=0 ; n<spectrum_size ; ++n){
            rePart[k] += (*dataOut)[n] * qCos(n * k * 2 * M_PI / spectrum_size);
        }

        // Imaginary part
        imPart[k] = 0;
        for (int n=0 ; n<spectrum_size ; ++n) {
            imPart[k] -= (*dataOut)[n] * sin(n * k * 2 * M_PI / spectrum_size);
        }

        // Power at kth frequency
        (*dataOut)[k] = (rePart[k] * rePart[k]) + (imPart[k] * imPart[k]);
    }
    return dataOut;
}

void Spectrogram::cleanGram() {
    for (int i=0; i < gram.size(); i++) {
        delete gram[i];
    }
    gram.resize(0);
}
