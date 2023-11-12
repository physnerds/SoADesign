#ifndef PADDED_SOAVIEW_H
#define PADDED_SOA_VIEW_H

#include "padded_SoA.h"
#include "soa.h"

using namespace soa;

const size_t nval = 5000;
Generate_Arrays(SoaView,
                AddArray(float,x,nval)
                AddArray(float,y,nval)
                AddArray(float,z,nval)
                AddScalar(int, id)
                AddScalar(int,r)
                )


#endif
