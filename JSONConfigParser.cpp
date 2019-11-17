#include "JSONConfigParser.h"
#include <QMap>

JsonConfigParser::JsonConfigParser(QString content)
{
    m_jsonDocument = QJsonDocument::fromJson(content.toUtf8());
    m_mainJsonObject = m_jsonDocument.object();
}

QMap<QString, QString> JsonConfigParser::GetMessages() const
{
    QJsonValue valueOfMessagesArr = m_mainJsonObject.value(QString("messages"));
    QJsonArray messagesArr = valueOfMessagesArr.toArray();

    QMap<QString, QString> m_messagesMap;

    for(auto it = messagesArr.begin(); it != messagesArr.end(); it++)
    {
        QJsonObject obj = it->toObject();
        QJsonValue objValue = obj.value(obj.keys()[0]);
        QJsonObject objValueContent = objValue.toObject();
        QJsonValue objMessageValue = objValueContent.value("message");
        QJsonObject messageObject = objMessageValue.toObject();
        QJsonDocument doc(messageObject);
        QString strJson(doc.toJson(QJsonDocument::Compact));

        m_messagesMap.insert(obj.keys()[0], strJson);
    }

    return m_messagesMap;
}

QString JsonConfigParser::GetLastUrl() const
{
    QJsonValue urlValue = m_mainJsonObject.value(QString("lastUrl"));

    return urlValue.toString();
}
