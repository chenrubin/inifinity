#include <unistd.h>          // sleep
#include "dispatcher.hpp"

using namespace ilrd;

void ObsPrintTemprature(const double& message);
void OBsGoodBye();

/******************************************************************************
 *  Observer1
******************************************************************************/
class Observer_ac
{
public:
    Observer_ac();
    ~Observer_ac();
    
    ObserverCallback<double, Observer_ac>* GetCallback();

private:
    ObserverCallback<double, Observer_ac> m_obsCallback;

    void ObsPrintTemprature1(const double& temprature);
    void ObsGoodBye1();
};

Observer_ac::Observer_ac()
    : m_obsCallback(this, &Observer_ac::ObsPrintTemprature1, &Observer_ac::ObsGoodBye1)
{
    std::cout << "observer Ctor\n";
}

Observer_ac::~Observer_ac()
{
    std::cout << "observer Dtor\n";
}

ObserverCallback<double, Observer_ac>* Observer_ac::GetCallback()
{
    return &m_obsCallback;
}

void Observer_ac::ObsPrintTemprature1(const double& temprature)
{
    std::cout << "Current temprature = " << temprature << std::endl;
}

void Observer_ac::ObsGoodBye1()
{
    std::cout << "!!!!!!    Shalom velo lehitraot       !!!!!!!!\n";
}
/******************************************************************************
 *  Observer2
******************************************************************************/
class Observer_bc
{
public:
    Observer_bc();
    ~Observer_bc();
    
    ObserverCallback<double, Observer_bc>* GetCallback();

private:
    ObserverCallback<double, Observer_bc> m_obsCallback;

    void ObsPrintTemprature2(const double& temprature);
    void ObsGoodBye2();
};

Observer_bc::Observer_bc()
    : m_obsCallback(this, &Observer_bc::ObsPrintTemprature2, &Observer_bc::ObsGoodBye2)
{}

Observer_bc::~Observer_bc()
{}

ObserverCallback<double, Observer_bc>* Observer_bc::GetCallback()
{
    return &m_obsCallback;
}

void Observer_bc::ObsPrintTemprature2(const double& temprature)
{
    std::cout << "bc --> Current temprature = " << temprature << std::endl;
}

void Observer_bc::ObsGoodBye2()
{
    std::cout << "!!!!!!  bc -->  Shalom velo lehitraot       !!!!!!!!\n";
}
/******************************************************************************
 *  Subject
******************************************************************************/
class Subject_therm
{
public:    
    Subject_therm();
    ~Subject_therm();

    void SubRegister(BaseCallback<double>* baseCallback_);
    void NotifyObserver();

private:
    Dispatcher<double> m_dispatcher;
};

Subject_therm::Subject_therm()
    : m_dispatcher()
{
    std::cout << "Subject Ctor\n";
}

Subject_therm::~Subject_therm()
{
    std::cout << "Subject Dtor\n";
}

void Subject_therm::SubRegister(BaseCallback<double>* baseCallback_)
{
    m_dispatcher.Register(baseCallback_);
}

void Subject_therm::NotifyObserver()
{
    double temprature_array[] = {1,5,-2.6,3,-8.5,10,2,25,215,-193};

    for (size_t i = 0; i < 10; ++i)
    {
        sleep(1);
        m_dispatcher.Notify(temprature_array[i]);
    }
}
/******************************************************************************
 *  Main test
******************************************************************************/
void TestBasicObsSub1();
void TestBasicObsSub2();

int main()
{
    /*  In both tests verify both observers print their messages regarding the 
        changed tempratures and that there is no valgrind */
    TestBasicObsSub1();
    TestBasicObsSub2();

    return 0;
}

void TestBasicObsSub1()
{
    std::cout << "\n!!!    TestBasicObsSub1    !!!\n";
    Subject_therm tempra;
    Observer_ac obs1;
    Observer_bc obs2;
    tempra.SubRegister(obs1.GetCallback());
    tempra.SubRegister(obs2.GetCallback());
    
    tempra.NotifyObserver();
}

void TestBasicObsSub2()
{
    /* here need also to see shalom velo lehitraot message  */
    std::cout << "\n!!!    TestBasicObsSub2    !!!\n";
    Observer_ac obs1;
    Observer_bc obs2;
    Subject_therm tempra;
    tempra.SubRegister(obs1.GetCallback());
    tempra.SubRegister(obs2.GetCallback());
    
    tempra.NotifyObserver();
}