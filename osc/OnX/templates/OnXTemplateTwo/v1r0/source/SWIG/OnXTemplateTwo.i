%module OnXTemplateTwo

%include std_string.i
%include std_vector.i

%template() std::vector<std::string>;

%include <Slash/Core/ISession.h>

%include <OnXTemplateTwo/Hello.h>

