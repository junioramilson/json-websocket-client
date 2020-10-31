#ifndef JSONCONFIGPARSER_H
#define JSONCONFIGPARSER_H

#include <QString>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QStringList>

class JsonConfigParser
{
public:
    JsonConfigParser(QString content);

    QMap<QString, QString> GetMessages() const;
    QString GetLastUrl() const;

    QJsonDocument GetJsonDocument() const { return m_jsonDocument; }

private:
    QJsonDocument m_jsonDocument;
    QJsonObject m_mainJsonObject;
};

#endif // JSONCONFIGPARSER_H
