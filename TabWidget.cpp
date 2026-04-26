#include "TabWidget.h"
#include <QWebEngineView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTabBar>

TabWidget::TabWidget(QWidget *parent) : QTabWidget(parent) {
    setTabsClosable(true);
    setMovable(true);
    setDocumentMode(true);
    
    setStyleSheet(
        "QTabWidget::pane { "
        "   border: none; "
        "   background: #050514; "
        "   top: 0px; "
        "} "
        "QTabWidget::tab-bar { "
        "   alignment: left; "
        "   left: 8px; "
        "} "
        "QTabBar { "
        "   background: transparent; "
        "   qproperty-drawBase: 0; "
        "   outline: none; "
        "} "
        "QTabBar::tab { "
        "   background: rgba(167, 139, 250, 0.04); "
        "   color: rgba(196, 181, 253, 0.55); "
        "   padding: 9px 16px; "
        "   margin: 7px 3px 7px 3px; "
        "   border-radius: 10px; "
        "   min-width: 150px; "
        "   max-width: 210px; "
        "   font-size: 12px; "
        "   font-weight: 500; "
        "   border: 1px solid rgba(167, 139, 250, 0.08); "
        "} "
        "QTabBar::tab:selected { "
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgba(167, 139, 250, 0.22), stop:1 rgba(96, 165, 250, 0.12)); "
        "   color: #e9e4ff; "
        "   border: 1px solid rgba(167, 139, 250, 0.45); "
        "} "
        "QTabBar::tab:hover:!selected { "
        "   background: rgba(167, 139, 250, 0.1); "
        "   color: rgba(224, 217, 255, 0.85); "
        "   border: 1px solid rgba(167, 139, 250, 0.2); "
        "} "
        "QTabBar::close-button { "
        "   subcontrol-position: right; "
        "   margin: 4px; "
        "   border-radius: 5px; "
        "   image: none; "
        "} "
        "QTabBar::close-button:hover { "
        "   background: rgba(239, 68, 68, 0.3); "
        "}"
    );
    
    connect(this, &QTabWidget::tabCloseRequested, this, &TabWidget::onTabCloseRequested);
    connect(this, &QTabWidget::currentChanged, this, &TabWidget::onCurrentChanged);
    
    createNewTab(QUrl("qrc:/resources/home.html"));
}

QWebEngineView* TabWidget::createNewTab(const QUrl &url) {
    QWebEngineView *webView = new QWebEngineView(this);
    QUrl targetUrl = url.isEmpty() || url == QUrl("https://www.google.com") ? QUrl("qrc:/resources/home.html") : url;
    webView->setUrl(targetUrl);
    
    setupTab(webView);
    
    int index = addTab(webView, "Новая вкладка");
    setCurrentIndex(index);
    
    return webView;
}

QWebEngineView* TabWidget::currentWebView() {
    return qobject_cast<QWebEngineView*>(currentWidget());
}

void TabWidget::closeCurrentTab() {
    int currentIndex = this->currentIndex();
    if (currentIndex >= 0) {
        if (count() > 1) {
            removeTab(currentIndex);
        }
    }
}

void TabWidget::setupTab(QWebEngineView *webView) {
    connect(webView, &QWebEngineView::titleChanged, this, [this, webView](const QString &title) {
        int index = indexOf(webView);
        if (index >= 0) {
            QString tabTitle = title.isEmpty() ? "Новая вкладка" : title;
            if (tabTitle.length() > 25) {
                tabTitle = tabTitle.left(25) + "...";
            }
            setTabText(index, tabTitle);
        }
        emit titleChanged(title);
    });
    
    connect(webView, &QWebEngineView::urlChanged, this, [this](const QUrl &url) {
        emit urlChanged(url);
    });
}

void TabWidget::onTabCloseRequested(int index) {
    if (count() > 1) {
        removeTab(index);
    }
}

void TabWidget::onCurrentChanged(int index) {
    if (index >= 0) {
        QWebEngineView *webView = qobject_cast<QWebEngineView*>(widget(index));
        if (webView) {
            emit urlChanged(webView->url());
            emit titleChanged(webView->title());
        }
    }
}
