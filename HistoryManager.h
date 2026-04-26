#ifndef HISTORYMANAGER_H
#define HISTORYMANAGER_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QList>

struct HistoryEntry {
    QString url;
    QString title;
    QDateTime timestamp;
};

class HistoryManager : public QObject {
    Q_OBJECT

public:
    explicit HistoryManager(QObject *parent = nullptr);
    void addEntry(const QString &url, const QString &title);
    QList<HistoryEntry> getHistory() const;
    void clearHistory();
    void saveToFile();
    void loadFromFile();

private:
    QList<HistoryEntry> history;
};

#endif // HISTORYMANAGER_H
