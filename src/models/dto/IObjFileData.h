#ifndef QT_3D_PROJECT_IOBJFILEDATA_H
#define QT_3D_PROJECT_IOBJFILEDATA_H
#include <QVector>
#include <QVector3D>
#include <QVector2D>

class IObjFileData {

public:
    virtual ~IObjFileData() = default;

    // Setters
    // QVector<QVector3D>
    virtual void addVertex(QVector3D vertex) = 0;
    virtual void addNormal(QVector3D normal) = 0;
    // QVector<QVector2D>
    virtual void addVertexTextureCoordinate(QVector2D vertexTextureCoordinate) = 0;
    // QVector<int>
    virtual void addPolygonVertexIndex(QVector<int*> polygonVertexIndex) = 0;
    virtual void addPolygonVertexTextureCoordinateIndex(QVector<int*> polygonVertexTextureCoordinatesIndex) = 0;
    virtual void addPolygonNormalIndex(QVector<int*> polygonNormalIndex) = 0;
    // QVector<QVector<QString>>
    virtual void addFace(QVector<QString*> face) = 0;

    // Getters
    virtual IObjFileData& getObjFIleData() = 0;
    // QVector<QVector3D>
    virtual QVector<QVector3D*>& getVertices() const = 0;
    virtual QVector<QVector3D*>& getNormals() const = 0;
    // QVector<QVector2D>
    virtual QVector<QVector2D*>& getVertexTextureCoordinates() const = 0;
    // QVector<QVector<int>>
    virtual QVector<QVector<int*>*>& getPolygonVertexIndices() const = 0;
    virtual QVector<QVector<int*>*>& getPolygonVertexTextureCoordinateIndices() const = 0;
    virtual QVector<QVector<int*>*>& getPolygonNormalIndices() const = 0;
    // QVector<QVector<QString>>
    virtual QVector<QVector<QString *> *> & getFaces() const = 0;

    virtual QString* getDescription() = 0;
};
#endif
