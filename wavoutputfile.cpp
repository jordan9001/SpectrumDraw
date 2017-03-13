#include "wavoutputfile.h"

WavOutputFile::WavOutputFile(QString filename) : QFile(filename)
{

}

WavOutputFile::~WavOutputFile()
{

}

bool WavOutputFile::writeBuf(QByteArray* buf)
{
    if (!this->open(QIODevice::WriteOnly)) {
        return false;
    }

    this->writeHeader(buf->size());

    this->write(*buf);

    this->close();
    return true;
}

void WavOutputFile::writeHeader(int size)
{
    QDataStream out(this);
    out.setByteOrder(QDataStream::LittleEndian);

    // chunk ID (RIFF)
    out.writeRawData("RIFF", 4);
    // chunk size (buffer size + 36)
    out << quint32(size + 36);
    // format (WAVE)
    out.writeRawData("WAVE", 4);
    // format chunk id
    out.writeRawData("fmt ", 4);
    // Subchunk1Size (16)
    out << quint32(16);
    // Audio format (1 for PCM)
    out << quint16(1);
    // number of channels
    out << quint16(SPD_NUM_CHAN);
    // sample rate
    out << quint32(SPD_SAMPLE_RATE);
    // byte rate (sample rate * chan * sample size / 8)
    out << quint32(SPD_SAMPLE_RATE * SPD_NUM_CHAN * SPD_SAMPLE_SIZE / 8);
    // block align (chan * sample size /8)
    out << quint16(SPD_NUM_CHAN * SPD_SAMPLE_SIZE / 8);
    // bits per sample (sample size)
    out << quint16(SPD_SAMPLE_SIZE);
    // Subchunk2ID (data)
    out.writeRawData("data", 4);
    // Subchunk2size (size of buf in bytes)
    out << quint32(size);

}
