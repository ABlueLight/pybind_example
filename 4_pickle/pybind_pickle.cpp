#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;


class Pickleable {
public:
    Pickleable(const std::string &value) : m_value(value) { }
    const std::string &value() const { return m_value; }

    void setExtra(int extra) { m_extra = extra; }
    int extra() const { return m_extra; }
private:
    std::string m_value;
    int m_extra = 0;
};


PYBIND11_MODULE(pybind_pickle, m) {
    py::class_<Pickleable>(m, "Pickleable")
        .def(py::init<std::string>())
        .def("value", &Pickleable::value)
        .def("extra", &Pickleable::extra)
        .def("setExtra", &Pickleable::setExtra)
        .def(py::pickle(
            [](const Pickleable &p) { // __getstate__
                /* Return a tuple that fully encodes the state of the object */
                return py::make_tuple(p.value(), p.extra());
            },
            [](py::tuple t) { // __setstate__
                if (t.size() != 2)
                    throw std::runtime_error("Invalid state!");

                /* Create a new C++ instance */
                Pickleable p(t[0].cast<std::string>());

                /* Assign any additional state */
                p.setExtra(t[1].cast<int>());

                return p;
            }
        ));

}

