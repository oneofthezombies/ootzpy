# sample.py

def multiply(a,b):
    result = a * b
    print("call multiply and value a = ", a, ", b = ", b, ", result = ", result)
    return result

class Adder:
    def __init__(self, a=0, b=0):
        self.left = a
        self.right = b
        print("called __init__ and set value left = ", a, ", right = ", b)

    def setValue(self, a, b):
        self.left = a
        self.right = b
        print("set value left = ", a, ", right = ", b)

    def getResult(self):
        result = self.left + self.right
        print("get result = ", result)
        return result
