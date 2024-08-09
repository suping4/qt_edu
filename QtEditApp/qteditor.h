#ifndef QTEDITOR_H
#define QTEDITOR_H

#include <QMainWindow>


class QAction;
class QMdiArea;
class QTextEdit;
class QFontComboBox;
class QDoubleSpinBox;

class QtEditor : public QMainWindow
{
    Q_OBJECT
private:
    QMdiArea *mdiArea;
    QFontComboBox *fontComboBox;
    QDoubleSpinBox *sizeSpinBox;
    QMenu *windowMenu;
    QHash<QAction*, QWidget*> windowHash;

    template<typename T, typename Functor>
    QAction *makeAction(QString icon, QString text, T shortCut, QString toolTip, QObject *recv, Functor lambda);


    template<typename T, typename Functor>
    QAction *editAction(QString icon, QString text, T shortCut, QString toolTip, Functor lambda);
public:
    QtEditor(QWidget *parent = nullptr);
    ~QtEditor();

public slots:
    QTextEdit *newFile();
    void undo();
    void redo();
    void copy();
    void cut();
    void paste();
    void zoomIn();
    void zoomOut();
    void openFile();
    void saveFile();
    void saveAsFile();
    void printFile();
    void quit();
    void setTextFont(QFont font);
    void setTextSize(qreal size);
    void setFontWidget();
    void about();
    void setColor();
    void setFont();
    void selectWindow();
};
#endif // QTEDITOR_H
