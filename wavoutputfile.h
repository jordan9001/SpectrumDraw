#ifndef WAVOUTPUTFILE_H
#define WAVOUTPUTFILE_H

#include <QObject>
#include <QFile>

class WavOutputFile : public QFile
{
public:
    WavOutputFile(QString filename);
    ~WavOutputFile();

    bool writeBuf(QByteArray* buf);

private:
    void writeHeader();
};

#endif // WAVOUTPUTFILE_H
