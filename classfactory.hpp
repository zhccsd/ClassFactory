/*
* Copyright (c) 2023 H.C.Chou
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#ifndef CLASSFACTORY_HPP
#define CLASSFACTORY_HPP
#include <functional>
#include <map>
#include <list>

template<typename KEY, typename... ARGS>
class ClassFactory
{
public:
    ClassFactory() {}
    ClassFactory(const ClassFactory& other) = delete;
    ClassFactory(ClassFactory&& other) = delete;
    ClassFactory& operator=(const ClassFactory& other) = delete;
    ClassFactory& operator=(ClassFactory&& other) = delete;

    ~ClassFactory()
    {
        for (auto iter = _d.begin(); iter != _d.end();)
        {
            _Base* b = iter->second;
            delete b;
            iter = _d.erase(iter);
        }
    }

    bool exists(const KEY& key) const
    {
        auto iter = _d.find(key);
        if (iter == _d.end())
        {
            return false;
        }
        return true;
    }

    std::list<KEY> keys() const
    {
        std::list<KEY> ret;
        for (auto iter = _d.begin(); iter != _d.end(); ++iter)
        {
            ret.push_back(iter->first);
        }
        return ret;
    }

    template<typename BASE_CLASS, typename DERIVRD_CLASS>
    void registerClass(const KEY& key)
    {
        std::function<BASE_CLASS* (ARGS...)> f = [](ARGS... args)->BASE_CLASS* { return new(std::nothrow) DERIVRD_CLASS(args...); };
        _Base* b = new(std::nothrow) _Derived<std::function<BASE_CLASS* (ARGS...)>>(f);
        if (nullptr == b)
        {
            return;
        }
        _d.insert(std::make_pair(key, b));
    }

    template<typename CLASS>
    CLASS* create(const KEY& key, ARGS... args)
    {
        auto iter = _d.find(key);
        if (iter == _d.end())
        {
            return nullptr;
        }
        _Derived<std::function<CLASS* (ARGS...)>>* d = dynamic_cast<_Derived<std::function<CLASS* (ARGS...)>>*>(iter->second);
        if (!d)
        {
            return nullptr;
        }
        CLASS* c = d->_ctor(args...);
        return c;
    }

private:
    struct _Base
    {
        _Base() {}
        virtual ~_Base() {}
    };

    template<typename CTOR>
    struct _Derived :public _Base
    {
        CTOR _ctor;
        _Derived(const CTOR& f) : _ctor(f) {}
    };

    std::map<KEY, _Base*> _d;
};

#endif
