#include "spectrogram.h"

Spectrogram::Spectrogram(QWidget* parent) : QWidget(parent, Qt::Window)
{
    // set up our brushes
    brush = QBrush(QColor(0,0,0,255));
    setWindowTitle("Spectrogram");
    //this->show();
}

Spectrogram::~Spectrogram()
{
    cleanGram();
}

void Spectrogram::drawGram(const QByteArray* buf, quint16 bpm, qreal highest_note)
{
    cleanGram();
    // split the thing up based on bpm and frequency
    // fill gram with the dfts
    int size = 60 * SPD_SAMPLE_RATE / bpm;
    int length = buf->size() / size;
    for (int i=0; i < length; i++) {
        QVector<qreal>* ft = dft(buf, i*size, size, highest_note);
        gram.push_back(ft);
    }
    // call update, to get a paint event
    this->show();
    this->update();
}

void Spectrogram::paintEvent(QPaintEvent* event)
{
    // Set up the painter
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    // paint the spectrogram
    painter.setBrush(brush);
    painter.fillRect(event->rect(), brush);
    qreal w = ((qreal)event->rect().width()) / (qreal) gram.size();
    qreal h = (qreal)event->rect().height();
    int hcount = gram[0]->size();
    if (gram.size() > 0 && gram[0] != NULL) {
        h = h / (qreal)hcount;
    }
    QColor c;
    c.setAlphaF(1.0);

    for (int i=0; i<gram.size(); i++) {
        qreal x = i * w;
        for (int k=0; k<gram[i]->size(); k++) {
            qreal y = (hcount - k - 1) * h;
            qreal val = gram[i]->at(k);
            getColor(&c, val);
            brush.setColor(c);
            painter.fillRect(QRectF(x,y,w,h), brush);
        }
    }


    painter.end();
}

QVector<qreal>* Spectrogram::dft(const QByteArray *buf, int start, int spectrum_size, qreal max_freq)
{
    qreal t;
    // create a qByteArray for our output
    QVector<qreal>* dataOut = new QVector<qreal>(spectrum_size);
    // take in the area of the buffer
    // filter it with a window function (blackman)

    for (int i = 0; i < spectrum_size; ++i) {
        // normalize
        t = ((qreal)(*buf)[i+start]) / (qreal)SPD_MAX_VAL;
        //blackman window function
        qreal multiplier = WIN_A0 - (WIN_A1 * qCos(WIN_C1 * i / (spectrum_size-1))) + (WIN_A2 * qCos(WIN_C2 * i / (spectrum_size-1)));
        //qreal multiplier = 1.0;
        (*dataOut)[i] = multiplier * t;
    }

    // do the dft
    int hl = (spectrum_size/2)+1;
    max_freq = (max_freq * spectrum_size) / SPD_SAMPLE_RATE;
    hl = (hl > max_freq * 2) ? (int)(max_freq*2)+1 : hl;
    qreal trig_const = -2.0 * M_PI / (qreal) spectrum_size;

    // this is super slow, if needs be we can change it to a real fft
    // but for now we will brute force the dft like so
    for (int k=0; k<hl; ++k) {
        qreal rePart = 0;
        qreal imPart = 0;
        for (int n=0; n<spectrum_size; ++n){
            qreal trig_param = trig_const * k * n;
            rePart += (*dataOut)[n] * qCos(trig_param);
            imPart += (*dataOut)[n] * qSin(trig_param);
        }

        // Power at kth frequency
        t = qSqrt((rePart * rePart) + (imPart * imPart)) / spectrum_size;

        (*dataOut)[k] = t;
    }

    dataOut->resize(hl);

    return dataOut;
}

void Spectrogram::getColor(QColor* c, qreal val) {
    // get a color based on the value
    // I need to start seeing color at 1 * 10^-4
    c->setBlueF(CAPF(val *  4500));
    c->setGreenF(CAPF(val * 3900));
    c->setRedF(CAPF(val * 2100));
}

void Spectrogram::cleanGram() {
    for (int i=0; i < gram.size(); i++) {
        delete gram[i];
    }
    gram.resize(0);
}
