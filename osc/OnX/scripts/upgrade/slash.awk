{

  if($3 == "none") {
    print "replace\\n" $2 "\\nSlash::" $1 "::" $2 ;
    print "replace\\nclass Slash::" $1 "::" $2 ";\\nnamespace Slash {namespace " $1 " {class " $2 ";}}";
    print "replace\\n<Lib/Interfaces/Slash::" $1 "::" $2 ".h>\\n<Slash/" $1 "/" $2 ".h>";
    print "";
  } else if($3 == "Lib") {
    print "replace\\nLib::" $2 "\\nSlash::" $1 "::" $2 ;
    print "replace\\nnamespace " $3 " {class " $2 ";\\nnamespace Slash {namespace " $1 " {class " $2 ";}";
    print "replace\\n<Lib/Interfaces/" $2 ".h>\\n<Slash/" $1 "/" $2 ".h>";
    print "";
  } else if($3 == "OnX") {
    print "replace\\n" $2 "\\nSlash::" $1 "::" $2 ;
    print "replace\\nclass Slash::" $1 "::" $2 ";\\nnamespace Slash {namespace " $1 " {class " $2 ";}}";
    print "replace\\n<OnX/Interfaces/Slash::" $1 "::" $2 ".h>\\n<Slash/" $1 "/" $2 ".h>";
    print "";
  }
}