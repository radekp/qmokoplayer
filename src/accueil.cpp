

#include "accueil.h"
#include <qpushbutton.h>
#include <qsoftmenubar.h>
#include <QAction>
#include <QMenu>
#include "radioplayer.h"
#include <QDir>
#include <QMessageBox>
#include <QProcess>

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
    connect(VideoButton,SIGNAL(clicked()), this, SLOT(VideoButton_clicked()));
    menu->addAction(actionClose);
    QSoftMenuBar::setHelpEnabled(this,false);
    
    //Test and create the .QmokoPlayer in the current home user if necessary
    QDir Directory(QDir::homePath () +"/.QMokoPlayer");
    if (!Directory.exists())
    	Directory.mkdir(QDir::homePath () +"/.QMokoPlayer");
    
    //verify if mplayer is already installed
	if (!QFile::exists("/usr/bin/mplayer"))
	{

		 if(QMessageBox::question(this, tr("qmplayer"),
                             tr("Program MPlayer must be downloaded. Please make sure you have internet connection and press yes to confirm download"),
                             QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
       {
           if(installMplayer())
           {
               QMessageBox::information(this, tr("qmplayer"), tr("MPlayer installed sucessfully"));
           }
           else
           	QMessageBox::warning(this, tr("qmplayer"), tr("Failed to install MPlayer"));
       }
			
	}

}


//this fonction comes from qmplayer, add here what you want to add in the configuration of mplayer
bool Accueil::installMplayer()
{
    QDir::home().mkdir(".mplayer");
    QFile f(QDir::homePath ()+"/.mplayer/config");
    f.open(QFile::WriteOnly);
    f.write("vo=glamo\n\n[default]\nafm=ffmpeg\nvfm=ffmpeg\n");
    f.close();

    return download("http://72.249.85.183/radekp/qmplayer/download/mplayer",
                    "/usr/bin/mplayer", "mplayer", false) &&
    QFile::setPermissions("/usr/bin/mplayer", QFile::ReadOwner |
                          QFile::WriteOwner | QFile::ExeOwner |
                          QFile::ReadUser | QFile::ExeUser |
                          QFile::ReadGroup | QFile::ExeGroup |
                          QFile::ReadOther | QFile::ExeOther);
}


bool Accueil::download(QString url, QString destPath, QString filename, bool justCheck)
{
    QString host = url;
    QString reqPath;
    int port = 80;

    if(url.startsWith("http://"))
    {
        host.remove(0, 7);
    }

    int colonIndex = host.indexOf(':');
    int slashIndex = host.indexOf('/');
    if(slashIndex < 0)
    {
        return false;
    }
    reqPath = host.right(host.length() - slashIndex).replace(" ", "%20");
    host = host.left(slashIndex);
    if(colonIndex > 0)
    {
        QString portStr = host.right(host.length() - colonIndex - 1);
        host = host.left(colonIndex);
        port = portStr.toInt(0, 10);
    }

connect:
    QTcpSocket sock(this);
    sock.setReadBufferSize(65535);
    sock.connectToHost(host, port);
    if(!sock.waitForConnected(5000))
    {
        QMessageBox::critical(this, tr("qmplayer"), sock.errorString());
        return false;
    }

    QByteArray req("GET ");
    req.append(reqPath);
    req.append(" HTTP/1.1\r\nHost: ");
    req.append(host);
    req.append(':');
    req.append(QByteArray::number(port));
    req.append("\r\n\r\n");

    sock.write(req);
    sock.flush();
    sock.waitForBytesWritten();

    int contentLen = 0;
    bool html = false;
    QByteArray line;
    for(;;)
    {
        line = sock.readLine();
        if(line.isEmpty())
        {
            if(sock.waitForReadyRead(5000))
            {
                continue;
            }
            break;
        }
        if(line.trimmed().isEmpty())
        {
            break;
        }
        html = html | (line.indexOf("Content-Type: text/html") == 0);
        if(line.indexOf("Content-Length: ") == 0)
        {
            contentLen = line.remove(0, 16).trimmed().toInt(0, 10);
        }
    }

    if(html)
    {
        QByteArray text = sock.readAll();
        sock.close();
        if(text.length() == 0)
        {
            QMessageBox::critical(this, tr("qmplayer"),
                                  tr("No response from ") + host);
            return false;
        }
        text.replace("</br>", "\n");
        if(QMessageBox::information(this, "qmplayer", text,
                                 QMessageBox::Ok | QMessageBox::Retry) == QMessageBox::Retry)
        {
            goto connect;
        }

        return false;
    }
    else if(justCheck)
    {
        sock.close();
        return true;
    }

    QFile f(destPath);
    if(!f.open(QFile::WriteOnly))
    {
        QMessageBox::critical(this, tr("qmplayer"),
                              tr("Unable to save file:\r\n\r\n") + f.errorString());
        sock.close();
        return false;
    }

    
#ifdef QTOPIA
     QtopiaApplication::setPowerConstraint(QtopiaApplication::DisableSuspend);
#endif

    if(contentLen <= 0)
    {
        contentLen = 1024 * 1024;
    }
    int remains = contentLen;

    char buf[65535];
    int count;
    bool abort = false;
    for(;;)
    {
        QApplication::processEvents();
        if(abort)
        {
            break;
        }
        if(sock.bytesAvailable() < 65535
           && sock.state() == QAbstractSocket::ConnectedState)
        {
            sock.waitForReadyRead(1000);
            continue;
        }
        count = sock.read(buf, 65535);
        if(count <= 0)
        {
            break;
        }
        f.write(buf, count);
        f.flush();
        remains -= count;
        if(remains <= 0)
        {
            remains = contentLen;
        }
    }
    f.close();
    sock.close();

#ifdef QTOPIA
    QtopiaApplication::setPowerConstraint(QtopiaApplication::Enable);
#endif

    return true;
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
void Accueil::VideoButton_clicked()
{
    m_v = new QMplayer();
    m_v->show();
}

void Accueil::options()
{
	m_o = new Options();
    m_o->show();
}
