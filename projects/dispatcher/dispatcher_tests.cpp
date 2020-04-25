#include "dispatcher.hpp"

using namespace ilrd;

void ObsPrintTemprature(const std::string& message);
void OBsGoodBye();

/******************************************************************************
 *  Observer
******************************************************************************/
class Observer_ac
{
public:
    Observer_ac();
    ~Observer_ac();

private:
    ObserverCallback<std::string, Observer_ac> m_obsCallback;

    void ObsPrintTemprature1(const std::string& message);
    void ObsGoodBye1();
};

Observer_ac::Observer_ac()
    : m_obsCallback(this, &Observer_ac::ObsPrintTemprature1, &Observer_ac::ObsGoodBye1)
{}

Observer_ac::~Observer_ac()
{}

void Observer_ac::ObsPrintTemprature1(const std::string& message)
{
    std::cout << "First AC Current temprature = " << message << std::endl;
}

void Observer_ac::ObsGoodBye1()
{
    std::cout << "!!!!!! First Ac    Shalom velo lehitraot       !!!!!!!!\n";
}

/******************************************************************************
 *  Subject
******************************************************************************/
class Subject_therm
{
public:    
    Subject_therm();
    ~Subject_therm();

    void SubRegister(BaseCallback<std::string>* baseCallback_);

private:
    Dispatcher<std::string> m_dispatcher;
};

Subject_therm::Subject_therm()
    : m_dispatcher()
{}

Subject_therm::~Subject_therm()
{}

void Subject_therm::SubRegister(BaseCallback<std::string>* baseCallback_)
{
    m_dispatcher.Register(baseCallback_);
}

int main()
{
    return 0;
}
/*
void ObsPrintTemprature2(const std::string& message)
{
    std::cout << "Second Ac Current temprature = " << message << std::endl;
}

void ObsGoodBye2()
{
    std::cout << "!!!!!! Second Ac    Shalom velo lehitraot       !!!!!!!!\n";
}
*/