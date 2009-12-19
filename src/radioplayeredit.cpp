
#include "radioplayeredit.h"
#include <qpushbutton.h>
#include <qsoftmenubar.h>
#include <QAction>
#include <QMenu>
//For reading the file witch contain the radio
#include <QTextStream>
#include <QDir>
#include <QString>
#include <QIODevice>


/*
 *  Constructs a Example which is a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'
 */
RadioPlayerEdit::RadioPlayerEdit(QWidget *parent, Qt::WFlags f)
    : QWidget(parent, f)
{
    setupUi(this); // reads the .ui file to determine widgets and layout
    // Construct context menu, available to the user via Qtopia's soft menu bar.
    QMenu *menu = QSoftMenuBar::menuFor(this);
    QAction *actionClose = new QAction(QIcon("close"),tr("Close"),this);
    connect(actionClose,SIGNAL(triggered()), this, SLOT(close()));
    
    connect(addButton,SIGNAL(clicked()), this, SLOT(addButton_clicked()));
    connect(saveButton,SIGNAL(clicked()), this, SLOT(saveButton_clicked()));
    connect(removeButton,SIGNAL(clicked()), this, SLOT(removeButton_clicked()));
    connect(editButton,SIGNAL(clicked()), this, SLOT(editButton_clicked()));
        
    connect(comboBoxRadio,SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxRadio_clicked(int)));

    menu->addAction(actionClose);
    //no help
    QSoftMenuBar::setHelpEnabled(this,false);
    
   	//loading radios
    load();
    
}

//load the radio from the file
void RadioPlayerEdit::load()
{
	//with contain the list of the radio (name|flux)
    QFile file(QDir::homePath()+"/.QMokoPlayer/radioList.txt");
    QString tmp="";
	if (file.open(QIODevice::ReadOnly))
	{	
		QTextStream ts( &file );
		while (!ts.atEnd ())
		{
			tmp=ts.readLine();
	    	comboBoxRadio->addItem(tmp.split('|').first ());
 	    	m_radioFlux.push_back (tmp.split('|').last());
		}
		file.close();
		aliasLineEdit->setText(comboBoxRadio->currentText ());
		fluxLineEdit->setText(m_radioFlux. value ((comboBoxRadio-> currentIndex())));
	}
}



/*
 *  Destroys the object and frees any allocated resources
 */
RadioPlayerEdit::~RadioPlayerEdit()
{
    // no need to delete child widgets, Qt does it all for us
}



/*
 *  Buttons signals!
 */
void RadioPlayerEdit::addButton_clicked()
{
	comboBoxRadio->addItem(aliasLineEdit->text());
 	m_radioFlux.push_back (fluxLineEdit->text());
}

void RadioPlayerEdit::removeButton_clicked()
{
	m_radioFlux.removeAt(comboBoxRadio-> currentIndex());
	comboBoxRadio->removeItem(comboBoxRadio-> currentIndex());
}

void RadioPlayerEdit::saveButton_clicked()
{
	saveButton->setEnabled(false);
	//with contain the list of the radio (name|flux)
    QFile file(QDir::homePath()+"/.QMokoPlayer/radioList.txt");
    QString tmp="";
	if (file.open(QIODevice::WriteOnly))
	{	
		QTextStream ts( &file );
		for (int i=0;i<comboBoxRadio->count();i++) 
		{
			ts<<comboBoxRadio->itemText(i)<<"|"<<m_radioFlux.value(i)<<"\n";
		}
		file.close();
	}
	emit update();
	saveButton->setEnabled(true);
}

void RadioPlayerEdit::editButton_clicked()
{
	m_radioFlux.replace(comboBoxRadio-> currentIndex(),fluxLineEdit->text());
	comboBoxRadio-> setItemText(comboBoxRadio-> currentIndex(),aliasLineEdit->text());
}


void RadioPlayerEdit::comboBoxRadio_clicked(int index)
{
	aliasLineEdit->setText(comboBoxRadio->currentText ());
	fluxLineEdit->setText(m_radioFlux. value(index));
}
