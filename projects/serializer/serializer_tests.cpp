#include <fstream> /* file open */
#include <cstring> /* strcmp */
#include <string> /* stoi */

#include "serializer.hpp"
#include "MyUtils.hpp"          /* test results */

using namespace ilrd;

#define NAX_INT_CHARACTERS (10)
#define MAX_NAME_CHARACTERS (10)

 // names are no more than 10 chrachters
class Animal
{
public:
    Animal(); 
    virtual ~Animal();
    virtual std::string GetName() = 0;
    virtual void SetName(std::string name_) = 0;
    virtual size_t GetNumOfLegs() = 0;
    virtual void SetNumOfLegs(size_t numOfLegs_) = 0;
    virtual void Output(std::ostream &ostream_) const = 0; 
    virtual void Input(std::istream &istream_) = 0;

    friend std::ostream &operator<<(std::ostream &ostream_, const Animal &this_);
    friend std::istream &operator>>(std::istream &istream_, Animal &this_);

private:
};

Animal::Animal()
{}

Animal::~Animal()
{}

std::ostream &operator<<(std::ostream &ostream_, const Animal &this_)
{
    this_.Output(ostream_);

    return ostream_;
}

std::istream &operator>>(std::istream &istream_, Animal &this_)
{
    this_.Input(istream_);

    return istream_;
}

class Dog : public Animal
{
public:
    virtual std::string GetName();
    virtual void SetName(std::string name_);
    virtual size_t GetNumOfLegs();
    virtual void SetNumOfLegs(size_t numOfLegs_);

    virtual void Output(std::ostream &ostream_) const; 
    virtual void Input(std::istream &istream_);
   
private:
    size_t m_numOfLegs;
    std::string m_name;
};

void Dog::Input(std::istream &istream_)
{
    std::string str;
    istream_ >> str;
    m_numOfLegs = std::atoi(str.c_str());

    istream_ >> str;
    SetName(str.c_str());
}

void Dog::Output(std::ostream &ostream_) const
{
    ostream_ << m_numOfLegs << "\n"; 
    ostream_ << m_name << "\n";
}

std::string Dog::GetName()
{
    return m_name;
}

void Dog::SetName(std::string name_)
{
    m_name = name_;
}

size_t Dog::GetNumOfLegs()
{
    return m_numOfLegs;
}

void Dog::SetNumOfLegs(size_t numOfLegs_)
{
    m_numOfLegs = numOfLegs_;
}

class Cat : public Animal
{
public:
    virtual std::string GetName();
    virtual void SetName(std::string name_);
    virtual size_t GetNumOfLegs();
    virtual void SetNumOfLegs(size_t numOfLegs_);

    virtual void Output(std::ostream &ostream_) const; 
    virtual void Input(std::istream &istream_);
   
private:
    size_t m_numOfLegs;
    std::string m_name;
};

void Cat::Input(std::istream &istream_)
{
    std::string str;
    istream_ >> str;
    m_numOfLegs = std::atoi(str.c_str());

    istream_ >> str;
    SetName(str.c_str());
}

void Cat::Output(std::ostream &ostream_) const
{
    ostream_ << m_numOfLegs << "\n"; 
    ostream_ << m_name << "\n";
}

std::string Cat::GetName()
{
    return m_name;
}

void Cat::SetName(std::string name_)
{
    m_name = name_;
}

size_t Cat::GetNumOfLegs()
{
    return m_numOfLegs;
}

void Cat::SetNumOfLegs(size_t numOfLegs_)
{
    m_numOfLegs = numOfLegs_;
}

void TestAddSerDes();

int main()
{
    TestAddSerDes();

    return 0;
}

void TestAddSerDes()
{
    Dog Sasha;
    Dog Shaddad;
    Sasha.SetName("Sasha");
    Sasha.SetNumOfLegs(10);
    Shaddad.SetName("Shaddad");
    Shaddad.SetNumOfLegs(22);
    Cat Ran;
    Ran.SetName("Ran");
    Ran.SetNumOfLegs(1);

    std::ofstream o_data_file;
    std::ifstream i_data_file;
    o_data_file.open("serializer.txt", std::ios::out | std::ios::trunc);
    
    Serializer<Animal> ser;
    ser.AddClass<Dog>();
    ser.AddClass<Cat>();

    ser.Serialize(Sasha,o_data_file);
    ser.Serialize(Shaddad,o_data_file);
    ser.Serialize(Ran,o_data_file);

    o_data_file.close();
    i_data_file.open("serializer.txt", std::ios::in | std::ios::out);

    boost::shared_ptr<Animal> SashaDes = ser.Deserialize(i_data_file);
    PRINTTESTRESULTS("TestAddSerDes",1, 0 == strcmp(SashaDes->GetName().c_str(), "Sasha"));
    PRINTTESTRESULTS("TestAddSerDes",1, 10 == SashaDes->GetNumOfLegs());

    boost::shared_ptr<Animal> ShaddadDes = ser.Deserialize(i_data_file);
    PRINTTESTRESULTS("TestAddSerDes",3, 0 == strcmp(ShaddadDes->GetName().c_str(), "Shaddad"));
    PRINTTESTRESULTS("TestAddSerDes",4, 22 == ShaddadDes->GetNumOfLegs());

    boost::shared_ptr<Animal> RanDes = ser.Deserialize(i_data_file);
    PRINTTESTRESULTS("TestAddSerDes",5, 0 == strcmp(RanDes->GetName().c_str(), "Ran"));
    PRINTTESTRESULTS("TestAddSerDes",6, 1 == RanDes->GetNumOfLegs());
}