

#ifndef ACCUEIL_H
#define ACCUEIL_H
#include "ui_accueil.h"
#include "radioplayer.h"
#include "options.h"
#include "qmplayer.h"

class Accueil : public QWidget, public Ui_Accueil
{
    Q_OBJECT
public:
    Accueil( QWidget *parent = 0, Qt::WFlags f = 0 );
    ~Accueil();

private slots:
    void RadioButton_clicked();
    void VideoButton_clicked();
    void options();
    void upgrade();
    bool installMplayer();
    bool download(QString url, QString destPath, QString filename, bool justCheck);
private:
   RadioPlayer * m_r;
   QMplayer * m_v;
   Options * m_o; 
};

#endif
