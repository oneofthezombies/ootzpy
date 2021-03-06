

/* include this header before standard library. 
 * https://docs.python.org/3.6/c-api/intro.html#include-files
 */
#include <ootzpy/essential.h>

// some std headers
#include <iostream>

// include ootzpy api
#include <ootzpy/detail.h>

int main()
{
    {
        using namespace ootzpy;

        // initialize interpreter.
        Interpreter interpreter;

        // add path "../../module/".
        interpreter.addPath("../../module/");

        // import "sample.py" module.
        Owner module_ = PyImport_ImportModule("sample");

        // create instance from module. call Adder().
        Owner instance = PyObject_CallMethod(module_.get(), "Adder", "()");

        // release and assign instance. call Adder(1, 2).
        instance = PyObject_CallMethod(module_.get(), "Adder", "(ii)", 1, 2);

        // call instance.setValue(10, 20).
        // no return method but PyObject_CallMethod returned new reference.
        // so assign to OPOwner some_variable for memory management.
        Owner result = PyObject_CallMethod(instance.get(), "setValue", "(ii)", 10, 20);

        // call instance.getResult().
        result = PyObject_CallMethod(instance.get(), "getResult", "()");

        // PyObject value to c++ primitive value
        long value = PyLong_AsLong(result.get());
        std::cout << "result in c++ = " << value << '\n';

        // call function multiply(8, 9).
        result = PyObject_CallMethod(module_.get(), "multiply", "(ii)", 8, 9);
        value = PyLong_AsLong(result.get());
        std::cout << "result in c++ = " << value << '\n';
    }

    return 0;
}

