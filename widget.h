#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QWebSocket>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QException>
#include <QMessageBox>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();


private slots:
    void on_pushButton_Start_clicked();
    void on_pushButton_Closed_clicked();
    void on_pushButton_Send_clicked();

    void onConnected();
    void closed();
    void onTextMessageReceived(QString msg);

private:
    Ui::Widget *ui;
    QWebSocket m_webSocket;
    QUrl m_url;
    QString contact;

private:
    void loadContacts(QJsonDocument doc);
};

#endif // WIDGET_H
