
#include "options.h"
#include <qpushbutton.h>
#include <qsoftmenubar.h>
#include <QAction>
#include <QMenu>
#include "radioplayer.h"
#include <QDir>
#include <QSettings>

/*
 *  Constructs a Example which is a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'
 */
Options::Options(QWidget *parent, Qt::WFlags f)
    : QWidget(parent, f)
{
    setupUi(this); // reads the .ui file to determine widgets and layout
    // Construct context menu, available to the user via Qtopia's soft menu bar.
    QMenu *menu = QSoftMenuBar::menuFor(this);

    QAction *actionClose = new QAction(QIcon("close"),tr("Close"),this);
    connect(actionClose,SIGNAL(triggered()), this, SLOT(close()));
    connect(SaveButton,SIGNAL(clicked()), this, SLOT(SaveButton_clicked()));
    menu->addAction(actionClose);
    QSoftMenuBar::setHelpEnabled(this,false);
    Load();
    
}

/*
 *  Destroys the object and frees any allocated resources
 */
Options::~Options()
{
    // no need to delete child widgets, Qt does it all for us
}


/*
 *  Every time the RadioButton button is clicked, this slot will be invoked
 *  (see QMetaObject::connectSlotsByName()).
 */
void Options::SaveButton_clicked()
{
	QSettings *setting = new QSettings(QDir::homePath () +"/.QMokoPlayer/QMokoPlayer_options.ini",QSettings::IniFormat);
	
	 //Music Player parameters
	 setting->beginGroup("MusicPlayer");
	 setting->setValue("checkBoxMPDisableSuspend",checkBoxMPDisableSuspend->isChecked ());
	 setting->setValue("checkBoxMPDisableLightOff",checkBoxMPDisableLightOff->isChecked ());
	 setting->endGroup();
	 //Radio Player parameters
	 setting->beginGroup("RadioPlayer");
	 setting->setValue("checkBoxRPDisableSuspend",checkBoxRPDisableSuspend->isChecked ());
	 setting->setValue("checkBoxRPDisableLightOff",checkBoxRPDisableLightOff->isChecked ());
	 setting->endGroup();
	 //Video Player parameters
	 setting->beginGroup("VideoPlayer");
	 setting->setValue("checkBoxVPDisableSuspend",checkBoxVPDisableSuspend->isChecked ());
	 setting->setValue("checkBoxVPDisableLightOff",checkBoxVPDisableLightOff->isChecked ());
	 setting->endGroup();
	 //Youtube Player parameters
	 setting->beginGroup("YoutubePlayer");
	 setting->setValue("checkBoxYPDisableSuspend",checkBoxYPDisableSuspend->isChecked ());
	 setting->setValue("checkBoxYPDisableLightOff",checkBoxYPDisableLightOff->isChecked ());
	 setting->endGroup();
}

//load the parameters from the file QMokoPLayer_options.ini
void Options::Load()
{
	QSettings *setting = new QSettings(QDir::homePath () +"/.QMokoPlayer/QMokoPlayer_options.ini",QSettings::IniFormat);
	
	 //Music Player parameters
	 setting->beginGroup("MusicPlayer");
	 checkBoxMPDisableSuspend->setChecked (setting->value("checkBoxMPDisableSuspend",false).toBool());
	 checkBoxMPDisableLightOff->setChecked (setting->value("checkBoxMPDisableLightOff",false).toBool());
	 setting->endGroup();
	 //Radio Player parameters
	 setting->beginGroup("RadioPlayer");
	 checkBoxRPDisableSuspend->setChecked (setting->value("checkBoxRPDisableSuspend",false).toBool());
	 checkBoxRPDisableLightOff->setChecked (setting->value("checkBoxRPDisableLightOff",false).toBool());
	 setting->endGroup();
	 //Video Player parameters
	 setting->beginGroup("VideoPlayer");
	 checkBoxVPDisableSuspend->setChecked (setting->value("checkBoxVPDisableSuspend",false).toBool());
	 checkBoxVPDisableLightOff->setChecked (setting->value("checkBoxVPDisableLightOff",false).toBool());
	 setting->endGroup();
	 //Youtube Player parameters
	 setting->beginGroup("YoutubePlayer");
	 checkBoxYPDisableSuspend->setChecked (setting->value("checkBoxYPDisableSuspend",false).toBool());
	 checkBoxYPDisableLightOff->setChecked (setting->value("checkBoxYPDisableLightOff",false).toBool());
	 setting->endGroup();
}


