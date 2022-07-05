#include "ObjFileData.h"


ObjFileData::ObjFileData() {
    // Inits for fields:
    m_objectName = QString();
    // QVector<QVector3D>
    m_vertices = QVector<QVector3D>();
    m_normals = QVector<QVector3D>();
    // QVector<QVector2D>
    m_vertexTextureCoordinates = QVector<QVector2D>();
    // QVector<QVector<int>>
    m_polygonVertexIndices = QVector<QVector<int>>();
    m_polygonVertexTextureCoordinateIndices = QVector<QVector<int>>();
    m_polygonNormalIndices = QVector<QVector<int>>();
    // QVector<QVector<QString>>
    m_faces = QVector<QVector<QString>>();
}

void ObjFileData::addVertex(QVector3D vertex) {
    m_vertices.append(vertex);
}

void ObjFileData::addNormal(QVector3D normal) {
    m_normals.append(normal);
}

void ObjFileData::addVertexTextureCoordinate(QVector2D textureCoordinate) {
    m_vertexTextureCoordinates.append(textureCoordinate);
}

void ObjFileData::addPolygonVertexIndex(QVector<int> polygonVertexIndex) {
    m_polygonVertexIndices.append(polygonVertexIndex);
}

void ObjFileData::addPolygonVertexTextureCoordinateIndex(QVector<int> polygonVertexTextureCoordinatesIndex) {
    m_polygonVertexTextureCoordinateIndices.append(polygonVertexTextureCoordinatesIndex);
}

void ObjFileData::addPolygonNormalIndex(QVector<int> polygonNormalIndex) {
    m_polygonVertexIndices.append(polygonNormalIndex);
}

void ObjFileData::addFace(QVector<QString> face) {
    m_faces.append(face);
}

QVector<QVector3D>& ObjFileData::getVertices() {
    return m_vertices;
}

QVector<QVector3D>& ObjFileData::getNormals() {
    return m_normals;
}

QVector<QVector<QString>>& ObjFileData::getFaces() {
    return m_faces;
}

QVector<QVector2D>& ObjFileData::getVertexTextureCoordinates() {
    return m_vertexTextureCoordinates;
}

QVector<QVector<int>>& ObjFileData::getPolygonVertexIndices() {
    return m_polygonVertexIndices;
}

QVector<QVector<int>>& ObjFileData::getPolygonVertexTextureCoordinateIndices() {
    return m_polygonVertexTextureCoordinateIndices;
}

QVector<QVector<int>>& ObjFileData::getPolygonNormalIndices() {
    return m_polygonVertexIndices;
}

QString ObjFileData::getDescription()
{
    QString fileDescription;

    fileDescription.append("Название объекта: " + m_objectName);

    fileDescription.append("Вершины ");
    fileDescription.append("\nКоличество: ");
    fileDescription.append(QString::fromStdString(std::to_string(m_vertices.length())));
    fileDescription.append("\n");

    fileDescription.append("Нормали ");
    fileDescription.append("\nКоличество: ");
    fileDescription.append(QString::fromStdString(std::to_string(m_normals.size())));
    fileDescription.append("\n");

    fileDescription.append("Текстурные координаты ");
    fileDescription.append("\nКоличество: ");
    fileDescription.append(QString::fromStdString(std::to_string(m_vertexTextureCoordinates.size())));
    fileDescription.append("\n");

    fileDescription.append("Грани ");
    fileDescription.append("\nКоличество: ");
    fileDescription.append(QString::fromStdString(std::to_string(m_faces.size())));
    fileDescription.append("\n");

    return fileDescription;
}

bool ObjFileData::isEmpty()
{

    if (m_vertices.count() != 0)
    {
        return false;
    }
    if (m_normals.count() != 0)
    {
        return false;
    }
    if (m_vertexTextureCoordinates.count() != 0)
    {
        return false;
    }
    if (m_polygonVertexIndices.count() != 0)
    {
        return false;
    }
    if (m_polygonNormalIndices.count() != 0)
    {
        return false;
    }
    if (m_polygonVertexTextureCoordinateIndices.count() != 0)
    {
        return false;
    }

    return true;
}

ObjFileData::~ObjFileData() {}

ObjFileData &ObjFileData::getObjFIleData() {
    return *this;
}

QString ObjFileData::getObjectName()
{
    return m_objectName;
}

ObjFileData::ObjFileData(ObjFileData &other)
{
    // Putting in new one
    // QString*
    m_objectName = other.getObjectName();
    // QVector<QVector3D*>*
    m_vertices = other.getVertices();
    m_normals = other.getNormals();
    // QVector<QVector2D*>*
    m_vertexTextureCoordinates = other.getVertexTextureCoordinates();
    // QVector<QVector<int*>*>*
    m_polygonVertexIndices = other.getPolygonVertexIndices();
    m_polygonVertexTextureCoordinateIndices = other.getPolygonVertexTextureCoordinateIndices();
    m_polygonNormalIndices = other.getPolygonNormalIndices();
    // QVector<QVector<QString*>*>*
    m_faces = other.getFaces();
}

ObjFileData::ObjFileData(ObjFileData &&other)  noexcept
{
    // Putting in new one
    // QString*
    m_objectName = other.getObjectName();
    // QVector<QVector3D*>*
    m_vertices = std::move(other.getVertices());
    m_normals =std::move(other.getNormals());
    // QVector<QVector2D*>*
    m_vertexTextureCoordinates = std::move(other.getVertexTextureCoordinates());
    // QVector<QVector<int*>*>*
    m_polygonVertexIndices = std::move(other.getPolygonVertexIndices());
    m_polygonVertexTextureCoordinateIndices = std::move(other.getPolygonVertexTextureCoordinateIndices());
    m_polygonNormalIndices = other.getPolygonNormalIndices();
    // QVector<QVector<QString*>*>*
    m_faces = std::move(other.getFaces());

}

ObjFileData &ObjFileData::operator=(ObjFileData other)
{
    swap(*this, other);

    return *this;
}

bool ObjFileData::operator!=(ObjFileData& other)
{
    return !(*this == other);
}

void ObjFileData::setObjectName(QString objectName)
{
    m_objectName = objectName;
}

bool ObjFileData::operator==(ObjFileData& other)
{
    // QVector<QVector3D*>*
    if (getVertices() != other.getVertices()){
        return false;
    }
    if (getNormals() != other.getNormals()){
        return false;
    }
    // QVector<QVector2D*>*
    if (getVertexTextureCoordinates() != other.getVertexTextureCoordinates()){
        return false;
    }
    // QVector<QVector<int*>*>*
    if (getPolygonVertexIndices() != other.getPolygonVertexIndices()){
        return false;
    }
    if (getPolygonVertexTextureCoordinateIndices() != other.getPolygonVertexTextureCoordinateIndices()){
        return false;
    }
    if (getPolygonNormalIndices() != other.getPolygonNormalIndices()){
        return false;
    }

    return true;
}

void swap(ObjFileData& first, ObjFileData& second) {
    // QString
    std::swap(first.getVertices(), second.getVertices());
    // QVector<QVector3D>
    std::swap(first.getVertices(), second.getVertices());
    std::swap(first.getNormals(), second.getNormals());
    std::swap(first.getFaces(), second.getFaces());
    // QVector<QVector2D>
    std::swap(first.getVertexTextureCoordinates(), second.getVertexTextureCoordinates());
    // QVector<QVector<int>>
    std::swap(first.getPolygonVertexIndices(), second.getPolygonVertexIndices());
    std::swap(first.getPolygonNormalIndices(), second.getPolygonNormalIndices());
    std::swap(first.getPolygonVertexTextureCoordinateIndices(), second.getPolygonVertexTextureCoordinateIndices());
}
