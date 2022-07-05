#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <iostream>
#include "ObjFileReader.h"


ObjFileReader &ObjFileReader::Instantiate() {
    static ObjFileReader objFileReader;
    return objFileReader;
}

bool ObjFileReader::readFile(const QString& filePath, ObjFileData& fileData, AbstractProgressNotifier* progressNotifier) {

    QFileInfo fileInfo(filePath);

    // Check for correct file format
    if (fileInfo.suffix() != "obj"){
        qCritical()<<QString("Incorrect file format in file" + filePath);
        return false;
    }

    // Variables inits
    QFile file(filePath);
    QTextStream in(&file);

    // Opening file etc.
    if (!file.open(QIODevice::ReadOnly)){
        qFatal("Cannot open file for reading");
        return false;
    }

    int fileNumberOfLines = 0;

    while (!in.atEnd()){
        fileNumberOfLines += 1;
        in.readLine();
    }
    in.seek(0);

    int progressStep = 0;

    if (progressNotifier != nullptr)
    {
        progressNotifier->start(0, fileNumberOfLines);
        progressNotifier->setProgress(progressStep);
    }

    // Filling up an object to return it in the end
    while(!in.atEnd()){
        QString line(in.readLine());

        QString dataType(line.mid(0, line.indexOf(" ")));
        line = QString(line.mid(line.indexOf(" ") + 1, line.length()));

        if (dataType != "#")
        {
            if (dataType == "o"){
                fileData.setObjectName(line);
            }

            if (dataType == "v") {
                // Resolving vertex
                QVector3D vertex_vector;

                // Do things with line if it is a vertex element
                QStringList line_data(line.split(" ").toList());

                vertex_vector.setX(line_data.at(0).toDouble());
                vertex_vector.setY(line_data.at(1).toDouble());
                vertex_vector.setZ(line_data.at(2).toDouble());

                fileData.addVertex(vertex_vector);

            } else if (dataType == "vn") {
                // Resolving vertex normals
                QVector3D vertex_normal_vector;

                // Do things with line if it is a vertex normal element
                QStringList line_data(line.split(" ").toList());

                vertex_normal_vector.setX(line_data.at(0).toDouble());
                vertex_normal_vector.setY(line_data.at(1).toDouble());
                vertex_normal_vector.setZ(line_data.at(2).toDouble());

                fileData.addNormal(vertex_normal_vector);

            } else if (dataType == "vt") {
                // Resolving vertex normals
                QVector2D vertex_texture_vector;

                // Do things with line if it is a vertex texture coordinates
                QStringList line_data(line.split(" ").toList());

                vertex_texture_vector.setX(line_data.at(0).toDouble());
                vertex_texture_vector.setY(line_data.at(1).toDouble());

                fileData.addVertexTextureCoordinate(vertex_texture_vector);

            } else if (dataType == "f") {
                // Resolving polygonal faces element
                QVector <QString> face_vector;

                // Do things with line if it is a face element
                QStringList line_data(line.split(" ").toList());

                foreach(
                const QString &str, line_data){
                    face_vector << str;
                }

                fileData.addFace(face_vector);
            }
        }

        if (progressNotifier != nullptr)
        {
            progressNotifier->setProgress(++progressStep);
        }
    }

    if (progressNotifier != nullptr)
    {
        progressNotifier->finish();
    }

    return !fileData.isEmpty();
}
