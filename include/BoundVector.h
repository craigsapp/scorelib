//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Wed May  7 11:12:33 PDT 2014
// Last Modified: Wed May  7 11:12:35 PDT 2014
// Filename:      BoundVector.h
// URL:           https://github.com/craigsapp/scorelib/blob/master/include/BoundVector.h
// Syntax:        C++11
//
// Description:   The BoundVector class is used to locate out-of-bounds
//                access in the stl vector class.
//

#ifndef _BOUNDVECTOR_H_INCLUDED
#define _BOUNDVECTOR_H_INCLUDED

#include <vector>
#include <iostream>

using namespace std;

template<class type>
class BoundVector : public vector<type> {
   public:

      using vector<type>::vector;

      type operator[](int index) const {
         const vector<type>& data = *this;
         if ((index < 0) || (index >= data.size())) {
            cerr << "Error: vector index out of range: " << index << endl;
            cerr << "vector size is: " << data.size() << endl;
            exit(1);
         }
         return data[index];
      }

      type& operator[](int index) {
         vector<type>& data = *this;
         if ((index < 0) || (index >= data.size())) {
            cerr << "Error: vector index out of range: " << index << endl;
            cerr << "vector size is: " << data.size() << endl;
            exit(1);
         }
         return data[index];
      }

};


#endif  /* _BOUNDVECTOR_H_INCLUDED */



