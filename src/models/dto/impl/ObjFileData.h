#ifndef QT_3D_PROJECT_OBJFILEDATA_H
#define QT_3D_PROJECT_OBJFILEDATA_H

#include "src/models/dto/IObjFileData.h"

class ObjFileData : public IObjFileData {

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
    ~ObjFileData() override;
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
    void addVertex(QVector3D vertex) override;
    void addNormal(QVector3D normal) override;
    // QVector<QVector2D>
    void addVertexTextureCoordinate(QVector2D textureCoordinate) override;
    // QVector<int>
    void addPolygonVertexIndex(QVector<int*> polygonVertexIndex) override;
    void addPolygonVertexTextureCoordinateIndex(QVector<int*> polygonVertexTextureCoordinatesIndex) override;
    void addPolygonNormalIndex(QVector<int*> polygonNormalIndex) override;
    // QVector<QVector<QString>>
    void addFace(QVector<QString*> face) override;

    // Getters
    IObjFileData &getObjFIleData() override;
    // QVector<QVector3D>
    QVector<QVector3D*>& getVertices() const override;
    QVector<QVector3D*>& getNormals() const override;
    QVector<QVector<QString *> *> & getFaces() const override;
    // QVector<QVector2D>
    QVector<QVector2D*>& getVertexTextureCoordinates() const override;
    // QVector<int>
    QVector<QVector<int*>*>& getPolygonVertexIndices() const override;
    QVector<QVector<int*>*>& getPolygonVertexTextureCoordinateIndices() const override;
    QVector<QVector<int*>*>& getPolygonNormalIndices() const override;

    // Getting file description
    QString* getDescription() override;

    // Friend operators
    friend void swap(ObjFileData& first, ObjFileData& second); //nothrow   
};

#endif
