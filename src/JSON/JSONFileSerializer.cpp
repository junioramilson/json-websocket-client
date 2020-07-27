#include "JSONFileSerializer.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDir>
#include <QJsonParseError>
#include <QJsonValue>

JSONFileSerializer::JSONFileSerializer(QObject *parent) : QObject(parent)
{
    m_configFilePath = QDir::currentPath() + "/config.json";
}

void JSONFileSerializer::Load()
{
    QFile file(m_configFilePath);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QJsonParseError jsonParseError;
        m_jsonDocument = QJsonDocument::fromJson(file.readAll(), &jsonParseError);

        if (m_jsonDocument.isEmpty())
        {
            SetupInitialConfigFileContents();
        }

        file.close();
    }

}

void JSONFileSerializer::SetupInitialConfigFileContents()
{
    QJsonObject rootObject = m_jsonDocument.object();

    rootObject["messages"] = QJsonValue::Array;

    m_jsonDocument.setObject(rootObject);

    QFile file(m_configFilePath);
    file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);
    file.write(m_jsonDocument.toJson(QJsonDocument::Indented));
    file.close();
}

void JSONFileSerializer::SaveMessages(QMap<QString, QString> messageListMap, const QString &fileName)
{
    QJsonObject rootObject;

    auto it = messageListMap.begin();
    while (it != messageListMap.end())
    {
        QString messageName = it.key();
        QString messageBody = it.value();

        ++it;
    }
}

QMap<QString, QString> JSONFileSerializer::ReadSavedMessages()
{
    QMap<QString, QString> messageListMap;

    QJsonObject rootObject = m_jsonDocument.object();

    QJsonArray messagesArray = rootObject.find("messages")->toArray();

    auto it = messagesArray.begin();
    while(it != messagesArray.end())
    {
        if (it->isObject())
        {
            QJsonObject messageObject = it->toObject();
            QString messageName = messageObject.find("name")->toString();
            QJsonObject messageBodyObject = messageObject.find("body")->toObject();
            QString messageBody = QJsonDocument(messageBodyObject).toJson(QJsonDocument::Indented);

            messageListMap.insert(messageName, messageBody);
        }

        ++it;
    }

    return messageListMap;
}
