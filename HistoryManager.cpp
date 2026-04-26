#include "HistoryManager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QStandardPaths>
#include <QDir>

HistoryManager::HistoryManager(QObject *parent) : QObject(parent) {
    loadFromFile();
}

void HistoryManager::addEntry(const QString &url, const QString &title) {
    HistoryEntry entry;
    entry.url = url;
    entry.title = title;
    entry.timestamp = QDateTime::currentDateTime();
    
    history.prepend(entry);
    
    if (history.size() > 1000) {
        history.removeLast();
    }
    
    saveToFile();
}

QList<HistoryEntry> HistoryManager::getHistory() const {
    return history;
}

void HistoryManager::clearHistory() {
    history.clear();
    saveToFile();
}

void HistoryManager::saveToFile() {
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(path);
    
    QFile file(path + "/history.json");
    if (file.open(QIODevice::WriteOnly)) {
        QJsonArray jsonArray;
        for (const HistoryEntry &entry : history) {
            QJsonObject obj;
            obj["url"] = entry.url;
            obj["title"] = entry.title;
            obj["timestamp"] = entry.timestamp.toString(Qt::ISODate);
            jsonArray.append(obj);
        }
        
        QJsonDocument doc(jsonArray);
        file.write(doc.toJson());
        file.close();
    }
}

void HistoryManager::loadFromFile() {
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QFile file(path + "/history.json");
    
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        file.close();
        
        if (doc.isArray()) {
            QJsonArray jsonArray = doc.array();
            history.clear();
            
            for (const QJsonValue &value : jsonArray) {
                QJsonObject obj = value.toObject();
                HistoryEntry entry;
                entry.url = obj["url"].toString();
                entry.title = obj["title"].toString();
                entry.timestamp = QDateTime::fromString(obj["timestamp"].toString(), Qt::ISODate);
                history.append(entry);
            }
        }
    }
}
