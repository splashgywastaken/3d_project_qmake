#ifndef QT_3D_PROJECT_OBJFILEDATA_H
#define QT_3D_PROJECT_OBJFILEDATA_H

#include <QVector>
#include <QVector2D>

class ObjFileData {

private:
    // QVector<QVector3D*>*
    QVector<QVector3D*>* vertices_;
    QVector<QVector3D*>* normals_;
    // QVector<QVector2D*>*
    QVector<QVector2D*>* vertexTextureCoordinates_;
    // QVector<QVector<int*>*>*
    QVector<QVector<int*>*>* polygonVertexIndices_;
    QVector<QVector<int*>*>* polygonVertexTextureCoordinateIndices_;
    QVector<QVector<int*>*>* polygonNormalIndices_;
    // QVector<QVector<QString*>>
    QVector<QVector<QString*>*>* faces_;

public:
    // Base constructor and destructor
    ObjFileData();
    ~ObjFileData();
    // Copy and move constructors
    ObjFileData(const ObjFileData& other);
    ObjFileData(ObjFileData&& other) noexcept;

    // Copy-assign operator
    ObjFileData& operator=(ObjFileData other);
    // Move-assign operator
    ObjFileData& operator=(ObjFileData&& other) noexcept ;

    // Boolean operators
    bool operator==(const ObjFileData& other) const;
    bool operator!=(const ObjFileData& other) const;

    // Setters
    // QVector<QVector3D>
    void addVertex(QVector3D vertex);
    void addNormal(QVector3D normal);
    // QVector<QVector2D>
    void addVertexTextureCoordinate(QVector2D textureCoordinate);
    // QVector<int>
    void addPolygonVertexIndex(QVector<int*> polygonVertexIndex);
    void addPolygonVertexTextureCoordinateIndex(QVector<int*> polygonVertexTextureCoordinatesIndex);
    void addPolygonNormalIndex(QVector<int*> polygonNormalIndex);
    // QVector<QVector<QString>>
    void addFace(QVector<QString*> face);

    // Getters
    ObjFileData &getObjFIleData();
    // QVector<QVector3D>
    QVector<QVector3D*>& getVertices() const;
    QVector<QVector3D*>& getNormals() const;
    QVector<QVector<QString *> *> & getFaces() const;
    // QVector<QVector2D>
    QVector<QVector2D*>& getVertexTextureCoordinates() const;
    // QVector<int>
    QVector<QVector<int*>*>& getPolygonVertexIndices() const;
    QVector<QVector<int*>*>& getPolygonVertexTextureCoordinateIndices() const;
    QVector<QVector<int*>*>& getPolygonNormalIndices() const;

    // Getting file description
    QString* getDescription();

    // Friend operators
    friend void swap(ObjFileData& first, ObjFileData& second); //nothrow   
};

#endif
