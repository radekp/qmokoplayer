

#include "radioplayer.h"
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
#include <QtopiaApplication>

/*
 *  Constructs a Example which is a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'
 */
RadioPlayer::RadioPlayer(QWidget *parent, Qt::WFlags f)
    : QWidget(parent, f)
{
    setupUi(this); // reads the .ui file to determine widgets and layout
    // Construct context menu, available to the user via Qtopia's soft menu bar.
    QMenu *menu = QSoftMenuBar::menuFor(this);
    menu->addAction(tr("Add/remove radio"), this, SLOT(launchRadioEdit()));
    QAction *actionClose = new QAction(QIcon("close"),tr("Close"),this);
    connect(actionClose,SIGNAL(triggered()), this, SLOT(close()));
    menu->addAction(actionClose);
    QSoftMenuBar::setHelpEnabled(this,false);
    
    //connection of the button
    connect(playButton,SIGNAL(clicked()), this, SLOT(PlayButton_clicked()));
    connect(stopButton,SIGNAL(clicked()), this, SLOT(StopButton_clicked()));
    connect(vUpButton,SIGNAL(clicked()), this, SLOT(VUpButton_clicked()));
    connect(vDownButton,SIGNAL(clicked()), this, SLOT(VDownButton_clicked()));

    
    //loading radios
    load();
	
}

void RadioPlayer::load()
{
	comboBoxRadio->clear();
	m_radioFlux.clear();
	
	//load the parameters
	QSettings *setting = new QSettings(QDir::homePath () +"/.QMokoPlayer/QMokoPlayer_options.ini",QSettings::IniFormat);
	setting->beginGroup("RadioPlayer");
	//for disable suspend if checked
	if (setting->value("checkBoxRPDisableSuspend",false).toBool())
		QtopiaApplication::setPowerConstraint(QtopiaApplication::DisableSuspend);
	
	//for disable lightoff if checked
	if (setting->value("checkBoxRPDisableLightOff",false).toBool())
	 	QtopiaApplication::setPowerConstraint(QtopiaApplication::DisableLightOff);
	 	
	setting->endGroup();
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
	}
}

/*
 *  Destroys the object and frees any allocated resources
 */
RadioPlayer::~RadioPlayer()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Every time the quit button is clicked, this slot will be invoked
 *  (see QMetaObject::connectSlotsByName()).
 */

void RadioPlayer::PlayButton_clicked()
{
	playButton->setEnabled ( false);
    myProcess = new QProcess(this);
    QStringList arguments;
    arguments <<  m_radioFlux. value ((comboBoxRadio-> currentIndex()));
    myProcess->start("mplayer",arguments);
    stopButton->setEnabled (true);
    vUpButton->setEnabled (true);
    vDownButton->setEnabled (true);
}

void RadioPlayer::StopButton_clicked()
{
	myProcess->write("q");
	myProcess->waitForFinished(1000);
    myProcess->terminate();
    stopButton->setEnabled (false);
    vUpButton->setEnabled (false);
    vDownButton->setEnabled (false);
    playButton->setEnabled (true);
}

void RadioPlayer::VUpButton_clicked()
{
	myProcess->write("*");
}

void RadioPlayer::VDownButton_clicked()
{
	myProcess->write("/");
}

void RadioPlayer::launchRadioEdit()
{
	m_re = new RadioPlayerEdit();
    m_re->show();
	connect(m_re,SIGNAL(update()), this, SLOT(load()));
}

