#include "ObjFileData.h"


ObjFileData::ObjFileData() {
    // Inits for fields:
    // QVector<QVector3D*>
    vertices_ = new QVector<QVector3D*>;
    normals_ = new QVector<QVector3D*>;
    // QVector<QVector2D*>
    vertexTextureCoordinates_ = new QVector<QVector2D*>;
    // QVector<QVector<int>>
    polygonVertexIndices_ = new QVector<QVector<int*>*>;
    polygonVertexTextureCoordinateIndices_ = new QVector<QVector<int*>*>;
    polygonNormalIndices_ = new QVector<QVector<int*>*>;
    // QVector<QVector<QString>>
    faces_ = new QVector<QVector<QString*>*>;
}

void ObjFileData::addVertex(QVector3D vertex) {
    vertices_->append(new QVector3D(vertex));
}

void ObjFileData::addNormal(QVector3D normal) {
    normals_->append(new QVector3D(normal));
}

void ObjFileData::addVertexTextureCoordinate(QVector2D textureCoordinate) {
    vertexTextureCoordinates_->append(new QVector2D(textureCoordinate));
}

void ObjFileData::addPolygonVertexIndex(QVector<int*> polygonVertexIndex) {
    polygonVertexIndices_->append(new QVector<int*>(polygonVertexIndex));
}

void ObjFileData::addPolygonVertexTextureCoordinateIndex(QVector<int*> polygonVertexTextureCoordinatesIndex) {
    polygonVertexTextureCoordinateIndices_->append(new QVector<int*>(polygonVertexTextureCoordinatesIndex));
}

void ObjFileData::addPolygonNormalIndex(QVector<int*> polygonNormalIndex) {
    polygonVertexIndices_->append(new QVector<int*>(polygonNormalIndex));
}

void ObjFileData::addFace(QVector<QString *> face) {
    faces_->append(new QVector<QString*>(face));
}

QVector<QVector3D*>& ObjFileData::getVertices() const {
    return *vertices_;
}

QVector<QVector3D*>& ObjFileData::getNormals() const {
    return *normals_;
}

QVector<QVector<QString *> *> & ObjFileData::getFaces() const {
    return *faces_;
}

QVector<QVector2D*>& ObjFileData::getVertexTextureCoordinates() const {
    return *vertexTextureCoordinates_;
}

QVector<QVector<int*>*>& ObjFileData::getPolygonVertexIndices() const {
    return *polygonVertexIndices_;
}

QVector<QVector<int*>*>& ObjFileData::getPolygonVertexTextureCoordinateIndices() const {
    return *polygonVertexTextureCoordinateIndices_;
}

QVector<QVector<int*>*>& ObjFileData::getPolygonNormalIndices() const {
    return *polygonVertexIndices_;
}

QString* ObjFileData::getDescription()
{

    QString* fileDescription = new QString;

    fileDescription->append("Вершины ");
    fileDescription->append("\nКоличество: ");
    fileDescription->append(QString::fromStdString(std::to_string(vertices_->length())));
    fileDescription->append("\n");

    fileDescription->append("Нормали ");
    fileDescription->append("\nКоличество: ");
    fileDescription->append(QString::fromStdString(std::to_string(normals_->size())));
    fileDescription->append("\n");

    fileDescription->append("Текстурные координаты ");
    fileDescription->append("\nКоличество: ");
    fileDescription->append(QString::fromStdString(std::to_string(vertexTextureCoordinates_->size())));
    fileDescription->append("\n");

    fileDescription->append("Грани ");
    fileDescription->append("\nКоличество: ");
    fileDescription->append(QString::fromStdString(std::to_string(faces_->size())));
    fileDescription->append("\n");

    return fileDescription;
}

ObjFileData::~ObjFileData() {
    // Clear elements in fields
    // Clear elements for QVector<QVector3D*>*
    if (vertices_->length() != 0)
    {
        vertices_->clear();
    }
    if (normals_->length() != 0)
    {
        normals_->clear();
    }
    if (faces_->length() != 0)
    {
        faces_->clear();
    }
    // Clear elements for QVector<QVector2D*>*
    vertexTextureCoordinates_->clear();

    // Clear QVector<int*>* for QVector<QVector<int*>*>*
    for (auto& element : *polygonVertexIndices_){
        element->clear();
    }
    for (auto& element : *polygonNormalIndices_){
        element->clear();
    }
    for (auto& element : *polygonVertexTextureCoordinateIndices_){
        element->clear();
    }

    //Clear elements for QVector<QVector<int*>*>*
    polygonVertexTextureCoordinateIndices_->clear();
    polygonNormalIndices_->clear();
    polygonVertexIndices_->clear();

    // Delete all fields
    // QVector<QVector3D*>*
    delete vertices_;
    delete normals_;
    // QVector<QVector2D*>*
    delete vertexTextureCoordinates_;
    // QVector<QVector<int*>*>*
    delete polygonVertexIndices_;
    delete polygonNormalIndices_;
    delete polygonVertexTextureCoordinateIndices_;
}

ObjFileData &ObjFileData::getObjFIleData() {
    return *this;
}

ObjFileData::ObjFileData(const ObjFileData &other) {
    // Putting in new one
    // QVector<QVector3D*>*
    vertices_ = new QVector<QVector3D *>(other.getVertices());
    normals_ = new QVector<QVector3D*>(other.getNormals());
    // QVector<QVector2D*>*
    vertexTextureCoordinates_ = new QVector<QVector2D*>(other.getVertexTextureCoordinates());
    // QVector<QVector<int*>*>*
    polygonVertexIndices_ = new QVector<QVector<int*>*>(other.getPolygonVertexIndices());
    polygonVertexTextureCoordinateIndices_ = new QVector<QVector<int*>*>(other.getPolygonVertexTextureCoordinateIndices());
    polygonNormalIndices_ = new QVector<QVector<int*>*>(other.getPolygonNormalIndices());
    // QVector<QVector<QString*>*>*
    faces_ = new QVector<QVector<QString*>*>(other.getFaces());
}

ObjFileData::ObjFileData(ObjFileData &&other)  noexcept {
    // Putting in new one
    // QVector<QVector3D*>*
    vertices_ = new QVector<QVector3D*>(std::move(other.getVertices()));
    normals_ = new QVector<QVector3D*>(std::move(other.getNormals()));
    // QVector<QVector2D*>*
    vertexTextureCoordinates_ = new QVector<QVector2D*>(std::move(other.getVertexTextureCoordinates()));
    // QVector<QVector<int*>*>*
    polygonVertexIndices_ = new QVector<QVector<int*>*>(std::move(other.getPolygonVertexIndices()));
    polygonVertexTextureCoordinateIndices_ = new QVector<QVector<int*>*>(std::move(other.getPolygonVertexTextureCoordinateIndices()));
    polygonNormalIndices_ = new QVector<QVector<int*>*>(other.getPolygonNormalIndices());
    // QVector<QVector<QString*>*>*
    faces_ = new QVector<QVector<QString*>*>(std::move(other.getFaces()));

}

ObjFileData &ObjFileData::operator=(ObjFileData other) {
    ObjFileData temp(other);
    swap(*this, temp);

    return *this;
}

ObjFileData &ObjFileData::operator=(ObjFileData &&other)  noexcept {
    ObjFileData temp(other);
    swap(*this, temp);

    return *this;
}

bool ObjFileData::operator!=(const ObjFileData& other) const {
    return !(*this == other);
}

bool ObjFileData::operator==(const ObjFileData& other) const {
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
    // Enabling ADL
    using std::swap;
    using std::move;

    auto tempObjFileData = new ObjFileData();

    // QVector<QVector3D*>*
    *tempObjFileData->vertices_ = first.getVertices();
    *tempObjFileData->normals_ = first.getNormals();
    *tempObjFileData->faces_ = first.getFaces();
    // QVector<QVector2D*>*
    *tempObjFileData->vertexTextureCoordinates_ = first.getVertexTextureCoordinates();
    // QVector<QVector<int*>*>*
    *tempObjFileData->polygonVertexIndices_ = first.getPolygonVertexIndices();
    *tempObjFileData->polygonNormalIndices_ = first.getPolygonNormalIndices();
    *tempObjFileData->polygonVertexTextureCoordinateIndices_ = first.getPolygonVertexTextureCoordinateIndices();

    first = second;
    second = *tempObjFileData;

    delete tempObjFileData;
}
