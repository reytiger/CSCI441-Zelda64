#pragma once

#include <iostream>

namespace Internal {

// Use Tag to make mostly identical types of Vectors which
// are mutually exclusive. Points, Vecs, and Colors are our main uses.
template <typename DataT, typename Tag>
class Vector {
public:
    using Data = DataT;
    using Self = Vector<Data, Tag>;
    union {
        // TODO: Parameterize on dimension.
        Data v[4];
        struct {
            Data x, y, z, w;
        };
        struct {
            Data r, g, b, a;
        };
    };

    Vector<Data, Tag>() : v() {}

    // Pretend these are named r, g, b, and a for colors.
    template <typename X, typename Y, typename Z>
    Vector<Data, Tag>(X x, Y y, Z z)
        : v{as<Data>(x), as<Data>(y), as<Data>(z), as<Data>(Tag::w_val)} {}

    template <typename X, typename Y, typename Z, typename W>
    Vector<Data, Tag>(X x, Y y, Z z, W w)
        : v{as<Data>(x), as<Data>(y), as<Data>(z), as<Data>(w)} {}

    Self cross() const;
    Data dot() const;
    Data norm() const;
    Self normed() const;
};

} // namespace Internal

// Clang really doesn't like what OP does to this function definition.
// clang-format off
#define def_comp_wise(OP, T, Data, Tag)                                        \
    inline T<Data, Tag>                                                        \
    operator OP(const T<Data, Tag> &a, const T<Data, Tag> &b) {                \
        return T<Data, Tag>(a.x OP b.x, a.y OP b.y, a.z OP b.z, a.w OP b.w);   \
}
// clang-format on

template <typename Data, typename Tag>
def_comp_wise(+, Internal::Vector, Data, Tag);

template <typename Data, typename Tag>
def_comp_wise(-, Internal::Vector, Data, Tag);

template <typename Data, typename Tag>
def_comp_wise(*, Internal::Vector, Data, Tag);

template <typename Data, typename Tag>
def_comp_wise(/, Internal::Vector, Data, Tag);

#define STR_IT(it) #it

template <typename Data, typename Tag>
std::ostream &operator<<(std::ostream &os,
                         const Internal::Vector<Data, Tag> &v) {
    return os << "InternalVector<" << STR_IT(Data) << ", " << STR_IT(Tag)
              << ">(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w
              << ")";
}
