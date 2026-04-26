#ifndef BROWSERWINDOW_H
#define BROWSERWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolBar>
#include <QLabel>
#include <QWebEngineHistory>

class TabWidget;
class HistoryManager;
class BookmarkManager;
class DataManager;

class BrowserWindow : public QMainWindow {
    Q_OBJECT

public:
    BrowserWindow(QWidget *parent = nullptr);
    ~BrowserWindow();

private slots:
    void onLoadPage();
    void onGoBack();
    void onGoForward();
    void onRefresh();
    void onHome();
    void onNewTab();
    void onCurrentTabChanged(int index);
    void onTitleChanged(const QString &title);
    void onUrlChanged(const QUrl &url);
    void onShowHistory();
    void onShowBookmarks();
    void onSaveData();

private:
    void setupUI();
    void createToolbar();
    void updateAddressBar(const QUrl &url);
    
    QLineEdit *addressBar;
    QPushButton *backButton;
    QPushButton *forwardButton;
    QPushButton *refreshButton;
    QPushButton *homeButton;
    QPushButton *newTabButton;
    QPushButton *historyButton;
    QPushButton *bookmarksButton;
    QPushButton *saveButton;
    
    TabWidget *tabWidget;
    HistoryManager *historyManager;
    BookmarkManager *bookmarkManager;
    DataManager *dataManager;
    
    QToolBar *mainToolBar;
};

#endif // BROWSERWINDOW_H
