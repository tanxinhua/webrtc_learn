#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_webrtc_learn.h"

class webrtc_learn : public QMainWindow
{
    Q_OBJECT

public:
    webrtc_learn(QWidget *parent = Q_NULLPTR);

private:
    Ui::webrtc_learnClass ui;
};
