#include <cstdio>
#include <cstring>
#include "classfactory.hpp"
#include <string>

class BaseClass
{
public:
    BaseClass(const std::string& arg = std::string())
        : _arg(arg)
    {}

    virtual ~BaseClass()
    {}

    virtual void sayHello() const = 0;

protected:
    void printArg() const
    {
        printf("arg is %s\n", _arg.c_str());
    }

private:
    std::string _arg;
};

class FirstClass: public BaseClass
{
public:
    FirstClass(const std::string& arg = std::string())
        : BaseClass(arg)
    {}

    void sayHello() const override
    {
        printf("Hello from FirstClass\n");
        printArg();
    }
};

class SecondClass: public BaseClass
{
public:
    SecondClass(const std::string& arg = std::string())
        : BaseClass(arg)
    {}

    void sayHello() const override
    {
        printf("Hello from SecondClass\n");
        printArg();
    }
};

class ThirdClass : public BaseClass
{
public:
    ThirdClass(const std::string& arg = std::string())
        : BaseClass(arg)
    {}

    void sayHello() const override
    {
        printf("Hello from ThirdClass\n");
        printArg();
    }
};

int main()
{
    ClassFactory<std::string, const std::string&> factory;
    factory.registerType<BaseClass, FirstClass>("first");
    factory.registerType<BaseClass, SecondClass>("second");
    factory.registerType<BaseClass, ThirdClass>("third");

    char buffer[256];
    while (true)
    {
        printf("Which class to create? ");
        memset(buffer, 0, 256);
        gets_s(buffer, 255);
        std::string className = buffer;
        if (!factory.exists(className))
        {
            printf("Class not found\n");
            continue;
        }

        printf("Arg for class %s? ", className.c_str());
        memset(buffer, 0, 256);
        gets_s(buffer, 255);
        std::string arg = buffer;

        BaseClass* instance = factory.create<BaseClass>(className, arg);
        if (!instance)
        {
            printf("Failed to create class\n");
            continue;
        }
        instance->sayHello();
        delete instance;
    }

    return 0;
}
