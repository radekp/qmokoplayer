

#ifndef RADIOPLAYER_H
#define RADIOPLAYER_H
#include "ui_radioplayer.h"
#include "radioplayeredit.h"
#include <QProcess>
#include <QStringList>


class RadioPlayer : public QWidget, public Ui_RadioPlayer
{
    Q_OBJECT
public:
    RadioPlayer( QWidget *parent = 0, Qt::WFlags f = 0 );
    ~RadioPlayer();

private slots:
    void PlayButton_clicked();
    void StopButton_clicked();
    void VUpButton_clicked();
    void VDownButton_clicked();
    void launchRadioEdit();
    void load();
private:
   QProcess *myProcess;
   QStringList m_radioFlux;
   RadioPlayerEdit * m_re;
 
};

#endif
