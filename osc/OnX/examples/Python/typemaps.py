#
# Example of uage of "typemaps" :
#

import OnX

test = OnX.OnX_SWIG_Test()

# std::vector<std::string> :
print('typemaps : OnX_SWIG_Test::arg_const_std_vec_string :')
vs = ['Item 1','Item 2','Item 3']
test.arg_const_std_vec_string(vs)
print('typemaps : OnX_SWIG_Test::arg_std_vec_string :')
test.arg_std_vec_string(vs)

print('typemaps : OnX_SWIG_Test::ret_std_vec_string :')
vs = test.ret_std_vec_string()
for s in vs:
  print(s)
  
print('typemaps : OnX_SWIG_Test::ret_const_std_vec_string :')
vs = test.ret_const_std_vec_string()
for s in vs:
  print(s)
  

# std::vector<double> :
print('typemaps : OnX_SWIG_Test::arg_const_std_vec_double :')
vd = [1.1,2.2,3.3]
test.arg_const_std_vec_double(vd)
print('typemaps : OnX_SWIG_Test::arg_std_vec_double :')
test.arg_std_vec_double(vd)

print('typemaps : OnX_SWIG_Test::ret_std_vec_double :')
vd = test.ret_std_vec_double()
for d in vd:
  print(d)
  
print('typemaps : OnX_SWIG_Test::ret_const_std_vec_double :')
vd = test.ret_const_std_vec_double()
for d in vd:
  print(d)
  

# std::vector<int> :
print('typemaps : OnX_SWIG_Test::arg_const_std_vec_int :')
vi = [1,2,3]
test.arg_const_std_vec_int(vi)
print('typemaps : OnX_SWIG_Test::arg_std_vec_int :')
test.arg_std_vec_int(vi)

print('typemaps : OnX_SWIG_Test::ret_std_vec_int :')
vi = test.ret_std_vec_int()
for i in vi:
  print(i)
  
print('typemaps : OnX_SWIG_Test::ret_const_std_vec_int :')
vi = test.ret_const_std_vec_int()
for i in vi:
  print(i)
  

