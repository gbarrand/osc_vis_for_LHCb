// this :
#include <OnXLab/Inventor/Styles.h>

#include <HEPVis/misc/SoTools.h>
#include <HEPVis/nodekits/SoRegion.h>
#include <HEPVis/nodekits/SoPage.h>

#include <inlib/scolor>

OnXLab::RegionSetParameter::RegionSetParameter(SoRegion* aRegion)
:fSoRegion(aRegion)
{}

OnXLab::RegionSetParameter::RegionSetParameter(const RegionSetParameter& aFrom)
:ISetParameter(aFrom)
,fSoRegion(aFrom.fSoRegion)
{}

OnXLab::RegionSetParameter::~RegionSetParameter(){}

OnXLab::RegionSetParameter& OnXLab::RegionSetParameter::operator=(const RegionSetParameter& aFrom) {
  fSoRegion = aFrom.fSoRegion;
  return *this;
}

bool OnXLab::RegionSetParameter::setParameter(const std::string& aParameter,const std::string& aValue) {
  // aParameter = field then do a set("<field> <value>")
  // aParameter = part.field then do a set("<part>","<field> <value>")
  return (SoTools::setKitPartValue(fSoRegion,aParameter.c_str(),aValue.c_str())==TRUE?true:false);
}

bool OnXLab::RegionSetParameter::parameterValue(const std::string& aParameter,std::string& aValue){
  SbString value;
  SbBool status = SoTools::getKitPartValue(fSoRegion,aParameter.c_str(),value);
  aValue = std::string(value.getString());
  return status==TRUE?true:false;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
OnXLab::PageSetParameter::PageSetParameter(
 SoPage* aPage
)
:fSoPage(aPage)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
OnXLab::PageSetParameter::PageSetParameter(
 const PageSetParameter& aFrom
)
:ISetParameter(aFrom)
,fSoPage(aFrom.fSoPage)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
OnXLab::PageSetParameter::~PageSetParameter(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
OnXLab::PageSetParameter& OnXLab::PageSetParameter::operator=(
 const PageSetParameter& aFrom
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fSoPage = aFrom.fSoPage;
  return *this;
}
void OnXLab::PageSetParameter::setSoPage(SoPage* aSoPage) {fSoPage = aSoPage;}
//////////////////////////////////////////////////////////////////////////////
bool OnXLab::PageSetParameter::setParameter(
 const std::string& aParameter
,const std::string& aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fSoPage) return false;
  return (SoTools::setKitPartValue(fSoPage,aParameter.c_str(),aValue.c_str())==TRUE?true:false);
}
//////////////////////////////////////////////////////////////////////////////
bool OnXLab::PageSetParameter::parameterValue(
 const std::string& aParameter
,std::string& aValue
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(!fSoPage) return false;
  SbString value;
  SbBool status = SoTools::getKitPartValue(fSoPage,aParameter.c_str(),value);
  aValue = std::string(value.getString());
  return status==TRUE?true:false;
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
OnXLab::FillStyle::FillStyle(
 ISetParameter& aSetParameter
,const std::string& aField
)
:fSetParameter(aSetParameter)
,fField(aField)
{
}
OnXLab::FillStyle::~FillStyle(
)
{
}
void OnXLab::FillStyle::reset(
)
{
  //FIXME
}
bool OnXLab::FillStyle::setParameter(
 const std::string& aName
,const std::string& aValue
)
{ 
  return fSetParameter.setParameter(fField+"."+aName,aValue);
}
std::string OnXLab::FillStyle::parameterValue(
 const std::string& aName
) const
{ 
  std::string v;
  fSetParameter.parameterValue(fField+"."+aName,v); 
  return v;
}
std::vector<std::string> OnXLab::FillStyle::availableParameters(
) const
{ 
  return std::vector<std::string>();
}
std::vector<std::string>  OnXLab::FillStyle::availableParameterOptions(
 const std::string& /*paramName*/
) const{ 
  return std::vector<std::string>();
}
std::vector<std::string> OnXLab::FillStyle::availableColors(
) const
{ 
  return std::vector<std::string>();
}
std::string OnXLab::FillStyle::color(
) const
{ 
  std::string v;
  fSetParameter.parameterValue(fField+".color",v);
  return v;
}
double OnXLab::FillStyle::opacity(
) const
{
  std::string s;
  fSetParameter.parameterValue(fField+".transparency",s);
  double v;
  inlib::to<double>(s,v);
  return v;
}

bool OnXLab::FillStyle::setColor(const std::string& aValue){
  std::string v;
  inlib::to_rgbs(aValue,v);
  return fSetParameter.setParameter(fField+".color",v);
}
bool OnXLab::FillStyle::setOpacity(
 double aValue
)
{
  return fSetParameter.setParameter(fField+".transparency",
    inlib::to<double>(1-aValue));
}
std::vector<std::string>  OnXLab::FillStyle::availablePatterns(
) const
{
  return std::vector<std::string>();
}
std::string OnXLab::FillStyle::pattern(
) const
{
  std::string v;
  fSetParameter.parameterValue(fField+".areaStyle",v);
  return v;
}
bool OnXLab::FillStyle::setPattern(
 const std::string& aValue
)
{
  return fSetParameter.setParameter(fField+".areaStyle",aValue);
}

//AIDA-v3r3p0
std::string OnXLab::FillStyle::name() const {
  return ""; //FIXME
}
std::string OnXLab::FillStyle::type() const {
  return ""; //FIXME
}
AIDA::IBaseStyle* OnXLab::FillStyle::child(const std::string& /*aName*/) const {
  return 0; //FIXME
}
std::vector<AIDA::IBaseStyle*> OnXLab::FillStyle::children() const {
  return std::vector<AIDA::IBaseStyle*>(); //FIXME
}
bool OnXLab::FillStyle::setVisible(bool /*aVisible*/) {
  return false; //FIXME
}
bool OnXLab::FillStyle::isVisible() const {
  return false; //FIXME
}
void* OnXLab::FillStyle::cast(const std::string& /*aClass*/) const {
  return 0; //FIXME
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
OnXLab::LineStyle::LineStyle(
 ISetParameter& aSetParameter
,const std::string& aField
)
:fSetParameter(aSetParameter)
,fField(aField)
{}

OnXLab::LineStyle::~LineStyle(){}
void OnXLab::LineStyle::reset(){
  //FIXME
}

bool OnXLab::LineStyle::setParameter(const std::string& aName,const std::string& aValue){ 
  return fSetParameter.setParameter(fField+"."+aName,aValue);
}

std::string OnXLab::LineStyle::parameterValue(const std::string& aName) const {
  std::string v;
  fSetParameter.parameterValue(fField+"."+aName,v); 
  return v;
}

std::vector<std::string> OnXLab::LineStyle::availableParameters() const { 
  return std::vector<std::string>();
}

std::vector<std::string>  OnXLab::LineStyle::availableParameterOptions(const std::string& /*paramName*/ ) const{ 
  return std::vector<std::string>();
}

std::vector<std::string> OnXLab::LineStyle::availableColors() const { 
  return std::vector<std::string>();
}

std::string OnXLab::LineStyle::color() const { 
  std::string v;
  fSetParameter.parameterValue(fField+".color",v);
  return v;
}

double OnXLab::LineStyle::opacity() const {
  std::string s;
  fSetParameter.parameterValue(fField+".transparency",s);
  double v;
  inlib::to<double>(s,v);
  return v;
}

bool OnXLab::LineStyle::setColor(const std::string& aValue){
  std::string v;
  inlib::to_rgbs(aValue,v);
  return fSetParameter.setParameter(fField+".color",v);
}

bool OnXLab::LineStyle::setOpacity(double aValue) {
  return fSetParameter.setParameter(fField+".transparency",
				    inlib::to<double>(1-aValue));
}

std::vector<std::string> OnXLab::LineStyle::availableLineTypes() const {
  return std::vector<std::string>();
}

std::string OnXLab::LineStyle::lineType() const {
  std::string v;
  fSetParameter.parameterValue(fField+".pattern",v);
  return v;
}

int OnXLab::LineStyle::thickness() const {
  std::string s;
  fSetParameter.parameterValue(fField+".width",s);
  int v;
  inlib::to<int>(s,v);
  return v;
}

bool OnXLab::LineStyle::setLineType(const std::string& aValue) {
  return fSetParameter.setParameter(fField+".pattern",aValue);
}

bool OnXLab::LineStyle::setThickness(int aValue) {
  return fSetParameter.setParameter(fField+".width",inlib::to<int>(aValue));
}

//AIDA-v3r3p0
std::string OnXLab::LineStyle::name() const {
  return ""; //FIXME
}
std::string OnXLab::LineStyle::type() const {
  return ""; //FIXME
}
AIDA::IBaseStyle* OnXLab::LineStyle::child(const std::string& /*aName*/) const {
  return 0; //FIXME
}
std::vector<AIDA::IBaseStyle*> OnXLab::LineStyle::children() const {
  return std::vector<AIDA::IBaseStyle*>(); //FIXME
}
bool OnXLab::LineStyle::setVisible(bool /*aVisible*/) {
  return false; //FIXME
}
bool OnXLab::LineStyle::isVisible() const {
  return false; //FIXME
}
void* OnXLab::LineStyle::cast(const std::string& /*aClass*/) const {
  return 0; //FIXME
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
OnXLab::MarkerStyle::MarkerStyle(
 ISetParameter& aSetParameter
,const std::string& aField
)
:fSetParameter(aSetParameter)
,fField(aField)
{
}
//////////////////////////////////////////////////////////////////////////////
OnXLab::MarkerStyle::~MarkerStyle(
)
{
}
void OnXLab::MarkerStyle::reset(
)
{
  //FIXME
}
bool OnXLab::MarkerStyle::setParameter(
 const std::string& aName
,const std::string& aValue
)
{ 
  return fSetParameter.setParameter(fField+"."+aName,aValue);
}
std::string OnXLab::MarkerStyle::parameterValue(
 const std::string& aName
) const
{  
  std::string v;
  fSetParameter.parameterValue(fField+"."+aName,v); 
  return v;
}
std::vector<std::string> OnXLab::MarkerStyle::availableParameters(
) const
{ 
  return std::vector<std::string>();
}
std::vector<std::string>  OnXLab::MarkerStyle::availableParameterOptions(
 const std::string& /*paramName*/
) const{ 
  return std::vector<std::string>();
}
std::vector<std::string> OnXLab::MarkerStyle::availableColors(
) const
{ 
  return std::vector<std::string>();
}
std::string OnXLab::MarkerStyle::color(
) const
{ 
  std::string v;
  fSetParameter.parameterValue(fField+".color",v);
  return v;
}
double OnXLab::MarkerStyle::opacity() const {
  std::string s;
  fSetParameter.parameterValue(fField+".transparency",s);
  double v;
  inlib::to<double>(s,v);
  return v;
}
bool OnXLab::MarkerStyle::setColor(
 const std::string& aValue
)
{
  std::string v;
  inlib::to_rgbs(aValue,v);
  return fSetParameter.setParameter(fField+".color",v);
}
bool OnXLab::MarkerStyle::setOpacity(
 double aValue
)
{
  return fSetParameter.setParameter(fField+".transparency",
    inlib::to<double>(1-aValue));
}
std::vector<std::string> OnXLab::MarkerStyle::availableShapes(
) const
{
  return std::vector<std::string>();
}
std::string OnXLab::MarkerStyle::shape(
) const
{
  std::string v;
  fSetParameter.parameterValue(fField+".markerStyle",v);
  return v;
}
bool OnXLab::MarkerStyle::setShape(
 const std::string& aValue
)
{
  return fSetParameter.setParameter(fField+".markerStyle",aValue);
}
//AIDA-v3r3p0
std::string OnXLab::MarkerStyle::name() const {
  return ""; //FIXME
}
std::string OnXLab::MarkerStyle::type() const {
  return ""; //FIXME
}
AIDA::IBaseStyle* OnXLab::MarkerStyle::child(const std::string& /*aName*/) const {
  return 0; //FIXME
}
std::vector<AIDA::IBaseStyle*> OnXLab::MarkerStyle::children() const {
  return std::vector<AIDA::IBaseStyle*>(); //FIXME
}
bool OnXLab::MarkerStyle::setVisible(bool /*aVisible*/) {
  return false; //FIXME
}
bool OnXLab::MarkerStyle::isVisible() const {
  return false; //FIXME
}
void* OnXLab::MarkerStyle::cast(const std::string& /*aClass*/) const {
  return 0; //FIXME
}

bool OnXLab::MarkerStyle::setSize(int /*size*/) {
  return false; //FIXME
}
int OnXLab::MarkerStyle::size() const {
  return 0; //FIXME
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
OnXLab::TextStyle::TextStyle(
 ISetParameter& aSetParameter
,const std::string& aField
)
:fSetParameter(aSetParameter)
,fField(aField)
{
}
OnXLab::TextStyle::~TextStyle(
)
{
}
void OnXLab::TextStyle::reset(
)
{
  //FIXME
}
bool OnXLab::TextStyle::setParameter(
 const std::string& aName
,const std::string& aValue
)
{ 
  return fSetParameter.setParameter(fField+"."+aName,aValue);
}
std::string OnXLab::TextStyle::parameterValue(
 const std::string& aName
) const
{  
  std::string v;
  fSetParameter.parameterValue(fField+"."+aName,v); 
  return v;
}
std::vector<std::string> OnXLab::TextStyle::availableParameters(
) const
{ 
  return std::vector<std::string>();
}
std::vector<std::string>  OnXLab::TextStyle::availableParameterOptions(
 const std::string& /*paramName*/
) const{ 
  return std::vector<std::string>();
}
std::vector<std::string> OnXLab::TextStyle::availableColors(
) const
{ 
  return std::vector<std::string>();
}
std::string OnXLab::TextStyle::color(
) const
{ 
  std::string v;
  fSetParameter.parameterValue(fField+".color",v);
  return v;
}
double OnXLab::TextStyle::opacity(
) const
{
  std::string s;
  fSetParameter.parameterValue(fField+".transparency",s);
  double v;
  inlib::to<double>(s,v);
  return v;
}
bool OnXLab::TextStyle::setColor(
 const std::string& aValue
)
{
  std::string v;
  inlib::to_rgbs(aValue,v);
  return fSetParameter.setParameter(fField+".color",v);
}
bool OnXLab::TextStyle::setOpacity(
 double aValue
)
{
  return fSetParameter.setParameter(fField+".transparency",
    inlib::to<double>(1-aValue));
}
std::vector<std::string> OnXLab::TextStyle::availableFonts(
) const
{
  return std::vector<std::string>();
}
double OnXLab::TextStyle::fontSize(
) const
{
  std::string s;
  fSetParameter.parameterValue(fField+".fontSize",s);
  double v;
  inlib::to<double>(s,v);
  return v;
}
bool OnXLab::TextStyle::setFontSize(double aValue) {
  return fSetParameter.setParameter(fField+".fontSize",
    inlib::to<int>(int(aValue)));
}
std::string OnXLab::TextStyle::font(
) const
{
  std::string v;
  fSetParameter.parameterValue(fField+".fontName",v);
  return v;
}
bool OnXLab::TextStyle::setFont(
 const std::string& aFont
)
{
  return fSetParameter.setParameter(fField+".fontName",aFont);
}
bool OnXLab::TextStyle::isBold(
) const
{
  return false;
}
bool OnXLab::TextStyle::isItalic(
) const
{
  return false;
}
bool OnXLab::TextStyle::isUnderlined(
) const
{
  return false;
}
bool OnXLab::TextStyle::setBold(
 bool /*bold*/
)
{
  return false;
}
bool OnXLab::TextStyle::setItalic(
 bool /*italic*/
)
{
  return false;
}
bool OnXLab::TextStyle::setUnderlined(
 bool /*underlined*/
)
{
  return false;
}
//AIDA-v3r3p0
std::string OnXLab::TextStyle::name() const {
  return ""; //FIXME
}
std::string OnXLab::TextStyle::type() const {
  return ""; //FIXME
}
AIDA::IBaseStyle* OnXLab::TextStyle::child(const std::string& /*aName*/) const {
  return 0; //FIXME
}
std::vector<AIDA::IBaseStyle*> OnXLab::TextStyle::children() const {
  return std::vector<AIDA::IBaseStyle*>(); //FIXME
}
bool OnXLab::TextStyle::setVisible(bool /*aVisible*/) {
  return false; //FIXME
}
bool OnXLab::TextStyle::isVisible() const {
  return false; //FIXME
}
void* OnXLab::TextStyle::cast(const std::string& /*aClass*/) const {
  return 0; //FIXME
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
OnXLab::TitleStyle::TitleStyle(
 ISetParameter& aSetParameter
,const std::string& aField
)
:fSetParameter(aSetParameter)
,fField(aField)
,fTextStyle(aSetParameter,aField)
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
#else
,fBoxStyle(aSetParameter,aField)
#endif
{
}
OnXLab::TitleStyle::~TitleStyle(
)
{
}
void OnXLab::TitleStyle::reset(
)
{
  //FIXME
}
bool OnXLab::TitleStyle::setParameter(
 const std::string& aName
,const std::string& aValue
)
{ 
  return fSetParameter.setParameter(fField+"."+aName,aValue);
}
std::string OnXLab::TitleStyle::parameterValue(
 const std::string& aName
) const
{  
  std::string v;
  fSetParameter.parameterValue(fField+"."+aName,v); 
  return v;
}
std::vector<std::string> OnXLab::TitleStyle::availableParameters(
) const
{ 
  return std::vector<std::string>();
}
std::vector<std::string>  OnXLab::TitleStyle::availableParameterOptions(
 const std::string& /*paramName*/
) const{ 
  return std::vector<std::string>();
}
AIDA::ITextStyle & OnXLab::TitleStyle::textStyle(
)
{
  return fTextStyle;
}
bool OnXLab::TitleStyle::setTextStyle(
 const AIDA::ITextStyle& /*aTextStyle*/
)
{
  return false;
}
//AIDA-v3r3p0
std::string OnXLab::TitleStyle::name() const {
  return ""; //FIXME
}
std::string OnXLab::TitleStyle::type() const {
  return ""; //FIXME
}
AIDA::IBaseStyle* OnXLab::TitleStyle::child(const std::string& /*aName*/) const {
  return 0; //FIXME
}
std::vector<AIDA::IBaseStyle*> OnXLab::TitleStyle::children() const {
  return std::vector<AIDA::IBaseStyle*>(); //FIXME
}
bool OnXLab::TitleStyle::setVisible(bool /*aVisible*/) {
  return false; //FIXME
}
bool OnXLab::TitleStyle::isVisible() const {
  return false; //FIXME
}
void* OnXLab::TitleStyle::cast(const std::string& /*aClass*/) const {
  return 0; //FIXME
}
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
#else
bool OnXLab::TitleStyle::setBoxStyle(const AIDA::IBoxStyle & ) {
  return false;
}
AIDA::IBoxStyle & OnXLab::TitleStyle::boxStyle() { return fBoxStyle;}
#endif
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
OnXLab::AxisStyle::AxisStyle(
 ISetParameter& aSetParameter
,const std::string& aField
)
:fSetParameter(aSetParameter)
,fField(aField)
,fLineStyle(aSetParameter,aField+".lineStyle")
,fTicksStyle(aSetParameter,aField+".ticksStyle")
,fLabelsStyle(aSetParameter,aField+".labelsStyle")
,fTitleStyle(aSetParameter,aField+".titleStyle")
{
}
OnXLab::AxisStyle::~AxisStyle(
) 
{
}
void OnXLab::AxisStyle::reset(
)
{
  //FIXME
}
bool OnXLab::AxisStyle::setParameter(
 const std::string& aName
,const std::string& aValue
)
{ 
  return fSetParameter.setParameter(fField+"."+aName,aValue);
}
std::string OnXLab::AxisStyle::parameterValue(
 const std::string& aName
) const
{  
  std::string v;
  fSetParameter.parameterValue(fField+"."+aName,v); 
  return v;
}
std::vector<std::string> OnXLab::AxisStyle::availableParameters(
) const
{ 
  return std::vector<std::string>();
}
std::vector<std::string>  OnXLab::AxisStyle::availableParameterOptions(
 const std::string& /*paramName*/
) const{ 
  return std::vector<std::string>();
}
AIDA::ILineStyle& OnXLab::AxisStyle::lineStyle(
)
{
  return fLineStyle;
}
AIDA::ITextStyle& OnXLab::AxisStyle::tickLabelStyle(
)
{
  return fLabelsStyle;
}
AIDA::ITextStyle& OnXLab::AxisStyle::labelStyle(
)
{
  return fTitleStyle;
}
bool OnXLab::AxisStyle::setlineStyle(
 const AIDA::ILineStyle& /*lineStyle*/
)
{
  return false;
}
bool OnXLab::AxisStyle::setLineStyle(
 const AIDA::ILineStyle& /*lineStyle*/
)
{
  return false;
}
bool OnXLab::AxisStyle::setTickLabelStyle(
 const AIDA::ITextStyle& /*tickLabelStyle*/
)
{
  return false;
}
bool OnXLab::AxisStyle::setLabelStyle(
 const AIDA::ITextStyle& /*labelStyle*/
)
{
  return false;
}
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
void OnXLab::AxisStyle::setLabel(
 const std::string& aLabel
)
{
  fSetParameter.setParameter(fField+".title",aLabel);
}
#else
bool OnXLab::AxisStyle::setLabel(
 const std::string& aLabel
)
{
  return fSetParameter.setParameter(fField+".title",aLabel);
}
#endif
bool OnXLab::AxisStyle::setScaling(const std::string& /*scaling*/) {
  return false; //FIXME
}
std::string OnXLab::AxisStyle::scaling() const {
  return ""; //FIXME  
}
//AIDA-v3r3p0
std::string OnXLab::AxisStyle::name() const {
  return ""; //FIXME
}
std::string OnXLab::AxisStyle::type() const {
  return ""; //FIXME
}
AIDA::IBaseStyle* OnXLab::AxisStyle::child(const std::string& /*aName*/) const {
  return 0; //FIXME
}
std::vector<AIDA::IBaseStyle*> OnXLab::AxisStyle::children() const {
  return std::vector<AIDA::IBaseStyle*>(); //FIXME
}
bool OnXLab::AxisStyle::setVisible(bool /*aVisible*/) {
  return false; //FIXME
}
bool OnXLab::AxisStyle::isVisible() const {
  return false; //FIXME
}
void* OnXLab::AxisStyle::cast(const std::string& /*aClass*/) const {
  return 0; //FIXME
}
std::string OnXLab::AxisStyle::label() const { return "";}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
OnXLab::DataStyle::DataStyle(
 ISetParameter& aSetParameter
,const std::string& aField
)
:fSetParameter(aSetParameter)
,fField(aField)
,fLineStyle(aSetParameter,fField)
,fMarkerStyle(aSetParameter,fField)
,fFillStyle(aSetParameter,fField)
,fErrorBarStyle(aSetParameter,fField) //FIXME
,fOutlineStyle(aSetParameter,fField) //FIXME
{
}
OnXLab::DataStyle::~DataStyle(
)
{
}
void OnXLab::DataStyle::reset(
)
{
  //FIXME
}
bool OnXLab::DataStyle::setParameter(
 const std::string& aName
,const std::string& aValue
)
{ 
  return fSetParameter.setParameter(fField+"."+aName,aValue);
}
std::string OnXLab::DataStyle::parameterValue(
 const std::string& aName
) const
{  
  std::string v;
  fSetParameter.parameterValue(fField+"."+aName,v); 
  return v;
}
std::vector<std::string> OnXLab::DataStyle::availableParameters(
) const
{ 
  return std::vector<std::string>();
}
std::vector<std::string>  OnXLab::DataStyle::availableParameterOptions(
 const std::string& /*paramName*/
) const{ 
  return std::vector<std::string>();
}
AIDA::ILineStyle& OnXLab::DataStyle::lineStyle(
)
{
  return fLineStyle;
}
AIDA::IMarkerStyle& OnXLab::DataStyle::markerStyle(
)
{
  return fMarkerStyle;
}
AIDA::IFillStyle& OnXLab::DataStyle::fillStyle(
)
{
  return fFillStyle;
}
bool OnXLab::DataStyle::setLineStyle(
 const AIDA::ILineStyle& /*lineStyle*/
)
{
  return false;
}
bool OnXLab::DataStyle::setMarkerStyle(
 const AIDA::IMarkerStyle& /*markerStyle*/
)
{
  return false;
}
bool OnXLab::DataStyle::setFillStyle(
 const AIDA::IFillStyle& /*fillStyle*/
)
{
  return false;
}
//AIDA-v3r3p0
std::string OnXLab::DataStyle::name() const {
  return ""; //FIXME
}
std::string OnXLab::DataStyle::type() const {
  return ""; //FIXME
}
AIDA::IBaseStyle* OnXLab::DataStyle::child(const std::string& /*aName*/) const {
  return 0; //FIXME
}
std::vector<AIDA::IBaseStyle*> OnXLab::DataStyle::children() const {
  return std::vector<AIDA::IBaseStyle*>(); //FIXME
}
bool OnXLab::DataStyle::setVisible(bool /*aVisible*/) {
  return false; //FIXME
}
bool OnXLab::DataStyle::isVisible() const {
  return false; //FIXME
}
void* OnXLab::DataStyle::cast(const std::string& /*aClass*/) const {
  return 0; //FIXME
}
AIDA::ILineStyle& OnXLab::DataStyle::errorBarStyle() {
  return fErrorBarStyle;
}
AIDA::ILineStyle& OnXLab::DataStyle::outlineStyle() {
  return fOutlineStyle;
}
bool OnXLab::DataStyle::setErrorBarStyle(const AIDA::ILineStyle&) {
  return false; //FIXME
}
bool OnXLab::DataStyle::setOutlineStyle(const AIDA::ILineStyle&){
  return false; //FIXME
}
bool OnXLab::DataStyle::setModel(const std::string&) { return false;}
std::string OnXLab::DataStyle::model() const { return "";}
void OnXLab::DataStyle::showInStatisticsBox(bool) { }
bool OnXLab::DataStyle::isShownInStatisticsBox() const { return false;}
void OnXLab::DataStyle::showInLegendBox(bool) {}
bool OnXLab::DataStyle::isShownInLegendBox() const { return false;}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
OnXLab::InfoStyle::InfoStyle(
 ISetParameter& aSetParameter
,const std::string& aField
)
:fSetParameter(aSetParameter)
,fField(aField)
,fTextStyle(aSetParameter,aField)
{
}
OnXLab::InfoStyle::~InfoStyle(
)
{
}
void OnXLab::InfoStyle::reset(
)
{
  //FIXME
}
bool OnXLab::InfoStyle::setParameter(
 const std::string& aName
,const std::string& aValue
)
{ 
  return fSetParameter.setParameter(fField+"."+aName,aValue);
}
std::string OnXLab::InfoStyle::parameterValue(
 const std::string& aName
) const
{  
  std::string v;
  fSetParameter.parameterValue(fField+"."+aName,v); 
  return v;
}
std::vector<std::string> OnXLab::InfoStyle::availableParameters(
) const
{ 
  return std::vector<std::string>();
}
std::vector<std::string>  OnXLab::InfoStyle::availableParameterOptions(
 const std::string& /*paramName*/
) const{ 
  return std::vector<std::string>();
}
AIDA::ITextStyle& OnXLab::InfoStyle::textStyle(
)
{
  return fTextStyle;
}
bool OnXLab::InfoStyle::setTextStyle(
 const AIDA::ITextStyle& /*textStyle*/
)
{
  return false;
}
//AIDA-v3r3p0
std::string OnXLab::InfoStyle::name() const {
  return ""; //FIXME
}
std::string OnXLab::InfoStyle::type() const {
  return ""; //FIXME
}
AIDA::IBaseStyle* OnXLab::InfoStyle::child(const std::string& /*aName*/) const {
  return 0; //FIXME
}
std::vector<AIDA::IBaseStyle*> OnXLab::InfoStyle::children() const {
  return std::vector<AIDA::IBaseStyle*>(); //FIXME
}
bool OnXLab::InfoStyle::setVisible(bool /*aVisible*/) {
  return false; //FIXME
}
bool OnXLab::InfoStyle::isVisible() const {
  return false; //FIXME
}
void* OnXLab::InfoStyle::cast(const std::string& /*aClass*/) const {
  return 0; //FIXME
}
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
#else
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
OnXLab::LegendBoxStyle::LegendBoxStyle(
 ISetParameter& aSetParameter
,const std::string& aField
)
:fSetParameter(aSetParameter)
,fField(aField)
,fTextStyle(aSetParameter,aField)
,fBoxStyle(aSetParameter,aField)
{
}
OnXLab::LegendBoxStyle::~LegendBoxStyle(
)
{
}
void OnXLab::LegendBoxStyle::reset(
)
{
  //FIXME
}
bool OnXLab::LegendBoxStyle::setParameter(
 const std::string& aName
,const std::string& aValue
)
{ 
  return fSetParameter.setParameter(fField+"."+aName,aValue);
}
std::string OnXLab::LegendBoxStyle::parameterValue(
 const std::string& aName
) const
{  
  std::string v;
  fSetParameter.parameterValue(fField+"."+aName,v); 
  return v;
}
std::vector<std::string> OnXLab::LegendBoxStyle::availableParameters(
) const
{ 
  return std::vector<std::string>();
}
std::vector<std::string>  OnXLab::LegendBoxStyle::availableParameterOptions(
 const std::string& /*paramName*/
) const{ 
  return std::vector<std::string>();
}
AIDA::ITextStyle& OnXLab::LegendBoxStyle::textStyle(
)
{
  return fTextStyle;
}
bool OnXLab::LegendBoxStyle::setTextStyle(
 const AIDA::ITextStyle& /*textStyle*/
)
{
  return false;
}
AIDA::IBoxStyle& OnXLab::LegendBoxStyle::boxStyle(
)
{
  return fBoxStyle;
}
bool OnXLab::LegendBoxStyle::setBoxStyle(
 const AIDA::IBoxStyle& /*textStyle*/
)
{
  return false;
}
//AIDA-v3r3p0
std::string OnXLab::LegendBoxStyle::name() const {
  return ""; //FIXME
}
std::string OnXLab::LegendBoxStyle::type() const {
  return ""; //FIXME
}
AIDA::IBaseStyle* OnXLab::LegendBoxStyle::child(const std::string& /*aName*/) const {
  return 0; //FIXME
}
std::vector<AIDA::IBaseStyle*> OnXLab::LegendBoxStyle::children() const {
  return std::vector<AIDA::IBaseStyle*>(); //FIXME
}
bool OnXLab::LegendBoxStyle::setVisible(bool /*aVisible*/) {
  return false; //FIXME
}
bool OnXLab::LegendBoxStyle::isVisible() const {
  return false; //FIXME
}
void* OnXLab::LegendBoxStyle::cast(const std::string& /*aClass*/) const {
  return 0; //FIXME
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
OnXLab::StatisticsBoxStyle::StatisticsBoxStyle(
 ISetParameter& aSetParameter
,const std::string& aField
)
:fSetParameter(aSetParameter)
,fField(aField)
,fTextStyle(aSetParameter,aField)
,fBoxStyle(aSetParameter,aField)
{
}
OnXLab::StatisticsBoxStyle::~StatisticsBoxStyle(
)
{
}
void OnXLab::StatisticsBoxStyle::reset(
)
{
  //FIXME
}
bool OnXLab::StatisticsBoxStyle::setParameter(
 const std::string& aName
,const std::string& aValue
)
{ 
  return fSetParameter.setParameter(fField+"."+aName,aValue);
}
std::string OnXLab::StatisticsBoxStyle::parameterValue(
 const std::string& aName
) const
{  
  std::string v;
  fSetParameter.parameterValue(fField+"."+aName,v); 
  return v;
}
std::vector<std::string> OnXLab::StatisticsBoxStyle::availableParameters(
) const
{ 
  return std::vector<std::string>();
}
std::vector<std::string>  OnXLab::StatisticsBoxStyle::availableParameterOptions(
 const std::string& /*paramName*/
) const{ 
  return std::vector<std::string>();
}
bool OnXLab::StatisticsBoxStyle::setVisibileStatistics(const std::string&) { 
  return false;
}
std::string OnXLab::StatisticsBoxStyle::visibleStatistics() const { 
  return "";
}
AIDA::ITextStyle& OnXLab::StatisticsBoxStyle::textStyle(
)
{
  return fTextStyle;
}
bool OnXLab::StatisticsBoxStyle::setTextStyle(
 const AIDA::ITextStyle& /*textStyle*/
)
{
  return false;
}
AIDA::IBoxStyle& OnXLab::StatisticsBoxStyle::boxStyle(
)
{
  return fBoxStyle;
}
bool OnXLab::StatisticsBoxStyle::setBoxStyle(
 const AIDA::IBoxStyle& /*textStyle*/
)
{
  return false;
}
//AIDA-v3r3p0
std::string OnXLab::StatisticsBoxStyle::name() const {
  return ""; //FIXME
}
std::string OnXLab::StatisticsBoxStyle::type() const {
  return ""; //FIXME
}
AIDA::IBaseStyle* OnXLab::StatisticsBoxStyle::child(const std::string& /*aName*/) const {
  return 0; //FIXME
}
std::vector<AIDA::IBaseStyle*> OnXLab::StatisticsBoxStyle::children() const {
  return std::vector<AIDA::IBaseStyle*>(); //FIXME
}
bool OnXLab::StatisticsBoxStyle::setVisible(bool /*aVisible*/) {
  return false; //FIXME
}
bool OnXLab::StatisticsBoxStyle::isVisible() const {
  return false; //FIXME
}
void* OnXLab::StatisticsBoxStyle::cast(const std::string& /*aClass*/) const {
  return 0; //FIXME
}
#endif
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
OnXLab::PlotterStyle::PlotterStyle(
 ISetParameter& aSetParameter
,const std::string& aField
)
:fSetParameter(aSetParameter)
,fField(aField)
//FIXME : pointsStyle, functionStyle, etc...
,fDataStyle(aSetParameter,aField+".binsStyle[0]")
,f_xAxisStyle(aSetParameter,aField+".xAxis")
,f_yAxisStyle(aSetParameter,aField+".yAxis")
,f_zAxisStyle(aSetParameter,aField+".zAxis")
,fTitleStyle(aSetParameter,aField+".titleStyle")
,fInfoStyle(aSetParameter,aField+".infosStyle")
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
#else
,fLegendBoxStyle(aSetParameter,aField+".legendStyle")
,fStatisticsBoxStyle(aSetParameter,aField+".statisticsBoxStyle")
,fGridStyle(aSetParameter,aField+".gridStyle")
,fRegionBoxStyle(aSetParameter,aField+".regionBoxStyle")
,fDataBoxStyle(aSetParameter,aField+".dataBoxStyle")
#endif
{
}
OnXLab::PlotterStyle::~PlotterStyle(
)
{
}
void OnXLab::PlotterStyle::reset(
)
{
  //FIXME
}
bool OnXLab::PlotterStyle::setParameter(
 const std::string& aName
,const std::string& aValue
)
{ 
  return fSetParameter.setParameter(fField+"."+aName,aValue);
}
std::string OnXLab::PlotterStyle::parameterValue(
 const std::string& aName
) const
{  
  std::string v;
  fSetParameter.parameterValue(fField+"."+aName,v); 
  return v;
}
std::vector<std::string> OnXLab::PlotterStyle::availableParameters(
) const
{ 
  return std::vector<std::string>();
}
std::vector<std::string>  OnXLab::PlotterStyle::availableParameterOptions(
 const std::string& /*paramName*/
) const{ 
  return std::vector<std::string>();
}
AIDA::IDataStyle& OnXLab::PlotterStyle::dataStyle(
)
{
  return fDataStyle;
}
AIDA::IAxisStyle& OnXLab::PlotterStyle::xAxisStyle(
)
{
  return f_xAxisStyle;
}
AIDA::IAxisStyle& OnXLab::PlotterStyle::yAxisStyle(
)
{
  return f_yAxisStyle;
}
AIDA::IAxisStyle& OnXLab::PlotterStyle::zAxisStyle(
)
{
  return f_zAxisStyle;
}
AIDA::ITitleStyle& OnXLab::PlotterStyle::titleStyle(
)
{
  return fTitleStyle;
}
AIDA::IInfoStyle& OnXLab::PlotterStyle::infoStyle(
)
{
  return fInfoStyle;
}
bool OnXLab::PlotterStyle::setDataStyle(
 const AIDA::IDataStyle& /*dataStyle*/
)
{
  return false;
}
bool OnXLab::PlotterStyle::setAxisStyleX(
 const AIDA::IAxisStyle& /*xAxisStyle*/
)
{
  return false;
}
bool OnXLab::PlotterStyle::setAxisStyleY(
 const AIDA::IAxisStyle& /*yAxisStyle*/
)
{
  return false;
}
bool OnXLab::PlotterStyle::setAxisStyleZ(
 const AIDA::IAxisStyle& /*zAxisStyle*/
)
{
  return false;
}
bool OnXLab::PlotterStyle::setTitleStyle(
 const AIDA::ITitleStyle& /*textStyle*/
)
{
  return false;
}
bool OnXLab::PlotterStyle::setInfoStyle(
 const AIDA::IInfoStyle& /*infoStyle*/
)
{
  return false;
}
//AIDA-v3r3p0
std::string OnXLab::PlotterStyle::name() const {
  return ""; //FIXME
}
std::string OnXLab::PlotterStyle::type() const {
  return ""; //FIXME
}
AIDA::IBaseStyle* OnXLab::PlotterStyle::child(const std::string& /*aName*/) const {
  return 0; //FIXME
}
std::vector<AIDA::IBaseStyle*> OnXLab::PlotterStyle::children() const {
  return std::vector<AIDA::IBaseStyle*>(); //FIXME
}
bool OnXLab::PlotterStyle::setVisible(bool /*aVisible*/) {
  return false; //FIXME
}
bool OnXLab::PlotterStyle::isVisible() const {
  return false; //FIXME
}
void* OnXLab::PlotterStyle::cast(const std::string& /*aClass*/) const {
  return 0; //FIXME
}
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
#else
AIDA::ILegendBoxStyle& OnXLab::PlotterStyle::legendBoxStyle() {
  return fLegendBoxStyle;
}
AIDA::IStatisticsBoxStyle& OnXLab::PlotterStyle::statisticsBoxStyle() {
  return fStatisticsBoxStyle;
}
AIDA::IGridStyle& OnXLab::PlotterStyle::gridStyle() {
  return fGridStyle;
}
AIDA::IBoxStyle& OnXLab::PlotterStyle::regionBoxStyle() {
  return fRegionBoxStyle;
}
AIDA::IBoxStyle& OnXLab::PlotterStyle::dataBoxStyle() {
  return fDataBoxStyle;
}
bool OnXLab::PlotterStyle::setLegendBoxStyle(const AIDA::ILegendBoxStyle&) {
  return false;
}
bool OnXLab::PlotterStyle::setStatisticsBoxStyle(const AIDA::IStatisticsBoxStyle&) {
  return false; 
}
bool OnXLab::PlotterStyle::setGridStyle(const AIDA::IGridStyle&) {
  return false;
}
bool OnXLab::PlotterStyle::setRegionBoxStyle(const AIDA::IBoxStyle&) {
  return false;
}
bool OnXLab::PlotterStyle::setDataBoxStyle(const AIDA::IBoxStyle&) {
  return false;
}
#endif
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
OnXLab::PlotterLayout::PlotterLayout(
 ISetParameter& aSetParameter
,const std::string& aField
)
:fSetParameter(aSetParameter)
,fField(aField)
{
}
void OnXLab::PlotterLayout::reset(
)
{
  //FIXME
}
bool OnXLab::PlotterLayout::setParameter(const std::string& aName,double aValue){  
  return fSetParameter.setParameter(fField+"."+aName,inlib::to<double>(aValue));
}
double OnXLab::PlotterLayout::parameterValue(const std::string& aName){
  std::string s;
  fSetParameter.parameterValue(fField+"."+aName,s);
  double v;
  inlib::to<double>(s,v);
  return v;
}
std::vector<std::string> OnXLab::PlotterLayout::availableParameters(
) const
{
  return std::vector<std::string>();
}
OnXLab::PlotterLayout::~PlotterLayout(
) 
{
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void OnXLab::Info::clear(
)
{
}
void OnXLab::Info::addText(
 const std::string& /*text*/
)
{
}
void OnXLab::Info::addLegend(
 const AIDA::IMarkerStyle& /*style*/
,const std::string& /*description*/
)
{
}
void OnXLab::Info::addLegend(
 const AIDA::ILineStyle& /*style*/
,const std::string& /*description*/
)
{
}
void OnXLab::Info::addLegend(
 const AIDA::IFillStyle& /*style*/
,const std::string& /*description*/
)
{
}
OnXLab::Info::~Info(
) 
{
}
#if (AIDA_VERSION_MAJOR<=3) && (AIDA_VERSION_MINOR<=2) 
#else
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
OnXLab::BoxStyle::BoxStyle(
 ISetParameter& aSetParameter
,const std::string& aField
)
:fSetParameter(aSetParameter)
,fField(aField)
,fBackgroundStyle(aSetParameter,aField+".backgroundStyle")
,fForegroundStyle(aSetParameter,aField+".foregroundStyle")
,fBorderStyle(aSetParameter,aField+".borderStyle")
{}
OnXLab::BoxStyle::~BoxStyle(){}
void OnXLab::BoxStyle::reset(){}
bool OnXLab::BoxStyle::setParameter(const std::string&, const std::string&){ return false;}
std::string OnXLab::BoxStyle::parameterValue(const std::string&) const { return "";}
std::vector<std::string> OnXLab::BoxStyle::availableParameters() const {
  return std::vector<std::string>();
}
std::vector<std::string> OnXLab::BoxStyle::availableParameterOptions(const std::string&) const {
  return std::vector<std::string>();
}
//AIDA-v3r3p0
std::string OnXLab::BoxStyle::name() const { return "";}
std::string OnXLab::BoxStyle::type() const { return "";}
AIDA::IBaseStyle* OnXLab::BoxStyle::child(const std::string&) const { 
  return 0;
}
std::vector<AIDA::IBaseStyle*> OnXLab::BoxStyle::children() const {
  return std::vector<AIDA::IBaseStyle*>();
}
bool OnXLab::BoxStyle::setVisible(bool) { return false;}
bool OnXLab::BoxStyle::isVisible() const { return false;}
void * OnXLab::BoxStyle::cast(const std::string &) const {
  return 0;
}
AIDA::IFillStyle& OnXLab::BoxStyle::backgroundStyle() {
  return fBackgroundStyle;
}
bool OnXLab::BoxStyle::setBackgroundStyle(const AIDA::IFillStyle&) { return false;}
AIDA::IFillStyle& OnXLab::BoxStyle::foregroundStyle() {  
  return fForegroundStyle;
}
bool OnXLab::BoxStyle::setForegroundStyle(const AIDA::IFillStyle &) {
  return false;
}
AIDA::IBorderStyle& OnXLab::BoxStyle::borderStyle() {
  return fBorderStyle;
}
bool OnXLab::BoxStyle::setBorderStyle(const AIDA::IBorderStyle &){
  return false;
}
bool OnXLab::BoxStyle::setX(double) { return false;}
bool OnXLab::BoxStyle::setY(double) { return false;}
double OnXLab::BoxStyle::x() const { return 0;}
double OnXLab::BoxStyle::y() const { return 0;}
bool OnXLab::BoxStyle::setHeight(double) { return false;}
bool OnXLab::BoxStyle::setWidth(double) { return false;}
double OnXLab::BoxStyle::height() const { return 0;}
double OnXLab::BoxStyle::width() const { return 0;}
bool OnXLab::BoxStyle::setPlacement(int) { return false;}
int OnXLab::BoxStyle::placement() { return 0;}
bool OnXLab::BoxStyle::setUnits(int) { return false;}
int OnXLab::BoxStyle::units() { return 0;}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
OnXLab::BorderStyle::BorderStyle(
 ISetParameter& aSetParameter
,const std::string& aField
)
:LineStyle(aSetParameter,aField)
{}
OnXLab::BorderStyle::~BorderStyle() {}
std::vector<std::string> OnXLab::BorderStyle::availableBorderTypes() const {
  return std::vector<std::string>();
}
bool OnXLab::BorderStyle::setBorderType(const std::string &) {
  return false;
}
std::string OnXLab::BorderStyle::borderType() const { return "";}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
OnXLab::GridStyle::GridStyle(
 ISetParameter& aSetParameter
,const std::string& aField
)
:LineStyle(aSetParameter,aField)
{}
OnXLab::GridStyle::~GridStyle() {}
bool OnXLab::GridStyle::setCellSize(double) { return false;}
double OnXLab::GridStyle::cellSize() const { return 0;}
bool OnXLab::GridStyle::setUnits(int) { return false;}
int OnXLab::GridStyle::units() { return 0;}

#endif
