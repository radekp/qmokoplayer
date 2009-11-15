

#include "accueil.h"
#include <qpushbutton.h>
#include <qsoftmenubar.h>
#include <QAction>
#include <QMenu>
#include "radioplayer.h"
#include <QDir>


/*
 *  Constructs a Example which is a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'
 */
Accueil::Accueil(QWidget *parent, Qt::WFlags f)
    : QWidget(parent, f)
{
    setupUi(this); // reads the .ui file to determine widgets and layout
    // Construct context menu, available to the user via Qtopia's soft menu bar.
    QMenu *menu = QSoftMenuBar::menuFor(this);
	menu->addAction(tr("Options"), this, SLOT(options()));
    QAction *actionClose = new QAction(QIcon("close"),tr("Close"),this);
    connect(actionClose,SIGNAL(triggered()), this, SLOT(close()));
    connect(RadioButton,SIGNAL(clicked()), this, SLOT(RadioButton_clicked()));
    menu->addAction(actionClose);
    QSoftMenuBar::setHelpEnabled(this,false);
    
    //Test and create the .QmokoPlayer in the current home user if necessary
    QDir Directory(QDir::homePath () +"/.QMokoPlayer");
    if (!Directory.exists())
    	Directory.mkdir(QDir::homePath () +"/.QMokoPlayer");
    
}

/*
 *  Destroys the object and frees any allocated resources
 */
Accueil::~Accueil()
{
    // no need to delete child widgets, Qt does it all for us
}


/*
 *  Every time the RadioButton button is clicked, this slot will be invoked
 *  (see QMetaObject::connectSlotsByName()).
 */
void Accueil::RadioButton_clicked()
{
    m_r = new RadioPlayer();
    m_r->show();

}

void Accueil::options()
{
	m_o = new Options();
    m_o->show();
}
