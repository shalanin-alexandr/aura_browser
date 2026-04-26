#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>
#include <QStringList>

class DataManager : public QObject {
    Q_OBJECT

public:
    explicit DataManager(QObject *parent = nullptr);
    void saveOpenTabs(const QStringList &urls);
    QStringList loadOpenTabs();
    
private:
    QString dataPath;
};

#endif // DATAMANAGER_H
