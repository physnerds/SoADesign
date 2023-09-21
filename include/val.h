namespace soa{
  template<typename T, int offset = 128-sizeof(T)%128>
    class myval{
        public:
            myval(const T& init_val):_val(init_val){
            };
            ~myval(){}
            T _val;
        SOA_HOST_DEVICE_INLINE  myval<T>& operator=(const T& v){
              _val = v;
              return *this;
        }

        private:
            char pad[offset];
    };
}