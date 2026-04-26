#include "BrowserWindow.h"
#include "SplashScreen.h"
#include <QApplication>
#include <QTimer>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    SplashScreen *splash = new SplashScreen();
    splash->show();
    
    BrowserWindow *window = new BrowserWindow();
    
    QObject::connect(splash, &SplashScreen::finished, [window]() {
        window->resize(1300, 850);
        
        QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(window);
        effect->setOpacity(0);
        window->setGraphicsEffect(effect);
        
        window->show();
        
        QPropertyAnimation *fadeIn = new QPropertyAnimation(effect, "opacity");
        fadeIn->setDuration(600);
        fadeIn->setStartValue(0.0);
        fadeIn->setEndValue(1.0);
        fadeIn->setEasingCurve(QEasingCurve::OutCubic);
        QObject::connect(fadeIn, &QPropertyAnimation::finished, [window]() {
            window->setGraphicsEffect(nullptr);
        });
        fadeIn->start(QAbstractAnimation::DeleteWhenStopped);
    });
    
    return app.exec();
}
