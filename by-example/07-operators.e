struct SimpleComplex {
    x, z: int

    // operators are always pub

    // SimpleComplex + SimpleComplex
    pub func _add(self, other: SimpleComplex) {
        SimpleComplex(self.x + other.x, self.z + other.z)
    }
}

// struct-like declaration init func
// declaration init func (classic)
// assignment op
// == !=
// + - / * %
