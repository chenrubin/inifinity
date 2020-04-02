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
    virtual std::string GetName();
    virtual void SetName(std::string name_);
    virtual size_t GetNumOfLegs();
    virtual void SetNumOfLegs(size_t numOfLegs_);
    virtual std::ostream &operator>>(std::ostream &ostream_) const; 
    virtual std::istream &operator<<(std::istream &istream_);

private:
    size_t m_numOfLegs;
    std::string m_name;
};

Animal::Animal()
{}

Animal::~Animal()
{}

std::string Animal::GetName()
{
    return m_name;
}

void Animal::SetName(std::string name_)
{
    m_name = name_;
}

size_t Animal::GetNumOfLegs()
{
    return m_numOfLegs;
}

void Animal::SetNumOfLegs(size_t numOfLegs_)
{
    m_numOfLegs = numOfLegs_;
}

std::istream &Animal::operator<<(std::istream &istream_)
{
    std::string str;
    istream_ >> str;
    m_numOfLegs = std::atoi(str.c_str());

    istream_ >> str;
    SetName(str.c_str());

    return istream_;
}

std::ostream &Animal::operator>>(std::ostream &ostream_) const
{
    ostream_ << m_numOfLegs << "\n"; 
    ostream_ << m_name << "\n";

    return ostream_;
}

class Dog : public Animal
{
public:
   
private:
};

class Cat : public Animal
{
public:
   
private:
};

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