#pragma once

#include "ootzpy/essential.h"

namespace ootzpy
{


/* to use for initialize and finalize of python c api */
class Interpreter
{
public:

    /* call initializer and add running path for module */
    Interpreter()
    {
        Py_Initialize();
        PyRun_SimpleString("import sys");
        PyRun_SimpleString("sys.path.append(\".\")");
    }

    /* call finalizer 
     * if an error is detected then program is terminated
     */
    ~Interpreter()
    {
        if (Py_FinalizeEx() < 0)
        {
            exit(120);
        }
    }

    /* add module path 
     * e.g. addPath("./module/");
     */
    void addPath(const char* path)
    {
        std::string command("sys.path.append(\"");
        command += path;
        command += "\")";
        PyRun_SimpleString(command.c_str());
    }
};


/* to use new reference return value 
 * this class is not used alone. 
 * this is used deleter of std::unique_ptr<PyObject*>.
 */
struct Releaser
{
    constexpr void operator()(PyObject* p)
    {
        Py_DECREF(p);
    }
};


/* for debug. 
 * if error is ocuured print error to console 
 */
constexpr PyObject* CheckError(PyObject* p)
{
#ifdef _DEBUG
    if (nullptr == p)
        PyErr_Print();
#endif
    return p;
}


/* PyObject*'s owner.
 * this class has std::unique_ptr<PyObject*, OPReleaser>.
 * Noncopyconstructable, Noncopyassignable, Moveconstructable, Moveassignable
 */
class Owner : protected std::unique_ptr<PyObject, Releaser>
{
    using Base = std::unique_ptr<PyObject, Releaser>;

public:
    Owner() 
        : Base(nullptr, Releaser())
    {}

    Owner(PyObject* pObject) 
        : Base(pObject, Releaser())
    {}

    ~Owner() = default;

    /* Noncopyconstructable */
    Owner(const Owner&) = delete;

    /* Noncopyassignable */
    Owner& operator=(const Owner&) = delete;

    /* Moveconstructable */
    constexpr Owner(Owner&&) = default;

    /* Moveassignable */
    constexpr Owner& operator=(Owner&&) = default;

    /* if assigned PyObject* then release current and assign new */
    Owner& operator=(PyObject* pObject)
    {
        reset(CheckError(pObject));
        return *this;
    }

    /* get raw pointer */
    PyObject* get() const { return Base::get(); }
};


} // namespace ootzpy