#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->pushButton_Closed->setEnabled(false);
    ui->lineEdit_Url->setText("ws://127.0.0.1:8000");
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_Start_clicked()
{
    ui->pushButton_Start->setEnabled(false);
    ui->pushButton_Closed->setEnabled(true);
    QString urlStr = ui->lineEdit_Url->text();

    QUrl url = QUrl(urlStr);
    connect(&m_webSocket, &QWebSocket::connected, this, &Widget::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &Widget::closed);
//    qDebug() << "url:" << url;
    try {
            m_webSocket.open(url);
    } catch (QException e) {
        qDebug() << "出错";
    }
}

void Widget::on_pushButton_Closed_clicked()
{
    ui->pushButton_Start->setEnabled(true);
    ui->pushButton_Closed->setEnabled(false);
    m_webSocket.close();
}

void Widget::on_pushButton_Send_clicked()
{
    QString msg = ui->textEdit_Msg->toPlainText();
    qDebug() << msg;
    m_webSocket.sendTextMessage(msg);
}

void Widget::onConnected()
{
    connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &Widget::onTextMessageReceived);
//    m_webSocket.sendTextMessage("Hello");
}

void Widget::closed()
{
//    qDebug() << "关闭";
}

void Widget::onTextMessageReceived(QString msg)
{
    qDebug() << "received:" << msg;
}
