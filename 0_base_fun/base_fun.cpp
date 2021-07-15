#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

int add(int i, int j) {
    return (i+j);
}

template <typename T>
T square(T x) {
    return x * x;
}

#if !defined(PYBIND11_OVERLOAD_CAST)
template <typename... Args>
using overload_cast_ = pybind11::detail::overload_cast_impl<Args...>;
#endif

int overload(int i) {
    return (i+1);
}

float overload(float i) {
    return (i+1.1);
}

float overload(float i, float j) {
    return (i+j+2.2);
}

void swap(int* a, int* b) {
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

PYBIND11_MODULE(base_fun,m)
{
  m.doc() = "pybind11 base_fun plugin";
   
  m.def("add", &add, "add all entries of a list by 2.0", py::arg("i")=1, py::arg("j")=2);

  m.def("add2", &add, "add all entries of a list by 2.0", py::arg("i")=2, py::arg("j")=3);

  m.attr("the_answer") = 42;
  py::object world = py::cast("World");
  m.attr("what") = world;

  m.def("square", &square<double>);
  m.def("square", &square<float>);
  m.def("square", &square<int>);

  m.def("overload", py::overload_cast<int>(&overload));
  m.def("overload", py::overload_cast<float>(&overload));
  m.def("overload", py::overload_cast<float, float>(&overload));

  m.def("swap", [](py::buffer a, py::buffer b) {\
        py::buffer_info a_info = a.request();
        py::buffer_info b_info = b.request();
        swap(static_cast<int*>(a_info.ptr), static_cast<int*>(b_info.ptr));
    });

}
