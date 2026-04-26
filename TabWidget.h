#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QTabWidget>
#include <QWebEngineView>
#include <QUrl>

class TabWidget : public QTabWidget {
    Q_OBJECT

public:
    explicit TabWidget(QWidget *parent = nullptr);
    QWebEngineView* createNewTab(const QUrl &url = QUrl("https://www.google.com"));
    QWebEngineView* currentWebView();
    void closeCurrentTab();
    
signals:
    void titleChanged(const QString &title);
    void urlChanged(const QUrl &url);

private slots:
    void onTabCloseRequested(int index);
    void onCurrentChanged(int index);
    
private:
    void setupTab(QWebEngineView *webView);
};

#endif // TABWIDGET_H
