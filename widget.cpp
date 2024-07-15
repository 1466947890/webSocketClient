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
    if(!ui->listWidget_contacts->currentItem())
    {
        QMessageBox::information(this, "提示", "请先选择联系人");
        qDebug() << "没有选中联系人";
        return;
    }
    QString contact = ui->listWidget_contacts->currentItem()->data(0).toString();
//    qDebug() << "当前联系人：" << contact;
    QString msg = ui->textEdit_Msg->toPlainText();

    QJsonObject obj;
    obj.insert("type", 2);
    obj.insert("msg", msg);
    obj.insert("contact", contact);

    QJsonDocument doc(obj);
    m_webSocket.sendTextMessage(doc.toJson());
}

void Widget::onConnected()
{
    connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &Widget::onTextMessageReceived);
    // 获取一次联系人列表
    QJsonObject info;
    info.insert("type", 1);
    QJsonDocument json(info);
    m_webSocket.sendTextMessage(json.toJson());

}

void Widget::closed()
{
    // 断开与槽的链接，修复重复发送的问题
    disconnect(&m_webSocket, nullptr, nullptr, nullptr);
}

void Widget::onTextMessageReceived(QString msg)
{
//    qDebug() << "received:" << msg;
//    ui->listWidget_Msg->addItem(msg);
    QJsonDocument doc = QJsonDocument::fromJson(msg.toUtf8());
    if(!doc.isObject())
    {
        qDebug() << "不是json数据";
        return;
    }
    int type = doc["type"].toInt();

    switch (type) {
    case 1:
        // 加载联系人列表
        loadContacts(doc);
        break;
    case 2:
        // 加载消息列表
        ui->listWidget_Msg->addItem(doc["msg"].toString());
        break;
    default:
        qDebug() << "当前类型为" << type << "没有找到类型";
    }
}

void Widget::loadContacts(QJsonDocument doc)
{
    // 初始化联系人列表
    ui->listWidget_contacts->clear();
    QJsonArray contactsArr = doc["contacts"].toArray();
    for(int i=0; i < contactsArr.size(); i++)
    {
//        qDebug() << contactsArr.at(i).toString();
        ui->listWidget_contacts->addItem(contactsArr.at(i).toString());
    }

}


