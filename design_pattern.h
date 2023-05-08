//
// Created by chenzhi on 2023/4/17.
//

#ifndef IKITS_DESIGN_PATTERN_H
#define IKITS_DESIGN_PATTERN_H

#include <iostream>
#include <string>
#include <mutex>
#include <assert.h>
#include <list>

using namespace std;

mutex mt;


//单例模式
class SingleTon{

public:

    static SingleTon *getInstance(){
        //双重判定
        if(m_instance == nullptr){

            mt.lock();
            if(m_instance == nullptr){

                m_instance = new SingleTon;
            }
            mt.unlock();
            return m_instance;
        }
        return m_instance;
    }

    ~SingleTon(){

        cout << "~SingleTon() called!" << endl;
    }

    void toDoSometing(){

        if(m_instance == nullptr){

            assert(!"呵呵哒");
        }

        cout << this <<"  雅喵蝶 是一只大蝴蝶" << endl;
    }

private:
    SingleTon(){}
    SingleTon(SingleTon &singleTon){}
    static SingleTon *m_instance;
};

//静态变量显示声明初始化
SingleTon *SingleTon::m_instance = nullptr;


//观察者模式
class Observer;

class Subject{

public:
    void attach(Observer *observer) {
        observers.push_back(observer);

    }
    void detach(Observer *observer) {
        observers.remove(observer);
    }
    void notify() {
        list<Observer*>::iterator it = observers.begin();
        while(it != observers.end()){

            (*it)->update();
            it++;
        }

    }
    ~Subject() {
        cout << "开始析构了" << endl;

        list<Observer*>::iterator it = observers.begin();
        while(it != observers.end()){

            cout << "开始删除: " << (*it)->getName() << endl;
            delete *it;
            it++;
        }
        observers.clear();
    }

private:
    //被观察对象对应的观察者
    list<Observer*> observers;
};

class Observer{

public:
    virtual void update(){}
    virtual string getName(){ return ""; }

};

class ConcreteSubject : public Subject{

public:
    string getSubjectState() {
        return subjectState;
    }
    void setSubjectState(const string &str) {
        subjectState = str;
    }


private:
    string subjectState;
};

class ConcreteObserver : public Observer{

public:
    ConcreteObserver(ConcreteSubject *subject, string name) {
        this->subject = subject;
        this->name = name;

    }
    void update() {
        observerState = subject->getSubjectState();
        cout << "发布者更新东西了 ！！！ 订阅者：" << name << "  状态：" << observerState << endl;
    }
    string getName();

private:
    string name;
    string observerState;
    ConcreteSubject *subject;
};

void test_observer() {
    ConcreteSubject *subject = new ConcreteSubject;
    //被观察者持有订阅者
    //被观察者有触发动作时直接notify订阅者执行操作
    subject->attach(new ConcreteObserver(subject, "第一个粉丝"));
    subject->attach(new ConcreteObserver(subject, "第二个粉丝"));
    subject->attach(new ConcreteObserver(subject, "第三个粉丝"));

    subject->setSubjectState("Hello");
    subject->notify();

    cout << "----------- 华 丽 的 分 割 线 -----------" << endl;
    subject->attach(new ConcreteObserver(subject, "王二麻子"));
    subject->setSubjectState("呵呵");
    subject->notify();

    cout << endl;
    delete subject;

}


//状态模式
class Work;

//定义一个抽象方法，写程序
class State{

public:
    virtual void writeProgram(Work *w){}
    virtual ~State(){}
};

//上午和中午工作状态
class ForenoonState : public State{

public:
    void writeProgram(Work *w) {
        if(w->getHour() < 12){

            cout << "当前时间：" << w->getHour() << "点 上午工作，精神百倍！" << endl;
        }
        else{

            w->setState(new NoonState());
            w->writeProgram();
        }

    }
    ~ForenoonState() {
        cout << "ForenoonState::~ForenoonState() desconstruction !" << endl;

    }
};

//中午工作状态
class NoonState : public State{

public:
    void writeProgram(Work *w);
    ~NoonState();
};

//下午工作
class AfternoonState : public State{

public:
    void writeProgram(Work *w) {
        if(w->getHour() < 13){

            cout << "当前时间：" << w->getHour() << "点 饿了，午饭；犯困，午休。" << endl;
        }
        else{

            w->setState(new AfternoonState());
            w->writeProgram();
        }

    }
    ~AfternoonState() {
        cout << "NoonState::~NoonState() desconstruction !" << endl;

    }
};

//晚间工作状态
class EveningState : public State{

public:
    void writeProgram(Work *w);
    ~EveningState();
};

//下班状态
class RestState: public State{

public:
    void writeProgram(Work *w);
    ~RestState();
};

//休眠状态
class SleepingState : public State{

public:
    void writeProgram(Work *w);
    ~SleepingState();
};

//工作类
class Work{

public:
    Work() {
        this->current = new ForenoonState();
        this->finish = false;
        this->hour = 6.0;

    }
    ~Work() {

        cout << "Work::~Work() desconstruction !" << endl;

    }

    void setHour(const double &h) {

        this->hour = h;

    }
    double getHour() {
        return hour;
    }

    void setTaskFinished(const bool &f) {
        finish = f;
    }
    bool getTaskFinished() {
        return finish;
    }

    void setState(State *s) {
        if(this->current != nullptr){

            delete current;
            //current = nullptr;	//这里有点 脱裤子 放屁
        }
        this->current = s;
    }
    void writeProgram() {
        this->current->writeProgram(this);

    }

private:
    State *current;
    double hour;
    bool finish;
};


//责任链模式
class Request {
public:
    Request(const std::string& type) : m_type(type) {}

    std::string getType() const {
        return m_type;
    }

private:
    std::string m_type;
};

class Handler {
public:
    Handler(const std::string& name) : m_name(name), m_successor(nullptr) {}

    void setSuccessor(Handler* successor) {
        m_successor = successor;
    }

    virtual void handleRequest(const Request& request) {
        if (m_successor) {
            m_successor->handleRequest(request);
        }
    }

protected:
    std::string m_name;
    Handler* m_successor;
};

class ConcreteHandlerA : public Handler {
public:
    ConcreteHandlerA() : Handler("ConcreteHandlerA") {}

    void handleRequest(const Request& request) override {
        if (request.getType() == "TypeA") {
            std::cout << m_name << " handled the request." << std::endl;
        } else {
            Handler::handleRequest(request);
        }
    }
};

class ConcreteHandlerB : public Handler {
public:
    ConcreteHandlerB() : Handler("ConcreteHandlerB") {}

    void handleRequest(const Request& request) override {
        if (request.getType() == "TypeB") {
            std::cout << m_name << " handled the request." << std::endl;
        } else {
            Handler::handleRequest(request);
        }
    }
};

class ConcreteHandlerC : public Handler {
public:
    ConcreteHandlerC() : Handler("ConcreteHandlerC") {}

    void handleRequest(const Request& request) override {
        if (request.getType() == "TypeC") {
            std::cout << m_name << " handled the request." << std::endl;
        } else {
            Handler::handleRequest(request);
        }
    }
};

int test_chain_of_response() {
    // 创建责任链
    Handler* handlerA = new ConcreteHandlerA();
    Handler* handlerB = new ConcreteHandlerB();
    Handler* handlerC = new ConcreteHandlerC();

    handlerA->setSuccessor(handlerB);
    handlerB->setSuccessor(handlerC);

    // 发送请求
    Request requestA("TypeA");
    Request requestB("TypeB");
    Request requestC("TypeC");
    Request requestD("TypeD");

    handlerA->handleRequest(requestA);
    handlerA->handleRequest(requestB);
    handlerA->handleRequest(requestC);
    handlerA->handleRequest(requestD);

    // 释放资源
    delete handlerA;
    delete handlerB;
    delete handlerC;

    return 0;
}


#endif //IKITS_DESIGN_PATTERN_H

