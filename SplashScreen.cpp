#include "SplashScreen.h"
#include <QPainter>
#include <QPainterPath>
#include <QVBoxLayout>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include <QApplication>
#include <QScreen>
#include <QtMath>
#include <QRandomGenerator>

SplashScreen::SplashScreen(QWidget *parent)
    : QWidget(parent), opacity(0.0), starFrame(0) {
    
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::SplashScreen);
    setAttribute(Qt::WA_TranslucentBackground);
    setFixedSize(600, 400);
    
    QScreen *screen = QApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    move((screenGeometry.width() - width()) / 2, (screenGeometry.height() - height()) / 2);
    
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(15);
    
    logoLabel = new QLabel("AURA");
    logoLabel->setAlignment(Qt::AlignCenter);
    logoLabel->setStyleSheet(
        "font-size: 80px; "
        "font-weight: 900; "
        "color: white; "
        "letter-spacing: 12px; "
        "background: transparent;"
    );
    
    QGraphicsOpacityEffect *logoEffect = new QGraphicsOpacityEffect(logoLabel);
    logoEffect->setOpacity(0);
    logoLabel->setGraphicsEffect(logoEffect);
    
    taglineLabel = new QLabel("EXPLORE THE UNIVERSE");
    taglineLabel->setAlignment(Qt::AlignCenter);
    taglineLabel->setStyleSheet(
        "font-size: 12px; "
        "color: rgba(167, 139, 250, 0.8); "
        "letter-spacing: 6px; "
        "background: transparent;"
    );
    
    QGraphicsOpacityEffect *taglineEffect = new QGraphicsOpacityEffect(taglineLabel);
    taglineEffect->setOpacity(0);
    taglineLabel->setGraphicsEffect(taglineEffect);
    
    layout->addStretch();
    layout->addWidget(logoLabel);
    layout->addWidget(taglineLabel);
    layout->addStretch();
    
    starsTimer = new QTimer(this);
    connect(starsTimer, &QTimer::timeout, this, [this]() {
        starFrame++;
        update();
    });
    starsTimer->start(50);
    
    QPropertyAnimation *logoAnim = new QPropertyAnimation(logoEffect, "opacity", this);
    logoAnim->setDuration(800);
    logoAnim->setStartValue(0.0);
    logoAnim->setEndValue(1.0);
    logoAnim->setEasingCurve(QEasingCurve::OutCubic);
    
    QPropertyAnimation *taglineAnim = new QPropertyAnimation(taglineEffect, "opacity", this);
    taglineAnim->setDuration(800);
    taglineAnim->setStartValue(0.0);
    taglineAnim->setEndValue(1.0);
    taglineAnim->setEasingCurve(QEasingCurve::OutCubic);
    
    QSequentialAnimationGroup *seq = new QSequentialAnimationGroup(this);
    seq->addAnimation(logoAnim);
    seq->addPause(200);
    seq->addAnimation(taglineAnim);
    seq->addPause(800);
    
    connect(seq, &QSequentialAnimationGroup::finished, this, [this]() {
        QGraphicsOpacityEffect *fadeEffect = new QGraphicsOpacityEffect(this);
        fadeEffect->setOpacity(1.0);
        setGraphicsEffect(fadeEffect);
        
        QPropertyAnimation *fadeOut = new QPropertyAnimation(fadeEffect, "opacity", this);
        fadeOut->setDuration(500);
        fadeOut->setStartValue(1.0);
        fadeOut->setEndValue(0.0);
        fadeOut->setEasingCurve(QEasingCurve::InCubic);
        
        connect(fadeOut, &QPropertyAnimation::finished, this, [this]() {
            starsTimer->stop();
            emit finished();
            close();
        });
        
        fadeOut->start();
    });
    
    seq->start();
}

void SplashScreen::startAnimation() {
}

void SplashScreen::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    QLinearGradient bgGradient(0, 0, width(), height());
    bgGradient.setColorAt(0, QColor(15, 10, 40, 250));
    bgGradient.setColorAt(0.5, QColor(30, 15, 60, 250));
    bgGradient.setColorAt(1, QColor(10, 5, 25, 250));
    
    QPainterPath path;
    path.addRoundedRect(rect(), 20, 20);
    painter.fillPath(path, bgGradient);
    
    QRadialGradient nebula1(width() * 0.3, height() * 0.4, 200);
    nebula1.setColorAt(0, QColor(167, 139, 250, 60));
    nebula1.setColorAt(1, QColor(167, 139, 250, 0));
    painter.fillPath(path, nebula1);
    
    QRadialGradient nebula2(width() * 0.7, height() * 0.6, 180);
    nebula2.setColorAt(0, QColor(96, 165, 250, 50));
    nebula2.setColorAt(1, QColor(96, 165, 250, 0));
    painter.fillPath(path, nebula2);
    
    painter.setPen(Qt::NoPen);
    QRandomGenerator gen(42);
    for (int i = 0; i < 60; i++) {
        int x = gen.bounded(width());
        int y = gen.bounded(height());
        int size = gen.bounded(1, 4);
        int phase = (starFrame + i * 7) % 60;
        int alpha = 100 + qAbs(50 - phase) * 3;
        painter.setBrush(QColor(255, 255, 255, alpha));
        painter.drawEllipse(x, y, size, size);
    }
    
    painter.setPen(QPen(QColor(167, 139, 250, 100), 1));
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(rect().adjusted(0, 0, -1, -1), 20, 20);
}
