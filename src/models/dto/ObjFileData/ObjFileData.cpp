#include "ObjFileData.h"

ObjReadingTools::ObjFileData::ObjFileData() {
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

void ObjReadingTools::ObjFileData::addVertex(QVector3D vertex) {
    m_vertices.append(vertex);
}

void ObjReadingTools::ObjFileData::addNormal(QVector3D normal) {
    m_normals.append(normal);
}

void ObjReadingTools::ObjFileData::addVertexTextureCoordinate(QVector2D textureCoordinate) {
    m_vertexTextureCoordinates.append(textureCoordinate);
}

void ObjReadingTools::ObjFileData::addPolygonVertexIndex(QVector<int> polygonVertexIndex) {
    m_polygonVertexIndices.append(polygonVertexIndex);
}

void ObjReadingTools::ObjFileData::addPolygonVertexTextureCoordinateIndex(QVector<int> polygonVertexTextureCoordinatesIndex) {
    m_polygonVertexTextureCoordinateIndices.append(polygonVertexTextureCoordinatesIndex);
}

void ObjReadingTools::ObjFileData::addPolygonNormalIndex(QVector<int> polygonNormalIndex) {
    m_polygonNormalIndices.append(polygonNormalIndex);
}

void ObjReadingTools::ObjFileData::addFace(QVector<QString> face) {
    m_faces.append(face);
}

void ObjReadingTools::ObjFileData::setVertices(const QVector<QVector3D> &vertices)
{
    m_vertices = vertices;
}

void ObjReadingTools::ObjFileData::setNormals(const QVector<QVector3D> &normals)
{
    m_normals = normals;
}

void ObjReadingTools::ObjFileData::setPolygonNormalIndices(const QVector<QVector<int> > &polygonNormalIndices)
{
    m_polygonNormalIndices = polygonNormalIndices;
}

const QVector<QVector3D>& ObjReadingTools::ObjFileData::getVertices() {
    return m_vertices;
}

const QVector<QVector3D>& ObjReadingTools::ObjFileData::getNormals() {
    return m_normals;
}

const QVector<QVector<QString>>& ObjReadingTools::ObjFileData::getFaces() {
    return m_faces;
}

const QVector<QVector2D>& ObjReadingTools::ObjFileData::getVertexTextureCoordinates() {
    return m_vertexTextureCoordinates;
}

const QVector<QVector<int>>& ObjReadingTools::ObjFileData::getPolygonVertexIndices() {
    return m_polygonVertexIndices;
}

const QVector<QVector<int>>& ObjReadingTools::ObjFileData::getPolygonVertexTextureCoordinateIndices() {
    return m_polygonVertexTextureCoordinateIndices;
}

const QVector<QVector<int>>& ObjReadingTools::ObjFileData::getPolygonNormalIndices() {
    return m_polygonNormalIndices;
}

QString ObjReadingTools::ObjFileData::getDescription()
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

bool ObjReadingTools::ObjFileData::isEmpty()
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

ObjReadingTools::ObjFileData::~ObjFileData() {}

ObjReadingTools::ObjFileData &ObjReadingTools::ObjFileData::getObjFIleData() {
    return *this;
}

const QString& ObjReadingTools::ObjFileData::getObjectName()
{
    return m_objectName;
}

ObjReadingTools::ObjFileData::ObjFileData(ObjReadingTools::ObjFileData &other)
{
    // Putting in new one
    // QString*
    m_objectName = other.getObjectName();
    // QVector<QVector3D*>*
    m_vertices = other.getVertices().toVector();
    m_normals = other.getNormals().toVector();
    // QVector<QVector2D*>*
    m_vertexTextureCoordinates = other.getVertexTextureCoordinates().toVector();
    // QVector<QVector<int*>*>*
    m_polygonVertexIndices = other.getPolygonVertexIndices().toVector();
    m_polygonVertexTextureCoordinateIndices = other.getPolygonVertexTextureCoordinateIndices().toVector();
    m_polygonNormalIndices = other.getPolygonNormalIndices().toVector();
    // QVector<QVector<QString*>*>*
    m_faces = other.getFaces().toVector();
}

ObjReadingTools::ObjFileData::ObjFileData(ObjReadingTools::ObjFileData &&other)  noexcept
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

ObjReadingTools::ObjFileData &ObjReadingTools::ObjFileData::operator=(ObjReadingTools::ObjFileData other)
{
    swap(*this, other);

    return *this;
}

bool ObjReadingTools::ObjFileData::operator!=(const ObjReadingTools::ObjFileData& other) const
{
    return !(*this == other);
}

void ObjReadingTools::ObjFileData::setObjectName(const QString objectName)
{
    m_objectName = objectName;
}

bool ObjReadingTools::ObjFileData::operator==(const ObjReadingTools::ObjFileData& other) const
{
    if (this->m_vertices != other.m_vertices){
        return false;
    }
    if (this->m_normals != other.m_normals){
        return false;
    }
    if (this->m_vertexTextureCoordinates != other.m_vertexTextureCoordinates){
        return false;
    }
    if (this->m_polygonVertexIndices != other.m_polygonVertexIndices){
        return false;
    }
    if (this->m_polygonVertexTextureCoordinateIndices != other.m_polygonVertexTextureCoordinateIndices){
        return false;
    }
    if (this->m_polygonNormalIndices != other.m_polygonNormalIndices){
        return false;
    }

    return true;
}
