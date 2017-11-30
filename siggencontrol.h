#ifndef SIGGENCONTROL_H
#define SIGGENCONTROL_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QString>

namespace Ui {
class sigGenControl;
}

class sigGenControl : public QMainWindow
{
    Q_OBJECT

public:
    explicit sigGenControl(QWidget *parent = 0);
    ~sigGenControl();
    QString host; //= "192.168.1.109";
    int port;// = 5025;
    void clearErr();
    bool rfOn;
    bool pset = false;
    bool hset = false;

private:
    Ui::sigGenControl *ui;
    QTcpSocket* socket;

private slots:
    void connectToHostSlot();
    void sendToHostSlot();
    void readyReadSlot();
    void setFreq();
    void setPow();
    void activateRF();
    void checkErrSlot();
    void setPort();
    void setHost();
    void setDefault();
    void dialValChangedSlot();

};

#endif // SIGGENCONTROL_H
