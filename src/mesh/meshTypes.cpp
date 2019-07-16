/**
 * \file
 * \author Timothy B. Costa
 * \brief Mesh types header file.
 */

#include "meshTypes.hpp"

//////////////// EDGE //////////////////

//--- Constructors and Destructors ---//
template <typename T>
porescale::edge<T>::edge() : length_(0.0)
{
    vertices_[0] = NULL;
    vertices_[1] = NULL;
}

template <typename T>
porescale::edge<T>::edge( 
    porescale::vertex<T>* v1, 
    porescale::vertex<T>* v2
)
{
    vertices_[0] = v1;
    vertices_[1] = v2;
    computeLength_();
}

//--- Public member functions ---//
template <typename T>
void
porescale::edge<T>::init(
    porescale::vertex<T>* v1,
    porescale::vertex<T>* v2
)
{
    vertices_[0] = v1;
    vertices_[1] = v2;
    computeLength_();

}

template <typename T>
porescale::vertex<T>*
porescale::edge<T>::vertices(
    psInt idx
)
{
    return vertices_[idx];
}

template <typename T>
T 
porescale::edge<T>::length(void) const { return length_; }

//--- Private member functions ---//
template <typename T>
void
porescale::edge<T>::computeLength_(void)
{
    porescale::vertex<T> *v1 = vertices_[0];
    porescale::vertex<T> *v2 = vertices_[1];

    T tmp;
    length_ = 0.0;
    for (int i = 0; i < 3; i++) 
    {
        tmp = (v1->coordinates[i] - v2->coordinates[i]);
        length_ += tmp*tmp;
    }

    length_ = sqrt(length_);

}

//--- Explicit type instantiations ---//
template class porescale::edge<float>;
template class porescale::edge<double>;


//////////////// FACE //////////////////

//--- Constructors and Destructors ---//
template <typename T>
porescale::face<T>::face() : vertices_(NULL), edges_(NULL), 
                             area_(0.0), nEdges_(0), nVertices_(0) { }

template <typename T>
porescale::face<T>::face( 
    psInt             nEdges,
    porescale::edge<T>* e1, 
    ...
)
{

    // Current support limited to triangular and quadrilateral type cells
    assert( nEdges == 3 || nEdges == 4);

    // Set edges
    nEdges_ = nEdges;
    edges_ = new edge<T>*[nEdges];

    edge<T>** edgesTmp = new edge<T>*[nEdges];

    va_list argPtr;
    va_start( argPtr, e1 );

    edgesTmp[0] = e1;
    for (int i = 1; i < nEdges; i++) {
        edgesTmp[i] = va_arg( argPtr, porescale::edge<T>* );
    }
    va_end( argPtr );

    init_( edgesTmp );

    computeArea_();

}

//--- Public member functions ---//
template <typename T>
void
porescale::face<T>::init(
    psInt             nEdges,
    porescale::edge<T>* e1, 
    ...
)
{

    // Current support limited to triangular and quadrilateral type cells
    assert( nEdges == 3 || nEdges == 4);

    // Set edges
    nEdges_ = nEdges;
    edges_ = new edge<T>*[nEdges];

    edge<T>** edgesTmp = new edge<T>*[nEdges];

    va_list argPtr;
    va_start( argPtr, e1 );

    edgesTmp[0] = e1;
    for (int i = 1; i < nEdges; i++) {
        edgesTmp[i] = va_arg( argPtr, porescale::edge<T>* );
    }
    va_end( argPtr );

    init_( edgesTmp );

    computeArea_();
}

template <typename T>
porescale::vertex<T>*
porescale::face<T>::vertices(
    psInt idx
)
{
    return vertices_[idx];
}

template <typename T>
porescale::edge<T>*
porescale::face<T>::edges(
    psInt idx
)
{
    return edges_[idx];
}

template <typename T>
psInt porescale::face<T>::nVertices(void) const { return nVertices_; }

template <typename T>
psInt porescale::face<T>::nEdges(void) const { return nEdges_; }

template <typename T>
T porescale::face<T>::area(void) const { return area_; }

//--- Private member functions ---//
template <typename T>
void
porescale::face<T>::init_( edge<T>** edgesTmp )
{

    std::unordered_set< psInt > placed;

    edges_[0] = edgesTmp[0];
    vertices_[0] = edges_[0]->vertices(0);
    vertices_[1] = edges_[0]->vertices(1);

    psInt done;

    // i tracks idx of edges_ array
    for (int i = 1; i < nEdges_; i++)
    {
        done = 0;
        // j tracks idx of edgesTmp array
        for (int j = 1; j < nEdges_; j++)
        {
            // is this a candidate
            if (placed.find(j) == placed.end()) 
            {
                if (edgesTmp[j]->vertices(0) == edges_[i-1]->vertices(1))
                {
                    // log that we've used this edge
                    placed.insert(j);
                    edges_[i] = edgesTmp[j];
                    vertices_[i+1] = edgesTmp[j]->vertices(1);
                    done = 1;
                }
                else if (edgesTmp[j]->vertices(1) == edges_[i-1]->vertices(1))
                {
                    // log that we've used this edge
                    placed.insert(j);
                    edges_[i] = edgesTmp[j];
                    vertices_[i+1] = edges_[i]->vertices(0);
                    done = 1;
                }

            }
            if (done) break;
        }
    }

    // Need a sanity check to establish a closed face was created by these edges

}

template <typename T>
void
porescale::face<T>::computeArea_(void)
{

    if (nEdges_ == 3) {

    }
    else if (nEdges_ == 4) {

        // Find two non-adjacent vertices
        porescale::vertex<T> *A, *C;
        porescale::edge<T> *a, *b, *c, *d;

        a = edges_[0];
        b = edges_[1];
        c = edges_[2];
        d = edges_[3];

        A = vertices(0);
        C = vertices(2);

        // Compute angles

        // Compute area



    }

    // Return unsuccessful until function is finished 
}

//--- Explicit type instantiations ---//
template class porescale::face<float>;
template class porescale::face<double>;