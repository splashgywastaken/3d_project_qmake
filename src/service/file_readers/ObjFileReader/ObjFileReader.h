#ifndef QT_3D_PROJECT_OBJFILEREADER_H
#define QT_3D_PROJECT_OBJFILEREADER_H


#include <QProgressBar>
#include "src/models/dto/ObjFileData/ObjFileData.h"

class ObjFileReader {
public:
    static ObjFileReader& Instantiate();

    static ObjFileData* readFile(const QString& filePath, QProgressBar* progressBar);


private:
    ObjFileReader() = default;
};


#endif
