#include "sharedvideo.h"
#include "ui_sharedvideo.h"

SharedVideo::SharedVideo(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SharedVideo)
{
    ui->setupUi(this);
}

SharedVideo::~SharedVideo()
{
    delete ui;
}
