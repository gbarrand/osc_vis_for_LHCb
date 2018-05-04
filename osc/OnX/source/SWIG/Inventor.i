//////////////////////////////////////////////////////////////
/// Pre SWIG /////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

// Inspired from Pivy-0.3.0/interfaces/coin.i

%define RefCount(aClass)
  %typemap(newfree) aClass * { 
    //printf("debug : SWIG : newfree : %s %lu\n",#aClass,(unsigned long)$1);
    $1->ref(); 
  }
  %extend aClass { 
    ~aClass() { 
      //printf("debug : SWIG : dstor %s %lu\n",#aClass,(unsigned long)self);
      self->unref(); 
    } 
  }
  %ignore aClass::~aClass();
%enddef

// Must come before the class declaration (because of the typemap).
RefCount(SoBase)
RefCount(SoNode)
RefCount(SoShape)
RefCount(SoBaseKit)
// SoNodes :
//RefCount(SoProtoInstance)
//RefCount(SoProto)
//RefCount(SoLabel)
RefCount(SoNonIndexedShape)
RefCount(SoScale)
RefCount(SoCylinder)
RefCount(SoDirectionalLight)
RefCount(SoTranslation)
//RefCount(SoIndexedTriangleStripSet)
//RefCount(SoRotor)
//RefCount(SoTriangleStripSet)
RefCount(SoLight)
RefCount(SoLightModel)
RefCount(SoRotationXYZ)
//RefCount(SoPointLight)
//RefCount(SoTextureCoordinateBinding)
//RefCount(SoBumpMapTransform)
//RefCount(SoPackedColor)
//RefCount(SoDrawStyle)
//RefCount(SoFaceSet)
//RefCount(SoNurbsSurface)
RefCount(SoMaterial)
//RefCount(SoLinearProfile)
//RefCount(SoIndexedNurbsSurface)
//RefCount(SoSurroundScale)
//RefCount(SoNurbsProfile)
//RefCount(SoText2)
//RefCount(SoText3)
//RefCount(SoCone)
//RefCount(SoVertexProperty)
RefCount(SoCube)
//RefCount(SoAnnotation)
//RefCount(SoFile)
//RefCount(SoSceneTexture2)
RefCount(SoTransformation)
//RefCount(SoFont)
//RefCount(SoInfo)
//RefCount(SoTextureScalePolicy)
//RefCount(SoTransformSeparator)
//RefCount(SoFontStyle)
RefCount(SoOrthographicCamera)
//RefCount(SoComplexity)
RefCount(SoSeparator)
//RefCount(SoWWWInline)
//RefCount(SoPickStyle)
//RefCount(SoBaseColor)
//RefCount(SoMaterialBinding)
//RefCount(SoBumpMapCoordinate)
//RefCount(SoTextureCoordinateEnvironment)
//RefCount(SoBlinker)
//RefCount(SoSelection)
//RefCount(SoPendulum)
//RefCount(SoTextureCoordinateFunction)
//RefCount(SoNormal)
//RefCount(SoUnits)
//RefCount(SoNormalBinding)
//RefCount(SoNurbsCurve)
//RefCount(SoWWWAnchor)
//RefCount(SoBumpMap)
RefCount(SoGroup)
//RefCount(SoPolygonOffset)
//RefCount(SoMarkerSet)
//RefCount(SoTextureUnit)
//RefCount(SoSpotLight)
//RefCount(SoSphere)
//RefCount(SoPointSet)
//RefCount(SoPathSwitch)
//RefCount(SoMatrixTransform)
//RefCount(SoCallback)
RefCount(SoVertexShape)
//RefCount(SoQuadMesh)
//RefCount(SoAsciiText)
//RefCount(SoProfileCoordinate2)
//RefCount(SoProfileCoordinate3)
//RefCount(SoSwitch)
//RefCount(SoExtSelection)
//RefCount(SoClipPlane)
RefCount(SoArray)
//RefCount(SoEnvironment)
//RefCount(SoLOD)
//RefCount(SoShuttle)
//RefCount(SoIndexedLineSet)
//RefCount(SoTextureCoordinateDefault)
//RefCount(SoTexture2Transform)
//RefCount(SoImage)
//RefCount(SoMultipleCopy)
RefCount(SoCoordinate3)
//RefCount(SoCoordinate4)
RefCount(SoCamera)
//RefCount(SoTransform)
//RefCount(SoLocateHighlight)
//RefCount(SoTexture3Transform)
//RefCount(SoEventCallback)
//RefCount(SoIndexedShape)
//RefCount(SoLevelOfDetail)
//RefCount(SoTextureCoordinatePlane)
//RefCount(SoAntiSquish)
//RefCount(SoTexture2)
//RefCount(SoTexture3)
//RefCount(SoListener)
RefCount(SoRotation)
//RefCount(SoIndexedFaceSet)
//RefCount(SoShapeHints)
//RefCount(SoTextureCoordinate2)
//RefCount(SoTextureCoordinate3)
RefCount(SoLineSet)
//RefCount(SoIndexedNurbsCurve)
RefCount(SoResetTransform)
//RefCount(SoColorIndex)
//RefCount(SoTransparencyType)
RefCount(SoPerspectiveCamera)
//RefCount(SoProfile)
//RefCount(SoVRMLCone)
//RefCount(SoVRMLVertexPoint)
//RefCount(SoVRMLCoordinate)
//RefCount(SoVRMLTouchSensor)
//RefCount(SoVRMLAnchor)
//RefCount(SoVRMLCylinderSensor)
//RefCount(SoVRMLSpotLight)
//RefCount(SoVRMLPlaneSensor)
//RefCount(SoVRMLIndexedShape)
//RefCount(SoVRMLNormal)
//RefCount(SoVRMLSphereSensor)
//RefCount(SoVRMLElevationGrid)
//RefCount(SoVRMLProximitySensor)
//RefCount(SoVRMLTimeSensor)
//RefCount(SoVRMLColor)
//RefCount(SoVRMLText)
//RefCount(SoVRMLSphere)
//RefCount(SoVRMLNavigationInfo)
//RefCount(SoVRMLSwitch)
//RefCount(SoVRMLTexture)
//RefCount(SoVRMLPointSet)
//RefCount(SoVRMLVertexShape)
//RefCount(SoVRMLInterpolator)
//RefCount(SoVRMLTransform)
//RefCount(SoVRMLMovieTexture)
//RefCount(SoVRMLScalarInterpolator)
//RefCount(SoVRMLColorInterpolator)
//RefCount(SoVRMLParent)
//RefCount(SoVRMLLight)
//RefCount(SoVRMLShape)
//RefCount(SoVRMLPointLight)
//RefCount(SoVRMLScript)
//RefCount(SoVRMLSound)
//RefCount(SoVRMLVertexLine)
//RefCount(SoVRMLDragSensor)
//RefCount(SoVRMLVisibilitySensor)
//RefCount(SoVRMLBillboard)
//RefCount(SoVRMLAppearance)
//RefCount(SoVRMLImageTexture)
//RefCount(SoVRMLTextureTransform)
//RefCount(SoVRMLGeometry)
//RefCount(SoVRMLBackground)
//RefCount(SoVRMLPositionInterpolator)
//RefCount(SoVRMLBox)
//RefCount(SoVRMLFog)
//RefCount(SoVRMLLOD)
//RefCount(SoVRMLSensor)
//RefCount(SoVRMLWorldInfo)
//RefCount(SoVRMLIndexedLineSet)
//RefCount(SoVRMLNormalInterpolator)
//RefCount(SoVRMLExtrusion)
//RefCount(SoVRMLOrientationInterpolator)
//RefCount(SoVRMLDirectionalLight)
//RefCount(SoVRMLIndexedLine)
//RefCount(SoVRMLCylinder)
//RefCount(SoVRMLTextureCoordinate)
//RefCount(SoVRMLGroup)
//RefCount(SoVRMLAudioClip)
//RefCount(SoVRMLCollision)
//RefCount(SoVRMLCoordinateInterpolator)
//RefCount(SoVRMLPixelTexture)
//RefCount(SoVRMLInline)
//RefCount(SoVRMLMaterial)
//RefCount(SoVRMLViewpoint)
//RefCount(SoVRMLFontStyle)
//RefCount(SoVRMLIndexedFaceSet)
//RefCount(SoInterpolate)
//RefCount(SoComposeMatrix)
//RefCount(SoConcatenate)
//RefCount(SoOneShot)
//RefCount(SoComposeRotation)
//RefCount(SoInterpolateFloat)
//RefCount(SoInterpolateRotation)
//RefCount(SoTimeCounter)
//RefCount(SoGate)
//RefCount(SoComposeVec2f)
//RefCount(SoComposeVec3f)
//RefCount(SoComposeVec4f)
//RefCount(SoComposeRotationFromTo)
//RefCount(SoDecomposeMatrix)
//RefCount(SoInterpolateVec2f)
//RefCount(SoInterpolateVec3f)
//RefCount(SoInterpolateVec4f)
//RefCount(SoSelectOne)
//RefCount(SoDecomposeRotation)
//RefCount(SoTransformVec3f)
//RefCount(SoTriggerAny)
//RefCount(SoCalculator)
//RefCount(SoOnOff)
//RefCount(SoCounter)
//RefCount(SoFieldConverter)
//RefCount(SoNodeEngine)
//RefCount(SoElapsedTime)
//RefCount(SoBoolOperation)
//RefCount(SoComputeBoundingBox)
//RefCount(SoEngine)
//RefCount(SoDecomposeVec2f)
//RefCount(SoDecomposeVec3f)
//RefCount(SoDecomposeVec4f)
//RefCount(SoHandleBoxDragger)
//RefCount(SoDragger)
//RefCount(SoScaleUniformDragger)
//RefCount(SoDirectionalLightDragger)
//RefCount(SoSpotLightDragger)
//RefCount(SoTrackballDragger)
//RefCount(SoPointLightDragger)
//RefCount(SoTabBoxDragger)
//RefCount(SoRotateSphericalDragger)
//RefCount(SoTranslate1Dragger)
//RefCount(SoTransformerDragger)
//RefCount(SoJackDragger)
//RefCount(SoTransformBoxDragger)
//RefCount(SoTranslate2Dragger)
//RefCount(SoScale2UniformDragger)
//RefCount(SoScale1Dragger)
//RefCount(SoCenterballDragger)
//RefCount(SoDragPointDragger)
//RefCount(SoRotateCylindricalDragger)
//RefCount(SoRotateDiscDragger)
//RefCount(SoTabPlaneDragger)
//RefCount(SoScale2Dragger)
//RefCount(SoJackManip)
//RefCount(SoTabBoxManip)
//RefCount(SoTransformManip)
//RefCount(SoClipPlaneManip)
//RefCount(SoTrackballManip)
//RefCount(SoTransformBoxManip)
//RefCount(SoPointLightManip)
//RefCount(SoSpotLightManip)
//RefCount(SoDirectionalLightManip)
//RefCount(SoTransformerManip)
//RefCount(SoHandleBoxManip)
//RefCount(SoCenterballManip)
//RefCount(SoLightKit)
//RefCount(SoCameraKit)
//RefCount(SoAppearanceKit)
//RefCount(SoNodeKitListPart)
//RefCount(SoWrapperKit)
//RefCount(SoShapeKit)
//RefCount(SoSeparatorKit)
//RefCount(SoInteractionKit)
//RefCount(SoSceneKit)

//////////////////////////////////////////////////////////////
/// Code to wrap /////////////////////////////////////////////
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
/// Basic ////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

class SbString {
public:
  SbString(void);
  SbString(const char*);
  SbString(const char*,int,int);
  SbString(const SbString &);
  SbString(const int);
  ~SbString();
public:
  const char* getString() const;
  int getLength() const;
};

class SbName {
public:
  SbName(void);
  SbName(const char *);
  SbName(const SbString &);
  SbName(const SbName &);
  ~SbName();
public:
  const char* getString() const;
  int getLength() const;
};

typedef int SbBool;

class SbVec2f {
public:
  SbVec2f(void);
  SbVec2f(const float v[2]);
  SbVec2f(const float x, const float y);
  float dot(const SbVec2f & v) const;
  SbBool equals(const SbVec2f & v, const float tolerance) const;
  const float * getValue(void) const;
  void getValue(float & x, float & y) const;
  float length(void) const;
  void negate(void);
  float normalize(void);
  SbVec2f & setValue(const float v[2]);
  SbVec2f & setValue(const float x, const float y);
/*
  float & operator [] (const int i);
  const float & operator [] (const int i) const;
  SbVec2f & operator *= (const float d);
  SbVec2f & operator /= (const float d);
  SbVec2f & operator += (const SbVec2f & u);
  SbVec2f & operator -= (const SbVec2f & u);
  SbVec2f operator-(void) const;
  friend COIN_DLL_API SbVec2f operator * (const SbVec2f & v, const float d);
  friend COIN_DLL_API SbVec2f operator * (const float d, const SbVec2f & v);
  friend COIN_DLL_API SbVec2f operator / (const SbVec2f & v, const float d);
  friend COIN_DLL_API SbVec2f operator + (const SbVec2f & v1, const SbVec2f & v2);
  friend COIN_DLL_API SbVec2f operator - (const SbVec2f & v1, const SbVec2f & v2);
  friend COIN_DLL_API int operator == (const SbVec2f & v1, const SbVec2f & v2);
  friend COIN_DLL_API int operator != (const SbVec2f & v1, const SbVec2f & v2);
*/

  //void print(FILE * fp) const;
};

class SbVec3f {
public:
  SbVec3f(void);
  SbVec3f(const float v[3]);
  SbVec3f(const float x, const float y, const float z);
  //SbVec3f(const SbPlane & p0, const SbPlane & p1, const SbPlane & p2);
  //SbVec3f(const SbVec3d & v);
  SbVec3f cross(const SbVec3f & v) const;
  float dot(const SbVec3f & v) const;
  SbBool equals(const SbVec3f & v, const float tolerance) const;
  SbVec3f getClosestAxis(void) const;
  const float * getValue(void) const;
  void getValue(float & x, float & y, float & z) const;
  float length(void) const;
  float sqrLength() const;
  void negate(void);
  float normalize(void);
  SbVec3f & setValue(const float v[3]);
  SbVec3f & setValue(const float x, const float y, const float z);
  SbVec3f & setValue(const SbVec3f & barycentric,
                     const SbVec3f & v0,
                     const SbVec3f & v1,
                     const SbVec3f & v2);
  //SbVec3f & setValue(const SbVec3d & v);
  /*FIXME
  float & operator [](const int i);
  const float & operator [](const int i) const;
  SbVec3f & operator *=(const float d);
  SbVec3f & operator /=(const float d);
  SbVec3f & operator +=(const SbVec3f & u);
  SbVec3f & operator -=(const SbVec3f & u);
  SbVec3f operator-(void) const;
  */
  //void print(FILE * fp) const;
};

class SbColor : public SbVec3f {
public:
  SbColor(void);
  SbColor(const SbVec3f& v);
  SbColor(const float* const rgb);
  SbColor(const float r, const float g, const float b);

  SbColor & setHSVValue(float h, float s, float v);
  SbColor & setHSVValue(const float hsv[3]);
  void getHSVValue(float &h, float &s, float &v) const;
  void getHSVValue(float hsv[3]) const;
  //SbColor & setPackedValue(const uint32_t rgba, float& transparency);
  //uint32_t getPackedValue(const float transparency = 0.0f) const;
};

class SbRotation {
public:
  SbRotation(void);
  SbRotation(const SbVec3f & axis, const float radians);
  SbRotation(const float q[4]);
  SbRotation(const float q0, const float q1, const float q2, const float q3);
  //SbRotation(const SbMatrix & m);
  SbRotation(const SbVec3f & rotateFrom, const SbVec3f & rotateTo);
  const float * getValue(void) const;
  void getValue(float & q0, float & q1, float & q2, float & q3) const;
  SbRotation & setValue(const float q0, const float q1,
                        const float q2, const float q3);
  void getValue(SbVec3f & axis, float & radians) const;
  //void getValue(SbMatrix & matrix) const;
  SbRotation & invert(void);
  SbRotation inverse(void) const;
  SbRotation & setValue(const float q[4]);
  //SbRotation & setValue(const SbMatrix & m);
  SbRotation & setValue(const SbVec3f & axis, const float radians);
  SbRotation & setValue(const SbVec3f & rotateFrom, const SbVec3f & rotateTo);
  //FIXME SbRotation & operator*=(const SbRotation & q);
  //FIXME SbRotation & operator*=(const float s);
  SbBool equals(const SbRotation & r, const float tolerance) const;
  void multVec(const SbVec3f & src, SbVec3f & dst) const;

  void scaleAngle(const float scaleFactor);
  static SbRotation slerp(const SbRotation & rot0, const SbRotation & rot1,
                          float t);
  static SbRotation identity(void);

  //void print(FILE * fp) const;
};

class SoType {
public:
  static SoType fromName(const SbName);
  SbName getName(void) const;
  const SoType getParent(void) const;
  SbBool isDerivedFrom(const SoType type) const;
  SbBool canCreateInstance(void) const;
  //void* createInstance(void) const; //Do it with an %extend
};

//////////////////////////////////////////////////////////////
/// Fields ///////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
class SoField {
protected:
  SoField(void);
public:
  virtual ~SoField();
  virtual SoType getTypeId(void) const = 0;
  static SoType getClassTypeId(void);
  SbBool isOfType(const SoType type) const;
public:
  void setIgnored(SbBool ignore);
  SbBool isIgnored(void) const;
  void setDefault(SbBool def);
  SbBool isDefault(void) const;
  SbBool set(const char * valuestring);
  void get(SbString & valuestring);
  virtual void touch(void);
  void evaluate(void) const;
};

class SoSField : public SoField {
public:
  virtual ~SoSField();
  static SoType getClassTypeId(void);
protected:
  SoSField(void);
};

class SoSFBool : public SoSField {
public:
  SoSFBool(void);
  virtual ~SoSFBool();
public: 
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
public: 
  //FIXME const SoSFBool& operator=(const SoSFBool&);
  //FIXME float operator=(float);
  SbBool getValue(void) const;
  void setValue(SbBool);
};

class SoSFShort : public SoSField {
public:
  SoSFShort(void);
  virtual ~SoSFShort();
public: 
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
public: 
  //FIXME const SoSFShort& operator=(const SoSFShort&);
  //FIXME short operator=(short);
  short getValue(void) const;
  void setValue(short);
};

class SoSFFloat : public SoSField {
public:
  SoSFFloat(void);
  virtual ~SoSFFloat();
public: 
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
public: 
  //FIXME const SoSFFloat& operator=(const SoSFFloat&);
  //FIXME float operator=(float);
  float getValue(void) const;
  void setValue(float);
};


typedef int int32_t;

class SoSFInt32 : public SoSField {
public:
  SoSFInt32(void);
  virtual ~SoSFInt32();
public: 
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
public: 
  //FIXME const SoSFInt32& operator=(const SoSFInt32&);
  //FIXME int32_t operator=(int32_t);
  int32_t getValue(void) const;
  void setValue(int32_t);
};

class SoSFBitMask : public SoSField {
public:
  SoSFBitMask(void);
  virtual ~SoSFBitMask();
public: 
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
public: 
  //FIXME const SoSFBitMask& operator=(const SoSFBitMask&);
  //FIXME int operator=(int);
  int getValue(void) const;
  void setValue(int);
};

class SoSFEnum : public SoSField {
public:
  SoSFEnum(void);
  virtual ~SoSFEnum();
public: 
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
public: 
  //FIXME const SoSFEnum& operator=(const SoSFEnum&);
  //FIXME int operator=(int);
  int getValue(void) const;
  void setValue(int);
public:
  void setValue(const SbName name);
  void setEnums(const int num, const int * vals, const SbName * names);

  int getNumEnums(void) const;
  int getEnum(const int idx, SbName & name) const;
};

class SoSFString : public SoSField {
public:
  SoSFString(void);
  virtual ~SoSFString();
public: 
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
public: 
  //FIXME const SoSFString& operator=(const SoSFString&);
  //FIXME const SbString& operator=(const SbString&);
  const SbString& getValue(void) const;
  void setValue(const SbString&);
public:
  void setValue(const char * str);
};

class SoSFVec2f : public SoSField {
public:
  SoSFVec2f(void);
  virtual ~SoSFVec2f();
public: 
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
public: 
  //FIXME const SoSFVec2f& operator=(const SoSFVec2f&);
  //FIXME const SbVec2f& operator=(const SbVec2f&);
  const SbVec2f& getValue(void) const;
  void setValue(const SbVec2f&);
public:
  void setValue(const float x, const float y);
  void setValue(const float xyz[2]);
};

class SoSFVec3f : public SoSField {
public:
  SoSFVec3f(void);
  virtual ~SoSFVec3f();
public: 
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
public: 
  //FIXME const SoSFVec3f& operator=(const SoSFVec3f&);
  //FIXME const SbVec3f& operator=(const SbVec3f&);
  const SbVec3f& getValue(void) const;
  void setValue(const SbVec3f&);
public:
  void setValue(const float x, const float y, const float z);
  void setValue(const float xyz[3]);
};

class SoSFColor : public SoSField {
public:
  SoSFColor(void);
  virtual ~SoSFColor();
public: 
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
public: 
  //FIXME const SoSFColor& operator=(const SoSFColor&);
  //FIXME const SbColor& operator=(const SbColor&);
  const SbColor& getValue(void) const;
  void setValue(const SbColor&);
public:
  void setValue(const SbVec3f & vec);
  void setValue(const float red, const float green, const float blue);
  void setValue(const float rgb[3]);
  void setHSVValue(const float h, const float s, const float v);
  void setHSVValue(const float hsv[3]);
};

class SoSFRotation : public SoSField {
public:
  SoSFRotation(void);
  virtual ~SoSFRotation();
public: 
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
public: 
  //FIXME const SoSFRotation& operator=(const SoSFRotation&);
  //FIXME const SbRotation& operator=(const SbRotation&);
  const SbRotation& getValue(void) const;
  void setValue(const SbRotation&);
public:
  void getValue(SbVec3f & axis, float & angle) const;
  void setValue(const float, const float, const float, const float);
  void setValue(const float q[4]);
  void setValue(const SbVec3f & axis, const float angle);
};

class SoMField : public SoField {
public:
  virtual ~SoMField();
  static SoType getClassTypeId(void);
  int getNum(void) const;
  void setNum(const int num);
  virtual void deleteValues(int start, int num = -1);
  virtual void insertSpace(int start, int num);
  SbBool set1(const int index, const char * const valuestring);
  void get1(const int index, SbString & valuestring);
  virtual void enableDeleteValues(void);
  virtual SbBool isDeleteValuesEnabled(void);
protected:
  SoMField(void);
};

class SoMFFloat : public SoMField {
public: 
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
public: 
  const float* getValues(const int start) const ;
  int find(float& value, SbBool addifnotfound = FALSE);
  void setValues(const int start, const int num, const float* newvals);
  void set1Value(const int idx, float& value);
  void setValue(float& value);
  //FIXME float& operator[](const int idx) const;
  //FIXME float& operator=(float& val);
  //FIXME SbBool operator==(const _class_ & field) const;
  //FIXME SbBool operator!=(const _class_ & field) const;
  float* startEditing(void);
  void finishEditing(void);
};

class SoMFInt32 : public SoMField {
public: 
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
public: 
  const int32_t* getValues(const int start) const ;
  int find(int32_t& value, SbBool addifnotfound = FALSE);
  void setValues(const int start, const int num, const int32_t* newvals);
  void set1Value(const int idx, int32_t& value);
  void setValue(int32_t& value);
  //FIXME int32_t& operator[](const int idx) const;
  //FIXME int32_t& operator=(int32_t& val);
  //FIXME SbBool operator==(const _class_ & field) const;
  //FIXME SbBool operator!=(const _class_ & field) const;
  int32_t* startEditing(void);
  void finishEditing(void);
};

class SoMFString : public SoMField {
public: 
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
public: 
  const SbString* getValues(const int start) const ;
  int find(SbString& value, SbBool addifnotfound = FALSE);
  void setValues(const int start, const int num, const SbString* newvals);
  void set1Value(const int idx, SbString& value);
  void setValue(SbString& value);
  //FIXME SbString& operator[](const int idx) const;
  //FIXME SbString& operator=(SbString& val);
  //FIXME SbBool operator==(const _class_ & field) const;
  //FIXME SbBool operator!=(const _class_ & field) const;
  SbString* startEditing(void);
  void finishEditing(void);
public: 
  void setValues(const int start, const int num, const char * strings[]);
  void setValue(const char * string);
  void deleteText(const int fromline, const int fromchar,
                  const int toline, const int tochar);
};

class SoMFColor : public SoMField {
public: 
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
public: 
  const SbColor* getValues(const int start) const ;
  int find(SbColor& value, SbBool addifnotfound = FALSE);
  void setValues(const int start, const int num, const SbColor* newvals);
  void set1Value(const int idx, SbColor& value);
  void setValue(SbColor& value);
  //FIXME SbColor& operator[](const int idx) const;
  //FIXME SbColor& operator=(SbColor& val);
  //FIXME SbBool operator==(const _class_ & field) const;
  //FIXME SbBool operator!=(const _class_ & field) const;
  SbColor* startEditing(void);
  void finishEditing(void);
public:
  void setValues(const int start, const int num, const float rgb[][3]);
  void setHSVValues(const int start, const int num, const float hsv[][3]);

  void setValue(const SbVec3f & vec);
  void setValue(const float r, const float g, const float b);
  void setValue(const float rgb[3]);

  void setHSVValue(const float h, const float s, const float v);
  void setHSVValue(const float hsv[3]);

  void set1Value(const int idx, const SbVec3f & vec);
  void set1Value(const int idx, const float r, const float g, const float b);
  void set1Value(const int idx, const float rgb[3]);

  void set1HSVValue(const int idx, const float h, const float s, const float v);
  void set1HSVValue(const int idx, const float hsv[3]);
};

class SoMFVec3f : public SoMField {
public: 
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
public: 
  const SbVec3f* getValues(const int start) const ;
  int find(SbVec3f& value, SbBool addifnotfound = FALSE);
  void setValues(const int start, const int num, const SbVec3f* newvals);
  void set1Value(const int idx, SbVec3f& value);
  void setValue(SbVec3f& value);
  //FIXME SbVec3f& operator[](const int idx) const;
  //FIXME SbVec3f& operator=(SbVec3f& val);
  //FIXME SbBool operator==(const _class_ & field) const;
  //FIXME SbBool operator!=(const _class_ & field) const;
  SbVec3f* startEditing(void);
  void finishEditing(void);
public:
  void setValues(const int start, const int num, const float xyz[][3]);
  void set1Value(const int idx, const float x, const float y, const float z);
  void set1Value(const int idx, const float xyz[3]);
  void setValue(const float x, const float y, const float z);
  void setValue(const float xyz[3]);
};

//////////////////////////////////////////////////////////////
/// Management ///////////////////////////////////////////////
//////////////////////////////////////////////////////////////

class SoInput {
public:
  SoInput(void);        
  virtual ~SoInput();
public:
  virtual SbBool openFile(const char*,SbBool okIfNotFound = FALSE);
};

class SoSeparator;
class SoDB {
public:
  static SoSeparator* readAll(SoInput*);
};

//////////////////////////////////////////////////////////////
/// Nodes ////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

class SoBase {
protected:
  SoBase(void);	
  virtual ~SoBase();
public:
  virtual SoType getTypeId(void) const = 0;
  SbBool isOfType(SoType type) const;
  static SoType getClassTypeId(void);
public:
  void ref(void) const;
  void unref(void) const;
  void unrefNoDelete(void) const;
  int getRefCount(void) const;
  void touch(void);
public:
  virtual SbName getName() const;
  virtual void setName(const SbName&);
};

class SoFieldContainer : public SoBase {
protected:
  SoFieldContainer(void);	
  ~SoFieldContainer();
public:
  static SoType getClassTypeId(void);
public:
  SbBool set(const char*);
  void get(SbString &);
  virtual SoField * getField(const SbName &) const;
};

class SoGetBoundingBoxAction;

class SoNode : public SoFieldContainer {
protected:
  SoNode(void);	
  virtual ~SoNode();
public:
  static SoType getClassTypeId(void);
public:
  virtual void getBoundingBox(SoGetBoundingBoxAction*);
};

class SoGroup : public SoNode {
protected: 
  virtual ~SoGroup();
public: 
  SoGroup();
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
public:
  virtual void addChild(SoNode*);
  virtual void insertChild(SoNode*,int);
  virtual SoNode* getChild(int) const;
  virtual int findChild(const SoNode*) const;
  virtual int getNumChildren();
  virtual void removeChild(int);
  //virtual void removeChild(SoNode*);
  virtual void removeAllChildren();
  virtual void replaceChild(int,SoNode*);
  //virtual void replaceChild(SoNode*,SoNode*);
public:
  virtual void getBoundingBox(SoGetBoundingBoxAction*);
};

class SoSeparator : public SoGroup {
protected:
  virtual ~SoSeparator();
public:
  SoSeparator();
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
public:
  virtual void getBoundingBox(SoGetBoundingBoxAction*);
public:
  enum CacheEnabled {
    OFF, ON, AUTO
  };
  SoSFEnum renderCaching;
  SoSFEnum boundingBoxCaching;
  SoSFEnum renderCulling;
  SoSFEnum pickCulling;
public:
  static void setNumRenderCaches(const int howmany);
  static int getNumRenderCaches(void);
  //virtual SbBool affectsState(void) const;
};

class SoArray : public SoGroup {
protected:
  virtual ~SoArray();
public:
  SoArray();
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
public:
  enum Origin {
    FIRST, CENTER, LAST
  };
  SoSFEnum origin;
  SoSFShort numElements1;
  SoSFShort numElements2;
  SoSFShort numElements3;
  SoSFVec3f separation1;
  SoSFVec3f separation2;
  SoSFVec3f separation3;
public:
  //virtual SbBool affectsState(void) const;
};

class SoAction {
public:
  virtual ~SoAction();
  static void initClass(void);
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const = 0;
  virtual SbBool isOfType(SoType type) const;
protected:
  SoAction(void);
};

class SbBox3f;
class SbVec3f;

class SoShape : public SoNode {
protected: 
  SoShape();
  virtual ~SoShape();
public:
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
public:
  virtual void computeBBox(SoAction*,SbBox3f&,SbVec3f&) =  0;
  virtual void getBoundingBox(SoGetBoundingBoxAction*);
};

class SoVertexShape : public SoShape {
protected: 
  virtual ~SoVertexShape();
  SoVertexShape();
public: 
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
public:
  //SoSFNode vertexProperty;
};

class SoNonIndexedShape : public SoVertexShape {
protected: 
  virtual ~SoNonIndexedShape();
  SoNonIndexedShape();
public: 
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
public:
  SoSFInt32 startIndex;
};

class SoCube : public SoShape {
protected: 
  virtual ~SoCube();
public: SoCube();
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
protected: 
  virtual void computeBBox(SoAction*,SbBox3f&,SbVec3f&);
public:
  SoSFFloat width;
  SoSFFloat height;
  SoSFFloat depth;
};

class SoCylinder : public SoShape {
protected: 
  virtual ~SoCylinder();
public: 
  SoCylinder();
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
protected: 
  virtual void computeBBox(SoAction*,SbBox3f&,SbVec3f&);
public:
  enum Part {
    SIDES =  0x1,
    TOP =    0x2,
    BOTTOM = 0x4,
    ALL =    SIDES|TOP|BOTTOM
  };
  SoSFFloat radius;
  SoSFFloat height;
  SoSFBitMask parts;
public:
  void addPart(SoCylinder::Part part);
  void removePart(SoCylinder::Part part);
  SbBool hasPart(SoCylinder::Part part) const;
};

class SoCamera : public SoNode {
protected: 
  SoCamera();
  virtual ~SoCamera();
public: 
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
public: 
  virtual void scaleHeight(float scalefactor) = 0;
public:
  enum ViewportMapping {
    CROP_VIEWPORT_FILL_FRAME,
    CROP_VIEWPORT_LINE_FRAME,
    CROP_VIEWPORT_NO_FRAME,
    ADJUST_CAMERA,
    LEAVE_ALONE
  };
  SoSFEnum viewportMapping;
  SoSFVec3f position;
  SoSFRotation orientation;
  SoSFFloat aspectRatio;
  SoSFFloat nearDistance;
  SoSFFloat farDistance;
  SoSFFloat focalDistance;
};

class SoOrthographicCamera : public SoCamera {
protected: 
  virtual ~SoOrthographicCamera();
public: 
  SoOrthographicCamera();
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
public: 
  virtual void scaleHeight(float scalefactor);
public: 
  SoSFFloat height;
};

class SoPerspectiveCamera : public SoCamera {
protected: 
  virtual ~SoPerspectiveCamera();
public: 
  SoPerspectiveCamera();
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
public: 
  virtual void scaleHeight(float scalefactor);
public: 
  SoSFFloat heightAngle;
};

class SoMaterial : public SoNode {
protected: 
  virtual ~SoMaterial();
public: 
  SoMaterial();
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
public:
  SoMFColor ambientColor;
  SoMFColor diffuseColor;
  SoMFColor specularColor;
  SoMFColor emissiveColor;
  SoMFFloat shininess;
  SoMFFloat transparency;
};

class SoTransformation : public SoNode {
protected: 
  SoTransformation();
  virtual ~SoTransformation();
};

class SoScale : public SoTransformation {
protected: 
  virtual ~SoScale();
public: 
  SoScale();
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
public: 
  SoSFVec3f scaleFactor;
};

class SoTranslation : public SoTransformation {
protected: 
  virtual ~SoTranslation();
public: 
  SoTranslation();
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
public:
  SoSFVec3f translation;
};

class SoRotation : public SoTransformation {
protected: 
  virtual ~SoRotation();
public: 
  SoRotation();
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
public:
  SoSFRotation rotation;
};

class SoRotationXYZ : public SoTransformation {
protected: 
  virtual ~SoRotationXYZ();
public: 
  SoRotationXYZ();
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
public:
  enum Axis {
    X, Y, Z
  };
  SoSFEnum axis;
  SoSFFloat angle;
  SbRotation getRotation(void) const;
};

class SoResetTransform : public SoTransformation {
protected: 
  virtual ~SoResetTransform();
public: 
  SoResetTransform();
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
public: 
  // These must be bit flags.
  enum ResetType {
    TRANSFORM = 0x01,
    BBOX = 0x02
  };
  SoSFBitMask whatToReset;
};

class SoLight : public SoNode {
protected: 
  virtual ~SoLight();
  SoLight();
public: 
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
public:
  SoSFBool on;
  SoSFFloat intensity;
  SoSFColor color;
};

class SoDirectionalLight : public SoLight {
protected: 
  virtual ~SoDirectionalLight();
public: 
  SoDirectionalLight();
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
public:
  SoSFVec3f direction;
};

class SoLightModel : public SoNode {
protected: 
  virtual ~SoLightModel();
public: 
  SoLightModel();
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
public:
  enum Model {
    BASE_COLOR /*= SoLazyElement::BASE_COLOR*/,
    PHONG /*= SoLazyElement::PHONG*/
  };
  SoSFEnum model;
};

class SoDrawStyle : public SoNode {
protected: 
  virtual ~SoDrawStyle();
public:
  SoDrawStyle();
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
public:
/*
  enum Style {
    FILLED = SoDrawStyleElement::FILLED,
    LINES = SoDrawStyleElement::LINES,
    POINTS = SoDrawStyleElement::POINTS,
    INVISIBLE = SoDrawStyleElement::INVISIBLE
  };
  SoSFEnum style;
*/
  SoSFFloat pointSize;
  SoSFFloat lineWidth;
  //SoSFUShort linePattern;
};

class SoCoordinate3 : public SoNode {
protected: 
  virtual ~SoCoordinate3();
public: 
  SoCoordinate3();
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
public:
  SoMFVec3f point;
};

class SoLineSet : public SoNonIndexedShape {
protected: 
  virtual ~SoLineSet();
  virtual void generatePrimitives(SoAction * action);
  virtual void computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center);
public: 
  SoLineSet();
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
public:
  SoMFInt32 numVertices;
};

//////////////////////////////////////////////////////////////
/// Nodekits /////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

class SoBaseKit : public SoNode {
protected:
  virtual ~SoBaseKit();
public: 
  virtual SoType getTypeId(void) const;
public:
  SoBaseKit(void);	
};

/*
class SoAnnotation : public SoNode {protected: virtual ~SoAnnotation();public: SoAnnotation();virtual SoType getTypeId(void) const;};
class SoBaseColor : public SoNode {protected: virtual ~SoBaseColor();public: SoBaseColor();virtual SoType getTypeId(void) const;};

class SoCallback : public SoNode {protected: virtual ~SoCallback();public: SoCallback();virtual SoType getTypeId(void) const;};
class SoClipPlane : public SoNode {protected: virtual ~SoClipPlane();public: SoClipPlane();virtual SoType getTypeId(void) const;};
class SoComplexity : public SoNode {protected: virtual ~SoComplexity();public: SoComplexity();virtual SoType getTypeId(void) const;};
class SoCone : public SoNode {protected: virtual ~SoCone();public: SoCone();virtual SoType getTypeId(void) const;};
class SoCoordinate3 : public SoNode {protected: virtual ~SoCoordinate3();public: SoCoordinate3();virtual SoType getTypeId(void) const;};
class SoCoordinate4 : public SoNode {protected: virtual ~SoCoordinate4();public: SoCoordinate4();virtual SoType getTypeId(void) const;};
class SoEnvironment : public SoNode {protected: virtual ~SoEnvironment();public: SoEnvironment();virtual SoType getTypeId(void) const;};
class SoEventCallback : public SoNode {protected: virtual ~SoEventCallback();public: SoEventCallback();virtual SoType getTypeId(void) const;};
class SoFaceSet : public SoNode {protected: virtual ~SoFaceSet();public: SoFaceSet();virtual SoType getTypeId(void) const;};
class SoFont : public SoNode {protected: virtual ~SoFont();public: SoFont();virtual SoType getTypeId(void) const;};
class SoIndexedFaceSet : public SoNode {protected: virtual ~SoIndexedFaceSet();public: SoIndexedFaceSet();virtual SoType getTypeId(void) const;};
class SoIndexedLineSet : public SoNode {protected: virtual ~SoIndexedLineSet();public: SoIndexedLineSet();virtual SoType getTypeId(void) const;};
class SoLabel : public SoNode {protected: virtual ~SoLabel();public: SoLabel();virtual SoType getTypeId(void) const;};
class SoLevelOfDetail : public SoNode {protected: virtual ~SoLevelOfDetail();public: SoLevelOfDetail();virtual SoType getTypeId(void) const;};
class SoLineSet : public SoNode {protected: virtual ~SoLineSet();public: SoLineSet();virtual SoType getTypeId(void) const;};
class SoMatrixTransform : public SoNode {protected: virtual ~SoMatrixTransform();public: SoMatrixTransform();virtual SoType getTypeId(void) const;};
class SoNormal : public SoNode {protected: virtual ~SoNormal();public: SoNormal();virtual SoType getTypeId(void) const;};
class SoNormalBinding : public SoNode {protected: virtual ~SoNormalBinding();public: SoNormalBinding();virtual SoType getTypeId(void) const;};
class SoNurbsCurve : public SoNode {protected: virtual ~SoNurbsCurve();public: SoNurbsCurve();virtual SoType getTypeId(void) const;};
class SoNurbsSurface : public SoNode {protected: virtual ~SoNurbsSurface();public: SoNurbsSurface();virtual SoType getTypeId(void) const;};
class SoPickStyle : public SoNode {protected: virtual ~SoPickStyle();public: SoPickStyle();virtual SoType getTypeId(void) const;};
class SoPointLight : public SoNode {protected: virtual ~SoPointLight();public: SoPointLight();virtual SoType getTypeId(void) const;};
class SoPointSet : public SoNode {protected: virtual ~SoPointSet();public: SoPointSet();virtual SoType getTypeId(void) const;};
class SoQuadMesh : public SoNode {protected: virtual ~SoQuadMesh();public: SoQuadMesh();virtual SoType getTypeId(void) const;};
class SoSelection : public SoNode {protected: virtual ~SoSelection();public: SoSelection();virtual SoType getTypeId(void) const;};
class SoShapeHints : public SoNode {protected: virtual ~SoShapeHints();public: SoShapeHints();virtual SoType getTypeId(void) const;};
class SoSphere : public SoNode {protected: virtual ~SoSphere();public: SoSphere();virtual SoType getTypeId(void) const;};
class SoSpotLight : public SoNode {protected: virtual ~SoSpotLight();public: SoSpotLight();virtual SoType getTypeId(void) const;};
class SoSwitch : public SoNode {protected: virtual ~SoSwitch();public: SoSwitch();virtual SoType getTypeId(void) const;};
class SoText2 : public SoNode {protected: virtual ~SoText2();public: SoText2();virtual SoType getTypeId(void) const;};
class SoText3 : public SoNode {protected: virtual ~SoText3();public: SoText3();virtual SoType getTypeId(void) const;};
class SoTexture2 : public SoNode {protected: virtual ~SoTexture2();public: SoTexture2();virtual SoType getTypeId(void) const;};
class SoTexture2Transform : public SoNode {protected: virtual ~SoTexture2Transform();public: SoTexture2Transform();virtual SoType getTypeId(void) const;};
class SoTransform : public SoNode {protected: virtual ~SoTransform();public: SoTransform();virtual SoType getTypeId(void) const;};
class SoTriangleStripSet : public SoNode {protected: virtual ~SoTriangleStripSet();public: SoTriangleStripSet();virtual SoType getTypeId(void) const;};
class SoUnits : public SoNode {protected: virtual ~SoUnits();public: SoUnits();virtual SoType getTypeId(void) const;};
class SoVertexProperty : public SoNode {protected: virtual ~SoVertexProperty();public: SoVertexProperty();virtual SoType getTypeId(void) const;};
class SoAppearanceKit : public SoNode {protected: virtual ~SoAppearanceKit();public: SoAppearanceKit();virtual SoType getTypeId(void) const;};
class SoBaseKit : public SoNode {protected: virtual ~SoBaseKit();public: SoBaseKit();virtual SoType getTypeId(void) const;};
class SoNodeKitListPart : public SoNode {protected: virtual ~SoNodeKitListPart();public: SoNodeKitListPart();virtual SoType getTypeId(void) const;};
class SoSeparatorKit : public SoNode {protected: virtual ~SoSeparatorKit();public: SoSeparatorKit();virtual SoType getTypeId(void) const;};
class SoShapeKit : public SoNode {protected: virtual ~SoShapeKit();public: SoShapeKit();virtual SoType getTypeId(void) const;};

class SoNodekitCatalog {public: SoNodekitCatalog();~SoNodekitCatalog();};

class SoTransformManip : public SoTransform {protected: virtual ~SoTransformManip();public: SoTransformManip();};

class SoTransformBoxManip : public SoTransformManip {protected: virtual ~SoTransformBoxManip();public: SoTransformBoxManip();};

class SoCenterballManip : public SoTransformManip {protected: virtual ~SoCenterballManip();public: SoCenterballManip();};

class SoClipPlaneManip : public SoClipPlane {protected: virtual ~SoClipPlaneManip();public: SoClipPlaneManip();};

class SoDirectionalLightManip : public SoDirectionalLight {protected: virtual ~SoDirectionalLightManip();public: SoDirectionalLightManip();};

class SoHandleBoxManip : public SoTransformManip {protected: virtual ~SoHandleBoxManip();public: SoHandleBoxManip();};

class SoJackManip : public SoTransformManip {protected: virtual ~SoJackManip();public: SoJackManip();};

class SoPointLightManip : public SoPointLight {protected: virtual ~SoPointLightManip();public: SoPointLightManip();};

class SoSpotLightManip : public SoSpotLight {protected: virtual ~SoSpotLightManip();public: SoSpotLightManip();};

class SoTabBoxManip : public SoTransformManip {protected: virtual ~SoTabBoxManip();public: SoTabBoxManip();};

class SoTrackballManip : public SoTransformManip {protected: virtual ~SoTrackballManip();public: SoTrackballManip();};

class SoTransformerManip : public SoTransformManip {protected: virtual ~SoTransformerManip();public: SoTransformerManip();};


*/

//////////////////////////////////////////////////////////////
/// Post SWIG ////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

%constant SbBool FALSE = 0;
%constant SbBool TRUE = 1;

%extend SoType {
  SoNode* createInstance(void) const {
    return (SoNode*)self->createInstance();
  }
}

%define cast_SoNode(aClass)
  aClass* cast_##aClass () {
    SoType soType = SoType::fromName(#aClass);
    if(self->isOfType(soType)==FALSE) return 0;
    return (aClass*)self;
  }
%enddef

%extend SoNode {
  cast_SoNode(SoSeparator);
  cast_SoNode(SoCylinder);
  cast_SoNode(SoCube);
}

//////////////////////////////////////////////////////////////
/// Trash ////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////


/*
//FIXME : have the below on %extend SoNode.
//FIXME : use typeinfo to get class name from T.
%inline {
template <class T> T* cast_SoNode(SoNode* aNode,const std::string& aT) {
  SoType soType = SoType::fromName(aT.c_str());
  if(aNode->isOfType(soType)==FALSE) return 0;
  return (T*)aNode;
}
};
//FIXME : have all nodes !
%template(cast_SoCylinder) cast_SoNode<SoCylinder>;
*/
