#ifndef QT_3D_PROJECT_OBJFILEREADER_H
#define QT_3D_PROJECT_OBJFILEREADER_H


#include <QProgressBar>
#include "src/models/dto/ObjFileData/ObjFileData.h"

#include <src/service/GlobalState.h>

class ObjFileReader {
public:
    static ObjFileReader& Instantiate();

    static ObjFileData* readFile(const QString& filePath, AbstractProgressNotifier* progressNotifier = nullptr);


private:
    ObjFileReader() = default;
};


#endif
