#ifndef SHAREDVIDEO_H
#define SHAREDVIDEO_H

#include <QMainWindow>
#include <QtAV>
#include <QtAVWidgets>
#include <QSlider>
#include <QPushButton>
#include <QTcpSocket>

namespace Ui {
class SharedVideo;
}

class SharedVideo : public QMainWindow
{
    Q_OBJECT

public:
    explicit SharedVideo(QWidget *parent = 0);
    ~SharedVideo();
public slots:
    void openMedia();
    void seekBySlider(int value);
    void seekBySlider();
    void playPause();

    void plus5secs();
    void minus5secs();
private slots:
    void updateSlider(qint64 value);
    void updateSlider();
    void updateSliderUnit();

    // network
    void onSokConnected();
    void onSokDisconnected();
    void onSokReadyRead();
    void onSokDisplayError(QAbstractSocket::SocketError socketError);
    void SendNetCommand(QString cmd);
private:
    Ui::SharedVideo *ui;
    QtAV::VideoOutput *m_vo;
    QtAV::AVPlayer *m_player;
    QSlider *m_slider;
    QPushButton *m_openBtn;
    QPushButton *m_playBtn;
    QPushButton *m_stopBtn;

    QPushButton *m_5secsback;
    QPushButton *m_5secsforward;
    int m_unit;
    QTcpSocket *sock; //сокет
    quint16 _blockSize;//текущий размер блока данных
    QString _name;//имя клиента
    QByteArray receiveddata; // полученные по сети байты
};

#endif // SHAREDVIDEO_H
