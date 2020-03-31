#include <boost/shared_ptr.hpp> /* shared_ptr */

#include "factory.hpp"
#include "MyUtils.hpp"

using namespace ilrd;
using namespace factory;

class Animal
{
public:
    Animal(); 
    virtual ~Animal();
    size_t GetNumOfLegs();
    virtual bool GetTailStatus();

private:
    size_t m_numOfLegs;
};

Animal::Animal()
{}

Animal::~Animal()
{}

bool Animal::GetTailStatus()
{
    return 0;
}

class Dog : public Animal
{
public:
    Dog(size_t numOflegs_, bool tailStatus);
    virtual ~Dog();
    void GetNumOfLegs();
    virtual bool GetTailStatus();

private:
    size_t m_numOfLegs;
    bool m_IsWavingTail;
};

Dog::Dog(size_t numOflegs_, bool tailStatus_)
    : m_numOfLegs(numOflegs_)
    , m_IsWavingTail(tailStatus_)
{}

Dog::~Dog()
{}

bool Dog::GetTailStatus()
{
    return m_IsWavingTail;
}

struct DogStruct
{
    size_t m_numOfLegs;
    bool m_IsWavingTail;
};

boost::shared_ptr<Animal> AddDog(DogStruct *ds);
boost::shared_ptr<Animal> AddDogExtra(DogStruct *ds);

void TestCreateAndAdd();

int main()
{
    TestCreateAndAdd();

    return 0;
}

void TestCreateAndAdd()
{
    bool status = false;

    DogStruct ds_happy;
    ds_happy.m_IsWavingTail = true;
    ds_happy.m_numOfLegs = 4;

    DogStruct ds_sad;
    ds_sad.m_IsWavingTail = false;
    ds_sad.m_numOfLegs = 4;

    Factory<Animal, std::string, DogStruct *> fact;
    status = fact.AddClass("HappyDog", AddDog);
    PRINTTESTRESULTS("TestCreateAndAdd",1, true == status);
    status = fact.AddClass("SadDog", AddDog);
    PRINTTESTRESULTS("TestCreateAndAdd",2, true == status);

    boost::shared_ptr<Animal> animal_ptr1 = fact.Create("HappyDog", &ds_happy);
    boost::shared_ptr<Animal> animal_ptr2 = fact.Create("SadDog", &ds_sad);

    PRINTTESTRESULTS("TestCreateAndAdd",1, true == animal_ptr1->GetTailStatus());
    PRINTTESTRESULTS("TestCreateAndAdd",2, false == animal_ptr2->GetTailStatus());
    
    status = fact.AddClass("HappyDog", AddDogExtra);
    PRINTTESTRESULTS("TestCreateAndAdd",1, false == status);
    boost::shared_ptr<Animal> animal_ptr3 = fact.Create("HappyDog", &ds_happy);
    PRINTTESTRESULTS("TestCreateAndAdd",1, true == animal_ptr1->GetTailStatus());

//  this should fail in compile 
 // boost::shared_ptr<Animal> animal_ptr4 = fact.Create("HappySpider", &ds_happy);
}

boost::shared_ptr<Animal> AddDog(DogStruct *ds)
{
    boost::shared_ptr<Animal> shared_ptr(new Dog(ds->m_numOfLegs, ds->m_IsWavingTail));

    return shared_ptr;
}

boost::shared_ptr<Animal> AddDogExtra(DogStruct *ds)
{
    boost::shared_ptr<Animal> shared_ptr(new Dog(ds->m_numOfLegs, ds->m_IsWavingTail));
    std::cout << "I am the best dog ever" << std::endl;

    return shared_ptr;
}