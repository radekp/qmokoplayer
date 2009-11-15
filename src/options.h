

#ifndef OPTIONS_H
#define OPTIONS_H
#include "ui_options.h"

class Options : public QWidget, public Ui_Options
{
    Q_OBJECT
public:
    Options( QWidget *parent = 0, Qt::WFlags f = 0 );
    ~Options();
    void Load();
 private slots:
     void SaveButton_clicked();   
};

#endif
