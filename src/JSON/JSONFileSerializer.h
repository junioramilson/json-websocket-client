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

    static JSONFileSerializer& Instance()
    {
        static JSONFileSerializer jfs;
        return jfs;
    }

    void Load();

    bool Serialize(QString newMessageName, QString newMessageBody);
    void SaveMessages(QMap<QString, QString> messageListMap, const QString& fileName = "");
    QMap<QString, QString> ReadSavedMessages();
    QString GetLastUrl();
    void SaveLastUrl(const QString& url);
    QJsonDocument GetDocument() const;
    void SaveHeaders(QMap<QString, QString> headers);
protected:
    void SetupInitialConfigFileContents();
    void WriteToConfig(const QJsonDocument& document);

private:
    QJsonDocument m_jsonDocument;
    QString m_configFilePath;

signals:

};

#endif // JSONFILESERIALIZER_H
