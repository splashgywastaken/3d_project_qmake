#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <iostream>
#include "ObjFileReader.h"

bool ObjReadingTools::readFile(const QString& filePath, ObjFileData& fileData, QString& errorMessage, AbstractProgressNotifier* progressNotifier) {

    QFileInfo fileInfo(filePath);

    // Check for correct file format
    if (fileInfo.suffix() != "obj"){
        qCritical()<<QString("Incorrect file format in file" + filePath);
        errorMessage = QString("Incorrect file format in file" + filePath);
        return false;
    }

    // Variables inits
    QFile file(filePath);
    if (!file.exists(filePath))
    {
        errorMessage = QString("File \"%1\" doesn\'t exist").arg(filePath);
        return false;
    }
    if (!file.open(QFile::ReadOnly))
    {
        errorMessage = QString("Failed to read file \"%1\"").arg(filePath);
        return false;
    }

    QByteArray bytes = file.readAll();
    QTextStream textStream(&bytes);

    return ObjReadingTools::readFile(textStream, fileData, errorMessage, progressNotifier);
}

bool ObjReadingTools::readFile(QTextStream &textStream, ObjFileData &fileData, QString& errorMessage, AbstractProgressNotifier *progressNotifier)
{
    ObjFileData data;
    int lineIndex = -1;
    int nLines = ObjReadingTools::nLines(textStream);
    if (progressNotifier != nullptr)
    {
        progressNotifier->start(0, nLines);
    }

    while (!textStream.atEnd())
    {
        lineIndex++;
        if ((progressNotifier != nullptr) && (lineIndex % 100 == 0))
        {
            progressNotifier->setProgress(lineIndex);
        }

        QString line = textStream.readLine();
        ObjToken token;
        QString body;
        if (!readTokenAndBody(line, token, body))
        {
            continue;
        }
        if (token == ObjToken::ObjectName)
        {
            QString objectName;
            if (!parseString(body, objectName, errorMessage))
            {
                if (progressNotifier != nullptr)
                {
                    progressNotifier->finish();
                }
                return false;
            }
            data.setObjectName(objectName);
            continue;
        }
        if (token == ObjToken::Vertex)
        {
            QVector3D vertex;
            if (!parseVector3D(body, vertex, errorMessage))
            {
                errorMessage = QString("Error in line %1. %2").arg(lineIndex).arg(errorMessage);
                if (progressNotifier != nullptr)
                {
                    progressNotifier->finish();
                }
                return false;
            }
            data.addVertex(vertex);
            continue;
        }
        if (token == ObjToken::TexCoord)
        {
            QVector2D texCoord;
            if (!parseVector2D(body, texCoord, errorMessage))
            {
                errorMessage = QString("Error in line %1. %2").arg(lineIndex).arg(errorMessage);
                if (progressNotifier != nullptr)
                {
                    progressNotifier->finish();
                }
                return false;
            }
            data.addVertexTextureCoordinate(texCoord);
            continue;
        }
        if (token == ObjToken::Normal)
        {
            QVector3D normal;
            if (!parseVector3D(body, normal, errorMessage))
            {
                errorMessage = QString("Error in line %1. %2").arg(lineIndex).arg(errorMessage);
                if (progressNotifier != nullptr)
                {
                    progressNotifier->finish();
                }
                return false;
            }
            data.addNormal(normal);
            continue;
        }
        if (token == ObjToken::Face)
        {
            QVector<int> vertexIndices;
            QVector<int> texCoordIndices;
            QVector<int> normalIndices;
            if (!parsePolygon(body, vertexIndices, texCoordIndices, normalIndices, errorMessage))
            {
                errorMessage = QString("Error in line %1. %2").arg(lineIndex).arg(errorMessage);
                if (progressNotifier != nullptr)
                {
                    progressNotifier->finish();
                }
                return false;
            }
            data.addPolygonVertexIndex(vertexIndices);
            if (!texCoordIndices.isEmpty())
            {
                data.addPolygonVertexTextureCoordinateIndex(texCoordIndices);
            }
            if (!normalIndices.isEmpty())
            {
                data.addPolygonNormalIndex(normalIndices);
            }
            continue;
        }
    }

    if (!checkPolygonIndices(
                data.getPolygonVertexIndices(),
                data.getPolygonVertexTextureCoordinateIndices(),
                data.getPolygonNormalIndices(),
                errorMessage
                )
            )
    {
        if (progressNotifier != nullptr)
        {
            progressNotifier->finish();
        }
        return false;
    }
    if (!checkPolygonVertexIndexRange(data.getPolygonVertexIndices(), data.getVertices().size() - 1, errorMessage)
            || !checkPolygonVertexIndexRange(data.getPolygonVertexTextureCoordinateIndices(), data.getVertexTextureCoordinates().size() - 1, errorMessage)
            || !checkPolygonVertexIndexRange(data.getPolygonNormalIndices(), data.getNormals().size() - 1, errorMessage)
            )
    {
        if (progressNotifier != nullptr)
        {
            progressNotifier->finish();
        }
        return false;
    }

    fileData = data;
    if (progressNotifier != nullptr)
    {
        progressNotifier->finish();
    }
    return true;
}

int ObjReadingTools::nLines(QTextStream &stream)
{
    int fileNumberOfLines = 0;

    while (!stream.atEnd()){
        fileNumberOfLines += 1;
        stream.readLine();
    }
    stream.seek(0);

    return fileNumberOfLines;
}

bool ObjReadingTools::parseVector3D(QString body, QVector3D& vector, QString &errorMessage)
{
    if (body.isEmpty())
    {
        errorMessage = "Empty body";
        return false;
    }

    QStringList bodyData = body.split(" ", Qt::SkipEmptyParts);
    if (bodyData.size() != 3)
    {
        errorMessage = "Number of items in bodyData is not equal to 3";
        return false;
    }

    bool isFloat = false;
    float components[3];
    for (int componentIndex = 0; componentIndex < 3; ++componentIndex)
    {
        components[componentIndex] = bodyData[componentIndex].toFloat(&isFloat);
        if (!isFloat){
            errorMessage = "Vector component is not a float value";
            return false;
        }
    }

    vector = QVector3D(components[0], components[1], components[2]);
    return true;
}

bool ObjReadingTools::parseVector2D(QString body, QVector2D &vector, QString &errorMessage)
{
    if (body.isEmpty())
    {
        errorMessage = "Empty body";
        return false;
    }

    QStringList bodyData(body.split(" ", Qt::SkipEmptyParts));
    if (bodyData.size() != 2)
    {
        errorMessage = "Number of items in bodyData is not equal to 2";
        return false;
    }

    bool isFloat = false;
    float components[2];
    for (int componentIndex = 0; componentIndex < 2; ++componentIndex)
    {
        components[componentIndex] = bodyData[componentIndex].toFloat(&isFloat);
        if (!isFloat){
            errorMessage = "Vector component is not a float value";
            return false;
        }
    }

    vector = QVector2D(components[0], components[1]);
    return true;
}

bool ObjReadingTools::parseInt(QString body, int &value)
{
    if (body.isEmpty())
    {
        return false;
    }

    bool isInt = false;
    int m_value = body.toInt(&isInt);
    if (!isInt)
    {
        return false;
    }

    value = m_value;
    return true;
}

bool ObjReadingTools::readTokenAndBody(QString string, ObjToken &token, QString &body)
{
    QString tokenString;
    if (!readTokenString(string, tokenString))
    {
        return false;
    }
    int tokenWithSpaceLength = tokenString.size() + 1;
    const bool isEmptyBody = (tokenWithSpaceLength >= string.size());
    if (isEmptyBody)
    {
        body = QString();
    }
    else
    {
        body = string.mid(tokenWithSpaceLength);
    }

    const int nTokens = 6;
    ObjToken tokens[nTokens] = { ObjToken::Vertex, ObjToken::Normal, ObjToken::TexCoord, ObjToken::Face, ObjToken::ObjectName, ObjToken::Unknown };
    QString tokenStrings[nTokens] = { "v", "vn", "vt", "f", "o", "#" };
    for (int tokenIndex = 0; tokenIndex < nTokens; ++tokenIndex)
    {
        if (tokenString == tokenStrings[tokenIndex])
        {
            token = tokens[tokenIndex];
            return true;
        }
    }
    token = ObjToken::Unknown;
    return true;
}

bool ObjReadingTools::readTokenString(QString string, QString &token)
{
    int firstSpaceIndex = string.indexOf(' ');
    bool entireStringIsToken = (firstSpaceIndex == -1);
    if (entireStringIsToken) {
        token = string;
        return true;
    }
    bool startsWithSpace = (firstSpaceIndex == 0);
    if (startsWithSpace)
    {
        return false;
    }
    token = string.left(firstSpaceIndex);
    return true;
}

bool ObjReadingTools::parsePolygonBlock(
        QString string,
        bool &hasVertex,
        int &vertexIndex,
        bool &hasTexCoord,
        int &texCoordIndex,
        bool &hasNormal,
        int &normalIndex,
        QString &errorMessage
        )
{
    if (string.isEmpty())
    {
        errorMessage = "Polygon block is empty";
        return false;
    }
    QStringList blocks = string.split("/");
    if (blocks.size() > 3)
    {
        errorMessage = "More than 3 polygon blocks";
        return false;
    }

    hasVertex = !blocks[0].isEmpty();
    if (hasVertex && !parseInt(blocks[0], vertexIndex))
    {
        errorMessage = "Vertex index is not an int";
        return false;
    }
    vertexIndex--;

    hasTexCoord = (blocks.size() > 1) && (!blocks[1].isEmpty());
    if (hasTexCoord && !parseInt(blocks[1], texCoordIndex))
    {
        errorMessage = "Texture coordinates index is not an int";
        return false;
    }
    texCoordIndex--;

    hasNormal = (blocks.size() > 2) && (!blocks[2].isEmpty());
    if (hasNormal && !parseInt(blocks[2], normalIndex))
    {
        errorMessage = "Normal index is not an int";
        return false;
    }
    normalIndex--;

    return true;
}

bool ObjReadingTools::parsePolygon(
        QString string,
        QVector<int> &vertexIndices,
        QVector<int> &texCoordIndices,
        QVector<int> &normalIndices,
        QString &errorMessage
        )
{
    if (string.isEmpty())
    {
        errorMessage = "Polygon body is empty";
        return false;
    }
    QStringList blocks = string.split(" ", Qt::SkipEmptyParts);
    int nVertices = blocks.size();
    if (nVertices < 3)
    {
        errorMessage = "Polygon contains less than 3 vertices";
        return false;
    }

    for (int blockIndex = 0; blockIndex < nVertices; blockIndex++)
    {
        bool hasVertex = false;
        int vertexIndex = -1;
        bool hasTexCoord = false;
        int texCoordIndex = -1;
        bool hasNormal = false;
        int normalIndex = -1;
        if (!parsePolygonBlock(
                    blocks[blockIndex],
                    hasVertex, vertexIndex,
                    hasTexCoord, texCoordIndex,
                    hasNormal, normalIndex,
                    errorMessage
                    ))
        {
            errorMessage = "Failed to parse polygon. " + errorMessage;
            return false;
        }

        if (!hasVertex)
        {
            errorMessage = "Polygon block doesn\'t contain a vertex index";
            return false;
        }
        vertexIndices.append(vertexIndex);
        if (hasTexCoord)
        {
            texCoordIndices.append(texCoordIndex);
        }
        if (hasNormal)
        {
            normalIndices.append(normalIndex);
        }
    }

    if ((texCoordIndices.size() > 0) && (texCoordIndices.size() < nVertices))
    {
        errorMessage = "Texture coordinate indices were not specifeid for all polygon vertices";
        return false;
    }

    if ((normalIndices.size() > 0) && (normalIndices.size() < nVertices))
    {
        errorMessage = "Normal indices were not specifeid for all polygon vertices";
        return false;
    }

    return true;
}

bool ObjReadingTools::checkPolygonIndices(QVector<QVector<int> > polygonVertexIndices, QVector<QVector<int> > polygonTexCoordIndices, QVector<QVector<int> > polygonNormalIndices, QString &errorMessage)
{
    int nPolygons = polygonVertexIndices.size();

    bool noneOrAllTexCoordIndicesSpecified = (polygonTexCoordIndices.isEmpty()) || (polygonTexCoordIndices.size() == nPolygons);
    if (!noneOrAllTexCoordIndicesSpecified)
    {
        errorMessage = "Texture coordinates indices were not specified  for all the polygons";
        return false;
    }

    bool noneOrAllNormalIndicesSpecified = (polygonNormalIndices.isEmpty()) || (polygonNormalIndices.size() == nPolygons);
    if (!noneOrAllNormalIndicesSpecified)
    {
        errorMessage = "Normal indices were not specified  for all the polygons";
        return false;
    }
    return true;
}

bool ObjReadingTools::checkPolygonVertexIndexRange(QVector<QVector<int> > polygonVertexIndices, int maxIndex, QString &errorMessage)
{
    for (int polygonIndex = 0; polygonIndex < polygonVertexIndices.size(); polygonIndex++)
    {
        int nVerticesInPolygon = polygonVertexIndices[polygonIndex].size();
        const QVector<int>& vertexIndices = polygonVertexIndices[polygonIndex];
        for (int polygonVertexIndex = 0; polygonVertexIndex < nVerticesInPolygon; polygonVertexIndex++)
        {
            int vertexIndex = vertexIndices[polygonVertexIndex];
            if ((vertexIndex < 0) || (vertexIndex > maxIndex))
            {
                errorMessage = QString("Polygon %1 refers to non-existing vertex %2").arg(polygonIndex).arg(vertexIndex);
                return false;
            }
        }
    }

    return true;
}

bool ObjReadingTools::parseString(QString body, QString &value, QString &errorMessage)
{
    if (body.isEmpty())
    {
        errorMessage = "Empty body";
        return false;
    }

    value = body;
    return true;
}
