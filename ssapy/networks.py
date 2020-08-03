import operator as op

from parse import parse


class ReactionNetwork:
    def __init__(self):
        self.species = []
        self.reactions = []

    @classmethod
    def from_script(cls, script):
        rn = cls()
        for expr in parse(script):
            if len(expr) == 2:
                species, count = expr
                rn.setspeciescount(species, count)
            elif len(expr) == 4:
                reactants, products, rate, steps = expr
                rn.addreaction(reactants, products, rate, steps)
        return rn

    def __contains__(self, species):
        return species in map(lambda x: x[0], self.species)

    def addspecies(self, identifier):
        if identifier not in self:
            self.species.append((identifier, 0))

    def addreaction(self, reactants, products, rate, steps=1):
        for reactant in reactants:
            self.addspecies(reactant)

        for product in products:
            self.addspecies(product)

        self.reactions.append((reactants, products, rate, steps))

    def setspeciescount(self, identifier, count):
        self.addspecies(identifier)
        for i, (name, oldcount) in enumerate(self.species):
            if name == identifier:
                self.species[i] = (name, count)

    @property
    def R(self):
        R = [[0] * self.n for _ in range(self.m)]
        species_index = [x[0] for x in self.species]
        for i, (reactants, _, _, _) in enumerate(self.reactions):
            for reactant in reactants:
                if isinstance(reactant, str):
                    R[i][species_index.index(reactant)] += 1
        return R

    @property
    def P(self):
        P = [[0] * self.n for _ in range(self.m)]
        species_index = [x[0] for x in self.species]
        for i, (_, products, _, _) in enumerate(self.reactions):
            for product in products:
                if isinstance(product, str):
                    P[i][species_index.index(product)] += 1
        return P

    @property
    def V(self):
        return [list(map(op.sub, self.P[i], self.R[i])) for i in range(self.m)]

    @property
    def m(self):
        return len(self.reactions)

    @property
    def n(self):
        return len(self.species)

    @property
    def adjmatrix(self):
        graph = [[0] * self.m for _ in range(self.m)]
        for i in range(self.m):
            for j in range(self.m):
                if any(map(op.and_, self.V[i], self.R[j])):
                    graph[i][j] = 1
        return graph

    @property
    def X(self):
        return [count for species, count in self.species]

    @property
    def k(self):
        return [rate for _, _, rate, _ in self.reactions]

    @property
    def steps(self):
        return [step for _, _, _, step in self.reactions]
