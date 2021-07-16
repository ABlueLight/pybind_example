#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

class Pet {
public:

    enum Kind {
        Dog = 0,
        Cat
    };

    Pet(int x) : age(x) {  }
    Pet(const std::string &name, int x) : name(name) , age(x) { }

    void setName(const std::string &name_) { name = name_; }
    const std::string &getName() const { return name; }

    void setPriName(const std::string &name_) { priname = name_; }
    const std::string &getPriName() const { return priname; }
 
    int getAge() { return age; }

    std::string name;
    const int age;
    static std::string st_name;
    Kind type;

private:
    std::string priname;
    
};

std::string Pet::st_name = "static name dog";

/*
class_::def_readwrite_static()，class_::def_readonly_static()，class_::def_property_static()，以及class_::def_property_readonly_static()用于绑定静态变量。
*/

PYBIND11_MODULE(base_struct, m)
{
    m.doc() = "pybind11 base struct test";

    py::class_<Pet> pet(m, "Pet", py::dynamic_attr());

     pet.def(py::init<int>())
        .def(py::init<const std::string &, int>())
        .def("setName", &Pet::setName)
        .def("getName", &Pet::getName)
        .def("getAge", &Pet::getAge)
        .def_readwrite("name", &Pet::name)
        .def_readwrite("type", &Pet::type)
        .def_readwrite_static("st_name", &Pet::st_name)
        .def_readonly("age", &Pet::age)
        .def_property("priname", &Pet::setPriName, &Pet::getPriName)
        .def("__repr__",
            [](const Pet &a) {
                return "<example.Pet named '" + a.name + "'>";
        });


    py::enum_<Pet::Kind>(pet, "Kind")
        .value("Dog", Pet::Kind::Dog)
        .value("Cat", Pet::Kind::Cat)
        .export_values();

}