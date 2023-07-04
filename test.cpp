#include <stdio.h>
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
        printf("hello from FirstClass\n");
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
        printf("hello from SecondClass\n");
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
        printf("hello from ThirdClass\n");
        printArg();
    }
};

int main()
{
    ClassFactory<std::string, const std::string&> f;
    f.registerClass<BaseClass, FirstClass>("first");
    f.registerClass<BaseClass, SecondClass>("second");
    f.registerClass<BaseClass, ThirdClass>("third");

    char buffer[256];
    while (1)
    {
        printf("class you want to create?");
        memset(buffer, 0, 256);
        gets_s(buffer, 255);
        std::string c = buffer;
        if (!f.exists(c))
        {
            printf("class not exists\n");
            continue;
        }

        printf("arg for class %s?", c.c_str());
        memset(buffer, 0, 256);
        gets_s(buffer, 255);
        std::string a = buffer;

        // no more if/else and switch
        BaseClass* p = f.create<BaseClass>(c, a);
        if (!p)
        {
            printf("class not created\n");
            continue;
        }
        p->sayHello();
        delete p;
    }

    return 0;
}
