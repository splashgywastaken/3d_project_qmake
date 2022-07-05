#ifndef QT_3D_PROJECT_OBJFILEREADER_H
#define QT_3D_PROJECT_OBJFILEREADER_H


#include <QProgressBar>
#include "src/models/dto/ObjFileData/ObjFileData.h"

#include <src/service/GlobalState.h>

class ObjFileReader {
public:
    static ObjFileReader& Instantiate();

    static bool readFile(const QString& filePath, ObjFileData&, AbstractProgressNotifier* progressNotifier = nullptr);

private:
    ObjFileReader() = default;
};


#endif
