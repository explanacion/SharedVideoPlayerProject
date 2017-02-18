#include "sharedvideo.h"
#include "ui_sharedvideo.h"
#include <QLayout>
#include <QLabel>
#include <QMessageBox>
#include <QFileDialog>

// http://forum.sources.ru/index.php?showtopic=385391
// https://en.wikipedia.org/wiki/TCP_hole_punching
// http://stackoverflow.com/questions/15914661/java-nat-traversal-for-chat-application
// http://nutss.gforge.cis.cornell.edu/stunt.php
// https://habrahabr.ru/post/233971/
// network issues

using namespace QtAV;

SharedVideo::SharedVideo(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SharedVideo)
{
    ui->setupUi(this);
    m_unit = 1000;
    m_player = new AVPlayer(this);
    QVBoxLayout *vl = new QVBoxLayout();
    this->centralWidget()->setLayout(vl);
    m_vo = new VideoOutput(this);
    if (!m_vo->widget()) {
        QMessageBox::warning(0, QString::fromLatin1("QtAV error"), tr("Can not create video renderer"));
        return;
    }
    m_player->setRenderer(m_vo);
    vl->addWidget(m_vo->widget());
    m_slider = new QSlider();
    m_slider->setOrientation(Qt::Horizontal);
    connect(m_slider, SIGNAL(sliderMoved(int)), SLOT(seekBySlider(int)));
    connect(m_slider, SIGNAL(sliderPressed()), SLOT(seekBySlider()));
    connect(m_player, SIGNAL(positionChanged(qint64)), SLOT(updateSlider(qint64)));
    connect(m_player, SIGNAL(started()), SLOT(updateSlider()));
    connect(m_player, SIGNAL(notifyIntervalChanged()), SLOT(updateSliderUnit()));

    vl->addWidget(m_slider);
    QHBoxLayout *hb = new QHBoxLayout();
    vl->addLayout(hb);
    m_openBtn = new QPushButton(tr("Open"));
    m_playBtn = new QPushButton(tr("Play/Pause"));
    m_stopBtn = new QPushButton(tr("Stop"));
    m_5secsback = new QPushButton(tr("-5 seconds"));
    m_5secsforward = new QPushButton(tr("+5 seconds"));
    hb->addWidget(m_openBtn);
    hb->addWidget(m_5secsback);
    hb->addWidget(m_playBtn);
    hb->addWidget(m_5secsforward);
    hb->addWidget(m_stopBtn);
    connect(m_openBtn, SIGNAL(clicked()), SLOT(openMedia()));
    connect(m_playBtn, SIGNAL(clicked()), SLOT(playPause()));
    connect(m_stopBtn, SIGNAL(clicked()), m_player, SLOT(stop()));
    connect(m_5secsback, SIGNAL(clicked()), SLOT(minus5secs()));
    connect(m_5secsforward, SIGNAL(clicked()), SLOT(plus5secs()));

    // establish network connection
    sock = new QTcpSocket();
    _blockSize = 0;
    connect(sock, SIGNAL(readyRead()), this, SLOT(onSokReadyRead()));
    connect(sock, SIGNAL(connected()), this, SLOT(onSokConnected()));
    connect(sock, SIGNAL(disconnected()), this, SLOT(onSokDisconnected()));
    connect(sock, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSokDisplayError(QAbstractSocket::SocketError)));

    sock->connectToHost("ns.krppc.ru",10000, QIODevice::ReadWrite);
}

void SharedVideo::onSokConnected()
{
    qDebug() << "connected";
    Sleep(1000);
    SendNetCommand("test");
}

void SharedVideo::onSokDisconnected()
{
    qDebug() << "disconnected";
}

void SharedVideo::onSokReadyRead()
{
    qDebug() << "reading";
    //int bsize = sock->bytesAvailable();
    //если считываем новый блок первый байт это его размер
    if (_blockSize == 0) {
        receiveddata.clear(); // очищаем буфер для свежих данных
        //считываем размер (1 байт)
        bool isFirstByteNumber = false;
        QByteArray tmp = sock->read(1);
        tmp.toInt(&isFirstByteNumber,16);
        if (isFirstByteNumber) {
            _blockSize = tmp.toInt(&isFirstByteNumber,16);
            qDebug() << "size of our data is " + QString::number(_blockSize);
        }
        else {
            qDebug() << "first byte isn't a number";
            qDebug() << sock->read(1);
        }
    }
    //ждем пока блок прийдет полностью
    if (sock->bytesAvailable() < _blockSize)
    {
        return;
    }
    else
    {
        receiveddata.append(sock->readAll());
        if (receiveddata.length() == 0) {
            qDebug() << "no data was currently available for reading, or that an error occurred";
        }
        qDebug() << "Received from the server: " << receiveddata;
        //можно принимать новый блок
        _blockSize = 0;
    }

    // since the second byte begins the command

    if (isdigit(receiveddata.at(2))) {
        int command = receiveddata.at(2);
        switch (command)
        {
        // register me
        case 1:
        {

        }
        break;
        // stop video
        case 2:
        {

        }
        break;
        // change position of the video
        case 3:
        {
        }
        // pause the video
        case 4:
        {
        }
        break;
        }
    }
}

void SharedVideo::onSokDisplayError(QAbstractSocket::SocketError socketError)
{

}

void SharedVideo::SendNetCommand(QString cmd)
{
    QByteArray block = cmd.toUtf8();
    //резервируем 1 байт для размера блока
    int length=block.size() + 1;
    block.insert(0,QChar(length));
    qDebug() << "trying to send: " + QString(block);
    sock->write(block);
}

void SharedVideo::minus5secs()
{
    qDebug() << "minus " << m_player->position();
    m_player->seek(m_player->position() - qint64(5*m_unit));
}

void SharedVideo::plus5secs()
{
    m_player->seek(m_player->position() + qint64(5*m_unit));
}

void SharedVideo::openMedia()
{
    QString file = QFileDialog::getOpenFileName(0, tr("Open a video"));
    if (file.isEmpty())
        return;
    m_player->play(file);
}

void SharedVideo::seekBySlider(int value)
{
    if (!m_player->isPlaying())
        return;
    m_player->seek(qint64(value*m_unit));
    updateSlider(qint64(value*m_unit));
}

void SharedVideo::seekBySlider()
{
    seekBySlider(m_slider->value());
}

void SharedVideo::playPause()
{
    if (!m_player->isPlaying()) {
        m_player->play();
        return;
    }
    m_player->pause(!m_player->isPaused());
}

void SharedVideo::updateSlider(qint64 value)
{
    m_slider->setRange(0, int(m_player->duration()/m_unit));
    m_slider->setValue(int(value/m_unit));
}

void SharedVideo::updateSlider()
{
    updateSlider(m_player->position());
}

void SharedVideo::updateSliderUnit()
{
    m_unit = m_player->notifyInterval();
    updateSlider();
}

SharedVideo::~SharedVideo()
{
    delete m_player;
    delete m_vo;
    delete ui;
}
