#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mcp2210.h"

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <QUrl>
#include <stdio.h>
#include <time.h>
#include <QTimer>
#include <qdebug.h>
#include <QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

/**
 * Test AD732x ADC
 * Note: the code below assumes that CS is connected to GP1.
 */

QString TestAD732x(hid_device* handle)
 {
    ChipSettingsDef chipDef;
    SPITransferSettingsDef def;
    SPIDataTransferStatusDef def1;

    byte spiCmdBuffer[4];
    unsigned int val=0;

    //set GPIO pins to be CS
    chipDef = GetChipSettings(handle);

    for (int i = 0; i < 9; i++)
        {
            chipDef.GP[i].PinDesignation = GP_PIN_DESIGNATION_CS;
            chipDef.GP[i].GPIODirection = GPIO_DIRECTION_OUTPUT;
            chipDef.GP[i].GPIOOutput = 1;
        }
    int r = SetChipSettings(handle, chipDef);

    //configure SPI

    def = GetSPITransferSettings(handle);

    //chip select is GP1
    def.ActiveChipSelectValue = 0xfffe;//1111 1111 1111 1110 CS=GP_0
    def.IdleChipSelectValue = 0xffff;
    def.BitRate = 700000l;
    def.BytesPerSPITransfer =2;
    def.SPIMode = 2;

    r = SetSPITransferSettings(handle, def);

    if (r != 0)
        {

           // return("Errror setting SPI parameters.\n");
        }



    //range
    spiCmdBuffer[1] = 0x00;
    spiCmdBuffer[0] = 0xA0;
    def1 = SPISendReceive(handle, spiCmdBuffer, 2);
    //CH0
    spiCmdBuffer[1] = 0x30; //00000110B (start, single ended, ch2)
    spiCmdBuffer[0] = 0x80; //(ch1 ch0)


   //while(!kbhit())

            def1 = SPISendReceive(handle, spiCmdBuffer, 2);

            val = (def1.DataReceived[0] <<8) | def1.DataReceived[1];
            val &= 0x1FFF;
            int a= (((long)val*10000>>12)-10000);
            QString b= QString(" %1").arg(a, 20, 'i', 2);


            //printf("%X\n", val);
            //printf("		CH0 = %ld mV\n", ((long)val*10000>>12)-10000); //res=(ad0*10000/4096-10000) mV
           return( b);        //res=(ad0*10000/4096-10000) mV")


}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEdit->setText("hello");
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(100);


}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::update()
{    qDebug()<<"am fuckin here";

     hid_device *handle;

        /**
         * initializing the MCP2210 device.
         */

      handle = InitMCP2210();

      if (handle == 0)
          {
              printf("ERROR opening device. Try using sudo.\n"); exit(-1);
          }
      QString va;


        va= TestAD732x(handle);
  //     QString v;
  //       v=(QString)va;
  //        QString("float %1").arg(f, 20, 'f', 20);
       ui->lineEdit->setText("mv");
       QNetworkAccessManager *manager = new QNetworkAccessManager(this);

       connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));

       QString v=va;
       QString s="https://macha-energy.000webhostapp.com/test3/testget.php?myvar=";
       s+=v;
       s+="&var=3";
       manager->get(QNetworkRequest(QUrl(s)));
       manager->get(QNetworkRequest(QUrl(s)));


     /**
       * release the handle
       */
      ReleaseMCP2210(handle);
}
