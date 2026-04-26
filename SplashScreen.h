#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QLabel>
#include <QTimer>

class SplashScreen : public QWidget {
    Q_OBJECT

public:
    explicit SplashScreen(QWidget *parent = nullptr);
    void startAnimation();

signals:
    void finished();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QLabel *logoLabel;
    QLabel *taglineLabel;
    QTimer *fadeTimer;
    qreal opacity;
    int starFrame;
    QTimer *starsTimer;
};

#endif // SPLASHSCREEN_H
