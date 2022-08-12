#ifndef QT_3D_PROJECT_OBJFILEDATA_H
#define QT_3D_PROJECT_OBJFILEDATA_H

#include <QVector>
#include <QVector2D>

namespace ObjReadingTools {

class ObjFileData {

private:
    QString m_objectName;
    QVector<QVector3D> m_vertices;
    QVector<QVector3D> m_normals;
    QVector<QVector2D> m_vertexTextureCoordinates;
    QVector<QVector<int>> m_polygonVertexIndices;
    QVector<QVector<int>> m_polygonVertexTextureCoordinateIndices;
    QVector<QVector<int>> m_polygonNormalIndices;
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
    bool operator==(const ObjFileData& other) const;
    bool operator!=(const ObjFileData& other) const;

    // Setters
    void setObjectName(QString objectName);
    void addVertex(QVector3D vertex);
    void addNormal(QVector3D normal);
    void addVertexTextureCoordinate(QVector2D textureCoordinate);
    void addPolygonVertexIndex(QVector<int> polygonVertexIndex);
    void addPolygonVertexTextureCoordinateIndex(QVector<int> polygonVertexTextureCoordinatesIndex);
    void addPolygonNormalIndex(QVector<int> polygonNormalIndex);
    void addFace(QVector<QString> face);

    void setVertices(const QVector<QVector3D>& vertices);
    void setNormals(const QVector<QVector3D>& normals);

    void setPolygonNormalIndices(const QVector<QVector<int>>& polygonNormalIndices);

    // Getters
    ObjFileData &getObjFIleData();
    const QString& getObjectName();
    const QVector<QVector3D>& getVertices();
    const QVector<QVector3D>& getNormals();
    const QVector<QVector<QString>>& getFaces();
    const QVector<QVector2D>& getVertexTextureCoordinates();
    const QVector<QVector<int>>& getPolygonVertexIndices();
    const QVector<QVector<int>>& getPolygonVertexTextureCoordinateIndices();
    const QVector<QVector<int>>& getPolygonNormalIndices();

    // Getting file description
    QString getDescription();

    bool isEmpty();

    // Friend operators
    friend void swap(ObjReadingTools::ObjFileData& first, ObjReadingTools::ObjFileData& second)
    {
        std::swap(first.m_objectName, second.m_objectName);
        std::swap(first.m_vertices, second.m_vertices);
        std::swap(first.m_normals, second.m_normals);
        std::swap(first.m_faces, second.m_faces);
        std::swap(first.m_vertexTextureCoordinates, second.m_vertexTextureCoordinates);
        std::swap(first.m_polygonVertexIndices, second.m_polygonVertexIndices);
        std::swap(first.m_polygonNormalIndices, second.m_polygonNormalIndices);
        std::swap(first.m_polygonVertexTextureCoordinateIndices, second.m_polygonVertexTextureCoordinateIndices);
    }
};

}

#endif
