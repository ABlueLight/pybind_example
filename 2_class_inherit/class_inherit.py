from class_inherit import *

d=Dog('dog')
pet=Pet('ani')

d.name
pet.name


b = Bird()
call_go(b)

#允许Python对Animal进行扩展
class Cat(Animal):
    def go(self, n_times):
        return "meow! " * n_times

c = Cat()
call_go(c)
