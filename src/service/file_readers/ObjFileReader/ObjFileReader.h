#ifndef QT_3D_PROJECT_OBJFILEREADER_H
#define QT_3D_PROJECT_OBJFILEREADER_H

#include <QTextStream>
#include <QProgressBar>

#include "src/models/dto/ObjFileData/ObjFileData.h"
#include <src/service/GlobalState.h>


namespace ObjReadingTools {

enum class ObjToken {
    Vertex,
    TexCoord,
    Normal,
    Face,
    ObjectName,
    Unknown
};

// Functions for reading
bool readFile(const QString& filePath, ObjFileData& fileData, QString& errorMessage, AbstractProgressNotifier* progressNotifier = nullptr);
bool readFile(QTextStream& textStream, ObjFileData& fileData, QString& errorMessage, AbstractProgressNotifier* progressNotifier = nullptr);

// Functions for data parsing
bool parseVector3D(QString body, QVector3D& vector, QString &errorMessage);
bool parseVector2D(QString body, QVector2D& vector, QString &errorMessage);
bool parseInt(QString body, int& value);
bool parsePolygonBlock(
        QString string,
        bool &hasVertex,
        int &vertexIndex,
        bool& hasTexCoord,
        int &texCoordIndex,
        bool &hasNormal,
        int &normalIndex,
        QString &errorMessage
        );
bool parsePolygon(
        QString string,
        QVector<int> &vertexindices,
        QVector<int> &texCoordIndices,
        QVector<int> &normalIndices,
        QString &errorMessage
        );
bool parseString(QString body, QString &value, QString &errorMessage);

// Read values functions
bool readTokenString(QString string, QString &token);
bool readTokenAndBody(QString string, ObjToken &token, QString &body);

// Check functions
bool checkPolygonIndices(
        QVector<QVector<int>> polygonVertexIndices,
        QVector<QVector<int>> polygonTexCoordIndices,
        QVector<QVector<int>> polygonNormalIndices,
        QString &errorMessage
        );
bool checkPolygonVertexIndexRange(QVector<QVector<int>> polygonVertexIndices, int maxIndex, QString &errorMessage);

// Functions for getting some values
int nLines(QTextStream &stream);
}


#endif
