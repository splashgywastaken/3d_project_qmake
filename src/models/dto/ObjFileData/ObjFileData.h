#ifndef QT_3D_PROJECT_OBJFILEDATA_H
#define QT_3D_PROJECT_OBJFILEDATA_H

#include <QVector>
#include <QVector2D>

class ObjFileData {

private:
    // QString
    QString m_objectName;
    // QVector<QVector3D>
    QVector<QVector3D> m_vertices;
    QVector<QVector3D> m_normals;
    // QVector<QVector2D>
    QVector<QVector2D> m_vertexTextureCoordinates;
    // QVector<QVector<int>>
    QVector<QVector<int>> m_polygonVertexIndices;
    QVector<QVector<int>> m_polygonVertexTextureCoordinateIndices;
    QVector<QVector<int>> m_polygonNormalIndices;
    // QVector<QVector<QString>>
    QVector<QVector<QString>> m_faces;

public:
    // Base constructor and destructor
    ObjFileData();
    ~ObjFileData();
    // Copy and move constructors
    ObjFileData(ObjFileData& other);
    ObjFileData(ObjFileData&& other) noexcept;

    // Copy-assign operator
    ObjFileData& operator=(ObjFileData other);
    // Move-assign operator
    ObjFileData& operator=(ObjFileData&& other) noexcept ;

    // Boolean operators
    bool operator==(ObjFileData& other);
    bool operator!=(ObjFileData& other);

    // Setters
    // QString*
    void setObjectName(QString objectName);
    // QVector<QVector3D>
    void addVertex(QVector3D vertex);
    void addNormal(QVector3D normal);
    // QVector<QVector2D>
    void addVertexTextureCoordinate(QVector2D textureCoordinate);
    // QVector<int>
    void addPolygonVertexIndex(QVector<int> polygonVertexIndex);
    void addPolygonVertexTextureCoordinateIndex(QVector<int> polygonVertexTextureCoordinatesIndex);
    void addPolygonNormalIndex(QVector<int> polygonNormalIndex);
    // QVector<QVector<QString>>
    void addFace(QVector<QString> face);

    // Getters
    ObjFileData &getObjFIleData();
    // QString
    QString getObjectName();
    // QVector<QVector3D>
    QVector<QVector3D>& getVertices();
    QVector<QVector3D>& getNormals();
    QVector<QVector<QString>>& getFaces();
    // QVector<QVector2D>
    QVector<QVector2D>& getVertexTextureCoordinates();
    // QVector<int>
    QVector<QVector<int>>& getPolygonVertexIndices();
    QVector<QVector<int>>& getPolygonVertexTextureCoordinateIndices();
    QVector<QVector<int>>& getPolygonNormalIndices();

    // Getting file description
    QString getDescription();

    bool isEmpty();

    // Friend operators
    friend void swap(ObjFileData& first, ObjFileData& second); //nothrow
};

#endif
