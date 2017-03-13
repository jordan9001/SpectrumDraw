#ifndef WAVOUTPUTFILE_H
#define WAVOUTPUTFILE_H

#include <QObject>
#include <QFile>
#include <QDataStream>
#include "audioconstants.h"

class WavOutputFile : public QFile
{
public:
    WavOutputFile(QString filename);
    ~WavOutputFile();

    bool writeBuf(QByteArray* buf);

private:
    void writeHeader(int size);
};

#endif // WAVOUTPUTFILE_H
