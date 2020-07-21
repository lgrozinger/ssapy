class ReactionNetwork:
    def __init__(self):
        self.species = set()
        self.reactions = set()

    def __contains__(self, species):
        return species in map(lambda x: x[0], self.species)

    def addspecies(self, identifier):
        if identifier not in self:
            self.species.add((identifier, 0))

    def addreaction(self, reactants, products, rate, steps=1, steprate=0.0):
        for reactant in reactants:
            self.addspecies(reactant)

        for product in products:
            self.addspecies(product)

        
        
