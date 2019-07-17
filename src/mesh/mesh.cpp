/**
 * \file
 * \author Timothy B. Costa
 * \brief Mesh source file.
 */

#include "mesh.hpp"

template <typename T>
porescale::mesh<T>::mesh() : built_(false), par_(NULL) {}

template <typename T>
porescale::mesh<T>::mesh(parameters<T> * par) : built_(false)
{
    par_ = par;
}

template <typename T>
bool
porescale::mesh<T>::built(void) const { return built_; }

//--- Explicit type instantiations ---//
template class porescale::mesh<double>;
template class porescale::mesh<float>;
