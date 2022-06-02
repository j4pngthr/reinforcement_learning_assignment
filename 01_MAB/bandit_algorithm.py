from abc import ABCMeta, abstractmethod

class BanditAlgorithm(metaclass = ABCMeta):
    @abstractmethod
    def action(self):
        pass

    @abstractmethod
    def get_name(self):
        pass
