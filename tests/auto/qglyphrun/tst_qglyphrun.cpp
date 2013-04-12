/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the test suite of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtTest/QtTest>

#include <qglyphrun.h>
#include <qpainter.h>
#include <qtextlayout.h>
#include <qfontdatabase.h>

// #define DEBUG_SAVE_IMAGE

class tst_QGlyphRun: public QObject
{
    Q_OBJECT

#if !defined(QT_NO_RAWFONT)
private slots:
    void initTestCase();
    void cleanupTestCase();

    void constructionAndDestruction();
    void copyConstructor();
    void assignment();
    void equalsOperator_data();
    void equalsOperator();
    void textLayoutGlyphIndexes();
    void drawExistingGlyphs();
    void drawNonExistentGlyphs();
    void drawMultiScriptText1();
    void drawMultiScriptText2();
    void drawStruckOutText();
    void drawOverlinedText();
    void drawUnderlinedText();
    void drawRightToLeft();
    void detach();
    void setRawData();
    void setRawDataAndGetAsVector();

private:
    int m_testFontId;
    QFont m_testFont;
#endif // QT_NO_RAWFONT

};

#if !defined(QT_NO_RAWFONT)

Q_DECLARE_METATYPE(QGlyphRun);

void tst_QGlyphRun::initTestCase()
{
    m_testFontId = QFontDatabase::addApplicationFont(SRCDIR "test.ttf");
    QVERIFY(m_testFontId >= 0);

    m_testFont = QFont("QtsSpecialTestFont");

    QCOMPARE(QFontInfo(m_testFont).family(), QString::fromLatin1("QtsSpecialTestFont"));
}

void tst_QGlyphRun::cleanupTestCase()
{
    QFontDatabase::removeApplicationFont(m_testFontId);
}

void tst_QGlyphRun::constructionAndDestruction()
{
    QGlyphRun glyphIndexes;
}

static QGlyphRun make_dummy_indexes()
{
    QGlyphRun glyphs;

    QVector<quint32> glyphIndexes;
    QVector<QPointF> positions;
    QFont font;
    font.setPointSize(18);

    glyphIndexes.append(1);
    glyphIndexes.append(2);
    glyphIndexes.append(3);

    positions.append(QPointF(1, 2));
    positions.append(QPointF(3, 4));
    positions.append(QPointF(5, 6));

    glyphs.setRawFont(QRawFont::fromFont(font));
    glyphs.setGlyphIndexes(glyphIndexes);
    glyphs.setPositions(positions);

    return glyphs;
}

void tst_QGlyphRun::copyConstructor()
{
    QGlyphRun glyphs;

    {
        QVector<quint32> glyphIndexes;
        QVector<QPointF> positions;
        QFont font;
        font.setPointSize(18);

        glyphIndexes.append(1);
        glyphIndexes.append(2);
        glyphIndexes.append(3);

        positions.append(QPointF(1, 2));
        positions.append(QPointF(3, 4));
        positions.append(QPointF(5, 6));

        glyphs.setRawFont(QRawFont::fromFont(font));
        glyphs.setGlyphIndexes(glyphIndexes);
        glyphs.setPositions(positions);
    }

    QGlyphRun otherGlyphs(glyphs);
    QCOMPARE(otherGlyphs.rawFont(), glyphs.rawFont());
    QCOMPARE(glyphs.glyphIndexes(), otherGlyphs.glyphIndexes());
    QCOMPARE(glyphs.positions(), otherGlyphs.positions());
}

void tst_QGlyphRun::assignment()
{
    QGlyphRun glyphs(make_dummy_indexes());

    QGlyphRun otherGlyphs = glyphs;
    QCOMPARE(otherGlyphs.rawFont(), glyphs.rawFont());
    QCOMPARE(glyphs.glyphIndexes(), otherGlyphs.glyphIndexes());
    QCOMPARE(glyphs.positions(), otherGlyphs.positions());
}

void tst_QGlyphRun::equalsOperator_data()
{
    QTest::addColumn<QGlyphRun>("one");
    QTest::addColumn<QGlyphRun>("two");
    QTest::addColumn<bool>("equals");

    QGlyphRun one(make_dummy_indexes());
    QGlyphRun two(make_dummy_indexes());

    QTest::newRow("Identical") << one << two << true;

    {
        QGlyphRun busted(two);

        QVector<QPointF> positions = busted.positions();
        positions[2] += QPointF(1, 1);
        busted.setPositions(positions);


        QTest::newRow("Different positions") << one << busted << false;
    }

    {
        QGlyphRun busted(two);

        QFont font;
        font.setPixelSize(busted.rawFont().pixelSize() * 2);
        busted.setRawFont(QRawFont::fromFont(font));

        QTest::newRow("Different fonts") << one << busted << false;
    }

    {
        QGlyphRun busted(two);

        QVector<quint32> glyphIndexes = busted.glyphIndexes();
        glyphIndexes[2] += 1;
        busted.setGlyphIndexes(glyphIndexes);

        QTest::newRow("Different glyph indexes") << one << busted << false;
    }

}

void tst_QGlyphRun::equalsOperator()
{
    QFETCH(QGlyphRun, one);
    QFETCH(QGlyphRun, two);
    QFETCH(bool, equals);

    QCOMPARE(one == two, equals);
    QCOMPARE(one != two, !equals);
}


void tst_QGlyphRun::textLayoutGlyphIndexes()
{
    QString s;
    s.append(QLatin1Char('A'));
    s.append(QChar(0xe000));

    QTextLayout layout(s);
    layout.setFont(m_testFont);
    layout.beginLayout();
    layout.createLine();
    layout.endLayout();

    QList<QGlyphRun> listOfGlyphs = layout.glyphRuns();
    QCOMPARE(listOfGlyphs.size(), 1);

    QGlyphRun glyphs = listOfGlyphs.at(0);

    QCOMPARE(glyphs.glyphIndexes().size(), 2);
    QCOMPARE(glyphs.glyphIndexes().at(0), quint32(2));
    QCOMPARE(glyphs.glyphIndexes().at(1), quint32(1));
}

void tst_QGlyphRun::drawExistingGlyphs()
{
    QPixmap textLayoutDraw(1000, 1000);
    QPixmap drawGlyphs(1000, 1000);

    textLayoutDraw.fill(Qt::white);
    drawGlyphs.fill(Qt::white);

    QString s;
    s.append(QLatin1Char('A'));
    s.append(QChar(0xe000));

    QTextLayout layout(s);
    layout.setFont(m_testFont);
    layout.beginLayout();
    layout.createLine();
    layout.endLayout();

    {
        QPainter p(&textLayoutDraw);
        layout.draw(&p, QPointF(50, 50));
    }

    QGlyphRun glyphs = layout.glyphRuns().size() > 0
                                 ? layout.glyphRuns().at(0)
                                 : QGlyphRun();

    {
        QPainter p(&drawGlyphs);
        p.drawGlyphRun(QPointF(50, 50), glyphs);
    }

#if defined(DEBUG_SAVE_IMAGE)
    textLayoutDraw.save("drawExistingGlyphs_textLayoutDraw.png");
    drawGlyphs.save("drawExistingGlyphs_drawGlyphIndexes.png");
#endif

    QCOMPARE(textLayoutDraw, drawGlyphs);
}

void tst_QGlyphRun::setRawData()
{
    QGlyphRun glyphRun;
    glyphRun.setRawFont(QRawFont::fromFont(m_testFont));
    glyphRun.setGlyphIndexes(QVector<quint32>() << 2 << 2 << 2);
    glyphRun.setPositions(QVector<QPointF>() << QPointF(2, 3) << QPointF(20, 3) << QPointF(10, 20));

    QPixmap baseline(100, 50);
    baseline.fill(Qt::white);
    {
        QPainter p(&baseline);
        p.drawGlyphRun(QPointF(3, 2), glyphRun);
    }

    QGlyphRun baselineCopied = glyphRun;

    quint32 glyphIndexArray[3] = { 2, 2, 2 };
    QPointF glyphPositionArray[3] = { QPointF(2, 3), QPointF(20, 3), QPointF(10, 20) };

    glyphRun.setRawData(glyphIndexArray, glyphPositionArray, 3);

    QPixmap rawDataGlyphs(100, 50);
    rawDataGlyphs.fill(Qt::white);
    {
        QPainter p(&rawDataGlyphs);
        p.drawGlyphRun(QPointF(3, 2), glyphRun);
    }

    quint32 otherGlyphIndexArray[1] = { 2 };
    QPointF otherGlyphPositionArray[1] = { QPointF(2, 3) };

    glyphRun.setRawData(otherGlyphIndexArray, otherGlyphPositionArray, 1);

    QPixmap baselineCopiedPixmap(100, 50);
    baselineCopiedPixmap.fill(Qt::white);
    {
        QPainter p(&baselineCopiedPixmap);
        p.drawGlyphRun(QPointF(3, 2), baselineCopied);
    }

#if defined(DEBUG_SAVE_IMAGE)
    baseline.save("setRawData_baseline.png");
    rawDataGlyphs.save("setRawData_rawDataGlyphs.png");
    baselineCopiedPixmap.save("setRawData_baselineCopiedPixmap.png");
#endif

    QCOMPARE(rawDataGlyphs, baseline);
    QCOMPARE(baselineCopiedPixmap, baseline);
}

void tst_QGlyphRun::setRawDataAndGetAsVector()
{
    QVector<quint32> glyphIndexArray;
    glyphIndexArray << 3 << 2 << 1 << 4;

    QVector<QPointF> glyphPositionArray;
    glyphPositionArray << QPointF(1, 2) << QPointF(3, 4) << QPointF(5, 6) << QPointF(7, 8);

    QGlyphRun glyphRun;
    glyphRun.setRawData(glyphIndexArray.constData(), glyphPositionArray.constData(), 4);

    QVector<quint32> glyphIndexes = glyphRun.glyphIndexes();
    QVector<QPointF> glyphPositions = glyphRun.positions();

    QCOMPARE(glyphIndexes.size(), 4);
    QCOMPARE(glyphPositions.size(), 4);

    QCOMPARE(glyphIndexes, glyphIndexArray);
    QCOMPARE(glyphPositions, glyphPositionArray);

    QGlyphRun otherGlyphRun;
    otherGlyphRun.setGlyphIndexes(glyphIndexArray);
    otherGlyphRun.setPositions(glyphPositionArray);

    QCOMPARE(glyphRun, otherGlyphRun);
}

void tst_QGlyphRun::drawNonExistentGlyphs()
{
    QVector<quint32> glyphIndexes;
    glyphIndexes.append(3);

    QVector<QPointF> glyphPositions;
    glyphPositions.append(QPointF(0, 0));

    QGlyphRun glyphs;
    glyphs.setGlyphIndexes(glyphIndexes);
    glyphs.setPositions(glyphPositions);
    glyphs.setRawFont(QRawFont::fromFont(m_testFont));

    QPixmap image(1000, 1000);
    image.fill(Qt::white);

    QPixmap imageBefore = image;
    {
        QPainter p(&image);
        p.drawGlyphRun(QPointF(50, 50), glyphs);
    }

#if defined(DEBUG_SAVE_IMAGE)
    image.save("drawNonExistentGlyphs.png");
#endif

    QCOMPARE(image, imageBefore); // Should be unchanged
}

void tst_QGlyphRun::drawMultiScriptText1()
{
    QString text;
    text += QChar(0x03D0); // Greek, beta

    QTextLayout textLayout(text);
    textLayout.beginLayout();
    textLayout.createLine();
    textLayout.endLayout();

    QPixmap textLayoutDraw(1000, 1000);
    textLayoutDraw.fill(Qt::white);

    QPixmap drawGlyphs(1000, 1000);
    drawGlyphs.fill(Qt::white);

    QList<QGlyphRun> glyphsList = textLayout.glyphRuns();
    QCOMPARE(glyphsList.size(), 1);

    {
        QPainter p(&textLayoutDraw);
        textLayout.draw(&p, QPointF(50, 50));
    }

    {
        QPainter p(&drawGlyphs);
        foreach (QGlyphRun glyphs, glyphsList)
            p.drawGlyphRun(QPointF(50, 50), glyphs);
    }

#if defined(DEBUG_SAVE_IMAGE)
    textLayoutDraw.save("drawMultiScriptText1_textLayoutDraw.png");
    drawGlyphs.save("drawMultiScriptText1_drawGlyphIndexes.png");
#endif

    QCOMPARE(drawGlyphs, textLayoutDraw);
}


void tst_QGlyphRun::drawMultiScriptText2()
{
    QString text;
    text += QChar(0x0621); // Arabic, Hamza
    text += QChar(0x03D0); // Greek, beta

    QTextLayout textLayout(text);
    textLayout.beginLayout();
    textLayout.createLine();
    textLayout.endLayout();

    QPixmap textLayoutDraw(1000, 1000);
    textLayoutDraw.fill(Qt::white);

    QPixmap drawGlyphs(1000, 1000);
    drawGlyphs.fill(Qt::white);

    QList<QGlyphRun> glyphsList = textLayout.glyphRuns();
    QCOMPARE(glyphsList.size(), 2);

    {
        QPainter p(&textLayoutDraw);
        textLayout.draw(&p, QPointF(50, 50));
    }

    {
        QPainter p(&drawGlyphs);
        foreach (QGlyphRun glyphs, glyphsList)
            p.drawGlyphRun(QPointF(50, 50), glyphs);
    }

#if defined(DEBUG_SAVE_IMAGE)
    textLayoutDraw.save("drawMultiScriptText2_textLayoutDraw.png");
    drawGlyphs.save("drawMultiScriptText2_drawGlyphIndexes.png");
#endif

    QCOMPARE(drawGlyphs, textLayoutDraw);
}

void tst_QGlyphRun::detach()
{
    QGlyphRun glyphs;

    glyphs.setGlyphIndexes(QVector<quint32>() << 1 << 2 << 3);

    QGlyphRun otherGlyphs;
    otherGlyphs = glyphs;

    QCOMPARE(otherGlyphs.glyphIndexes(), glyphs.glyphIndexes());

    otherGlyphs.setGlyphIndexes(QVector<quint32>() << 4 << 5 << 6);

    QCOMPARE(otherGlyphs.glyphIndexes(), QVector<quint32>() << 4 << 5 << 6);
    QCOMPARE(glyphs.glyphIndexes(), QVector<quint32>() << 1 << 2 << 3);
}

void tst_QGlyphRun::drawStruckOutText()
{
    QPixmap textLayoutDraw(1000, 1000);
    QPixmap drawGlyphs(1000, 1000);

    textLayoutDraw.fill(Qt::white);
    drawGlyphs.fill(Qt::white);

    QString s = QString::fromLatin1("Foobar");

    QFont font;
    font.setStrikeOut(true);

    QTextLayout layout(s);
    layout.setFont(font);
    layout.beginLayout();
    layout.createLine();
    layout.endLayout();

    {
        QPainter p(&textLayoutDraw);
        layout.draw(&p, QPointF(50, 50));
    }

    QGlyphRun glyphs = layout.glyphRuns().size() > 0
                                 ? layout.glyphRuns().at(0)
                                 : QGlyphRun();

    {
        QPainter p(&drawGlyphs);
        p.drawGlyphRun(QPointF(50, 50), glyphs);
    }

#if defined(DEBUG_SAVE_IMAGE)
    textLayoutDraw.save("drawStruckOutText_textLayoutDraw.png");
    drawGlyphs.save("drawStruckOutText_drawGlyphIndexes.png");
#endif

    QCOMPARE(textLayoutDraw, drawGlyphs);
}

void tst_QGlyphRun::drawOverlinedText()
{
    QPixmap textLayoutDraw(1000, 1000);
    QPixmap drawGlyphs(1000, 1000);

    textLayoutDraw.fill(Qt::white);
    drawGlyphs.fill(Qt::white);

    QString s = QString::fromLatin1("Foobar");

    QFont font;
    font.setOverline(true);

    QTextLayout layout(s);
    layout.setFont(font);
    layout.beginLayout();
    layout.createLine();
    layout.endLayout();

    {
        QPainter p(&textLayoutDraw);
        layout.draw(&p, QPointF(50, 50));
    }

    QGlyphRun glyphs = layout.glyphRuns().size() > 0
                                 ? layout.glyphRuns().at(0)
                                 : QGlyphRun();

    {
        QPainter p(&drawGlyphs);
        p.drawGlyphRun(QPointF(50, 50), glyphs);
    }

#if defined(DEBUG_SAVE_IMAGE)
    textLayoutDraw.save("drawOverlineText_textLayoutDraw.png");
    drawGlyphs.save("drawOverlineText_drawGlyphIndexes.png");
#endif

    QCOMPARE(textLayoutDraw, drawGlyphs);
}

void tst_QGlyphRun::drawUnderlinedText()
{
    QPixmap textLayoutDraw(1000, 1000);
    QPixmap drawGlyphs(1000, 1000);

    textLayoutDraw.fill(Qt::white);
    drawGlyphs.fill(Qt::white);

    QString s = QString::fromLatin1("Foobar");

    QFont font;
    font.setUnderline(true);

    QTextLayout layout(s);
    layout.setFont(font);
    layout.beginLayout();
    layout.createLine();
    layout.endLayout();

    {
        QPainter p(&textLayoutDraw);
        layout.draw(&p, QPointF(50, 50));
    }

    QGlyphRun glyphs = layout.glyphRuns().size() > 0
                                 ? layout.glyphRuns().at(0)
                                 : QGlyphRun();

    {
        QPainter p(&drawGlyphs);
        p.drawGlyphRun(QPointF(50, 50), glyphs);
    }

#if defined(DEBUG_SAVE_IMAGE)
    textLayoutDraw.save("drawUnderlineText_textLayoutDraw.png");
    drawGlyphs.save("drawUnderlineText_drawGlyphIndexes.png");
#endif

    QCOMPARE(textLayoutDraw, drawGlyphs);
}

void tst_QGlyphRun::drawRightToLeft()
{
    QString s;
    s.append(QChar(1575));
    s.append(QChar(1578));

    QPixmap textLayoutDraw(1000, 1000);
    QPixmap drawGlyphs(1000, 1000);

    textLayoutDraw.fill(Qt::white);
    drawGlyphs.fill(Qt::white);

    QFont font;
    font.setUnderline(true);

    QTextLayout layout(s);
    layout.setFont(font);
    layout.beginLayout();
    layout.createLine();
    layout.endLayout();

    {
        QPainter p(&textLayoutDraw);
        layout.draw(&p, QPointF(50, 50));
    }

    QGlyphRun glyphs = layout.glyphRuns().size() > 0
                                 ? layout.glyphRuns().at(0)
                                 : QGlyphRun();

    {
        QPainter p(&drawGlyphs);
        p.drawGlyphRun(QPointF(50, 50), glyphs);
    }

#if defined(DEBUG_SAVE_IMAGE)
    textLayoutDraw.save("drawRightToLeft_textLayoutDraw.png");
    drawGlyphs.save("drawRightToLeft_drawGlyphIndexes.png");
#endif

    QCOMPARE(textLayoutDraw, drawGlyphs);

}

#endif // QT_NO_RAWFONT

QTEST_MAIN(tst_QGlyphRun)
#include "tst_qglyphrun.moc"
