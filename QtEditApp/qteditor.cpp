#include "qteditor.h"

#include <QLabel>
#include <QApplication>
#include <QTextEdit>
#include <QMenuBar>
#include <QMenu>
#include <QKeySequence>
#include <QToolBar>
#include <QFontComboBox>
#include <QDoubleSpinBox>
#include <QStatusBar>
#include <QDockWidget>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QColorDialog>
#include <QFontDialog>
#include <QInputDialog>
#include <QPrinter>
#include <QPrintDialog>
#include <QFileInfo>

QtEditor::QtEditor(QWidget *parent)
    : QMainWindow(parent)
{
    mdiArea = new QMdiArea(this);
    setCentralWidget(mdiArea);


    QWidget *w = new QWidget(this);
    QLabel *label = new QLabel("Dock Widget", w);
    QDockWidget *dock = new QDockWidget("Dock Widget", this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, dock);
    dock->setWidget(w);

    fontComboBox = new QFontComboBox(this);
    connect(fontComboBox, SIGNAL(currentFontChanged(QFont)), SLOT(setTextFont(QFont)));
    sizeSpinBox = new QDoubleSpinBox(this);
    sizeSpinBox->setValue(13.0);
    connect(sizeSpinBox, SIGNAL(valueChanged(double)), SLOT(setTextSize(qreal)));

    //QTextEdit *textEdit = new QTextEdit(this);
    //setCentralWidget(textEdit);

    QMenuBar *menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    QMenu *fileMenu = menuBar->addMenu("&File");
    QMenu *editMenu = menuBar->addMenu("&Edit");
    QMenu *alignMenu = menuBar->addMenu("&Align");

    /*
    QAction *newAct = new QAction(QIcon("new.png"), "&New", this);
    newAct->setShortcut(tr("Ctrl+N"));
    newAct->setStatusTip(tr("make new file"));
    connect(newAct, SIGNAL(triggered()), SLOT(newFile()));
    */

    //file 액션 구현
    QAction *newAct = makeAction(":/images/new.png", tr("&New"), QKeySequence::New, tr("make new file"), this, SLOT(newFile()));
    QAction *openAct = makeAction(":/images/open.png", tr("&Open"), QKeySequence::Open, tr("open new file"), this, SLOT(openFile()));
    QAction *saveAct = makeAction(":/images/save.png", tr("&Save"), QKeySequence::Save, tr("save file"), this, SLOT(saveFile()));
    QAction *saveAsAct = makeAction(":/images/save_as.png", tr("&Save As..."), QKeySequence::SaveAs, tr("save as file"), this, SLOT(saveAsFile()));
    QAction *printAct = makeAction(":/images/print.png", tr("&Print"), QKeySequence::Print, tr("print file"), this, SLOT(printFile()));
    QAction *quitAct = makeAction(":/images/exit.png", tr("&Quit"), QKeySequence::Quit, tr("quit"), this, SLOT(quit()));

    //edit 액션 구현
    QAction *undoAct = editAction(":/images/edit_undo.png", tr("&Undo"), QKeySequence::Undo, tr("undo"), SLOT(undo()));
    QAction *redoAct = editAction(":/images/edit_redo.png", tr("&Redo"), QKeySequence::Redo, tr("redi"), SLOT(redo()));
    QAction *copyAct = editAction(":/images/copy.png", tr("&Copy"), QKeySequence::Copy, tr("copy"), SLOT(copy()));
    QAction *cutAct = editAction(":/images/cut.png", tr("&Cut"), QKeySequence::Cut, tr("cut"), SLOT(cut()));
    QAction *pasteAct = editAction(":/images/paste.png", tr("&Paste"), QKeySequence::Paste, tr("paste"), SLOT(paste()));
    QAction *zoomInAct = editAction(":/images/zoom_in.png", tr("&Zoom In"), QKeySequence::ZoomIn, tr("zoom in"), SLOT(zoomIn()));
    QAction *zoomOutAct = editAction(":/images/zoom_out.png", tr("&Zoom Out"), QKeySequence::ZoomOut, tr("zoom out"), SLOT(zoomOut()));
    QAction *cascadeAct = new QAction(tr("&Cascade"), this);
    connect(cascadeAct, &QAction::triggered, mdiArea, &QMdiArea::cascadeSubWindows);
    QAction *tileAct = new QAction(tr("&Tile"), this);
    connect(tileAct, &QAction::triggered, mdiArea, &QMdiArea::tileSubWindows);
    QAction *nextWindowAct = new QAction(tr("Next Window"), this);
    nextWindowAct->setShortcut(QKeySequence(tr("Ctrl+1")));
    connect(nextWindowAct, &QAction::triggered, mdiArea, &QMdiArea::activateNextSubWindow);
    QAction *prevWindowAct = new QAction(tr("Previous Window"), this);
    prevWindowAct->setShortcut(QKeySequence(tr("Ctrl+2")));
    connect(prevWindowAct, &QAction::triggered, mdiArea, &QMdiArea::activatePreviousSubWindow);


    //정렬 액션 구현
    QAction *alignCenterAct = new QAction("&Center", this);
    alignCenterAct->setIcon(QIcon(":/images/align-center.png"));
    QAction *alignLeftAct = new QAction("&Left", this);
    alignLeftAct->setIcon(QIcon(":/images/align-left.png"));
    QAction *alignRightAct = new QAction("&Right", this);
    alignRightAct->setIcon(QIcon(":/images/align-right.png"));
    QAction *alignJustifyAct = new QAction("&Justify", this);


    QAction *aboutAct = makeAction(":/images/about.png", tr("&About"), "Ctrl+Shift+A", tr("about"), qApp, SLOT(aboutQt()));
    QAction *colorAct = new QAction("&Color", this);
    connect(colorAct, SIGNAL(triggered()), this, SLOT(setColor()));
    QAction *fontAct = new QAction("&Font", this);
    connect(fontAct, SIGNAL(triggered()), this, SLOT(setFont()));

    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addAction(printAct);
    fileMenu->addAction(quitAct);

    editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(cutAct);
    editMenu->addAction(pasteAct);
    editMenu->addAction(zoomInAct);
    editMenu->addAction(zoomOutAct);
    editMenu->addAction(cascadeAct);
    editMenu->addAction(tileAct);
    editMenu->addAction(nextWindowAct);
    editMenu->addAction(prevWindowAct);

    alignMenu->addAction(alignLeftAct);
    alignMenu->addAction(alignCenterAct);
    alignMenu->addAction(alignRightAct);
    alignMenu->addAction(alignJustifyAct);

    QToolBar *fileToolBar = addToolBar("&File");

    fileToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    fileToolBar->addAction(newAct);
    fileToolBar->addSeparator();
    fileToolBar->addAction(openAct);
    fileToolBar->addSeparator();
    fileToolBar->addAction(saveAct);
    fileToolBar->addSeparator();
    fileToolBar->addAction(saveAsAct);
    fileToolBar->addSeparator();
    fileToolBar->addAction(quitAct);
    fileToolBar->addSeparator();
    fileToolBar->addAction(cascadeAct);
    fileToolBar->addSeparator();
    fileToolBar->addAction(tileAct);

    QMenu *helpMenu = menuBar->addMenu("&Help");
    helpMenu->addAction(aboutAct);

    QMenu *formatMenu = menuBar->addMenu("&Format");
    formatMenu->addAction(colorAct);
    formatMenu->addAction(fontAct);

    windowMenu = menuBar->addMenu("&Window");
    QMenu *toolbarMenu = windowMenu->addMenu("&Toolbar");
    toolbarMenu->addAction(fileToolBar->toggleViewAction());
    toolbarMenu->addAction(dock->toggleViewAction());



    addToolBarBreak();

    QToolBar *formatToolbar = addToolBar("&Format");

    formatToolbar->addSeparator();
    formatToolbar->addWidget(fontComboBox);
    formatToolbar->addWidget(sizeSpinBox);

    QStatusBar *statusbar = statusBar();
    QLabel *statusLabel = new QLabel(tr("Qt Editor"), statusbar);
    statusLabel->setObjectName("StatusLabel");
    statusbar->addPermanentWidget(statusLabel);
    statusbar->showMessage("started",1500);
#if 0
    QTextEdit *textEdit = new QTextEdit(this);
    mdiArea->addSubWindow(textEdit);
    mdiArea->cascadeSubWindows();
#else
    QTextEdit *textEdit = newFile();
#endif

    connect(alignCenterAct, &QAction::triggered, this, [=](){textEdit->setAlignment(Qt::AlignCenter);});
    connect(alignLeftAct, &QAction::triggered, this, [=](){textEdit->setAlignment(Qt::AlignLeft);});
    connect(alignRightAct, &QAction::triggered, this, [=](){textEdit->setAlignment(Qt::AlignRight);});
    connect(alignJustifyAct, &QAction::triggered, this, [=](){textEdit->setAlignment(Qt::AlignJustify);});
}

template <typename T, typename Functor>
QAction *QtEditor::makeAction(QString icon, QString text, T shortCut, QString toolTip, \
                              QObject* recv, Functor lambda)
{
    QAction *act = new QAction(text,this);
    if(icon.length()) act->setIcon(QIcon(icon));
    QKeySequence keySequence(shortCut);
    act->setShortcut(keySequence);
    act->setStatusTip(toolTip);
    connect(act, SIGNAL(triggered()), recv, lambda);
    return act;
}

template <typename T, typename Functor>
QAction *QtEditor::editAction(QString icon, QString text, T shortCut, QString toolTip, \
                                Functor lambda)
{
    QAction *act = new QAction(text,this);
    if(icon.length()) act->setIcon(QIcon(icon));
    QKeySequence keySequence(shortCut);
    act->setShortcut(keySequence);
    act->setStatusTip(toolTip);
    connect(act, SIGNAL(triggered()), lambda);
    return act;

}


void QtEditor::openFile() {
    qDebug("Open File");
    QString filename = QFileDialog::getOpenFileName(this, "Select file to open", QDir::home().dirName(), "Text File (*.txt *.html *.c *cpp *h)");

    if(!filename.length()) return;

    QFileInfo fileInfo(filename);
    if(fileInfo.isReadable()) {
        QFile file(filename);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QByteArray msg = file.readAll();
        file.close();
        QTextEdit *textedit = newFile();
        textedit->setWindowTitle(filename);
        windowHash.key(textedit)->setText(filename);

        if(fileInfo.suffix() == "htm" || fileInfo.suffix() == "html")
            textedit->setHtml(msg);
        else
            textedit->setPlainText(msg);
    }
    else
        QMessageBox::warning(this, "Error", "Can't Read this file", QMessageBox::Ok);
}

void QtEditor::saveFile( )
{
    qDebug("Save this file");
    QMdiSubWindow *window = mdiArea->currentSubWindow( );
    if(window != nullptr) {
        QTextEdit *textedit = qobject_cast<QTextEdit*>(window->widget( ));
        QString filename = textedit->windowTitle();
        windowHash.key(textedit)->setText(filename);

        if(!filename.length()){
            filename = QFileDialog::getSaveFileName(this, tr("Select file to save"), ".", tr("Text Files (*.txt *.html *.htm *.c *.cpp *.h *.hpp *.cs)"));
            if(!filename.length()) return;
            textedit->setWindowTitle(filename);
            windowHash.key(textedit)->setText(filename);
        }
        QFile file(filename);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QFileInfo fileInfo(filename);
        if(fileInfo.isWritable()){
            QByteArray msg = textedit->toHtml().toUtf8();
            file.write(msg);
        }
        else
            QMessageBox::warning(this, "Error", "Can't Save this file", QMessageBox::Ok);
        file.close();
    }
}
void QtEditor::saveAsFile() {
    qDebug("Save As File");
    QMdiSubWindow *window = mdiArea->currentSubWindow( );
    if(window != nullptr) {
        QString filename = QFileDialog::getSaveFileName(this, tr("Select file to save"), ".", tr("Text Files (*.txt *.html *.htm *.c *.cpp *.h *.hpp *.cs)"));
        QTextEdit *textedit = qobject_cast<QTextEdit*>(window->widget());

        if(!filename.length()) return;
        textedit->setWindowTitle(filename);

        QFile file(filename);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QFileInfo fileInfo(filename);
        if(fileInfo.isWritable()){
            QByteArray msg = textedit->toHtml().toUtf8();
            file.write(msg);
        }
        else
            QMessageBox::warning(this, "Error", "Can't Save this file", QMessageBox::Ok);
        file.close();
    }
}
void QtEditor::printFile() {
    QPrinter printer(QPrinter::HighResolution);
    printer.setFullPage(true);
    QPrintDialog printDialog(&printer, this);
    if(printDialog.exec() == QDialog::Accepted) {
        QTextEdit* textedit = (QTextEdit*)mdiArea->currentSubWindow()->widget();
        textedit->print(&printer);
    }
}

void QtEditor::quit()
{
    qDebug("Quit File");
}


QtEditor::~QtEditor() {}

QTextEdit *QtEditor::newFile()
{
    qDebug("Make New File");
    QAction *newFileAct = makeAction(":/images/about.png", tr("&New File"), "", tr("new file"), this, SLOT(selectWindow()));
    windowMenu->addAction(newFileAct);

    QTextEdit *textEdit = new QTextEdit;
    windowHash[newFileAct] = textEdit;
    mdiArea->addSubWindow(textEdit);
    textEdit->show();
    return textEdit;
}

void QtEditor::undo()
{
    QMdiSubWindow *subWindow = mdiArea->currentSubWindow();
    if(subWindow != nullptr){
        QTextEdit *textEdit = dynamic_cast<QTextEdit*>(subWindow->widget());
        textEdit->undo();
    }
}

void QtEditor::redo()
{
    QMdiSubWindow *subWindow = mdiArea->currentSubWindow();
    if(subWindow != nullptr){
        QTextEdit *textEdit = dynamic_cast<QTextEdit*>(subWindow->widget());
        textEdit->redo();
    }
}

void QtEditor::copy()
{
    QMdiSubWindow *subWindow = mdiArea->currentSubWindow();
    if(subWindow != nullptr){
        QTextEdit *textEdit = dynamic_cast<QTextEdit*>(subWindow->widget());
        textEdit->copy();
    }
}

void QtEditor::cut()
{
    QMdiSubWindow *subWindow = mdiArea->currentSubWindow();
    if(subWindow != nullptr){
        QTextEdit *textEdit = dynamic_cast<QTextEdit*>(subWindow->widget());
        textEdit->cut();
    }
}

void QtEditor::paste()
{
    QMdiSubWindow *subWindow = mdiArea->currentSubWindow();
    if(subWindow != nullptr){
        QTextEdit *textEdit = dynamic_cast<QTextEdit*>(subWindow->widget());
        textEdit->paste();
    }
}

void QtEditor::zoomIn()
{
    QMdiSubWindow *subWindow = mdiArea->currentSubWindow();
    if(subWindow != nullptr){
        QTextEdit *textEdit = dynamic_cast<QTextEdit*>(subWindow->widget());
        textEdit->zoomIn();
    }
}

void QtEditor::zoomOut()
{
    QMdiSubWindow *subWindow = mdiArea->currentSubWindow();
    if(subWindow != nullptr){
        QTextEdit *textEdit = dynamic_cast<QTextEdit*>(subWindow->widget());
        textEdit->zoomOut();
    }
}

void QtEditor::setTextFont(QFont font)
{
    QTextEdit *textedit = (QTextEdit*)mdiArea->currentSubWindow()->widget();
    textedit->setCurrentFont(font);
}

void QtEditor::setTextSize(qreal size)
{
    QTextEdit *textedit = (QTextEdit*)mdiArea->currentSubWindow()->widget();
    QFont font = textedit->currentFont();
    font.setPointSizeF(size);
    textedit->setCurrentFont(font);
}

void QtEditor::setFontWidget()
{
    QTextEdit *textedit = (QTextEdit*)mdiArea->currentSubWindow()->widget();
    QFont font = textedit->currentFont();
    fontComboBox->setCurrentFont(font);
    sizeSpinBox->setValue(font.pointSizeF());
}
void QtEditor::about()
{
    QMessageBox::question(this, "QMessageBox::showQuestion()", "Question Message", QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
}

void QtEditor::setColor()
{
    QTextEdit *textedit = (QTextEdit*)mdiArea->currentSubWindow()->widget();
    QColor color = QColorDialog::getColor(textedit->textColor(), this);
    if(color.isValid()) textedit->setTextColor(color);
}

void QtEditor::setFont()
{
    bool ok;
    QTextEdit *textedit = (QTextEdit*)mdiArea->currentSubWindow()->widget();
    QFont font = QFontDialog::getFont(&ok, textedit->currentFont(), this);
    if(ok) textedit->setCurrentFont(font);
}

void QtEditor::selectWindow()
{
    QTextEdit *textEdit = (QTextEdit*)windowHash[(QAction*)sender()];
    textEdit->setFocus();
}

