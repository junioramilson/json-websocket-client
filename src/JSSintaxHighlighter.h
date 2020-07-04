#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE

//! [0]
class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(QTextDocument *parent = 0)
        : QSyntaxHighlighter(parent)
    {
        HighlightingRule rule;

            keywordFormat.setForeground(Qt::darkBlue);
            keywordFormat.setFontWeight(QFont::Bold);
        //! [1]

        //! [4]
            jsonKeysFormat.setForeground(QBrush(QColor("#FF7043")));
            rule.pattern = QRegularExpression(QStringLiteral("(?m)^[ ]*([^\r\n:]+?)\\s*\""));
            rule.format = jsonKeysFormat;
            highlightingRules.append(rule);
        //! [4]

        //! [5]
            jsonValuesFormat.setForeground(QBrush(QColor("#FFCA28")));
            rule.pattern = QRegularExpression(QStringLiteral("[:^]\\s(\".*\")"));
            rule.format = jsonValuesFormat;
            highlightingRules.append(rule);
        //! [5]
    }

protected:
    void highlightBlock(const QString &text) override
    {
        for (const HighlightingRule &rule : qAsConst(highlightingRules)) {
            QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
            while (matchIterator.hasNext()) {
                QRegularExpressionMatch match = matchIterator.next();
                setFormat(match.capturedStart(), match.capturedLength(), rule.format);
            }
        }
        //! [7] //! [8]
        setCurrentBlockState(0);
    }

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;
    QTextCharFormat keywordFormat;
    QTextCharFormat jsonKeysFormat;
    QTextCharFormat jsonValuesFormat;
};
//! [0]

#endif // HIGHLIGHTER_H
