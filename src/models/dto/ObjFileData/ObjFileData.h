#ifndef QT_3D_PROJECT_OBJFILEDATA_H
#define QT_3D_PROJECT_OBJFILEDATA_H

#include <QVector>
#include <QVector2D>

namespace ObjReadingTools {

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
    bool operator==(const ObjFileData& other) const;
    bool operator!=(const ObjFileData& other) const;

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

    void setVertices(const QVector<QVector3D>& vertices);

    // Getters
    ObjFileData &getObjFIleData();
    // QString
    const QString& getObjectName();
    // QVector<QVector3D>
    const QVector<QVector3D>& getVertices();
    const QVector<QVector3D>& getNormals();
    const QVector<QVector<QString>>& getFaces();
    // QVector<QVector2D>
    const QVector<QVector2D>& getVertexTextureCoordinates();
    // QVector<int>
    const QVector<QVector<int>>& getPolygonVertexIndices();
    const QVector<QVector<int>>& getPolygonVertexTextureCoordinateIndices();
    const QVector<QVector<int>>& getPolygonNormalIndices();

    // Getting file description
    QString getDescription();

    bool isEmpty();

    // Friend operators
    friend void swap(ObjReadingTools::ObjFileData& first, ObjReadingTools::ObjFileData& second)
    {
        // QString
        std::swap(first.m_objectName, second.m_objectName);
        // QVector<QVector3D>
        std::swap(first.m_vertices, second.m_vertices);
        std::swap(first.m_normals, second.m_normals);
        std::swap(first.m_faces, second.m_faces);
        // QVector<QVector2D>
        std::swap(first.m_vertexTextureCoordinates, second.m_vertexTextureCoordinates);
        // QVector<QVector<int>>
        std::swap(first.m_polygonVertexIndices, second.m_polygonVertexIndices);
        std::swap(first.m_polygonNormalIndices, second.m_polygonNormalIndices);
        std::swap(first.m_polygonVertexTextureCoordinateIndices, second.m_polygonVertexTextureCoordinateIndices);
    }
};

}

#endif
