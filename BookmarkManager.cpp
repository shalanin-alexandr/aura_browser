#include "BookmarkManager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QStandardPaths>
#include <QDir>

BookmarkManager::BookmarkManager(QObject *parent) : QObject(parent) {
    loadFromFile();
}

void BookmarkManager::addBookmark(const QString &url, const QString &title) {
    if (!isBookmarked(url)) {
        Bookmark bookmark;
        bookmark.url = url;
        bookmark.title = title;
        bookmarks.append(bookmark);
        saveToFile();
    }
}

void BookmarkManager::removeBookmark(const QString &url) {
    for (int i = 0; i < bookmarks.size(); i++) {
        if (bookmarks[i].url == url) {
            bookmarks.removeAt(i);
            saveToFile();
            break;
        }
    }
}

QList<Bookmark> BookmarkManager::getBookmarks() const {
    return bookmarks;
}

bool BookmarkManager::isBookmarked(const QString &url) const {
    for (const Bookmark &bookmark : bookmarks) {
        if (bookmark.url == url) {
            return true;
        }
    }
    return false;
}

void BookmarkManager::saveToFile() {
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(path);
    
    QFile file(path + "/bookmarks.json");
    if (file.open(QIODevice::WriteOnly)) {
        QJsonArray jsonArray;
        for (const Bookmark &bookmark : bookmarks) {
            QJsonObject obj;
            obj["url"] = bookmark.url;
            obj["title"] = bookmark.title;
            jsonArray.append(obj);
        }
        
        QJsonDocument doc(jsonArray);
        file.write(doc.toJson());
        file.close();
    }
}

void BookmarkManager::loadFromFile() {
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QFile file(path + "/bookmarks.json");
    
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        file.close();
        
        if (doc.isArray()) {
            QJsonArray jsonArray = doc.array();
            bookmarks.clear();
            
            for (const QJsonValue &value : jsonArray) {
                QJsonObject obj = value.toObject();
                Bookmark bookmark;
                bookmark.url = obj["url"].toString();
                bookmark.title = obj["title"].toString();
                bookmarks.append(bookmark);
            }
        }
    }
}
