#include <QApplication>
#include <QPushButton>
#include <QLabel>

#include "Calcwidget.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent), m_isFirst(true)
{
    m_label = new QLabel("0", this);
    m_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    m_label->setGeometry(10,5,230,40);
    const char ButtonChar[16][2] = {
        "7","8","9","/",
        "4","5","6","x",
        "1","2","3","-",
        "0","C","=","+",
    };
    for(int y=0; y<WIDTH; y++){
        for(int x=0; x<WIDTH; x++){
            m_buttons.append(new QPushButton(ButtonChar[x+y*WIDTH], this));  //버튼 선언 및 초기화
            m_buttons.at(x+y*WIDTH)->setGeometry(5+60*x, 50+60*y, 60, 60);           //버튼 배치
        }
    }
    //숫자를 위한 버튼들의 시그널/슬롯 연결
    connect(m_buttons.at(0), SIGNAL(clicked()), SLOT(setNum()));      //9
    connect(m_buttons.at(1), SIGNAL(clicked()), SLOT(setNum()));      //8
    connect(m_buttons.at(2), SIGNAL(clicked()), SLOT(setNum()));      //7
    connect(m_buttons.at(4), SIGNAL(clicked()), SLOT(setNum()));      //6
    connect(m_buttons.at(5), SIGNAL(clicked()), SLOT(setNum()));      //5
    connect(m_buttons.at(6), SIGNAL(clicked()), SLOT(setNum()));      //4
    connect(m_buttons.at(8), SIGNAL(clicked()), SLOT(setNum()));      //3
    connect(m_buttons.at(9), SIGNAL(clicked()), SLOT(setNum()));      //2
    connect(m_buttons.at(10), SIGNAL(clicked()), SLOT(setNum()));     //1
    connect(m_buttons.at(12), SIGNAL(clicked()), SLOT(setNum()));     //0

    //clear 버튼
    connect(m_buttons.at(13), &QPushButton::clicked, this, [=](){ m_label->setText("0"); });     //Clear

    //연산자를 위한 버튼
    connect(m_buttons.at(3), SIGNAL(clicked()), SLOT(setOp()));
    connect(m_buttons.at(7), SIGNAL(clicked()), SLOT(setOp()));
    connect(m_buttons.at(11), SIGNAL(clicked()), SLOT(setOp()));
    connect(m_buttons.at(15), SIGNAL(clicked()), SLOT(setOp()));

    //계산 버튼 및 동작 구현
    connect(m_buttons.at(14), &QPushButton::clicked, this, [this](){
        qreal result = 0;
        m_isFirst = true;
        if(m_op == "+")
            result = m_num1.toDouble() + m_label->text().toDouble();    //덧셈 작업 및 문자열을 toDouble
        else if(m_op == "-")
            result = m_num1.toDouble() - m_label->text().toDouble();
        else if(m_op == "x")
            result = m_num1.toDouble() * m_label->text().toDouble();
        else if(m_op == "/"){
            if(m_label->text().toDouble()){
            result = m_num1.toDouble() / m_label->text().toDouble();
            }
            else{
                m_label->setText("Can't divide by zero.");
                return;
            }
        }
        m_label->setText(QString::number(result));
    });

    setMinimumSize(250,295);
    setMaximumSize(250,295);
    setWindowTitle("Caculator");
}

void Widget::setNum()
{
    QPushButton *button = dynamic_cast<QPushButton*>(sender());     //형 변환
    QString bStr;
    if(button != nullptr) bStr = button->text();
    if(m_label != nullptr){
        QString IStr = m_label->text();
        //레이블의 문자가 0이면 버튼의 문자를 레이블에 그대로 설정
        //0이 아니면 레이블의 문자열 + 버튼의 문자열을 레이블에 설정
        m_label->setText((IStr == "0"|m_isFirst)?bStr:IStr+bStr);
        m_isFirst = false;
    }
}

void Widget::setOp()
{
    QPushButton *button = dynamic_cast<QPushButton*>(sender());
    if(button != nullptr) m_op = button->text();    //버튼의 문자열이 연산자가 됨
    if(m_label != nullptr){
        m_num1 = m_label->text();
        m_isFirst = true;
    }
}

Widget::~Widget() {
    delete m_label;
    m_buttons.clear();
}
