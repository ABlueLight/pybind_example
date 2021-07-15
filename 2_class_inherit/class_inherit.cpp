#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

class Pet {
public:
    Pet(const std::string &name) : name(name) { }
    std::string name;
};

class Dog : public Pet {
public:
    Dog(const std::string &name) : Pet(name) { }
    std::string bark() const { return "woof!"; }
};


//In C++, a type is only considered polymorphic if it has at least one virtual function and pybind11 will automatically recognize this:
struct PolymorphicPet {
    virtual ~PolymorphicPet() = default;
};

struct PolymorphicDog : PolymorphicPet {
    std::string bark() const { return "woof!"; }
};

PYBIND11_MODULE(class_inherit, m)
{
    py::class_<Pet>(m, "Pet")
        .def(py::init<const std::string &>())
        .def_readwrite("name", &Pet::name);

    // Method 1: template parameter:
    py::class_<Dog, Pet>(m, "Dog")
        .def(py::init<const std::string &>())
        .def("bark", &Dog::bark);

    //error Method
    /*
    m.def("pet_store", []() { return std::unique_ptr<Pet>(new Dog("Molly")); });

    //if use in Python like is a wrong method
        p = example.pet_store()
        type(p)  # `Dog` instance behind `Pet` pointer
        Pet          # no pointer downcasting for regular non-polymorphic types
        p.bark()
        AttributeError: 'Pet' object has no attribute 'bark'
    */

    // Same binding code
    py::class_<PolymorphicPet>(m, "PolymorphicPet");
    py::class_<PolymorphicDog, PolymorphicPet>(m, "PolymorphicDog")
        .def(py::init<>())
        .def("bark", &PolymorphicDog::bark);

    // Again, return a base pointer to a derived instance, 
    m.def("pet_store2", []() { return std::unique_ptr<PolymorphicPet>(new PolymorphicDog); });
}

// Method 2: pass parent class_ object:
/*
py::class_<Pet> pet(m, "Pet");
pet.def(py::init<const std::string &>())
   .def_readwrite("name", &Pet::name);

py::class_<Dog>(m, "Dog", pet)
    .def(py::init<const std::string &>())
    .def("bark", &Dog::bark);
*/