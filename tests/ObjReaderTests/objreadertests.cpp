#include "objreadertests.h"
#include <QDebug>

using namespace ObjReadingTools;

ObjReaderTests::ObjReaderTests(QObject *parent)
    : QObject{parent}
{

}

void ObjReaderTests::testParseVector3D()
{
    QVector3D vertex;
    QString errorMsg;

    QVERIFY(parseVector3D("1 2 3", vertex, errorMsg));
    QCOMPARE(vertex, QVector3D({1, 2, 3}));

    QVERIFY(parseVector3D("-1 -2 -3", vertex, errorMsg));
    QCOMPARE(vertex, QVector3D({-1, -2, -3}));

    QVERIFY(parseVector3D("1e-3 2 3", vertex, errorMsg));
    QCOMPARE(vertex, QVector3D({1e-3, 2, 3}));

    QVERIFY(parseVector3D("   1  2 3", vertex, errorMsg));
    QCOMPARE(vertex, QVector3D({1, 2, 3}));

    QVERIFY(!parseVector3D("", vertex, errorMsg));

    QVERIFY(!parseVector3D("1 2", vertex, errorMsg));

    QVERIFY(!parseVector3D("1 2 3 4", vertex, errorMsg));

    QVERIFY(!parseVector3D("1 2 abc", vertex, errorMsg));
}

void ObjReaderTests::testParseVector2D()
{
    QVector2D vertex;
    QString errorMsg;

    QVERIFY(parseVector2D("1 2", vertex, errorMsg));
    QCOMPARE(vertex, QVector2D(1, 2));

    QVERIFY(parseVector2D("-1 -2", vertex, errorMsg));
    QCOMPARE(vertex, QVector2D(-1, -2));

    QVERIFY(parseVector2D("1e-3 2", vertex, errorMsg));
    QCOMPARE(vertex, QVector2D(1e-3, 2));

    QVERIFY(parseVector2D("   1  2", vertex, errorMsg));
    QCOMPARE(vertex, QVector2D(1, 2));

    QVERIFY(!parseVector2D("1 2 3", vertex, errorMsg));

    QVERIFY(!parseVector2D("0.7500 0.8500 0.0000", vertex, errorMsg));

    QVERIFY(!parseVector2D("", vertex, errorMsg));

    QVERIFY(!parseVector2D("1", vertex, errorMsg));

    QVERIFY(!parseVector2D("1 abc", vertex, errorMsg));
}

void ObjReaderTests::testParseInt()
{
    int value = -1;

    QVERIFY(parseInt("123", value));
    QCOMPARE(value, 123);

    QVERIFY(parseInt("-1", value));
    QCOMPARE(value, -1);

    QVERIFY(parseInt("0001", value));
    QCOMPARE(value, 1);

    QVERIFY(!parseInt("-1e-3", value));

    QVERIFY(!parseInt("", value));

    QVERIFY(!parseInt("abc", value));
}

void ObjReaderTests::testParsePolygonBlock()
{
    bool hasVertex;
    bool hasTexCoord;
    bool hasNormal;
    int vertexInd;
    int texCoordInd;
    int normalInd;
    QString errorMsg;

    QVERIFY(parsePolygonBlock("1/2/3", hasVertex, vertexInd, hasTexCoord, texCoordInd, hasNormal, normalInd, errorMsg));
    QVERIFY(hasVertex);
    QVERIFY(hasTexCoord);
    QVERIFY(hasNormal);
    QCOMPARE(vertexInd, 0);
    QCOMPARE(texCoordInd, 1);
    QCOMPARE(normalInd, 2);

    QVERIFY(parsePolygonBlock("1", hasVertex, vertexInd, hasTexCoord, texCoordInd, hasNormal, normalInd, errorMsg));
    QVERIFY(hasVertex);
    QVERIFY(!hasTexCoord);
    QVERIFY(!hasNormal);
    QCOMPARE(vertexInd, 0);

    QVERIFY(parsePolygonBlock("1/", hasVertex, vertexInd, hasTexCoord, texCoordInd, hasNormal, normalInd, errorMsg));
    QVERIFY(hasVertex);
    QVERIFY(!hasTexCoord);
    QVERIFY(!hasNormal);
    QCOMPARE(vertexInd, 0);

    QVERIFY(parsePolygonBlock("1//", hasVertex, vertexInd, hasTexCoord, texCoordInd, hasNormal, normalInd, errorMsg));
    QVERIFY(hasVertex);
    QVERIFY(!hasTexCoord);
    QVERIFY(!hasNormal);
    QCOMPARE(vertexInd, 0);

    QVERIFY(parsePolygonBlock("1//3", hasVertex, vertexInd, hasTexCoord, texCoordInd, hasNormal, normalInd, errorMsg));
    QVERIFY(hasVertex);
    QVERIFY(!hasTexCoord);
    QVERIFY(hasNormal);
    QCOMPARE(vertexInd, 0);
    QCOMPARE(normalInd, 2);

    QVERIFY(parsePolygonBlock("1/2", hasVertex, vertexInd, hasTexCoord, texCoordInd, hasNormal, normalInd, errorMsg));
    QVERIFY(hasVertex);
    QVERIFY(hasTexCoord);
    QVERIFY(!hasNormal);
    QCOMPARE(vertexInd, 0);
    QCOMPARE(texCoordInd, 1);

    QVERIFY(parsePolygonBlock("1/2/", hasVertex, vertexInd, hasTexCoord, texCoordInd, hasNormal, normalInd, errorMsg));
    QVERIFY(hasVertex);
    QVERIFY(hasTexCoord);
    QVERIFY(!hasNormal);
    QCOMPARE(vertexInd, 0);
    QCOMPARE(texCoordInd, 1);

    QVERIFY(parsePolygonBlock("/", hasVertex, vertexInd, hasTexCoord, texCoordInd, hasNormal, normalInd, errorMsg));
    QVERIFY(!hasVertex);
    QVERIFY(!hasTexCoord);
    QVERIFY(!hasNormal);

    QVERIFY(parsePolygonBlock("//", hasVertex, vertexInd, hasTexCoord, texCoordInd, hasNormal, normalInd, errorMsg));
    QVERIFY(!hasVertex);
    QVERIFY(!hasTexCoord);
    QVERIFY(!hasNormal);

    QVERIFY(parsePolygonBlock("/2/3", hasVertex, vertexInd, hasTexCoord, texCoordInd, hasNormal, normalInd, errorMsg));
    QVERIFY(!hasVertex);
    QVERIFY(hasTexCoord);
    QVERIFY(hasNormal);
    QCOMPARE(texCoordInd, 1);
    QCOMPARE(normalInd, 2);

    QVERIFY(!parsePolygonBlock("", hasVertex, vertexInd, hasTexCoord, texCoordInd, hasNormal, normalInd, errorMsg));

    QVERIFY(!parsePolygonBlock(" /2/3", hasVertex, vertexInd, hasTexCoord, texCoordInd, hasNormal, normalInd, errorMsg));

    QVERIFY(!parsePolygonBlock("1/2/3/4", hasVertex, vertexInd, hasTexCoord, texCoordInd, hasNormal, normalInd, errorMsg));

    QVERIFY(!parsePolygonBlock("1/2/a", hasVertex, vertexInd, hasTexCoord, texCoordInd, hasNormal, normalInd, errorMsg));
}

void ObjReaderTests::testParsePolygon01()
{
    QVector<int> vertexIndices;
    QVector<int> texCoordIndices;
    QVector<int> normalIndices;
    QString errorMsg;
    QVERIFY(parsePolygon("1/2/3 4/5/6 7/8/9 10/11/12", vertexIndices, texCoordIndices, normalIndices, errorMsg));
    QCOMPARE(vertexIndices, QVector<int>({0, 3, 6, 9}));
    QCOMPARE(texCoordIndices, QVector<int>({1, 4, 7, 10}));
    QCOMPARE(normalIndices, QVector<int>({2, 5, 8, 11}));
}

void ObjReaderTests::testParsePolygon02()
{
    QVector<int> vertexIndices;
    QVector<int> texCoordIndices;
    QVector<int> normalIndices;
    QString errorMsg;
    QVERIFY(!parsePolygon("", vertexIndices, texCoordIndices, normalIndices, errorMsg));
}

void ObjReaderTests::testParsePolygon03()
{
    QVector<int> vertexIndices;
    QVector<int> texCoordIndices;
    QVector<int> normalIndices;
    QString errorMsg;
    QVERIFY(!parsePolygon("1/2/3", vertexIndices, texCoordIndices, normalIndices, errorMsg));
}

void ObjReaderTests::testParsePolygon04()
{
    QVector<int> vertexIndices;
    QVector<int> texCoordIndices;
    QVector<int> normalIndices;
    QString errorMsg;
    QVERIFY(parsePolygon("1//3 4//6 7//9 10//12", vertexIndices, texCoordIndices, normalIndices, errorMsg));
    QCOMPARE(vertexIndices, QVector<int>({0, 3, 6, 9}));
    QCOMPARE(texCoordIndices, QVector<int>());
    QCOMPARE(normalIndices, QVector<int>({2, 5, 8, 11}));
}

void ObjReaderTests::testParsePolygon05()
{
    QVector<int> vertexIndices;
    QVector<int> texCoordIndices;
    QVector<int> normalIndices;
    QString errorMsg;
    QVERIFY(parsePolygon("1/2/ 4/5/ 7/8/ 10/11/", vertexIndices, texCoordIndices, normalIndices, errorMsg));
    QCOMPARE(vertexIndices, QVector<int>({0, 3, 6, 9}));
    QCOMPARE(texCoordIndices, QVector<int>({1, 4, 7, 10}));
    QCOMPARE(normalIndices, QVector<int>());
}

void ObjReaderTests::testParsePolygon06()
{
    QVector<int> vertexIndices;
    QVector<int> texCoordIndices;
    QVector<int> normalIndices;
    QString errorMsg;
    QVERIFY(!parsePolygon("/2/3 /5/6 /8/9 /11/12", vertexIndices, texCoordIndices, normalIndices, errorMsg));
}

void ObjReaderTests::testParsePolygon07()
{
    QVector<int> vertexIndices;
    QVector<int> texCoordIndices;
    QVector<int> normalIndices;
    QString errorMsg;
    QVERIFY(!parsePolygon("1/2/3 4/5/6 7/8/9 10//12", vertexIndices, texCoordIndices, normalIndices, errorMsg));
}

void ObjReaderTests::testParsePolygon08()
{
    QVector<int> vertexIndices;
    QVector<int> texCoordIndices;
    QVector<int> normalIndices;
    QString errorMsg;
    QVERIFY(!parsePolygon("1/2/3 4/5/6 7/8 10/11/12", vertexIndices, texCoordIndices, normalIndices, errorMsg));
}

void ObjReaderTests::testReadTokenString()
{
    QString token;

    QVERIFY(readTokenString("v 1 2 3", token));
    QCOMPARE(token, QString("v"));

    QVERIFY(readTokenString("vt 1 0.5", token));
    QCOMPARE(token, QString("vt"));

    QVERIFY(readTokenString("# ", token));
    QCOMPARE(token, QString("#"));

    QVERIFY(readTokenString("#", token));
    QCOMPARE(token, QString("#"));

    QVERIFY(!readTokenString(" #", token));
}

void ObjReaderTests::testReadTokenAndBody()
{
    ObjToken token;
    QString body;

    QVERIFY(readTokenAndBody("v 1 2 3", token, body));
    QCOMPARE(token, ObjToken::Vertex);
    QCOMPARE(body, QString("1 2 3"));

    QVERIFY(readTokenAndBody("vt 1 0.5", token, body));
    QCOMPARE(token, ObjToken::TexCoord);
    QCOMPARE(body, QString("1 0.5"));

    QVERIFY(readTokenAndBody("vn 1 2 3", token, body));
    QCOMPARE(token, ObjToken::Normal);
    QCOMPARE(body, QString("1 2 3"));

    QVERIFY(readTokenAndBody("# ", token, body));
    QCOMPARE(token, ObjToken::Unknown);
    QVERIFY(body.isEmpty());

    QVERIFY(readTokenAndBody("# comment", token, body));
    QCOMPARE(token, ObjToken::Unknown);
    QCOMPARE(body, QString("comment"));

    QVERIFY(readTokenAndBody("#", token, body));
    QCOMPARE(token, ObjToken::Unknown);
    QVERIFY(body.isEmpty());

    QVERIFY(!readTokenAndBody(" #", token, body));
}

void ObjReaderTests::testReadObj01()
{
    QString objString =
            "v 0 0 0\n"
            "v 1 0 0\n"
            "v 2 0 0\n"
            "v 2 1 0\n"
            "v 1 1 0\n"
            "f 1 2 5\n"
            "f 2 3 4 5\n";
    ObjFileData objData;
    QString errorMsg;
    QTextStream stream(&objString);
    QVERIFY(readFile(stream, objData, errorMsg));
    ObjFileData expectedData;

    expectedData.addVertex({ 0, 0, 0 });
    expectedData.addVertex({ 1, 0, 0 });
    expectedData.addVertex({ 2, 0, 0 });
    expectedData.addVertex({ 2, 1, 0 });
    expectedData.addVertex({ 1, 1, 0});

    expectedData.addPolygonVertexIndex({ 0, 1, 4 });
    expectedData.addPolygonVertexIndex({ 1, 2, 3, 4 });

    QCOMPARE(objData, expectedData);
}

void ObjReaderTests::testReadObj02()
{
    QString objString =
            "v 0 0 0\n"
            "v 1 0 0\n"
            "v 2 0 0\n"
            "v 2 1 0\n"
            "v 1 1 0\n"
            "f 1 2 5\n"
            "f 2 3 4 6\n";
    ObjFileData objData;
    QString errorMsg;
    QTextStream stream(&objString);
    QVERIFY(!readFile(stream, objData, errorMsg));
}

void ObjReaderTests::testReadObj03()
{
    QString objString =
            "v 0 0 0\n"
            "v 1 0 0\n"
            "v 1 1 0\n"
            "vn 0 0 -1\n"
            "vn 0 0 -1\n"
            "vn 0 0 -1\n"
            "f 1//1 2//2 3//3\n"
            "f 1/ 2/ 3/\n";
    ObjFileData objData;
    QString errorMsg;
    QTextStream stream(&objString);
    QVERIFY(!readFile(stream, objData, errorMsg));
}
