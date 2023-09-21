#ifdef __CUDACC__
#define SOA_HOST_ONLY __host__
#define SOA_DEVICE_ONLY __device__
#define SOA_HOST_DEVICE_INLINE __host__ __device__ __forceinline__
#else
#define SOA_HOST_ONLY
#define SOA_DEVICE_ONLY
#define SOA_HOST_DEVICE_INLINE inline
#endif