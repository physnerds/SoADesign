#include <iostream>
#include <utility>
#include "boost/preprocessor.hpp"

#ifdef __CUDACC__
#define SOA_HOST_ONLY __host__
#define SOA_DEVICE_ONLY __device__
#define SOA_HOST_DEVICE_INLINE __host__ __device__ __forceinline__
#else
#define SOA_HOST_ONLY
#define SOA_DEVICE_ONLY
#define SOA_HOST_DEVICE_INLINE inline
#endif

// Exception throwing (or willful crash in kernels)
#if defined(__CUDACC__) && defined(__CUDA_ARCH__)
#define SOA_THROW_OUT_OF_RANGE(A) \
  {                               \
    printf(A "\n");               \
    *((char*)nullptr) = 0;        \
  }
#else
#define SOA_THROW_OUT_OF_RANGE(A) \
  { throw std::out_of_range(A); }
#endif

#define __VALUE_TYPE_SCALAR 0
#define __VALUE_TYPE_COLUMN 1

namespace  soa{
enum class soacolumntype{
  scalar = __VALUE_TYPE_SCALAR,
  column = __VALUE_TYPE_COLUMN
};

//optimize using __restrict__ to the pointers and reference...
//This basically tells compiler that multiple pointers do not point to same memory location...

enum class RestrictQualify : bool { Enabled, Disabled, Default = Disabled };

enum class RangeChecking : bool { Enabled, Disabled, Default = Disabled };

template <typename T, RestrictQualify RESTRICT_QUALIFY>
struct add_restrict {};

template <typename T>
struct add_restrict<T, RestrictQualify::Enabled> {
  typedef T Value;
  typedef T* __restrict__ Pointer;
  typedef T& __restrict__ Reference;
  typedef const T ConstValue;
  typedef const T* __restrict__ PointerToConst;
  typedef const T& __restrict__ ReferenceToConst;
};

template <typename T>
struct add_restrict<T, RestrictQualify::Disabled> {
  typedef T Value;
  typedef T* Pointer;
  typedef T& Reference;
  typedef const T ConstValue;
  typedef const T* PointerToConst;
  typedef const T& ReferenceToConst;
};

template<soacolumntype column_type, typename T>
struct soaParametersImpl;

template<soacolumntype column_type, typename T>
struct soaconstParametersImpl{
  static const soacolumntype columntype = column_type;
  typedef T valuetype;
  typedef const valuetype* tupleOrpointertype;
  const valuetype* addr_ = nullptr;

  SOA_HOST_DEVICE_INLINE soaconstParametersImpl(const valuetype* addr):addr_(addr){}
  SOA_HOST_DEVICE_INLINE soaconstParametersImpl(const soaconstParametersImpl& o){addr_=o.addr_;}
  SOA_HOST_DEVICE_INLINE soaconstParametersImpl(const soaParametersImpl<columntype, valuetype>&o){addr_ = o.addr;}
  static bool checkalignment(valuetype* addr, size_t byteAlignment){
    return reinterpret_cast<intptr_t>(addr)%byteAlignment;
  }
};
  template<soacolumntype column_type,typename T>
  struct soaParametersImpl{
    static const soacolumntype columntype = column_type;
    typedef T valuetype;
    typedef const valuetype* tupleOrpointertype;
    typedef soaconstParametersImpl<columntype, valuetype>constType;
    friend constType;
    valuetype* addr_ = nullptr;
    SOA_HOST_DEVICE_INLINE soaParametersImpl(valuetype* addr): addr_(addr){}
    SOA_HOST_DEVICE_INLINE soaParametersImpl(){}
    static bool checkalignment(valuetype* addr, size_t byteAlignment){
      return reinterpret_cast<intptr_t>(addr)%byteAlignment;
    }
  };

template<soacolumntype column_type, typename T, size_t ALIGNMENT, RestrictQualify RESTRICT_QUALIFY = RestrictQualify::Disabled>
class soavalue{
public:
  typedef add_restrict<T,RESTRICT_QUALIFY>Restr;
  typedef typename Restr::Value val;
  typedef typename Restr::Pointer ptr;
  typedef typename Restr::Reference ref;
  typedef typename Restr::PointerToConst ptrToconst;
  typedef typename Restr::ReferenceToConst refToconst;

  SOA_HOST_DEVICE_INLINE soavalue(size_t i, T* col):idx_(i), col_(col){}
  SOA_HOST_DEVICE_INLINE soavalue(size_t i, soaParametersImpl<column_type,T>params):idx_(i),col_(params.addr_){}
  SOA_HOST_DEVICE_INLINE ref operator()(){
    ptr col = alignedCol();
    return col[idx_];
  }
  SOA_HOST_DEVICE_INLINE refToconst operator()()const{
    ptrToconst col = alignedCol();
    return col[idx_];
  }
  SOA_HOST_DEVICE_INLINE ptr operator&(){
    return &alignedCol()[idx_]; 
  }
  SOA_HOST_DEVICE_INLINE ptrToconst operator&()const{
    return &alignedCol()[idx_];
  }
  template<typename T2>
  SOA_HOST_DEVICE_INLINE ref operator=(const T2&v){
    return alignedCol()[idx_]=v;
  }

  typedef val valuetype;
  static constexpr auto valuesize = sizeof(T);

  private:
    SOA_HOST_DEVICE_INLINE ptr alignedCol() const{
      if constexpr (ALIGNMENT)
        return reinterpret_cast<ptr>(__builtin_assume_aligned)(col_,ALIGNMENT);
        else return reinterpret_cast<ptr>(col_);
    }
    size_t idx_;
    T* col_;
}; //end class soavalue
 
} //end namespace soa



