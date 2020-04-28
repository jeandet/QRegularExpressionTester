/*------------------------------------------------------------------------------
-- This file is a part of the QRegularExpressionTester program
-- Copyright (C) 2020, Plasma Physics Laboratory - CNRS
--
-- This program is free software; you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation; either version 2 of the License, or
-- (at your option) any later version.
--
-- This program is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
-- GNU General Public License for more details.
--
-- You should have received a copy of the GNU General Public License
-- along with this program; if not, write to the Free Software
-- Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
-------------------------------------------------------------------------------*/
/*-- Author : Alexis Jeandet
-- Mail : alexis.jeandet@member.fsf.org
----------------------------------------------------------------------------*/

#include "regextester.h"
#include "ui_regextester.h"

#include <QColor>
#include <QRandomGenerator>
#include <QRegularExpression>
#include <QSyntaxHighlighter>
#include <QDebug>

static QColor colours[8] = {
    QColor("magenta"),   QColor("red"),   QColor("darkGreen"),
    QColor("darkRed"), QColor("darkCyan"),  QColor("darkMagenta"),
    QColor("green"), QColor("blue")};

class RegexHighlighter : public QSyntaxHighlighter {
  Q_OBJECT
  QString m_regex;

public:
  RegexHighlighter(QTextDocument *parent) : QSyntaxHighlighter(parent) {}
  void highlightBlock(const QString &text) override {
    QTextCharFormat format, boldFormat;
    boldFormat.setFontWeight(QFont::Bold);
    int colourIndex = 0;
    QRegularExpression expression(m_regex);
    QRegularExpressionMatchIterator i = expression.globalMatch(text);
    while (i.hasNext()) {
      format.setForeground(colours[colourIndex]);
      colourIndex = (colourIndex + 1) % 8;
      boldFormat.setForeground(format.foreground());
      boldFormat.setBackground(QColor{"lightGrey"});
      QRegularExpressionMatch match = i.next();
      setFormat(match.capturedStart(), match.capturedLength(), format);
      for (int j = 1; j <= match.lastCapturedIndex(); j++) {
        setFormat(match.capturedStart(j), match.capturedLength(j), boldFormat);
      }
    }
  }

  void updateRegex(const QString &regex) {
    m_regex = regex;
    rehighlight();
  }
};

RegexTester::RegexTester(QWidget *parent)
    : QWidget(parent), ui(new Ui::RegexTester) {
  ui->setupUi(this);
  auto highlighter = new RegexHighlighter(ui->text->document());
  connect(ui->regex, &QLineEdit::textChanged, highlighter,
          &RegexHighlighter::updateRegex);
}

RegexTester::~RegexTester() { delete ui; }

#include "regextester.moc"
