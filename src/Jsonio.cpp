#include "Jsonio.h"

#include <QFile>

// ファイル全読み込み
QByteArray load_QJson(const QString& fileName){
    QFile file(fileName);

    if(!file.open(QIODevice::ReadOnly)){
        throw std::runtime_error("Failed to open json file");
    }

    QByteArray data = file.readAll();
    file.close();

    return data;
}
