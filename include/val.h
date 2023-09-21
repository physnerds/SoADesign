namespace soa{
  template<typename T, int offset = 128-sizeof(T)%128>
    class myval{
        public:
            myval(const T& init_val):_val(init_val){
            };
            ~myval(){}
        SOA_HOST_DEVICE_INLINE  myval<T>& operator=(const T& v){
              _val = v;
              return *this;
        }
        /*SOA_HOST_DEVICE_INLINE*/ myval<T> operator*() const{
            return *_val;
        }
        //Define a custom stream operator Needed for cout, printf etc..
        friend std::ostream& operator<<(std::ostream&os, const myval<T,offset>& obj){
            os<<obj._val;
            return os;
        }
        private:
            char pad[offset];
            T _val;
    };
}