

#ifndef ACCUEIL_H
#define ACCUEIL_H
#include "ui_accueil.h"
#include "radioplayer.h"
#include "options.h"

class Accueil : public QWidget, public Ui_Accueil
{
    Q_OBJECT
public:
    Accueil( QWidget *parent = 0, Qt::WFlags f = 0 );
    ~Accueil();

private slots:
    void RadioButton_clicked();
    void options();
private:
   RadioPlayer * m_r;
   Options * m_o; 
};

#endif
