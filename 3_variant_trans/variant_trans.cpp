#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

void print_dict(py::dict dict) {
    /* Easily interact with Python types */
    for (auto item : dict)
        std::cout << "key=" << std::string(py::str(item.first)) << ", "
                  << "value=" << std::string(py::str(item.second)) << std::endl;
}


void generic(py::args args, py::kwargs kwargs) {
    /// .. do something with args
    if (kwargs) {
        
    }
        /// .. do something with kwargs
}


PYBIND11_MODULE(variant_trans, m) {
    m.doc() = "pybind11 test variant_trans";
    m.def("print_dict",&print_dict);

}