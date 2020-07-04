#ifndef JSONFILESERIALIZER_H
#define JSONFILESERIALIZER_H

#include <QObject>
#include <QMap>
#include <QJsonDocument>


class JSONFileSerializer : public QObject
{
    Q_OBJECT
public:
    explicit JSONFileSerializer(QObject *parent = nullptr);

    void Load();

    bool Serialize(QString newMessageName, QString newMessageBody);
    void SaveMessages(QMap<QString, QString> messageListMap, const QString& fileName = "");
    QMap<QString, QString> ReadSavedMessages();

protected:
    void SetupInitialConfigFileContents();

private:
    QJsonDocument m_jsonDocument;
    QString m_configFilePath;

signals:

};

#endif // JSONFILESERIALIZER_H
