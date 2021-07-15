from base_struct import *

pet = Pet(12)
pet = Pet("dog", 12)

pet.age
#static vale
pet.st_name

pet.setName("cat")
pet.getName()

#attri
pet.name = "snake"


#py::dynamic_attr
pet.address='nanjing'

pet.type = Pet.Dog

pet.__dict__