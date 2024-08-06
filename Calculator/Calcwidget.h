#ifndef CALCWIDGET_H
#define CALCWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVector>

class QLabel;
class QPushButton;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    QLabel *m_label;
    QVector<QPushButton*> m_buttons;
    QString m_num1, m_op;
    bool m_isFirst;
    const qint32 WIDTH=4;

signals:
    void clicked();

public slots:
    void setNum();
    void setOp();
};
#endif // CALCWIDGET_H
