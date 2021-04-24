// Copyright (C) 2010, Guy Barrand. All rights reserved.
// See the file exlib.license for terms.

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
%include std_string.i

%include std_vector.i
//%template(std_vector_string)  std::vector<std::string>;
%template(std_vector_double)  std::vector<double>;
//%template(std_vector_int)     std::vector<int>;

#ifdef SWIGLUA
%include <inlib/get_cout>
#else
%include std_iostream.i
#endif

%include std_pair.i
%include std_map.i
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

%ignore vsprintf;
%ignore vsnpf;
%include <inlib/sprintf>

%rename(random_flat)  inlib::random::flat;
%rename(random_gauss) inlib::random::gauss; 
%rename(random_bw)    inlib::random::bw;
%rename(random_exp)   inlib::random::exp;   

%include <inlib/random>

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

//%include <inlib/nostream>
%include <inlib/system>

%rename(file_write)  inlib::file::write; //clash with inlib::waxml::file::write
%include <inlib/file>

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
#include <inlib/histo/base_histo>

%template(base_histo_duidd) inlib::histo::base_histo<double,unsigned int,double,double>;

#include <inlib/histo/b1>
#include <inlib/histo/b2>
#include <inlib/histo/b3>

%template(b1_duidd) inlib::histo::b1<double,unsigned int,double,double>;
%template(b2_duidd) inlib::histo::b2<double,unsigned int,double,double>;
%template(b3_duidd) inlib::histo::b3<double,unsigned int,double,double>;

#include <inlib/histo/h1>
#include <inlib/histo/h2>
#include <inlib/histo/h3>
#include <inlib/histo/p1>
#include <inlib/histo/p2>

%template(h1_duidd)  inlib::histo::h1<double,unsigned int,double,double>;
%template(h2_duidd)  inlib::histo::h2<double,unsigned int,double,double>;
%template(h3_duidd)  inlib::histo::h3<double,unsigned int,double,double>;
%template(p1_duiddd) inlib::histo::p1<double,unsigned int,double,double,double>;
%template(p2_duiddd) inlib::histo::p2<double,unsigned int,double,double,double>;

%rename(histo_h1d)    inlib::histo::h1d;
%rename(histo_h2d)    inlib::histo::h2d;
%rename(histo_h3d)    inlib::histo::h3d;
%rename(histo_p1d)    inlib::histo::p1d;
%rename(histo_p2d)    inlib::histo::p2d;

#include <inlib/histo/h1d>
#include <inlib/histo/h2d>
#include <inlib/histo/h3d>
#include <inlib/histo/p1d>
#include <inlib/histo/p2d>

//#include <inlib/histo/sliced>

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
%rename(waxml_file)   inlib::waxml::file;

#include <inlib/waxml/file>

#include <inlib/vec2>
%template(vec2_float) inlib::vec2<float>;
#include <inlib/vec3>
%template(vec3_float) inlib::vec3<float>;
%template(vec3_double) inlib::vec3<double>;
#include <inlib/vec4>
%template(vec4_float) inlib::vec4<float>;
#include <inlib/mat4>
%template(mat_float_4) inlib::mat<float,4>;
%template(mat4_float) inlib::mat4<float>;
#include <inlib/qrot>
%template(qrot_float) inlib::qrot<float>;

#include <inlib/vec3f>

#include <inlib/rotf>

#include <inlib/colorf>

#include <inlib/rcmp>

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
#include <inlib/sg/bsf>
%template(bsf_bool)       inlib::sg::bsf<bool>;
%template(bsf_float)      inlib::sg::bsf<float>;
%template(bsf_string)     inlib::sg::bsf<std::string>;
%template(bsf_rotf)       inlib::sg::bsf<inlib::rotf>;
%template(bsf_colorf)     inlib::sg::bsf<inlib::colorf>;
%template(bsf_winding_type)  inlib::sg::bsf<inlib::sg::winding_type>;

#include <inlib/sg/sf>
%template(sf_bool)  inlib::sg::sf<bool>;
%template(sf_float) inlib::sg::sf<float>;

#include <inlib/sg/sfs>
%template(sf_vec_colorf) inlib::sg::sf_vec<inlib::colorf,float>;
%template(sf_enum_winding_type) inlib::sg::sf_enum<inlib::sg::winding_type>;

#include <inlib/sg/bmf>
%template(bmf_string)     inlib::sg::bmf<std::string>;
#include <inlib/sg/mf>

#include <inlib/sg/strings>

%template(std_pair_std_string_inlib_colorf)  std::pair<std::string,inlib::colorf>;
#include <inlib/sg/style_color>

%template(std_map_unsigned_int_inlib_sg_style_color)  std::map<unsigned int,inlib::sg::style_color>;
#include <inlib/sg/style_colormap>

%ignore inlib::sg::base_colormap::set_colors; //WIN32 : induces warnings.
#include <inlib/sg/colormap>

#include <inlib/sg/style>
#include <inlib/sg/text_style>
#include <inlib/sg/line_style>

#include <inlib/sg/node>
#include <inlib/sg/write_action>

#include <inlib/sg/group>
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

