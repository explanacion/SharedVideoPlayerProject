#ifndef SHAREDVIDEO_H
#define SHAREDVIDEO_H

#include <QMainWindow>
#include <QtAV>
#include <QtAVWidgets>

namespace Ui {
class SharedVideo;
}

class SharedVideo : public QMainWindow
{
    Q_OBJECT

public:
    explicit SharedVideo(QWidget *parent = 0);
    ~SharedVideo();

private:
    Ui::SharedVideo *ui;
};

#endif // SHAREDVIDEO_H
