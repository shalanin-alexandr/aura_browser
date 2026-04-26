#include "BrowserWindow.h"
#include "TabWidget.h"
#include "HistoryManager.h"
#include "BookmarkManager.h"
#include "DataManager.h"
#include <QWebEngineView>
#include <QWebEngineHistory>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSplitter>
#include <QMessageBox>
#include <QStandardPaths>
#include <QSizePolicy>
#include <QAbstractButton>
#include <QFile>
#include <QDate>
#include <QTabBar>

BrowserWindow::BrowserWindow(QWidget *parent)
    : QMainWindow(parent) {
    
    historyManager = new HistoryManager(this);
    bookmarkManager = new BookmarkManager(this);
    dataManager = new DataManager(this);
    
    setupUI();
    
    QStringList savedTabs = dataManager->loadOpenTabs();
    if (!savedTabs.isEmpty()) {
        tabWidget->currentWebView()->setUrl(QUrl(savedTabs.first()));
        for (int i = 1; i < savedTabs.size(); i++) {
            tabWidget->createNewTab(QUrl(savedTabs[i]));
        }
    }
}

BrowserWindow::~BrowserWindow() {
    QStringList urls;
    for (int i = 0; i < tabWidget->count(); i++) {
        QWebEngineView *webView = qobject_cast<QWebEngineView*>(tabWidget->widget(i));
        if (webView) {
            urls.append(webView->url().toString());
        }
    }
    dataManager->saveOpenTabs(urls);
}

void BrowserWindow::setupUI() {
    setWindowTitle("AURA Browser");
    setStyleSheet("QMainWindow { background: #050514; } QMenu { background: #0f0f23; color: #e0e0ff; border: 1px solid rgba(167, 139, 250, 0.3); border-radius: 8px; padding: 5px; } QMenu::item { padding: 8px 20px; border-radius: 5px; } QMenu::item:selected { background: rgba(167, 139, 250, 0.2); }");
    
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setStyleSheet("background: #050514;");
    setCentralWidget(centralWidget);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    
    QString navBtnStyle = 
        "QPushButton { "
        "   background: transparent; "
        "   color: rgba(224, 224, 255, 0.7); "
        "   border: none; "
        "   padding: 0; "
        "   font-size: 18px; "
        "   border-radius: 10px; "
        "   min-width: 36px; "
        "   max-width: 36px; "
        "   min-height: 36px; "
        "   max-height: 36px; "
        "} "
        "QPushButton:hover { "
        "   background: rgba(255, 255, 255, 0.08); "
        "   color: #e0e0ff; "
        "} "
        "QPushButton:pressed { "
        "   background: rgba(167, 139, 250, 0.2); "
        "} "
        "QPushButton:disabled { "
        "   color: rgba(224, 224, 255, 0.2); "
        "}";
    
    QWidget *topBar = new QWidget();
    topBar->setStyleSheet("background: #0a0a1f; border-bottom: 1px solid rgba(167, 139, 250, 0.08);");
    QHBoxLayout *topLayout = new QHBoxLayout(topBar);
    topLayout->setContentsMargins(12, 8, 12, 8);
    topLayout->setSpacing(4);
    
    backButton = new QPushButton("‹");
    backButton->setStyleSheet(navBtnStyle);
    backButton->setToolTip("Назад");
    connect(backButton, &QPushButton::clicked, this, &BrowserWindow::onGoBack);
    topLayout->addWidget(backButton);
    
    forwardButton = new QPushButton("›");
    forwardButton->setStyleSheet(navBtnStyle);
    forwardButton->setToolTip("Вперёд");
    connect(forwardButton, &QPushButton::clicked, this, &BrowserWindow::onGoForward);
    topLayout->addWidget(forwardButton);
    
    refreshButton = new QPushButton("↺");
    refreshButton->setStyleSheet(navBtnStyle);
    refreshButton->setToolTip("Обновить");
    connect(refreshButton, &QPushButton::clicked, this, &BrowserWindow::onRefresh);
    topLayout->addWidget(refreshButton);
    
    homeButton = new QPushButton("⌂");
    homeButton->setStyleSheet(navBtnStyle);
    homeButton->setToolTip("Главная");
    connect(homeButton, &QPushButton::clicked, this, &BrowserWindow::onHome);
    topLayout->addWidget(homeButton);
    
    topLayout->addSpacing(8);
    
    addressBar = new QLineEdit();
    addressBar->setPlaceholderText("Поиск или URL");
    addressBar->setStyleSheet(
        "QLineEdit { "
        "   background: rgba(255, 255, 255, 0.04); "
        "   color: #e0e0ff; "
        "   border: 1px solid rgba(255, 255, 255, 0.06); "
        "   border-radius: 10px; "
        "   padding: 0 16px; "
        "   font-size: 13px; "
        "   selection-background-color: rgba(167, 139, 250, 0.4); "
        "} "
        "QLineEdit:focus { "
        "   border: 1px solid rgba(167, 139, 250, 0.5); "
        "   background: rgba(255, 255, 255, 0.06); "
        "} "
        "QLineEdit:hover:!focus { "
        "   background: rgba(255, 255, 255, 0.06); "
        "}"
    );
    addressBar->setMinimumHeight(36);
    connect(addressBar, &QLineEdit::returnPressed, this, &BrowserWindow::onLoadPage);
    topLayout->addWidget(addressBar, 1);
    
    topLayout->addSpacing(8);
    
    bookmarksButton = new QPushButton("★");
    bookmarksButton->setStyleSheet(navBtnStyle);
    bookmarksButton->setToolTip("Закладки");
    connect(bookmarksButton, &QPushButton::clicked, this, &BrowserWindow::onShowBookmarks);
    topLayout->addWidget(bookmarksButton);
    
    historyButton = new QPushButton("⧗");
    historyButton->setStyleSheet(navBtnStyle);
    historyButton->setToolTip("История");
    connect(historyButton, &QPushButton::clicked, this, &BrowserWindow::onShowHistory);
    topLayout->addWidget(historyButton);
    
    saveButton = new QPushButton("⤓");
    saveButton->setStyleSheet(navBtnStyle);
    saveButton->setToolTip("Сохранить данные");
    connect(saveButton, &QPushButton::clicked, this, &BrowserWindow::onSaveData);
    topLayout->addWidget(saveButton);
    
    mainLayout->addWidget(topBar);
    
    QWidget *tabBarContainer = new QWidget();
    tabBarContainer->setStyleSheet("background: #07071a; border-bottom: 1px solid rgba(167, 139, 250, 0.08);");
    QHBoxLayout *tabBarLayout = new QHBoxLayout(tabBarContainer);
    tabBarLayout->setContentsMargins(4, 0, 8, 0);
    tabBarLayout->setSpacing(4);
    
    tabWidget = new TabWidget(this);
    connect(tabWidget, &TabWidget::titleChanged, this, &BrowserWindow::onTitleChanged);
    connect(tabWidget, &TabWidget::urlChanged, this, &BrowserWindow::onUrlChanged);
    
    tabBarLayout->addWidget(tabWidget->tabBar(), 1);
    
    newTabButton = new QPushButton("+");
    newTabButton->setStyleSheet(navBtnStyle);
    newTabButton->setToolTip("Новая вкладка");
    connect(newTabButton, &QPushButton::clicked, this, &BrowserWindow::onNewTab);
    tabBarLayout->addWidget(newTabButton);
    
    mainLayout->addWidget(tabBarContainer);
    mainLayout->addWidget(tabWidget, 1);
    
    newTabButton->setEnabled(false);
    newTabButton->setToolTip("Сначала откройте страницу");
    
    resize(1300, 850);
}

void BrowserWindow::createToolbar() {
}

void BrowserWindow::onLoadPage() {
    QString input = addressBar->text().trimmed();
    if (input.isEmpty()) return;
    
    QUrl url;
    if (input.startsWith("http://") || input.startsWith("https://")) {
        url = QUrl(input);
    } else if (input.contains(".") && !input.contains(" ")) {
        url = QUrl("https://" + input);
    } else {
        url = QUrl("https://www.google.com/search?q=" + input);
    }
    
    if (tabWidget->currentWebView()) {
        tabWidget->currentWebView()->setUrl(url);
    }
}

void BrowserWindow::onGoBack() {
    if (tabWidget->currentWebView()) {
        tabWidget->currentWebView()->back();
    }
}

void BrowserWindow::onGoForward() {
    if (tabWidget->currentWebView()) {
        tabWidget->currentWebView()->forward();
    }
}

void BrowserWindow::onRefresh() {
    if (tabWidget->currentWebView()) {
        tabWidget->currentWebView()->reload();
    }
}

void BrowserWindow::onHome() {
    if (tabWidget->currentWebView()) {
        tabWidget->currentWebView()->setUrl(QUrl("qrc:/resources/home.html"));
    }
}

void BrowserWindow::onNewTab() {
    if (tabWidget->currentWebView()) {
        QString currentUrl = tabWidget->currentWebView()->url().toString();
        if (currentUrl.startsWith("qrc:/resources/home.html") || currentUrl == "about:blank") {
            tabWidget->setCurrentWidget(tabWidget->currentWebView());
            addressBar->setFocus();
            return;
        }
    }
    tabWidget->createNewTab();
}

void BrowserWindow::onCurrentTabChanged(int index) {
    Q_UNUSED(index);
    if (tabWidget->currentWebView()) {
        updateAddressBar(tabWidget->currentWebView()->url());
    }
}

void BrowserWindow::onTitleChanged(const QString &title) {
    Q_UNUSED(title);
    if (tabWidget->currentWebView()) {
        updateAddressBar(tabWidget->currentWebView()->url());
        
        QString url = tabWidget->currentWebView()->url().toString();
        QString pageTitle = tabWidget->currentWebView()->title();
        historyManager->addEntry(url, pageTitle);
    }
}

void BrowserWindow::onUrlChanged(const QUrl &url) {
    updateAddressBar(url);
    
    if (tabWidget->currentWebView()) {
        backButton->setEnabled(tabWidget->currentWebView()->history()->canGoBack());
        forwardButton->setEnabled(tabWidget->currentWebView()->history()->canGoForward());
    }
    
    QString urlStr = url.toString();
    bool onHome = urlStr.startsWith("qrc:/resources/home.html") || urlStr == "about:blank" || urlStr.isEmpty();
    newTabButton->setEnabled(!onHome);
    newTabButton->setToolTip(onHome ? "Сначала откройте страницу" : "Новая вкладка");
}

void BrowserWindow::updateAddressBar(const QUrl &url) {
    QString urlStr = url.toString();
    if (urlStr.startsWith("qrc:/")) {
        addressBar->setText("");
        addressBar->setPlaceholderText("✎  Поиск в интернете или введите URL...");
    } else {
        addressBar->setText(urlStr);
    }
}

static QString messageBoxStyle() {
    return "QMessageBox { background: #0f0f23; } "
           "QMessageBox QLabel { color: #e0e0ff; font-size: 13px; min-width: 400px; } "
           "QMessageBox QPushButton { "
           "   background: rgba(167, 139, 250, 0.2); "
           "   color: #e0e0ff; "
           "   border: 1px solid rgba(167, 139, 250, 0.4); "
           "   border-radius: 6px; "
           "   padding: 6px 20px; "
           "   min-width: 80px; "
           "} "
           "QMessageBox QPushButton:hover { background: rgba(167, 139, 250, 0.35); }";
}

static QString loadTemplate() {
    QFile file(":/resources/page_template.html");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString content = QString::fromUtf8(file.readAll());
        file.close();
        return content;
    }
    return QString();
}

static QString escapeHtml(const QString &text) {
    QString result = text;
    result.replace("&", "&amp;");
    result.replace("<", "&lt;");
    result.replace(">", "&gt;");
    result.replace("\"", "&quot;");
    return result;
}

static QString getFavicon(const QString &url) {
    QString lower = url.toLower();
    if (lower.contains("google")) return "G";
    if (lower.contains("youtube")) return "▶";
    if (lower.contains("github")) return "◆";
    if (lower.contains("wikipedia")) return "W";
    if (lower.contains("stackoverflow")) return "S";
    if (lower.contains("twitter") || lower.contains("x.com")) return "X";
    if (lower.contains("facebook")) return "f";
    if (lower.contains("reddit")) return "R";
    return "✦";
}

void BrowserWindow::onShowHistory() {
    QList<HistoryEntry> history = historyManager->getHistory();
    QString tmpl = loadTemplate();
    
    QString content;
    if (history.isEmpty()) {
        content = "<div class='empty'><div class='empty-icon'>⧖</div><div class='empty-text'>История пуста</div></div>";
    } else {
        QDate currentDate;
        content = "<div class='items'>";
        int idx = 0;
        for (const HistoryEntry &entry : history) {
            QDate entryDate = entry.timestamp.date();
            if (entryDate != currentDate) {
                if (idx > 0) content += "</div>";
                currentDate = entryDate;
                QString dateStr;
                QDate today = QDate::currentDate();
                if (entryDate == today) dateStr = "Сегодня";
                else if (entryDate == today.addDays(-1)) dateStr = "Вчера";
                else dateStr = entryDate.toString("d MMMM yyyy");
                content += QString("<div class='date-section'>%1</div><div class='items'>").arg(dateStr);
            }
            
            QString safeUrl = escapeHtml(entry.url);
            QString safeTitle = escapeHtml(entry.title.isEmpty() ? entry.url : entry.title);
            QString time = entry.timestamp.toString("hh:mm");
            QString favicon = getFavicon(entry.url);
            
            content += QString(
                "<a href='%1' class='item' style='animation-delay:%2ms'>"
                "<div class='item-favicon'>%3</div>"
                "<div class='item-info'>"
                "<div class='item-title'>%4</div>"
                "<div class='item-url'>%1</div>"
                "</div>"
                "<div class='item-meta'>%5</div>"
                "</a>"
            ).arg(safeUrl).arg(qMin(idx * 30, 600)).arg(favicon).arg(safeTitle).arg(time);
            idx++;
        }
        content += "</div>";
    }
    
    QString html = tmpl;
    html.replace("{{TITLE}}", "История - AURA");
    html.replace("{{ICON}}", "⧖");
    html.replace("{{HEADING}}", "История");
    html.replace("{{SUBTITLE}}", "БРАУЗЕРНАЯ АКТИВНОСТЬ");
    html.replace("{{COUNT}}", QString("%1 записей").arg(history.size()));
    html.replace("{{CONTENT}}", content);
    
    QWebEngineView *view = tabWidget->createNewTab(QUrl("about:blank"));
    view->setHtml(html, QUrl("aura://history/"));
}

void BrowserWindow::onShowBookmarks() {
    QString currentUrl = tabWidget->currentWebView() ? tabWidget->currentWebView()->url().toString() : "";
    QString currentTitle = tabWidget->currentWebView() ? tabWidget->currentWebView()->title() : "";
    
    if (!currentUrl.isEmpty() && !currentUrl.startsWith("qrc:/") && !currentUrl.startsWith("aura:")) {
        if (bookmarkManager->isBookmarked(currentUrl)) {
            bookmarkManager->removeBookmark(currentUrl);
        } else {
            bookmarkManager->addBookmark(currentUrl, currentTitle);
        }
    }
    
    QList<Bookmark> bookmarks = bookmarkManager->getBookmarks();
    QString tmpl = loadTemplate();
    
    QString content;
    if (bookmarks.isEmpty()) {
        content = "<div class='empty'><div class='empty-icon'>★</div><div class='empty-text'>Нет сохранённых закладок<br><br><span style='font-size:14px;opacity:0.7'>Нажмите на ★ во время посещения сайта</span></div></div>";
    } else {
        content = "<div class='items'>";
        int idx = 0;
        for (const Bookmark &bookmark : bookmarks) {
            QString safeUrl = escapeHtml(bookmark.url);
            QString safeTitle = escapeHtml(bookmark.title.isEmpty() ? bookmark.url : bookmark.title);
            QString favicon = getFavicon(bookmark.url);
            
            content += QString(
                "<a href='%1' class='item' style='animation-delay:%2ms'>"
                "<div class='item-favicon'>%3</div>"
                "<div class='item-info'>"
                "<div class='item-title'>%4</div>"
                "<div class='item-url'>%1</div>"
                "</div>"
                "</a>"
            ).arg(safeUrl).arg(qMin(idx * 50, 800)).arg(favicon).arg(safeTitle);
            idx++;
        }
        content += "</div>";
    }
    
    QString html = tmpl;
    html.replace("{{TITLE}}", "Закладки - AURA");
    html.replace("{{ICON}}", "★");
    html.replace("{{HEADING}}", "Закладки");
    html.replace("{{SUBTITLE}}", "СОХРАНЁННЫЕ САЙТЫ");
    html.replace("{{COUNT}}", QString("%1 закладок").arg(bookmarks.size()));
    html.replace("{{CONTENT}}", content);
    
    QWebEngineView *view = tabWidget->createNewTab(QUrl("about:blank"));
    view->setHtml(html, QUrl("aura://bookmarks/"));
}

void BrowserWindow::onSaveData() {
    historyManager->saveToFile();
    bookmarkManager->saveToFile();
    
    QStringList urls;
    for (int i = 0; i < tabWidget->count(); i++) {
        QWebEngineView *webView = qobject_cast<QWebEngineView*>(tabWidget->widget(i));
        if (webView) {
            urls.append(webView->url().toString());
        }
    }
    dataManager->saveOpenTabs(urls);
    
    QMessageBox box(this);
    box.setWindowTitle("Сохранено");
    box.setText("✨ Данные успешно сохранены!");
    box.setStyleSheet(messageBoxStyle());
    box.exec();
}
