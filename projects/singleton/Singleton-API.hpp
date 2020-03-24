

template<typename T>
class Singleton: private Uncopyable
{
public:
    static T *GetInstance();
    ~Singleton();

    friend class SingletonDestroyer;

private:
    explicit Singleton();

    static bool destructed;
    static T *m_instance;
    static SingletonDestroyer m_destroyer;
};

template<typename T>
class SingletonDestroyer
{
public:
    SingletonDestroyer();
    ~SingletonDestroyer();
    void SetSingleton(T *singleton_);

private:
    T *m_singleton;
};

template <typename T>
Singleton<T> *Singleton<T>::m_instance = 0;

template <typename T>
Singleton<T>::Singleton()
{}