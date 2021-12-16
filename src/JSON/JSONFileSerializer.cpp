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
    rootObject["last_url"] = "ws://localhost:3030";
    rootObject["messages"] = QJsonValue::Array;

    m_jsonDocument.setObject(rootObject);

    WriteToConfig(m_jsonDocument);
}

void JSONFileSerializer::WriteToConfig(const QJsonDocument &document)
{
    QFile file(m_configFilePath);
    file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);
    file.write(document.toJson(QJsonDocument::Indented));
    file.close();
}

void JSONFileSerializer::SaveLastUrl(const QString& url)
{
    QJsonObject rootObject = m_jsonDocument.object();
    rootObject["last_url"] = url;
    m_jsonDocument.setObject(rootObject);

    WriteToConfig(m_jsonDocument);
}

QJsonDocument JSONFileSerializer::GetDocument() const
{
    return m_jsonDocument;
}

void JSONFileSerializer::SaveHeaders(QMap<QString, QString> headersMap)
{
    QJsonObject rootObject = m_jsonDocument.object();
    QJsonArray headersArray;

    QMapIterator<QString, QString> it(headersMap);
    while (it.hasNext())
    {
        it.next();

        QJsonObject headerObject;
        headerObject[it.key()] = it.value();
        headersArray.push_back(headerObject);
    }

    rootObject["headers"] = headersArray;
    m_jsonDocument.setObject(rootObject);

    WriteToConfig(m_jsonDocument);
}

void JSONFileSerializer::SaveMessages(QMap<QString, QString> messageListMap, const QString &fileName)
{
    QJsonDocument document;
    QJsonArray messagesArr;
    QJsonObject newRootObject = document.object();

    auto it = messageListMap.begin();
    while (it != messageListMap.end())
    {
        QString messageName = it.key();
        QString messageBody = it.value();

        QJsonObject messageObject;
        messageObject["name"] = messageName;
        messageObject["body"] = QJsonDocument::fromJson(messageBody.toUtf8()).object();

        messagesArr.push_back(messageObject);

        ++it;
    }

    newRootObject["messages"] = messagesArr;
    document.setObject(newRootObject);

    WriteToConfig(document);
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

QString JSONFileSerializer::GetLastUrl()
{
     QJsonObject rootObject = m_jsonDocument.object();
     return rootObject["last_url"].toString();
}
