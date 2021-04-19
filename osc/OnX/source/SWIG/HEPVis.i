// Used by OnX SWIG wraping to wrap without pivy.

//////////////////////////////////////////////////////////////////////////////
/// Pre SWIG /////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

RefCount(SoViewportRegion)
RefCount(SoImageWriter)
RefCount(HEPVis_SoImage)
RefCount(SoTextHershey)
RefCount(SoTextTTF)
RefCount(SoTextVtk)
RefCount(SoGrid)
RefCount(SoHighlightMaterial)
RefCount(SoStyleCache)
RefCount(SoRegion)
RefCount(SoDisplayRegion)
RefCount(SoImageRegion)
RefCount(SoTextRegion)
RefCount(SoCanvas)
RefCount(SoPage)
RefCount(SoAxis)
RefCount(SoAxes2D)
RefCount(SoAxes3D)
RefCount(SoStyle)
RefCount(SoLineStyle)
RefCount(SoTextStyle)
RefCount(SoAreaStyle)
RefCount(SoPyramid)
RefCount(SoSplineCurve)

RefCount(SoTubs)
RefCount(SoBox)
RefCount(SoCons)
RefCount(SoTrap)
RefCount(SoTrd)
RefCount(SoTorus)
RefCount(SoPcon)
RefCount(SoWedge)
RefCount(SoPolyVol)
RefCount(SoPolyhedron)
RefCount(SoArrow)
RefCount(SoEllipsoid)
RefCount(SoEllipsoidSegment)
RefCount(SoEllipticalPrism)
RefCount(So3DErrorBar)
RefCount(SoCoordinateAxis)
RefCount(SoDetectorExample)
RefCount(SoDetectorTreeKit)

RefCount(SoPlotter)
RefCount(SoPlotterRegion)

//////////////////////////////////////////////////////////////////////////////
/// Code to wrap /////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

class SoAlternateRepAction : public SoAction {
public:
  SoAlternateRepAction();
  virtual ~SoAlternateRepAction();  
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId();
  static void initClass();
};

class SoTools {
public:
  //static SoNode* searchNode(SoNode*,SoType);
  static SoNode* searchNode(SoNode*,const SbString&);
  static SbBool setKitPartValue(SoBaseKit*,const SbString&,const SbString&);
  static SbBool getKitPartValue(SoBaseKit*,const SbString&,SbString&);
  static SbBool toggleKitPartValue(SoBaseKit*,const SbString&);
  static void resetHighlight(SoNode*);
  //static void highlight(SoFullPath&);
};

class SoViewportRegion : public SoNode {
protected: 
  virtual ~SoViewportRegion();
public: 
  SoViewportRegion();
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId();
  static void initClass();
public:
  enum Unit {
    PERCENT,
    PIXELS
  };
  SoSFEnum originUnit;
  SoSFEnum sizeUnit;
  SoSFVec2f origin; // In percentage of window size.
  SoSFVec2f size; // In percentage of window size.
  SoSFShort horizontalBorder; // In pixels.
  SoSFShort verticalBorder; // In pixels.
  SoSFBool background;
  SoSFColor backgroundColor;
  SoSFColor borderColor;
  SoSFBool highlighted;
  SoSFColor highlightColor;
  SoSFShort horizontalHighlightBorder; // In pixels.
  SoSFShort verticalHighlightBorder; // In pixels.
  SoSFShort horizontalShadow; // In pixels.
  SoSFShort verticalShadow; // In pixels.
  SoSFBool sensitiveBorder;
  SoSFShort horizontalOriginOffset; // In pixels.
  SoSFShort verticalOriginOffset; // In pixels.
};

class SoImageWriter : public SoNode {
protected: 
  virtual ~SoImageWriter();
public: 
  SoImageWriter();
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId();
  static void initClass();
};

class HEPVis_SoImage : public SoNode {
protected: 
  virtual ~HEPVis_SoImage();
public: 
  HEPVis_SoImage();
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId();
  static void initClass();
public:
  enum Format {
    FITS,
    GIF,
    JPEG,
    DVI,
    TEX
  };
  SoSFEnum format;
  SoSFString fileName;
};

class SoStyle : public SoNode {
protected: 
  virtual ~SoStyle();
public: 
  SoStyle();
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId();
  static void initClass();
};

class SoLineStyle : public SoNode {
protected: 
  virtual ~SoLineStyle();
public: 
  SoLineStyle();
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId();
  static void initClass();
public:
  SoSFColor color;
  SoSFFloat transparency;
  SoSFInt32 width;
  SoSFUShort pattern;
  SoSFBool visible;
public:
  void reset();
  SbBool setFromString(const SbString&);
  SbStyle getSbStyle() const;
};

class SoAreaStyle : public SoNode {
protected: 
  virtual ~SoAreaStyle();
public: 
  SoAreaStyle();
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId();
  static void initClass();
public:
  SoSFColor color;
  SoSFFloat transparency;
  SoSFBool visible;
public:
  void reset();
  SbBool setFromString(const SbString&);
  SbStyle getSbStyle() const;
};

class SoTextStyle : public SoNode {
protected: 
  virtual ~SoTextStyle();
public: 
  SoTextStyle();
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId();
  static void initClass();
public:
  SoSFColor color;
  SoSFFloat transparency;
  SoSFBool visible;
public:
  void reset();
  SbBool setFromString(const SbString&);
  SbStyle getSbStyle() const;
};

class SoTextHershey : public SoShape {
protected: 
  virtual ~SoTextHershey();
public:
  SoTextHershey();
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId();
  static void initClass();
protected:
  virtual void computeBBox(SoAction*,SbBox3f&,SbVec3f&);
  virtual void generatePrimitives(SoAction*);
};

class SoTextTTF : public SoShape {
protected: 
  virtual ~SoTextTTF();
public:
  SoTextTTF();
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId();
  static void initClass();
protected:
  virtual void computeBBox(SoAction*,SbBox3f&,SbVec3f&);
  virtual void generatePrimitives(SoAction*);
};

class SoGrid : public SoShape {
protected: 
  virtual ~SoGrid();
public:
  SoGrid();
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId();
  static void initClass();
protected:
  virtual void computeBBox(SoAction*,SbBox3f&,SbVec3f&);
  virtual void generatePrimitives(SoAction*);
};

class SoTextVtk : public SoShape {
protected: 
  virtual ~SoTextVtk();
public:
  SoTextVtk();
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId();
  static void initClass();
protected:
  virtual void computeBBox(SoAction*,SbBox3f&,SbVec3f&);
  virtual void generatePrimitives(SoAction*);
};


class SoPyramid : public SoShape {
protected: 
  virtual ~SoPyramid();
public:
  SoPyramid();
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId();
  static void initClass();
protected:
  virtual void computeBBox(SoAction*,SbBox3f&,SbVec3f&);
  virtual void generatePrimitives(SoAction*);
};

class SoSplineCurve : public SoShape {
protected: 
  virtual ~SoSplineCurve();
public:
  SoSplineCurve();
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId();
  static void initClass();
protected:
  virtual void computeBBox(SoAction*,SbBox3f&,SbVec3f&);
  virtual void generatePrimitives(SoAction*);
};

/*
// HEPVisUtils :
class HEPVis_SoMarkerSet : public SoNode {protected: virtual ~HEPVis_SoMarkerSet();public: HEPVis_SoMarkerSet();virtual SoType getTypeId(void) const;static SoType getClassTypeId();};

class SoEdgedFaceSet : public SoNode {
protected: 
  virtual ~SoEdgedFaceSet();
public: 
  SoEdgedFaceSet();
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId();
};
*/

class SoHighlightMaterial : public SoMaterial {
protected: 
  virtual ~SoHighlightMaterial();
public: 
  SoHighlightMaterial();
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId();
  static void initClass();
};

typedef unsigned short SbLinePattern;

class SoStyleCache : public SoGroup {
public:
  SoStyleCache();
protected:
  ~SoStyleCache();
public:
  SoMaterial* getMaterial(float,float,float,float); 
  SoHighlightMaterial* getHighlightMaterial(float,float,float,float,float,float,float); 
  SoDrawStyle* getLineStyle(SbLinePattern,float); 
  SoDrawStyle* getFilled(); 
  SoLightModel* getLightModelPhong(); 
  SoLightModel* getLightModelBaseColor(); 
  SoResetTransform* getResetTransform();
};

class SbDoIt {
public:
  virtual void* cast(const SbString&) const = 0;
};

class SbAddNode : public virtual SbDoIt {
public: //SbDoIt
  virtual void* cast(const SbString& aClass) const;
public:
  SbAddNode(SoNode*,const SbString&);
  virtual ~SbAddNode();
};

class SoRegion : public SoBaseKit {
protected: 
  virtual ~SoRegion();
public: 
  SoRegion();
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId();
  static void initClass();
public:
  SoViewportRegion* getViewportRegion() const;
  SoNode* findPart(const SbName&);
  SoNode* findNode(const SbString&,const SbString&);
  SoStyleCache* styleCache() const;
public:
  virtual SoCamera* getCamera() const;
  virtual void clear(const SbString& = "");
  virtual void refresh();
  virtual SbBool doIt(const SbDoIt&);
  virtual void undo();
  virtual void resetUndo();
  virtual SbBool connectFrom(SoRegion*);
  virtual SbBool setNode(const SbString&,const SbString&,const SbString&); 
  virtual void setPosition(float,float);
  virtual void setSize(float,float); 
  virtual SbBool readScene(const SbString& file,
                           const SbString& format,
                           const SbString& placement = "");
public:
  SoSFBool visible;
  SoSFBool verbose;
  SoSFColor color;
  SoSFShort horizontalMargin; //In pixels
  SoSFShort verticalMargin; //In pixels
};

class SoDisplayRegion : public SoRegion {
protected: 
  virtual ~SoDisplayRegion();
public: 
  SoDisplayRegion();
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId();
  static void initClass();
public: 
  SoSeparator* getStaticScene() const;
  SoSeparator* getDynamicScene() const;
};

class SoImageRegion : public SoRegion {
protected: 
  virtual ~SoImageRegion();
public: 
  SoImageRegion();
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId();
  static void initClass();
public:
  HEPVis_SoImage* getImage() const;
  virtual SoCamera* getCamera() const;
};

class SoTextRegion : public SoRegion {
protected: 
  virtual ~SoTextRegion();
public:
  SoTextRegion();
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId();
  static void initClass();
};

class SoCanvas : public SoBaseKit {
protected: 
  virtual ~SoCanvas();
public: 
  SoCanvas();
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId();
  static void initClass();
public: //SoCanvas
  // Region factories :
  SoRegion* createRegion(const SbString&,float,float,float,float);
  void deleteRegion(SoRegion*);
  void clearRegions();
  void deleteRegions();
  void refreshRegions();
  int getNumberOfRegions() const;
  SoRegion* getRegion(int) const;
  int getRegionIndex(SoRegion*) const;
  SoRegion* getNextRegion(SoRegion*) const;
  SoRegion* getPreviousRegion(SoRegion*) const;
  void setRegionOnTop(SoRegion*);
  //SoRegion* getPathRegion(const SoFullPath&) const;
  SoRegion* getPickedRegion(int,int) const;
  //void setSetRegionCallback(SbSetRegionCallback*);
  // Highlighted region :
  SoRegion* getHighlightedRegion() const;
  void highlightRegion(SoRegion*);
  void highlightNextRegion();
  void highlightPreviousRegion();
  void deleteHighlightedRegion();
  // Root regions :
  int getNumberOfRootRegions() const;
  SbBool isRootRegion(SoRegion*) const;
  SoRegion* getRootRegion(int) const;
  int getRootRegionIndex(SoRegion*) const;
  SoRegion* getNextRootRegion(SoRegion*) const;
  SoRegion* getPreviousRootRegion(SoRegion*) const;
  SoRegion* currentRegion() const;
  SbBool setCurrentRegion(int);
  SbBool setCurrentRegion(SoRegion*);
  SbBool connectCurrentRegion(int);
public:
  SoSFBool verbose;
};

class SoPage : public SoCanvas {
protected: 
  virtual ~SoPage();
public: 
  SoPage();
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId();
  static void initClass();
public:
  void createRegions(const SbString&,int,int,int);
  void addRegions(const SbString&,int,int);
  void mapHighlightedRegionToPage();
  SoRegion* createGridRegion(const SbString&,int,int,int);
public:
  SoMFString title;
  SoSFBool titleVisible;
  SoSFFloat titleHeight;
  enum ResizePolicy {
    LEAVE_ALONE,
    PAW_GRID
  };
  SoSFEnum regionsResizePolicy;
  // PAW_GRID parameters :
  SoSFFloat pageWidth;         //PAW XSIZ
  SoSFFloat pageHeight;        //PAW YSIZ
  SoSFFloat leftMargin;        //PAW XMGL
  SoSFFloat rightMargin;       //PAW XMGR
  SoSFFloat bottomMargin;      //PAW YMGL
  SoSFFloat topMargin;         //PAW YMGU
  SoSFFloat horizontalSpacing; //PAW XWIN
  SoSFFloat verticalSpacing;   //PAW YWIN
};


class SoAxis : public SoBaseKit {
protected: 
  virtual ~SoAxis();
public: 
  SoAxis();
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId();
  static void initClass();
public:
  void reset();
  void updateChildren();
  //SoLineStyle* getLineStyle(); 
  //SoLineStyle* getTicksStyle(); 
  //SoTextStyle* getLabelsStyle(); 
  //SoTextStyle* getTitleStyle(); 
  //SoTextStyle* getMagStyle(); 
public:
  //SoSFNode alternateRep;
  SoSFBool verbose;
  SoSFString modeling; //hippo, hplot
  SoSFFloat width;
  SoSFFloat minimumValue;
  SoSFFloat maximumValue;
  //SoSFInt32 divisions;
  SoSFBool logScale;
  SoSFBool tickUp;
  SoSFFloat tickLength;
  SoSFFloat labelToAxis;
  SoSFFloat labelHeight;
  SoSFString title;
  SoSFFloat titleToAxis;
  SoSFFloat titleHeight;
  enum Justification {
    LEFT   = 0x00,
    CENTER = 0x01,
    RIGHT  = 0x02
  };
  SoSFEnum titleToAxisJustification;

  SoSFFloat ttfScale;

  // filled by computeTicks
  //SoSFUInt32 tickNumber; //Output
  SoMFString labels;     //Output
  SoMFFloat values;      //Output
  SoMFFloat coords;      //Output
  //SoSFInt32 magnitude;   //Output
};

class SoAxes2D : public SoBaseKit {
protected: 
  virtual ~SoAxes2D();
public: 
  SoAxes2D();
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId();
  static void initClass();
public:
  SoAxis* getXAxis();
  SoAxis* getYAxis();
public:
  enum LineType{
    SOLID = 0,
    DASHED,
    DOTTED,
    DASH_DOTTED
  };
  SoSFColor gridColor;  
  SoSFFloat gridTransparency; 
  //FIXME SoSFUShort gridLinePattern;  
  SoSFBool gridVisible;  

  SoSFColor wallColor; 
  SoSFFloat wallTransparency; 
  SoSFFloat wallOffset; 
  SoSFBool wallVisible;  

  SoSFBool topAxisVisible;  
  SoSFFloat topAxisY;
  SoSFBool rightAxisVisible;  
  SoSFFloat rightAxisX;
};

class SoAxes3D : public SoBaseKit {
protected: 
  virtual ~SoAxes3D();
public: 
  SoAxes3D();
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId();
  static void initClass();
public:
  SoAxis* getXAxis();
  SoAxis* getYAxis();
  SoAxis* getZAxis();
public:
  SoSFColor gridColor;  
  SoSFFloat gridTransparency; 
  //FIXME SoSFUShort gridLinePattern;  
  SoSFBool gridVisible;  

  SoSFColor wallColor; 
  SoSFFloat wallTransparency; 
  SoSFFloat wallOffset; 
  SoSFBool wallVisible;  
};

// HEPVisGeometry :
class SoTubs : public SoShape {
protected: 
  virtual ~SoTubs();
public:
  SoTubs();
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId();
  static void initClass();
protected:
  virtual void computeBBox(SoAction*,SbBox3f&,SbVec3f&);
  virtual void generatePrimitives(SoAction*);
};

class SoBox : public SoShape {
protected: 
  virtual ~SoBox();
public:
  SoBox();
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId();
  static void initClass();
protected:
  virtual void computeBBox(SoAction*,SbBox3f&,SbVec3f&);
  virtual void generatePrimitives(SoAction*);
};

class SoCons : public SoShape {
protected: 
  virtual ~SoCons();
public:
  SoCons();
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId();
  static void initClass();
protected:
  virtual void computeBBox(SoAction*,SbBox3f&,SbVec3f&);
  virtual void generatePrimitives(SoAction*);
};

class SoTrap : public SoShape {
protected: 
  virtual ~SoTrap();
public:
  SoTrap();
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId();
  static void initClass();
protected:
  virtual void computeBBox(SoAction*,SbBox3f&,SbVec3f&);
  virtual void generatePrimitives(SoAction*);
};

class SoTrd : public SoShape {
protected: 
  virtual ~SoTrd();
public:
  SoTrd();
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId();
  static void initClass();
protected:
  virtual void computeBBox(SoAction*,SbBox3f&,SbVec3f&);
  virtual void generatePrimitives(SoAction*);
};

class SoTorus : public SoShape {
protected: 
  virtual ~SoTorus();
public:
  SoTorus();
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId();
  static void initClass();
protected:
  virtual void computeBBox(SoAction*,SbBox3f&,SbVec3f&);
  virtual void generatePrimitives(SoAction*);
};

class SoPcon : public SoShape {
protected: 
  virtual ~SoPcon();
public:
  SoPcon();
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId();
  static void initClass();
protected:
  virtual void computeBBox(SoAction*,SbBox3f&,SbVec3f&);
  virtual void generatePrimitives(SoAction*);
};

class SoWedge : public SoShape {
protected: 
  virtual ~SoWedge();
public:
  SoWedge();
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId();
  static void initClass();
protected:
  virtual void computeBBox(SoAction*,SbBox3f&,SbVec3f&);
  virtual void generatePrimitives(SoAction*);
};

class SoPolyVol : public SoShape {
protected: 
  virtual ~SoPolyVol();
public:
  SoPolyVol();
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId();
  static void initClass();
protected:
  virtual void computeBBox(SoAction*,SbBox3f&,SbVec3f&);
  virtual void generatePrimitives(SoAction*);
};

class SoPolyhedron : public SoShape {
protected: 
  virtual ~SoPolyhedron();
public:
  SoPolyhedron();
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId();
  static void initClass();
protected:
  virtual void computeBBox(SoAction*,SbBox3f&,SbVec3f&);
  virtual void generatePrimitives(SoAction*);
};

class SoArrow : public SoShape {
protected: 
  virtual ~SoArrow();
public:
  SoArrow();
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId();
  static void initClass();
protected:
  virtual void computeBBox(SoAction*,SbBox3f&,SbVec3f&);
  virtual void generatePrimitives(SoAction*);
};

class SoEllipsoid : public SoShape {
protected: 
  virtual ~SoEllipsoid();
public:
  SoEllipsoid();
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId();
  static void initClass();
protected:
  virtual void computeBBox(SoAction*,SbBox3f&,SbVec3f&);
  virtual void generatePrimitives(SoAction*);
};

class SoEllipsoidSegment : public SoShape {
protected: 
  virtual ~SoEllipsoidSegment();
public:
  SoEllipsoidSegment();
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId();
  static void initClass();
protected:
  virtual void computeBBox(SoAction*,SbBox3f&,SbVec3f&);
  virtual void generatePrimitives(SoAction*);
};

class SoEllipticalPrism : public SoShape {
protected: 
  virtual ~SoEllipticalPrism();
public:
  SoEllipticalPrism();
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId();
  static void initClass();
protected:
  virtual void computeBBox(SoAction*,SbBox3f&,SbVec3f&);
  virtual void generatePrimitives(SoAction*);
};

class So3DErrorBar : public SoShape {
protected: 
  virtual ~So3DErrorBar();
public:
  So3DErrorBar();
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId();
  static void initClass();
protected:
  virtual void computeBBox(SoAction*,SbBox3f&,SbVec3f&);
  virtual void generatePrimitives(SoAction*);
};

class SoCoordinateAxis : public SoShape {
protected: 
  virtual ~SoCoordinateAxis();
public:
  SoCoordinateAxis();
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId();
  static void initClass();
protected:
  virtual void computeBBox(SoAction*,SbBox3f&,SbVec3f&);
  virtual void generatePrimitives(SoAction*);
};

class SoDetectorExample : public SoBaseKit {
protected: 
  virtual ~SoDetectorExample();
public: 
  SoDetectorExample();
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId();
  static void initClass();
};


// HEPVisDetector :
class SoDetectorTreeKit : public SoBaseKit {
protected: 
  virtual ~SoDetectorTreeKit();
public: 
  SoDetectorTreeKit();
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId();
  static void initClass();
};

/*
class SoHelicalTrack : public SoNode {protected: virtual ~SoHelicalTrack();public: SoHelicalTrack();virtual SoType getTypeId(void) const;static SoType getClassTypeId();};
class SoMeterStickKit : public SoNode {protected: virtual ~SoMeterStickKit();public: SoMeterStickKit();virtual SoType getTypeId(void) const;static SoType getClassTypeId();};
class SoCrystalHit : public SoNode {protected: virtual ~SoCrystalHit();public: SoCrystalHit();virtual SoType getTypeId(void) const;static SoType getClassTypeId();};
class SoHadEmCrystalHit : public SoNode {protected: virtual ~SoHadEmCrystalHit();public: SoHadEmCrystalHit();virtual SoType getTypeId(void) const;static SoType getClassTypeId();};
class SoDblSiStripDetKit : public SoNode {protected: virtual ~SoDblSiStripDetKit();public: SoDblSiStripDetKit();virtual SoType getTypeId(void) const;static SoType getClassTypeId();};
class SoJet : public SoNode {protected: virtual ~SoJet();public: SoJet();virtual SoType getTypeId(void) const;static SoType getClassTypeId();};
class SoReconTrack : public SoNode {protected: virtual ~SoReconTrack();public: SoReconTrack();virtual SoType getTypeId(void) const;static SoType getClassTypeId();};
class SoSiStripDetKit : public SoNode {protected: virtual ~SoSiStripDetKit();public: SoSiStripDetKit();virtual SoType getTypeId(void) const;static SoType getClassTypeId();};
class SoSiStrips : public SoNode {protected: virtual ~SoSiStrips();public: SoSiStrips();virtual SoType getTypeId(void) const;static SoType getClassTypeId();};
class SoStereoSiStripDetKit : public SoNode {protected: virtual ~SoStereoSiStripDetKit();public: SoStereoSiStripDetKit();virtual SoType getTypeId(void) const;static SoType getClassTypeId();};
class SoSiStripWedgeDetKit : public SoNode {protected: virtual ~SoSiStripWedgeDetKit();public: SoSiStripWedgeDetKit();virtual SoType getTypeId(void) const;static SoType getClassTypeId();};
class SoDblSiStripWedgeDetKit : public SoNode {protected: virtual ~SoDblSiStripWedgeDetKit();public: SoDblSiStripWedgeDetKit();virtual SoType getTypeId(void) const;static SoType getClassTypeId();};
class SoSiWedgeStrips : public SoNode {protected: virtual ~SoSiWedgeStrips();public: SoSiWedgeStrips();virtual SoType getTypeId(void) const;static SoType getClassTypeId();};
class SoVtxReconTrack : public SoNode {protected: virtual ~SoVtxReconTrack();public: SoVtxReconTrack();virtual SoType getTypeId(void) const;static SoType getClassTypeId();};
class SoIdealTrackKit : public SoNode {protected: virtual ~SoIdealTrackKit();public: SoIdealTrackKit();virtual SoType getTypeId(void) const;static SoType getClassTypeId();};
class SoIdealBeamTrackKit : public SoNode {protected: virtual ~SoIdealBeamTrackKit();public: SoIdealBeamTrackKit();virtual SoType getTypeId(void) const;static SoType getClassTypeId();};

// HEPVisPlotter :
//class SbPlottableObject {
//public:
//  virtual void* cast(const char*) const = 0;
//};
*/

class SoPlotter : public SoBaseKit {
protected: 
  virtual ~SoPlotter();
public: 
  SoPlotter();
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId();
  static void initClass();
public:
  //void setPlottableObject(SbPlottableObject*);
  //void addPlottableObject(SbPlottableObject*);
  //void removePlottableObject(SbPlottableObject*);
  SbBool isEmpty() const;
  //const SbPList& getPlottableObjects() const;
  SoSeparator* getEtcSeparator() const;
  SoSeparator* getEtcDataSeparator() const;
};

class SoPlotterRegion : public SoRegion {
protected: 
  virtual ~SoPlotterRegion();
public: 
  SoPlotterRegion();
  virtual SoType getTypeId(void) const;
  static SoType getClassTypeId();
  static void initClass();
public: 
  SoPlotter* getPlotter() const;
  SoTextRegion* getInfosRegion() const;
  SoTextRegion* getLegendRegion() const;
  //void addPlottableObject(SbPlottableObject*);
  SoSeparator* getEtcSeparator() const;
};

/*
// HEPVisGUI :
class SoBackPrimitive : public SoNode {protected: virtual ~SoBackPrimitive();public: SoBackPrimitive();virtual SoType getTypeId(void) const;static SoType getClassTypeId();};
class SoPrimitive : public SoNode {protected: virtual ~SoPrimitive();public: SoPrimitive();virtual SoType getTypeId(void) const;static SoType getClassTypeId();};
class SoForm : public SoNode {protected: virtual ~SoForm();public: SoForm();virtual SoType getTypeId(void) const;static SoType getClassTypeId();};
class SoPushButton : public SoNode {protected: virtual ~SoPushButton();public: SoPushButton();virtual SoType getTypeId(void) const;static SoType getClassTypeId();};
class SoArrowButton : public SoNode {protected: virtual ~SoArrowButton();public: SoArrowButton();virtual SoType getTypeId(void) const;static SoType getClassTypeId();};
class SoScrolledList : public SoNode {protected: virtual ~SoScrolledList();public: SoScrolledList();virtual SoType getTypeId(void) const;static SoType getClassTypeId();};
class SoSelectionBox : public SoNode {protected: virtual ~SoSelectionBox();public: SoSelectionBox();virtual SoType getTypeId(void) const;static SoType getClassTypeId();};
class SoList : public SoNode {protected: virtual ~SoList();public: SoList();virtual SoType getTypeId(void) const;static SoType getClassTypeId();};
class SoText : public SoNode {protected: virtual ~SoText();public: SoText();virtual SoType getTypeId(void) const;static SoType getClassTypeId();};
class SoCommand : public SoNode {protected: virtual ~SoCommand();public: SoCommand();virtual SoType getTypeId(void) const;static SoType getClassTypeId();};
class SoFileSelectionBox : public SoNode {protected: virtual ~SoFileSelectionBox();public: SoFileSelectionBox();virtual SoType getTypeId(void) const;static SoType getClassTypeId();};
class SoShell : public SoNode {protected: virtual ~SoShell();public: SoShell();virtual SoType getTypeId(void) const;static SoType getClassTypeId();};
*/

//////////////////////////////////////////////////////////////
/// Post SWIG ////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

#ifdef SWIGPYTHON
%pythoncode %{

def Utils_initClasses():
  SoLineStyle.initClass()
  SoTextStyle.initClass()
  SoAreaStyle.initClass()
  SoStyle.initClass()
  #HEPVis_SoMarkerSet.initClass()
  HEPVis_SoImage.initClass()
  SoImageWriter.initClass()
  SoViewportRegion.initClass()
  SoTextHershey.initClass()
  SoTextTTF.initClass()
  SoTextVtk.initClass()
  SoGrid.initClass()
  #SoEdgedFaceSet.initClass()
  SoHighlightMaterial.initClass()
  SoSplineCurve.initClass()
  SoPyramid.initClass()

  SoAxis.initClass()
  SoAxes2D.initClass()
  SoAxes3D.initClass()
  #SoSimpleButton.initClass()
  SoRegion.initClass()
  SoTextRegion.initClass()
  SoImageRegion.initClass()
  SoDisplayRegion.initClass()
  #SoRulerRegion.initClass()
  #SoFormRegion.initClass()
  SoCanvas.initClass()
  SoPage.initClass()

  #SoPainterAction.initClass()
  #SoGL2PSAction.initClass()
  #SoCollectAction.initClass()
  #SoCounterAction.initClass()
  SoAlternateRepAction.initClass()

%}
#endif

