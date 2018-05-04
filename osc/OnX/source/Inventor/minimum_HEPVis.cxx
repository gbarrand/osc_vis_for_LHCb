// this :

#include <OnX/Inventor/HEPVis.h>

#include <HEPVis/actions/SoAlternateRepAction.h>
#include <HEPVis/actions/SoCollectAction.h>
#include <HEPVis/actions/SoCounterAction.h>
#include <HEPVis/actions/SoGL2PSAction.h>
#include <HEPVis/actions/SoIntersectionDetectionAction.h>
#include <HEPVis/actions/SoPainterAction.h>
#include <HEPVis/draggers/SoDragPoint2D.h>
#include <HEPVis/fields/SoSFPolyhedron.h>
#include <HEPVis/misc/SoStyleCache.h>
#include <HEPVis/misc/SoTools.h>
#include <HEPVis/misc/SoTools2.h>
#include <HEPVis/nodekits/SoAxes2D.h>
#include <HEPVis/nodekits/SoAxes3D.h>
#include <HEPVis/nodekits/SoAxis.h>
#include <HEPVis/nodekits/SoCanvas.h>
#include <HEPVis/nodekits/SoDetectorTreeKit.h>
#include <HEPVis/nodekits/SoDisplayRegion.h>
#include <HEPVis/nodekits/SoImageRegion.h>
#include <HEPVis/nodekits/SoInfosRegion.h>
#include <HEPVis/nodekits/SoLegendRegion.h>
#include <HEPVis/nodekits/SoPage.h>
#include <HEPVis/nodekits/SoPlotter.h>
#include <HEPVis/nodekits/SoPlotterRegion.h>
#include <HEPVis/nodekits/SoRegion.h>
#include <HEPVis/nodekits/SoTextRegion.h>
#include <HEPVis/nodes/SoEllipsoid.h>
#include <HEPVis/nodes/SoGrid.h>
#include <HEPVis/nodes/SoHighlightMaterial.h>
#include <HEPVis/nodes/SoImage.h>
#include <HEPVis/nodes/SoImageWriter.h>
#include <HEPVis/nodes/SoLineStyle.h>
#include <HEPVis/nodes/SoMarkerSet.h>
#include <HEPVis/nodes/SoPlottableStyles.h>
#include <HEPVis/nodes/SoPolyhedron.h>
#include <HEPVis/nodes/SoSceneGraph.h>
#include <HEPVis/nodes/SoSplineCurve.h>
#include <HEPVis/nodes/SoStyle.h>
#include <HEPVis/nodes/SoSubNode.h>
#include <HEPVis/nodes/SoTextHershey.h>
#include <HEPVis/nodes/SoTextStyle.h>
#include <HEPVis/nodes/SoTextTTF.h>
#include <HEPVis/nodes/SoViewportRegion.h>

#include <HEPVis/nodes/SoFile.h>

//////////////////////////////////////////////////////////////////////////////
void OnX::HEPVis::initClasses(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(HEPVis_SoFile::getClassTypeId()!=SoType::badType()) return; //done.

  // SO_NODE_SOURCE :
  SoEllipsoid::initClass();
  HEPVis_SoFile::initClass();
  SoGrid::initClass();
  SoHighlightMaterial::initClass();
  HEPVis_SoImage::initClass();
  SoImageWriter::initClass();
  SoLineStyle::initClass();
  HEPVis_SoMarkerSet::initClass();
  SoBinsStyle::initClass();
  SoPointsStyle::initClass();
  SoFunctionStyle::initClass();
  SoRightHatchStyle::initClass();
  SoLeftHatchStyle::initClass();
  SoErrorsStyle::initClass();
  SoPolyhedron::initClass();
  SoSceneGraph::initClass();
  SoSplineCurve::initClass();
  SoStyle::initClass();
  SoTextHershey::initClass();
  SoTextStyle::initClass();
  SoTextTTF::initClass();
  SoViewportRegion::initClass();

  // SO_KIT_SOURCE :
  SoAxes2D::initClass();
  SoAxes3D::initClass();
  SoAxis::initClass();
  SoCanvas::initClass();
  SoDetectorTreeKit::initClass();
  SoDisplayRegion::initClass();
  SoImageRegion::initClass();
  SoInfosRegion::initClass();
  SoLegendRegion::initClass();
  SoPage::initClass();
  SoPlotter::initClass();
  SoPlotterRegion::initClass();
  SoRegion::initClass();
  SoTextRegion::initClass();

  // SO_ACTION_SOURCE :
  SoAlternateRepAction::initClass();
  SoCollectAction::initClass();
  SoCounterAction::initClass();
  SoGL2PSAction::initClass();
  HEPVis_SoIntersectionDetectionAction::initClass();
  SoPainterAction::initClass();

  // SO_SFIELD_SOURCE :
  SoSFPolyhedron::initClass();

}

