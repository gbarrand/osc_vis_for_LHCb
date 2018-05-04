#ifndef OnX_GuiViewer_h
#define OnX_GuiViewer_h

#include <inlib/touplow>
#include <inlib/path>
#include <inlib/args>
#include <inlib/pointer>
#include <inlib/system>
#include <inlib/scolor>

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/SoDB.h>
#include <Inventor/SoInput.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/SoOffscreenRenderer.h>
#include <Inventor/SoSceneManager.h>

#include <HEPVis/SbPainterPS.h>
#include <HEPVis/SbString.h>
#include <HEPVis/misc/SoTools.h>
#include <HEPVis/nodes/SoViewportRegion.h>
#include <HEPVis/nodes/SoImageWriter.h>
#include <HEPVis/nodes/SoImage.h>
#include <HEPVis/nodekits/SoPage.h>
#include <HEPVis/nodekits/SoRegion.h>
#include <HEPVis/nodekits/SoDisplayRegion.h>
#include <HEPVis/nodekits/SoImageRegion.h>
#include <HEPVis/actions/SoPainterAction.h>
#include <HEPVis/actions/SoGL2PSAction.h>

template <class SoGuiViewer> 
class GuiPageSetRegionCallback : public virtual SbSetRegionCallback {
public:
  GuiPageSetRegionCallback(SoGuiViewer& aViewer):fViewer(aViewer){}
  virtual void setCamera(SoCamera* aCamera) {
    if(aCamera==fViewer.getCamera()) return; //done.
    fViewer.setCamera(aCamera);
  }
private:
  SoGuiViewer& fViewer;
};

template <class SoGuiViewer> 
inline void GuiViewer_initialize(SoGuiViewer& aThis) {
  const SbViewportRegion& vpRegion = aThis.getViewportRegion();
  SoGL2PSAction* action = new SoGL2PSAction(vpRegion);
  aThis.setGLRenderAction(action);

  SoPage* soPage = new SoPage;
  soPage->ref();
  // SetRegionCallback object deleted by the SoPage.
  soPage->setSetRegionCallback
    (new GuiPageSetRegionCallback<SoGuiViewer>(aThis));
    
  aThis.setSceneGraph(soPage);
    
  soPage->highlightRegion(soPage->getRootRegion(0));

  // Disconnect headlight since all regions have their own light.
  // (Headlight will add a directinal light, then scene will
  // be more bright).
  // OSF1/cxx needs the "this->".
  aThis.setHeadlight(FALSE);
}

template <class SoGuiViewer> 
inline SoPage* GuiViewer_SoPage(SoGuiViewer& aThis) {
  SoNode* node = aThis.getSceneGraph();
  if(!node) return 0;        
  if(node->isOfType(SoPage::getClassTypeId())==FALSE) return 0;
  return (SoPage*)node;
}

template <class SoGuiViewer> 
inline bool GuiViewer_finalize(SoGuiViewer& aThis) {
  SoPage* soPage = GuiViewer_SoPage<SoGuiViewer>(aThis);
  if(!soPage) return false;
  soPage->setSetRegionCallback(0);
  soPage->unref();
  return true;
}

template <class SoGuiViewer> 
inline bool GuiViewer_writeGIF(SoGuiViewer& aThis,const std::string& aFile) {
  SoPage* soPage = GuiViewer_SoPage<SoGuiViewer>(aThis);
  if(!soPage) return false;
  SoRegion* soRegion = soPage->getHighlightedRegion();
  if(!soRegion) return false;

  SoImageWriter* imageWriter = soPage->getImageWriter();
  imageWriter->fileName.setValue(aFile.c_str());
  imageWriter->format.setValue(SoImageWriter::GIF);
  imageWriter->enable();

  SoViewportRegion* viewportRegion = soRegion->getViewportRegion();
  viewportRegion->highlighted.setValue(FALSE);
  aThis.render();
  viewportRegion->highlighted.setValue(TRUE);

  imageWriter->disable();
  if(imageWriter->getStatus()) {
    SoDebugError::postInfo("GuiViewer_writeGIF",
                           "%s produced.",
                           imageWriter->fileName.getValue().getString());
    return true;
  } else {
    SoDebugError::postInfo("GuiViewer_writeGIF",
                           "%s not produced.",
                           imageWriter->fileName.getValue().getString());
    return false;
  }
}

template <class SoGuiViewer> 
inline bool GuiViewer_writeJPEG(SoGuiViewer& aThis,const std::string& aFile,int aQuality = 60) {
  SoPage* soPage = GuiViewer_SoPage<SoGuiViewer>(aThis);
  if(!soPage) return false;
  SoRegion* soRegion = soPage->getHighlightedRegion();
  if(!soRegion) return false;

  SoImageWriter* imageWriter = soPage->getImageWriter();
  imageWriter->fileName.setValue(aFile.c_str());
  imageWriter->format.setValue(SoImageWriter::JPEG);
  imageWriter->quality.setValue(aQuality);
  imageWriter->enable();

  SoViewportRegion* viewportRegion = soRegion->getViewportRegion();
  viewportRegion->highlighted.setValue(FALSE);
  aThis.render();
  viewportRegion->highlighted.setValue(TRUE);

  imageWriter->disable();
  if(imageWriter->getStatus()) {
    SoDebugError::postInfo("GuiViewer_writeJPEG",
                           "%s produced.",
                           imageWriter->fileName.getValue().getString());
    return true;
  } else {
    SoDebugError::postInfo("GuiViewer_writeJPEG",
                           "%s not produced.",
                           imageWriter->fileName.getValue().getString());
    return false;
  }
}

template <class SoGuiViewer> 
inline bool GuiViewer_writePS(SoGuiViewer& aThis,const std::string& aFile) {
  SoPage* soPage = GuiViewer_SoPage<SoGuiViewer>(aThis);
  if(!soPage) return false;
  SoRegion* soRegion = soPage->getHighlightedRegion();
  if(!soRegion) return false;
  SoImageWriter* imageWriter = soPage->getImageWriter();
  imageWriter->fileName.setValue(aFile.c_str());
  imageWriter->format.setValue(SoImageWriter::POST_SCRIPT);
  imageWriter->enable();

  SoViewportRegion* viewportRegion = soRegion->getViewportRegion();
  viewportRegion->highlighted.setValue(FALSE);
  aThis.render();
  viewportRegion->highlighted.setValue(TRUE);

  imageWriter->disable();
  if(imageWriter->getStatus()) {
    SoDebugError::postInfo("GuiViewer_writePS",
                           "%s produced.",
                           imageWriter->fileName.getValue().getString());
    return true;
  } else {
    SoDebugError::postInfo("GuiViewer_writePS",
                           "%s not produced.",
                           imageWriter->fileName.getValue().getString());
    return false;
  }
}  

inline bool GuiViewer_writePSVec(
 const std::string& aFile
,const SbViewportRegion& aRegion
,const SbColor& aBackground
,SoNode* aSceneGraph) {
  // Open file :
  SbPainterPS painterPS;
  painterPS.openFileForWriting(aFile.c_str());
  // Produce view :
  SoPainterAction* action = new SoPainterAction(aRegion,&painterPS);
  action->setBackgroundColor(aBackground);
  action->apply(aSceneGraph);
  delete action;
  // Close file :
  bool status;
  if(painterPS.getStream()) {
    SoDebugError::postInfo("GuiViewer_writePSVec",
                           "%s produced.",
                           painterPS.getFileName());
    status = true;
  } else {
    SoDebugError::postInfo("GuiViewer_writePSVec",
                           "%s not produced.",
                           painterPS.getFileName());
    status = false;
  }
  painterPS.closeStream();
  return status;
}

template <class SoGuiViewer> 
inline SoNode* GuiViewer_getWhat(SoGuiViewer& aThis,const std::string& aWhat) {
  if(aWhat=="viewerSceneGraph") {
    SoNode* sg = aThis.getSceneGraph();
    if(!sg) {
      SoDebugError::postInfo("GuiViewer_getWhat",
                             "Viewer has no scene graph.");
      return 0;
    }
    return sg;   
  } else if(aWhat=="viewerAllSceneGraph") {
    SoSceneManager* sm = aThis.getSceneManager();
    if(!sm) {
      SoDebugError::postInfo("GuiViewer_getWhat",
                             "Viewer has no scene manager.");
      return 0;
    }
    SoNode* sg = sm->getSceneGraph();
    if(!sg) {
      SoDebugError::postInfo("GuiViewer_getWhat",
                             "Scene manager has no scene graph.");
      return 0;
    }
    return sg;   
  }

  //page, region, regionTopSeparator, scene, staticScene, dynamicScene

  SoNode* sg = aThis.getSceneGraph();
  if(!sg) {
    SoDebugError::postInfo("GuiViewer_getWhat",
                           "Viewer has no scene graph.");
    return 0;
  }

  if(sg->isOfType(SoPage::getClassTypeId())==TRUE) {
    if(aWhat=="page") {
      return sg;
    } else if(aWhat=="region") {
      SoPage* soPage = (SoPage*)sg;
      SoNode* node = soPage->getHighlightedRegion();
      if(!node) {
        SoDebugError::postInfo("GuiViewer_getWhat",
                               "Can't get current region.");
        return 0;
      }
      return node;
    } else { //regionTopSeparator, scene, dynamicScene, staticScene :
      SoPage* soPage = (SoPage*)sg;
      SoRegion* soRegion = soPage->getHighlightedRegion();
      if(!soRegion) {
        SoDebugError::postInfo("GuiViewer_getWhat",
                               "Can't get current region.");
        return 0;
      }
      //regionTopSeparator is the default of getNodeOfInterest.
      SoNode* node = soRegion->getNodeOfInterest(aWhat.c_str());
      if(!node) {
        SoDebugError::postInfo("GuiViewer_getWhat",
                               "Can't get write node for \"%s\".",
                               aWhat.c_str());
        return 0;
      }
      return node;
    }
  } else {
    return sg;
  }
}

template <class SoGuiViewer> 
inline bool GuiViewer_writeGL2(SoGuiViewer& aThis,
                        const std::string& aFile,
                        SoGL2PSAction::Format aFormat,
                        const std::string& aOptions) {
  SoGLRenderAction* action = aThis.getGLRenderAction();  
  if(!action) return false;
  if(action->isOfType(SoGL2PSAction::getClassTypeId())) { 
    SoGL2PSAction* gl2ps_action = (SoGL2PSAction*)action;
    gl2ps_action->setPageOptions(aOptions.c_str());
    gl2ps_action->setFileFormat(aFormat);
    gl2ps_action->setFileName(aFile.c_str());
    gl2ps_action->enableFileWriting();
    aThis.render();
    gl2ps_action->disableFileWriting();
    return true;
  } else {
    SoDebugError::postInfo("GuiViewer_writeGL2",
			   "SoGLRenderAction not a SoGL2PSAction.");
    return false;
  }
}

template <class SoGuiViewer> 
inline bool GuiViewer_write(SoGuiViewer& aThis,
                     const std::string& aFormat,
                     const std::string& aFile,
                     const std::string& aOptions) {
    //printf("debug : GuiViewer_write : \"%s\" \"%s\"\n",
    //aFormat.c_str(),aFile.c_str());
    std::string format(aFormat);
    inlib::tolowercase(format);

    if(format=="guessed") format = inlib::suffix(aFile);

    if(format=="gif") {
      return GuiViewer_writeGIF(aThis,aFile);
    } else if(format=="jpeg") {
      int quality;
      if(aOptions.empty()) {
        quality = 60;
      } else {
        if(!inlib::to<int>(aOptions,quality)) {
          SoDebugError::postInfo("GuiViewer_write",
                                 "quality option \"%s\" not an int.",
                                 aOptions.c_str());
          return false;
        }
      }
      return GuiViewer_writeJPEG(aThis,aFile,quality);
    } else if(format=="ps") {
      GuiViewer_writePS(aThis,aFile);
      return true;
    } else if(format=="gl2ps") {
      return GuiViewer_writeGL2(aThis,aFile,SoGL2PSAction::EPS,aOptions);
    } else if(format=="gl2tex") {
      return GuiViewer_writeGL2(aThis,aFile,SoGL2PSAction::TEX,aOptions);
    } else if(format=="gl2pdf") {
      return GuiViewer_writeGL2(aThis,aFile,SoGL2PSAction::PDF,aOptions);
    } else if(format=="gl2svg") {
      return GuiViewer_writeGL2(aThis,aFile,SoGL2PSAction::SVG,aOptions);
    } else if(format=="gl2pgf") {
      return GuiViewer_writeGL2(aThis,aFile,SoGL2PSAction::PGF,aOptions);
    } else if(format=="psvec") {
      return GuiViewer_writePSVec(aFile,
                 aThis.getViewportRegion(),
                 aThis.getBackgroundColor(),
                 aThis.getSceneGraph());
    } else if( (format=="hiv") || (format=="iv") || (format=="inventor")  ||
               (format=="wrl") || (format=="vrml2") || 
               (format=="pov") ){

      bool binary = false;
      bool altRep = false;
      std::string what = "scene"; //scene staticScene dynamicScene region page

     {inlib::args args(aOptions,";",true);
      args.find("binary",binary);
      args.find("altRep",altRep);
      std::string value;
      if(args.find("what",value)) what = value;}

      SoNode* node = GuiViewer_getWhat(aThis,what);
      if(!node) return false;

      if(SoTools::write(*node,
                        aFile.c_str(),
                        format.c_str(),
                        binary?TRUE:FALSE,
                        altRep?TRUE:FALSE)==FALSE) {
        SoDebugError::postInfo("GuiViewer_write",
                               "Can't write file \"%s\".",aFile.c_str());
        return false;
      }

      SoDebugError::postInfo("GuiViewer_write",
                             "file \"%s\" produced.",aFile.c_str());

      return true;

    } else if(format=="offscreen") {

      const SbVec2s& win_size = aThis.getViewportRegion().getWindowSize();

      unsigned int width = win_size[0];
      unsigned int height = win_size[1];
      float scale = 1;
      std::string what = "viewerAllSceneGraph";
      bool no_tiling = false;

     {inlib::args args(aOptions,";",true);
      if(!args.find<float>("scale",scale)) scale = 1;
      std::string value;
     {if(args.find("width",value)) {
        if(value=="scale*window") {
          width = (unsigned int)(scale*win_size[0]);
        } else {
          if(!inlib::to<unsigned int>(value,width)) {
            width = win_size[0];
          }
        }
      }}
     {if(args.find("height",value)) {
        if(value=="scale*window") {
          height = (unsigned int)(scale*win_size[1]);
        } else {
          if(!inlib::to<unsigned int>(value,height)) {
            height = win_size[1];
          }
        }
      }}
      if(!args.find("no_tiling",no_tiling)) no_tiling = false;
      if(args.find("what",value)) what = value;}

      //COIN_FORCE_TILED_OFFSCREENRENDERING
      //COIN_DEBUG_SOOFFSCREENRENDERER
      //COIN_OFFSCREENRENDERER_TILEWIDTH
      //COIN_OFFSCREENRENDERER_TILEHEIGHT

      //SoDebugError::postInfo("GuiViewer_write",
      //  "window size %d %d. Wanted size %d %d. What %s. No tiling %d",
      //                       win_size[0],win_size[1],
      //                       width,height,what.c_str(),no_tiling);

      SoNode* node = GuiViewer_getWhat(aThis,what);
      if(!node) return false;

      std::string ow,oh;
      bool owis,ohis;

      if(no_tiling) {
        owis = inlib::getenv("COIN_OFFSCREENRENDERER_TILEWIDTH",ow);
        ohis = inlib::getenv("COIN_OFFSCREENRENDERER_TILEHEIGHT",oh);
	inlib::putenv("COIN_OFFSCREENRENDERER_TILEWIDTH",
                      inlib::to<unsigned int>(width));
	inlib::putenv("COIN_OFFSCREENRENDERER_TILEHEIGHT",
                      inlib::to<unsigned int>(height));
      }

      bool status = true; 

      SbViewportRegion vpr = aThis.getViewportRegion();
      vpr.setWindowSize(width,height);
      SoOffscreenRenderer osr(vpr);
      if(osr.render(node)==FALSE) {
        SoDebugError::postInfo("GuiViewer_write",
                               "SoOffscreenRenderer.render failed.");
        status = false;
      } else {
        SbVec2f printsize(8.5f, 11.0f); //A4
        if(osr.writeToPostScript(aFile.c_str(),printsize)==FALSE) {
          SoDebugError::postInfo("GuiViewer_write",
                             "SoOffscreenRenderer.writeToPostScript failed.");

          status = false;
        } else {
          SoDebugError::postInfo("GuiViewer_write",
                                 "%s produced.",
                                 aFile.c_str());
        }
      }

      if(no_tiling) {
        if(owis) inlib::putenv("COIN_OFFSCREENRENDERER_TILEWIDTH",ow);
        else inlib::rmenv("COIN_OFFSCREENRENDERER_TILEWIDTH");

        if(ohis) inlib::putenv("COIN_OFFSCREENRENDERER_TILEHEIGHT",oh);
        else inlib::rmenv("COIN_OFFSCREENRENDERER_TILEHEIGHT");
      }

      return status;

    } else {
      SoDebugError::postInfo("GuiViewer_write",
                             "unknown format \"%s\".",
                             aFormat.c_str());
    }
    return false;
}

//template <class SoGuiViewer> 
//inline bool GuiViewer_collect(SoGuiViewer& aThis,int aX,int aY,int aW,int aH){
//  const SbViewportRegion& vpRegion = aThis.getViewportRegion();
//  SoPage* soPage = GuiViewer_SoPage<SoGuiViewer>(aThis);
//  if(!soPage) return false;
//  soPage->collect(aX,aY,aW,aH,vpRegion);
//  return true;
//}

template <class SoGuiViewer> 
inline bool GuiViewer_map(SoGuiViewer& aThis,
                 const SbVec2s& aPos,  // Rectangle position (GL coordinates).
                 const SbVec2s& aSize, // Rectangle size.
                 const SbVec2s& aWindow) {
  SoPage* soPage = GuiViewer_SoPage<SoGuiViewer>(aThis);
  if(!soPage) return false;
  SoRegion* soRegion = soPage->getHighlightedRegion();
  if(!soRegion) return false;
  soRegion->pushCamera();
  soRegion->viewRectangle(aPos,aSize,aWindow);
  return true;
}

template <class SoGuiViewer> 
inline bool GuiViewer_isInCurrentRegion(SoGuiViewer& aThis,const SbVec2s& aPos) {
  // aPos is in GL coordinates.
  SoPage* soPage = GuiViewer_SoPage<SoGuiViewer>(aThis);
  if(!soPage) return false;
  SoRegion* soRegion = soPage->getHighlightedRegion();
  if(!soRegion) return false;
  SoViewportRegion* soViewportRegion = soRegion->getViewportRegion();
  int rx,ry;
  soViewportRegion->getPositionPixels(rx,ry);
  unsigned int rw,rh;
  soViewportRegion->getSizePixels(rw,rh);
  if(aPos[0]<int(rx)) return false;
  if(aPos[0]>=int(rx+rw)) return false;
  if(aPos[1]<int(ry)) return false;
  if(aPos[1]>=int(ry+rh)) return false;
  return true;
}

template <class SoGuiViewer> 
inline bool GuiViewer_pushCamera(SoGuiViewer& aThis) {
  SoPage* soPage = GuiViewer_SoPage<SoGuiViewer>(aThis);
  if(!soPage) return false;
  SoRegion* soRegion = soPage->getHighlightedRegion();
  if(!soRegion) return false;
  soRegion->pushCamera();
  return true;
}

template <class SoGuiViewer> 
inline bool GuiViewer_popCamera(SoGuiViewer& aThis) {
  SoPage* soPage = GuiViewer_SoPage<SoGuiViewer>(aThis);
  if(!soPage) return false;
  SoRegion* soRegion = soPage->getHighlightedRegion();
  if(!soRegion) return false;
  soRegion->popCamera();
  return true;
}

//uuuu

/*
  if(!aNode) {
    SoDebugError::postInfo("node_proxy","No scene graph.");
    return false;
  }
*/

inline bool node_proxy(
 SoNode& aNode
,const std::string& aWhat
,const std::vector<std::string>& aArgs
,void*& aReturn
){
  aReturn = 0;

  if(aWhat=="interest") {
    if(aArgs.size()!=1) {
      SoDebugError::postInfo("viewer_proxy",
                             "one arguments expected for keyword %s.",
                             aWhat.c_str());
      return false;
    }
    std::string interest = aArgs[0];

    if(aNode.isOfType(SoPage::getClassTypeId())==TRUE) {
      //interest : page, region, scene staticScene dynamicScene
      SoPage* soPage = (SoPage*)&aNode;
      if(interest=="page") {
        aReturn = soPage;
        return true;
      } else if(interest=="region") {
        //SoRegion* soRegion = soPage->getHighlightedRegion();
        SoRegion* soRegion = soPage->currentRegion();
        if(!soRegion) {
          SoDebugError::postInfo("viewer_proxy",
                                 "Can't get region.");
          return false;
        }
        aReturn = soRegion;
        return true;
      } else {
        //SoRegion* soRegion = soPage->getHighlightedRegion();
        SoRegion* soRegion = soPage->currentRegion();
        if(!soRegion) {
          SoDebugError::postInfo("viewer_proxy",
                                 "Can't get region.");
          return false;
        }
        SoNode* node = soRegion->getNodeOfInterest(interest.c_str());
        if(!node) {
          SoDebugError::postInfo("viewer_proxy",
                                 "Can't get interest node for \"%s\".",
                                 interest.c_str());
          return false;
        }
        aReturn = node;
        return true;
      }

    } else {

      if(interest=="scene") {
        aReturn = &aNode;
        return true;
      }

      SoDebugError::postInfo("viewer_proxy",
                             "unknown inerest keyword %s.",interest.c_str());
      return false;

    }
    return true;

  } else if(aWhat=="read_scene") {
    if(aArgs.size()!=3) {
      SoDebugError::postInfo("viewer_proxy",
                             "three arguments expected for keyword %s.",
                             aWhat.c_str());
      return false;
    }
    std::vector<std::string> files = inlib::words(aArgs[0],"\n");
    if(files.empty()) return true;

    //std::string format = aArgs[1];
    //std::string placement = aArgs[2];

    if(aNode.isOfType(SoPage::getClassTypeId())) {
      SoPage* soPage = (SoPage*)&aNode;
      SoRegion* soRegion = soPage->getHighlightedRegion();
      //SoRegion* soRegion = soPage->currentRegion();
      if(!soRegion) return false;
      if(soRegion->isOfType(SoImageRegion::getClassTypeId())) {
        SoImageRegion* soImageRegion = (SoImageRegion*)soRegion;
        HEPVis_SoImage* soImage = soImageRegion->getImage();

        for(std::vector<std::string>::iterator it = files.begin();
          it!=files.end();++it) {
          std::string format = aArgs[1];
          if(format=="guessed") format = inlib::suffix(*it);
          if(format=="gif")  {
            soImage->format.setValue(HEPVis_SoImage::GIF);
            soImage->fileName.setValue((*it).c_str());
          } else if( (format=="jpg") || (format=="jpeg"))   {
            soImage->format.setValue(HEPVis_SoImage::JPEG);
            soImage->fileName.setValue((*it).c_str());
          } else if( (format=="fits") || (format=="fit")) {
            soImage->format.setValue(HEPVis_SoImage::FITS);
            soImage->fileName.setValue((*it).c_str());
          } else {
            SoDebugError::postInfo("viewer_proxy",
                                   "image format %s not supported.",
                                   format.c_str());
          }
        }

      } else {

        //aArgs[1] not used.
        std::string placement = aArgs[2];
        for(std::vector<std::string>::iterator it = files.begin();
          it!=files.end();++it) {
          SoInput soInput;
          if(!soInput.openFile((*it).c_str())) return false;
          SoSeparator* separator = SoDB::readAll(&soInput); 
          if(!separator) return false;
          //separator ref count is 0.
          separator->ref();

          SbAddNode sbAdd(separator,placement.c_str());
          bool status = (soRegion->doIt(sbAdd)==TRUE?true:false);
          sbAdd.reset(); //to disconnect dstor unref.

          separator->unref();
          return status;
        }

      }
    } else if(aNode.isOfType(SoGroup::getClassTypeId())) {
      //aArgs[1] not used.
      //aArgs[2] not used.
      for(std::vector<std::string>::iterator it = files.begin();
        it!=files.end();++it) {
        SoInput soInput;
        if(!soInput.openFile((*it).c_str())) return false;
        SoSeparator* separator = SoDB::readAll(&soInput); 
        if(!separator) return false;
        //separator ref count is 0.
        ((SoGroup*)(&aNode))->addChild(separator);      
      }
    } else {
      return false;
    }
    return true;

  } else if(aWhat=="clear") {
    if(aArgs.size()!=1) {
      SoDebugError::postInfo("viewer_proxy",
                             "one argument expected for keyword %s.",
                             aWhat.c_str());
      return false;
    }
    if(aNode.isOfType(SoPage::getClassTypeId())) {
      SoPage* soPage = (SoPage*)&aNode;    
      SoRegion* soRegion = soPage->getHighlightedRegion();
      //SoRegion* soRegion = soPage->currentRegion();
      if(!soRegion) return false;
      std::string what = aArgs[0];
      soRegion->clear(what.c_str());
    } else if(aNode.isOfType(SoGroup::getClassTypeId())) {
      ((SoGroup*)&aNode)->removeAllChildren();      
    } else {
      return false;
    }
    return true;

  } else if(aWhat=="remove_manips") {

    if(aNode.isOfType(SoPage::getClassTypeId())) {
      SoPage* soPage = (SoPage*)&aNode;    
      SoRegion* soRegion = soPage->getHighlightedRegion();
      //SoRegion* soRegion = soPage->currentRegion();
      if(!soRegion) return false;
      if(!soRegion->isOfType(SoDisplayRegion::getClassTypeId())) return false;
      SoDisplayRegion* displayRegion = (SoDisplayRegion*)soRegion;
      SoSeparator* sep = (SoSeparator*)displayRegion->getSceneSeparator();
      SoTools::removeManips(*sep);
    } else {
      SoTools::removeManips(aNode);
    }
    return true;

  } else if(aWhat=="detect_intersections") {

    if(aArgs.size()!=2) {
      SoDebugError::postInfo("viewer_proxy",
                             "two arguments expected for keyword %s.",
                             aWhat.c_str());
      return false;
    }
    bool first;
    if(!inlib::to(aArgs[0],first)) {
      SoDebugError::postInfo("viewer_proxy",
                             "option \"%s\" not a boolean.",
                             aArgs[0].c_str());
      return false;
    }
    std::string what = aArgs[1];

    if(aNode.isOfType(SoPage::getClassTypeId())) {
      SoPage* soPage = (SoPage*)&aNode;    
      SoRegion* soRegion = soPage->getHighlightedRegion();
      //SoRegion* soRegion = soPage->currentRegion();
      if(!soRegion) return false;
      SoNode* node = soRegion->getNodeOfInterest(what.c_str());
      if(!node) {
        SoDebugError::postInfo("viewer_proxy",
                               "Can't get node of interest \"%s\".",
                               what.c_str());
        return false;
      }
      SoTools::removeManips(*node);
      if(first) {
        SoTools::detectIntersection(*node,TRUE);
      } else {
        SoTools::detectIntersection(*node,FALSE);
      }
    } else {
      SoTools::removeManips(aNode);
      if(first) {
        SoTools::detectIntersection(aNode,TRUE);
      } else {
        SoTools::detectIntersection(aNode,FALSE);
      }
    }
    return true;

  } else {
    SoDebugError::postInfo("node_proxy","bad keyword %s.",aWhat.c_str());
    return false;
  }

  return true;
}

template <class SoGuiFullViewer> 
inline bool viewer_proxy(
 void* aNativeViewer
,const std::string& aWhat
,const std::vector<std::string>& aArgs
,void*& aReturn
){
  SoGuiFullViewer* viewer = (SoGuiFullViewer*)aNativeViewer;
  aReturn = 0;

  if(aWhat=="write") {

    if(aArgs.size()!=3) {
      SoDebugError::postInfo("viewer_proxy",
                             "three arguments expected for keyword %s.",
                             aWhat.c_str());
      return false;
    }
    std::string file = aArgs[0];
    std::string format = aArgs[1];
    std::string opts = aArgs[2];

    return GuiViewer_write<SoGuiFullViewer>(*viewer,format,file,opts);

  } else if(aWhat=="render") {
    viewer->render();
    return true;

  } else if(aWhat=="view_all") {
    viewer->viewAll();
    return true;

  } else if(aWhat=="set_auto_clipping") {

    if(aArgs.size()!=1) {
      SoDebugError::postInfo("viewer_proxy",
                             "one arguments expected for keyword %s.",
                             aWhat.c_str());
      return false;
    }
    bool value;
    if(!inlib::to(aArgs[0],value)) {
      SoDebugError::postInfo("viewer_proxy",
                             "option \"%s\" not a boolean.",
                             aArgs[0].c_str());
      return false;
    }
    if(value) {
      if(viewer->isAutoClipping()==FALSE) viewer->setAutoClipping(TRUE);
    } else {
      if(viewer->isAutoClipping()==TRUE) viewer->setAutoClipping(FALSE);
    }
    return true;

  } else if(aWhat=="set_decoration") {
    if(aArgs.size()!=1) {
      SoDebugError::postInfo("viewer_proxy",
                             "one arguments expected for keyword %s.",
                             aWhat.c_str());
      return false;
    }
    bool value;
    if(!inlib::to(aArgs[0],value)) {
      SoDebugError::postInfo("viewer_proxy",
                             "option \"%s\" not a boolean.",
                             aArgs[0].c_str());
      return false;
    }

    if(value) {
      if(viewer->isDecoration()==FALSE) viewer->setDecoration(TRUE);
    } else {
      if(viewer->isDecoration()==TRUE) viewer->setDecoration(FALSE);
    }
    return true;

  } else if(aWhat=="set_viewing") {
    if(aArgs.size()!=1) {
      SoDebugError::postInfo("viewer_proxy",
                             "one arguments expected for keyword %s.",
                             aWhat.c_str());
      return false;
    }
    bool value;
    if(!inlib::to(aArgs[0],value)) {
      SoDebugError::postInfo("viewer_proxy",
                             "option \"%s\" not a boolean.",
                             aArgs[0].c_str());
      return false;
    }

    if(value) {
      if(viewer->isViewing()==FALSE) viewer->setViewing(TRUE);
    } else {
      if(viewer->isViewing()==TRUE) viewer->setViewing(FALSE);
    }
    return true;

  } else if(aWhat=="set_head_light") {
    if(aArgs.size()!=1) {
      SoDebugError::postInfo("viewer_proxy",
                             "one arguments expected for keyword %s.",
                             aWhat.c_str());
      return false;
    }
    bool value;
    if(!inlib::to(aArgs[0],value)) {
      SoDebugError::postInfo("viewer_proxy",
                             "option \"%s\" not a boolean.",
                             aArgs[0].c_str());
      return false;
    }

    if(value) {
      if(viewer->isHeadlight()==FALSE) viewer->setHeadlight(TRUE);
    } else {
      if(viewer->isHeadlight()==TRUE) viewer->setHeadlight(FALSE);
    }
    return true;

  } else if(aWhat=="set_smoothing") {
    SoGLRenderAction* action = viewer->getGLRenderAction();  
    if(!action) return false;

    if(aArgs.size()!=1) {
      SoDebugError::postInfo("viewer_proxy",
                             "one arguments expected for keyword %s.",
                             aWhat.c_str());
      return false;
    }
    bool value;
    if(!inlib::to(aArgs[0],value)) {
      SoDebugError::postInfo("viewer_proxy",
                             "option \"%s\" not a boolean.",
                             aArgs[0].c_str());
      return false;
    }

    if(value) {
      if(action->isSmoothing()==FALSE) action->setSmoothing(TRUE);
    } else {
      if(action->isSmoothing()==TRUE) action->setSmoothing(FALSE);
    }
    return true;

  } else if(aWhat=="set_background_color") {

    if(aArgs.size()!=1) {
      SoDebugError::postInfo("viewer_proxy",
                             "one arguments expected for keyword %s.",
                             aWhat.c_str());
      return false;
    }
    double r,g,b;
    if(!inlib::to_rgb(aArgs[0],r,g,b)) return false;
    viewer->setBackgroundColor(SbColor((float)r,(float)g,(float)b));
    return true;

  } else if(aWhat=="set_title") {
    if(aArgs.size()!=1) {
      SoDebugError::postInfo("viewer_proxy",
                             "one arguments expected for keyword %s.",
                             aWhat.c_str());
      return false;
    }
    viewer->setTitle(aArgs[0].c_str());
    return true;

  } else if(aWhat=="viewport_region") {

    const SbViewportRegion& vpr = viewer->getViewportRegion();
    aReturn = (void*)(&vpr);
    return true;

  } else if(aWhat=="getGLRenderAction") {

    SoGLRenderAction* action = viewer->getGLRenderAction();
    aReturn = (void*)action;
    return true;

  } else if(aWhat=="setGLRenderAction") {

    if(aArgs.size()!=1) {
      SoDebugError::postInfo("viewer_proxy",
                             "one arguments expected for keyword %s.",
                             aWhat.c_str());
      return false;
    }

    void* p;
    if(!inlib::to_pointer(aArgs[0],p)) {
      SoDebugError::postInfo("setGLRenderAction",
                             "option \"%s\" not a pointer.",
                             aArgs[0].c_str());
      return false;
    }

    viewer->setGLRenderAction((SoGLRenderAction*)p);
    return true;

  } else if(aWhat=="setTransparencyType") {
    if(aArgs.size()!=1) {
      SoDebugError::postInfo("setTransparencyType",
                             "one arguments expected for keyword %s.",
                             aWhat.c_str());
      return false;
    }

    int type;
    if(!inlib::to<int>(aArgs[0],type)) {
      SoDebugError::postInfo("setTransparencyType",
                             "option \"%s\" not an int.",
                             aArgs[0].c_str());
      return false;
    }

    viewer->setTransparencyType((SoGLRenderAction::TransparencyType)type);
    return true;

  } else if(aWhat=="transparencyType") {

    SoGLRenderAction::TransparencyType type = viewer->getTransparencyType();
    int* i = new int; //should be deleted by the caller.
    *i = (int)type;
    aReturn = (void*)i;
    return true;

  } else if(aWhat=="collect") {
    if(aArgs.size()!=4) {
      SoDebugError::postInfo("viewer_proxy",
                             "four arguments expected for keyword %s.",
                             aWhat.c_str());
      return false;
    }

    int x;
    if(!inlib::to<int>(aArgs[0],x)) {
      SoDebugError::postInfo("viewer_proxy",
                             "option \"%s\" not an int.",
                             aArgs[0].c_str());
      return false;
    }
    int y;
    if(!inlib::to<int>(aArgs[1],y)) {
      SoDebugError::postInfo("viewer_proxy",
                             "option \"%s\" not an int.",
                             aArgs[1].c_str());
      return false;
    }
    unsigned int w;
    typedef unsigned int uint_t;
    if(!inlib::to<uint_t>(aArgs[2],w)) {
      SoDebugError::postInfo("viewer_proxy",
                             "option \"%s\" not an unsigned int.",
                             aArgs[2].c_str());
      return false;
    }
    unsigned int h;
    if(!inlib::to<uint_t>(aArgs[3],h)) {
      SoDebugError::postInfo("viewer_proxy",
                             "option \"%s\" not an unsigned int.",
                             aArgs[3].c_str());
      return false;
    }

    const SbViewportRegion& vpr = viewer->getViewportRegion();

    SoNode* soNode = viewer->getSceneGraph();
    if(!soNode) return false;
    if(soNode->isOfType(SoPage::getClassTypeId())) {
      SoPage* soPage = (SoPage*)soNode;    
      soPage->collect(x,y,w,h,vpr);
    }
    return true;
  }

  SoNode* node = viewer->getSceneGraph();
  if(!node) {
    SoDebugError::postInfo("viewer_proxy","No scene graph.");
    return false;
  }

  return node_proxy(*node,aWhat,aArgs,aReturn);
}

template <class SoGuiFullViewer,class SoGuiExaminerViewer> 
inline bool viewer_proxy(
 void* aNativeViewer
,const std::string& aWhat
,const std::vector<std::string>& aArgs
,void*& aReturn
){
  aReturn = 0;

  SoGuiExaminerViewer* viewer = (SoGuiExaminerViewer*)aNativeViewer;

  if(aWhat=="set_feedback_visibility") {
    if(aArgs.size()!=1) {
      SoDebugError::postInfo("viewer_proxy",
                             "one arguments expected for keyword %s.",
                             aWhat.c_str());
      return false;
    }
    bool value;
    if(!inlib::to(aArgs[0],value)) {
      SoDebugError::postInfo("viewer_proxy",
                             "option \"%s\" not a boolean.",
                             aArgs[0].c_str());
      return false;
    }

    if(value) {
      if(viewer->isFeedbackVisible()==FALSE) 
        viewer->setFeedbackVisibility(TRUE);
    } else {
      if(viewer->isFeedbackVisible()==TRUE) 
        viewer->setFeedbackVisibility(FALSE);
    }
    return true;

  } else if(aWhat=="set_animation") {
    if(aArgs.size()!=1) {
      SoDebugError::postInfo("viewer_proxy",
                             "one arguments expected for keyword %s.",
                             aWhat.c_str());
      return false;
    }
    bool value;
    if(!inlib::to(aArgs[0],value)) {
      SoDebugError::postInfo("viewer_proxy",
                             "option \"%s\" not a boolean.",
                             aArgs[0].c_str());
      return false;
    }
    if(value) {
      if(viewer->isAnimationEnabled()==FALSE) 
        viewer->setAnimationEnabled(TRUE);
    } else {
      if(viewer->isAnimationEnabled()==TRUE) 
        viewer->setAnimationEnabled(FALSE);
    }
    return true;

  } else if(aWhat=="stop_animating") {
    viewer->stopAnimating();
    return true;
  }

  return viewer_proxy<SoGuiFullViewer>(aNativeViewer,aWhat,aArgs,aReturn);
}



#endif
