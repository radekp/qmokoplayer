
#ifndef RADIOPLAYEREDIT_H
#define RADIOPLAYEREDIT_H
#include "ui_radioplayeredit.h"

class RadioPlayerEdit : public QWidget, public Ui_RadioPlayerEdit
{
  Q_OBJECT
public:
    RadioPlayerEdit( QWidget *parent = 0, Qt::WFlags f = 0 );
    ~RadioPlayerEdit();

private slots:
    void addButton_clicked();
    void saveButton_clicked();
    void removeButton_clicked();
    void editButton_clicked();
    void load();
    void comboBoxRadio_clicked(int index);
    
signals:
    void update();    
private:
	QStringList m_radioFlux;
    
};

#endif
