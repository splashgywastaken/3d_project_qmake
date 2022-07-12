#ifndef OBJREADERTESTS_H
#define OBJREADERTESTS_H

#include <QObject>
#include "src/service/FileReaders/ObjFileReader/ObjFileReader.h"
#include <QTest>

class ObjReaderTests : public QObject
{
    Q_OBJECT
public:
    explicit ObjReaderTests(QObject *parent = nullptr);
private slots:
    void testParseVector3D();
    void testParseVector2D();
    void testParseInt();
    void testParsePolygonBlock();
    void testParsePolygon01();
    void testParsePolygon02();
    void testParsePolygon03();
    void testParsePolygon04();
    void testParsePolygon05();
    void testParsePolygon06();
    void testParsePolygon07();
    void testParsePolygon08();
    void testReadTokenString();
    void testReadTokenAndBody();
    void testReadObj01();
    void testReadObj02();
    void testReadObj03();

};

#endif // OBJREADERTESTS_H
