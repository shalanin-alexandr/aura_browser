#ifndef BOOKMARKMANAGER_H
#define BOOKMARKMANAGER_H

#include <QObject>
#include <QString>
#include <QList>

struct Bookmark {
    QString url;
    QString title;
};

class BookmarkManager : public QObject {
    Q_OBJECT

public:
    explicit BookmarkManager(QObject *parent = nullptr);
    void addBookmark(const QString &url, const QString &title);
    void removeBookmark(const QString &url);
    QList<Bookmark> getBookmarks() const;
    bool isBookmarked(const QString &url) const;
    void saveToFile();
    void loadFromFile();

private:
    QList<Bookmark> bookmarks;
};

#endif // BOOKMARKMANAGER_H
