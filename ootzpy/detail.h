#pragma once

#include "ootzpy/essential.h"

namespace ootzpy
{


/* to use for initialize and finalize of python c api */
struct OPInterpreter
{

    /* call initializer and add running path for module */
    OPInterpreter()
    {
        Py_Initialize();
        PyRun_SimpleString("import sys");
        PyRun_SimpleString("sys.path.append(\".\")");
    }

    /* call finalizer 
     * if an error is detected then program is terminated
     */
    ~OPInterpreter()
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
struct OPReleaser
{
    constexpr void operator()(PyObject* p)
    {
        Py_DECREF(p);
    }
};


/* for debug. 
 * if error is ocuured print error to console 
 */
constexpr PyObject* OPCheckError(PyObject* p)
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
class OPOwner : protected std::unique_ptr<PyObject, OPReleaser>
{
    using Base = std::unique_ptr<PyObject, OPReleaser>;

public:
    OPOwner() 
        : Base(nullptr, OPReleaser())
    {}

    OPOwner(PyObject* pObject) 
        : Base(pObject, OPReleaser())
    {}

    ~OPOwner() = default;

    /* Noncopyconstructable */
    OPOwner(const OPOwner&) = delete;

    /* Noncopyassignable */
    OPOwner& operator=(const OPOwner&) = delete;

    /* Moveconstructable */
    constexpr OPOwner(OPOwner&&) = default;

    /* Moveassignable */
    constexpr OPOwner& operator=(OPOwner&&) = default;

    /* if assigned PyObject* then release current and assign new */
    OPOwner& operator=(PyObject* pObject)
    {
        reset(OPCheckError(pObject));
        return *this;
    }

    /* get raw pointer */
    PyObject* get() const { return Base::get(); }
};


} // namespace ootzpy