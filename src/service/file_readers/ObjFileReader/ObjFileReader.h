#ifndef QT_3D_PROJECT_OBJFILEREADER_H
#define QT_3D_PROJECT_OBJFILEREADER_H


#include <QProgressBar>
#include "../../../models/dto/impl/ObjFileData.h"

class ObjFileReader {
public:
    static ObjFileReader& Instantiate();

    static IObjFileData* readFile(const QString& filePath, QProgressBar* progressBar);


private:
    ObjFileReader() = default;
};


#endif
