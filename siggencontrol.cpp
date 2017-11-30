#include "siggencontrol.h"
#include "ui_siggencontrol.h"
#include "qdebug.h"
#include <QTime>

sigGenControl::sigGenControl(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::sigGenControl)
{
    ui->setupUi(this);
    ui->history->setReadOnly(true);
    ui->history->setText("host : not set\nport : not set");
    ui->history->append(QTime::currentTime().toString() + " : No connection");
    socket = new QTcpSocket(this);
    connect(ui->connPb,QPushButton::clicked,this,sigGenControl::connectToHostSlot);
    connect(ui->sendToHostPb,QPushButton::clicked,this,sigGenControl::sendToHostSlot);
    connect(ui->input,QLineEdit::returnPressed,this,sigGenControl::sendToHostSlot);
    connect(socket,QTcpSocket::readyRead,this,sigGenControl::readyReadSlot);
    connect(ui->freqEdit,QLineEdit::returnPressed,this,sigGenControl::setFreq);
    connect(ui->amptdEdit,QLineEdit::returnPressed,this,sigGenControl::setPow);
    connect(ui->radioButton,QRadioButton::clicked,this,sigGenControl::activateRF);
    connect(ui->errPb,QPushButton::clicked,this,sigGenControl::clearErr);
    connect(ui->hostEdit,QLineEdit::returnPressed,this,sigGenControl::setHost);
    connect(ui->portEdit,QLineEdit::returnPressed,this,sigGenControl::setPort);
    connect(ui->checkBox,QCheckBox::clicked,this,sigGenControl::setDefault);
    connect(ui->dial, QDial::valueChanged,this,sigGenControl::dialValChangedSlot);


    ui->hist->setReadOnly(true);
    QString deactRF = ":OUTP 0\n";
    socket->write(deactRF.toLocal8Bit());
    this->rfOn = false;

    ui->input->setReadOnly(true);
    ui->freqEdit->setReadOnly(true);
    ui->amptdEdit->setReadOnly(true);
    ui->radioButton->setEnabled(false);
    ui->sendToHostPb->setEnabled(false);
    ui->errPb->setEnabled(false);
    ui->connPb->setDisabled(true);

}

sigGenControl::~sigGenControl()
{
    delete ui;
}

void sigGenControl::connectToHostSlot(){
  socket->connectToHost(host,port);
  if (socket->waitForConnected(1000))
        qDebug("Connected!");
  ui->history->append(QTime::currentTime().toString() + " : Connected");

  ui->input->setReadOnly(false);
  ui->freqEdit->setReadOnly(false);
  ui->amptdEdit->setReadOnly(false);
  ui->radioButton->setEnabled(true);
  ui->sendToHostPb->setEnabled(true);
  ui->errPb->setEnabled(true);
  ui->input->setText("*IDN?");
}

void sigGenControl::sendToHostSlot(){
    socket->write(ui->input->text().toLocal8Bit()+"\n");
}

void sigGenControl::readyReadSlot(){
    qDebug("ready read!");
    QByteArray dataArr;


    dataArr = socket->readAll();
    ui->hist->append(QTime::currentTime().toString() + " " + dataArr);


}

void sigGenControl::clearErr(){
    QString errTxt = ":SYST:ERR?\n";
    socket->write(errTxt.toLocal8Bit());

}

void sigGenControl::setFreq(){
    QString fr = " kHz";
    QString cmd = ":FREQ ";
    QString command = cmd+ui->freqEdit->text()+fr+"\n";
    qDebug() << command;
    socket->write(command.toLocal8Bit());
}

void sigGenControl::setPow(){
    QString amp = " dBm";
    QString cmd = ":POW ";
    socket->write(cmd.toLocal8Bit() + ui->amptdEdit->text().toLocal8Bit() + amp.toLocal8Bit() + "\n");
}

void sigGenControl::activateRF(){
    QString actRF = ":OUTP 1\n";
    QString deactRF = ":OUTP 0\n";
    if(this->rfOn == false){
    socket->write(actRF.toLocal8Bit());
    rfOn = true;
    }else{
        socket->write(deactRF.toLocal8Bit());
        rfOn = false;
    }
}

void sigGenControl::checkErrSlot(){
    clearErr();
}

void sigGenControl::setHost(){
   this->host = ui->hostEdit->text();
    ui->history->append("host: " + host);
    ui->checkBox->setEnabled(true);
    hset = true;
    if((pset && hset) == true){
        ui->connPb->setEnabled(true);
    }
}

void sigGenControl::setPort(){
   this->port = ui->portEdit->text().toInt();
    ui->history->append("port: "  + QString::number(port));
    ui->checkBox->setEnabled(true);
    pset = true;
    if((pset && hset) == true){
        ui->connPb->setEnabled(true);
    }
}

void sigGenControl::setDefault(){
    port = 5025;
    host = "192.168.1.109";
     ui->history->append(QTime::currentTime().toString() + " " + "host: " + host);
     ui->history->append(QTime::currentTime().toString() + " " + "port: "  + QString::number(port));
     ui->checkBox->setDisabled(true);
     ui->connPb->setEnabled(true);
     ui->hostEdit->setText(host);
     ui->portEdit->setText(QString::number(port));

}

void sigGenControl::dialValChangedSlot(){
   qDebug()<< "value : " << ui->dial->value();

  if(ui->amplCheckBox->isChecked() == false){
      ui->dial->setMinimum(100);
      ui->dial->setMaximum(20000000);
      QString fr = " kHz";
      QString cmd = ":FREQ ";
      QString command = cmd+QString::number(ui->dial->value())+fr+"\n";
      qDebug() << command;
      socket->write(command.toLocal8Bit());
      ui->label->setText("FREQUENCY :" + QString::number(ui->dial->value()) + " kHz" );

  }else if(ui->amplCheckBox->isChecked() == true){
      ui->dial->setMinimum(-130);
      ui->dial->setMaximum(15);
      QString pw = " dBm";
      QString cmd = ":POW ";
      QString command = cmd+QString::number(ui->dial->value())+pw+"\n";
      qDebug() << command;
      socket->write(command.toLocal8Bit());
      ui->label->setText("AMPLITUDE :" + QString::number(ui->dial->value()) + " dBm" );
  }

}
