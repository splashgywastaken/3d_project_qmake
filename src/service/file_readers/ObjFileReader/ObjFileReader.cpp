#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <iostream>
#include "ObjFileReader.h"


ObjFileReader &ObjFileReader::Instantiate() {
    static ObjFileReader objFileReader;
    return objFileReader;
}

ObjFileData* ObjFileReader::readFile(const QString& filePath, QProgressBar* progressBar) {

    QFileInfo *fileInfo = new QFileInfo(filePath);

    // Check for correct file format
    if (fileInfo->suffix() != "obj"){
        delete fileInfo;
        return nullptr;
    }

    delete fileInfo;

    // Made an object to fill with data
    ObjFileData *objFileData = new ObjFileData();

    // Variables inits
    QFile *file = new QFile(filePath);
    QTextStream* in = new QTextStream(&(*file));

    // Opening file etc.
    if (!file->open(QIODevice::ReadOnly)){
        qWarning("Cannot open file for reading");

        delete in;
        delete file;
        delete objFileData;

        return nullptr;
    }

    int* fileNumberOfLines = new int(0);

    while (!in->atEnd()){
        *fileNumberOfLines += 1;
        in->readLine();
    }
    in->seek(0);

    int* counter = new int(0);

    progressBar->setRange(*counter, *fileNumberOfLines);
    progressBar->setValue(*counter);

    // Filling up an object to return it in the end
    while(!in->atEnd()){        
        QString* line = new QString(in->readLine());

        QString* dataType = new QString(line->mid(0, line->indexOf(" ")));
        *line = QString(line->mid(line->indexOf(" ") + 1, line->length()));

        if (*dataType != "#")
        {
            if (*dataType == "v") {
                // Resolving vertex
                QVector3D * vertex_vector = new QVector3D;

                // Do things with line if it is a vertex element
                QStringList *line_data = new QList(line->split(" ").toList());

                vertex_vector->setX(line_data->at(0).toDouble());
                vertex_vector->setY(line_data->at(1).toDouble());
                vertex_vector->setZ(line_data->at(2).toDouble());

                objFileData->addVertex(*vertex_vector);

                // Memory management
                delete line_data;
                delete vertex_vector;
            } else if (*dataType == "vn") {
                // Resolving vertex normals
                QVector3D * vertex_normal_vector = new QVector3D;

                // Do things with line if it is a vertex normal element
                QStringList *line_data = new QList(line->split(" ").toList());

                vertex_normal_vector->setX(line_data->at(0).toDouble());
                vertex_normal_vector->setY(line_data->at(1).toDouble());
                vertex_normal_vector->setZ(line_data->at(2).toDouble());

                objFileData->addNormal(*vertex_normal_vector);

                // Memory management
                delete line_data;
                delete vertex_normal_vector;
            } else if (*dataType == "vt") {
                // Resolving vertex normals
                QVector2D * vertex_texture_vector = new QVector2D;

                // Do things with line if it is a vertex texture coordinates
                QStringList *line_data = new QList(line->split(" ").toList());

                vertex_texture_vector->setX(line_data->at(0).toDouble());
                vertex_texture_vector->setY(line_data->at(1).toDouble());

                objFileData->addVertexTextureCoordinate(*vertex_texture_vector);

                // Memory management
                delete line_data;
                delete vertex_texture_vector;
            } else if (*dataType == "f") {
                // Resolving polygonal faces element
                QVector < QString * > *face_vector = new QVector<QString *>;

                // Do things with line if it is a face element
                QStringList *line_data = new QList(line->split(" ").toList());

                foreach(
                const QString &str, *line_data){
                    *face_vector << new QString(str);
                }

                objFileData->addFace(*face_vector);

                // Memory management
                delete line_data;
                delete face_vector;
            }
        }

        // Memory management
        delete line;
        delete dataType;

        progressBar->setValue(++*counter);
    }

    // Memory management
    delete counter;
    delete fileNumberOfLines;
    delete file;
    delete in;

    return objFileData;
}
