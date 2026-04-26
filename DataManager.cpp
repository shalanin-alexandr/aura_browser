#include "DataManager.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QStandardPaths>
#include <QDir>

DataManager::DataManager(QObject *parent) : QObject(parent) {
    dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(dataPath);
}

void DataManager::saveOpenTabs(const QStringList &urls) {
    QFile file(dataPath + "/tabs.json");
    if (file.open(QIODevice::WriteOnly)) {
        QJsonArray jsonArray;
        for (const QString &url : urls) {
            jsonArray.append(url);
        }
        
        QJsonDocument doc(jsonArray);
        file.write(doc.toJson());
        file.close();
    }
}

QStringList DataManager::loadOpenTabs() {
    QStringList urls;
    QFile file(dataPath + "/tabs.json");
    
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        file.close();
        
        if (doc.isArray()) {
            QJsonArray jsonArray = doc.array();
            for (const QJsonValue &value : jsonArray) {
                urls.append(value.toString());
            }
        }
    }
    
    return urls;
}
