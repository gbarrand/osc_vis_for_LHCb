// included by Polyhedron.cxx

  // From LHCb Rich1 debugging :
 {double mm = 0.001;
  //double mm = 1.;
  double GasDownStreamBoxXSize = 2600.0 * mm;
  double GasDownStreamBoxYSize = 2400.0 * mm;
  double GasDownStreamBoxZSize = 490.0 * mm;

  double GasTrapezXStartSize = 860.0 * mm;
  double GasTrapezYStartSize = 1200.0 * mm;
  double GasTrapezXEndSize = GasDownStreamBoxXSize;
  double GasTrapezYEndSize = GasTrapezYStartSize;
  double GasTrapezZSize = 440.0 * mm;
  double GasTrapezX = 0.0*mm;
  double GasTrapezY = 0.0*mm;
  double GasTrapezZ = -(GasDownStreamBoxZSize+GasTrapezZSize)/2.0;

  double GasUpStreamBoxXSize = GasTrapezXStartSize;
  double GasUpStreamBoxYSize = GasTrapezYStartSize;
  double GasUpStreamBoxZSize = 80.0*mm;
  double GasUpStreamBoxX = 0.0*mm;
  double GasUpStreamBoxY = 0.0*mm;
  double GasUpStreamBoxZ = 
    -(GasDownStreamBoxZSize/2.0+GasTrapezZSize+GasUpStreamBoxZSize/2.0);

  double GasTopBoxXSize = GasDownStreamBoxXSize;
  double GasTopBoxYSize = (GasDownStreamBoxYSize-GasTrapezYEndSize)/2.0;
  double GasTopBoxZSize = GasTrapezZSize;

  double GasTopBoxX = 0.0*mm;
  double GasTopBoxY = (GasTrapezYEndSize+GasTopBoxYSize)/2.0;
  double GasTopBoxZ = GasTrapezZ;

  double GasBotBoxXSize = GasDownStreamBoxXSize;
  double GasBotBoxYSize = (GasDownStreamBoxYSize-GasTrapezYEndSize)/2.0;
  double GasBotBoxZSize = GasTrapezZSize;
  double GasBotBoxX = 0.0*mm;
  double GasBotBoxY = -(GasTrapezYEndSize+GasBotBoxYSize)/2.0;
  double GasBotBoxZ = GasTrapezZ;

  SbPolyhedronBox box7_1(GasDownStreamBoxXSize/2,
                         GasDownStreamBoxYSize/2,
                         GasDownStreamBoxZSize/2);
  SbPolyhedron op7(box7_1);
  SbPolyhedronBox box7_2(GasTopBoxXSize/2,
                         GasTopBoxYSize/2,
                         GasTopBoxZSize/2);
  box7_2.Transform(HEPVis::SbRotation(),SbVec3d(GasTopBoxX,GasTopBoxY,GasTopBoxZ));
  op7 = op7.add(box7_2);
  SbPolyhedronBox box7_3(GasBotBoxXSize/2,
                         GasBotBoxYSize/2,
                         GasBotBoxZSize/2);
  box7_3.Transform(HEPVis::SbRotation(),SbVec3d(GasBotBoxX,GasBotBoxY,GasBotBoxZ));
  op7 = op7.add(box7_3);
  SbPolyhedronBox box7_4(GasUpStreamBoxXSize/2,
                         GasUpStreamBoxYSize/2,
                         GasUpStreamBoxZSize/2);
  box7_4.Transform(HEPVis::SbRotation(),
                   SbVec3d(GasUpStreamBoxX,GasUpStreamBoxY,GasUpStreamBoxZ));
  op7 = op7.add(box7_4);
  SbPolyhedronTrd2 trd7(GasTrapezXStartSize/2,
                        GasTrapezXEndSize/2,
                        GasTrapezYStartSize/2,
                        GasTrapezYEndSize/2,
                        GasTrapezZSize/2);
  trd7.Transform(HEPVis::SbRotation(),SbVec3d(GasTrapezX,GasTrapezY,GasTrapezZ));
  op7 = op7.add(trd7);
  SoPolyhedron* polyhedron7 = new SoPolyhedron(op7);
  polyhedron7->solid.setValue(solid);
  polyhedron7->reducedWireFrame.setValue(reducedWireFrame);
  sceneGraph->addChild(polyhedron7);}

  // A union of four boxes :
  SoTranslation* translation6 = new SoTranslation();
  translation6->translation.setValue(SbVec3f(6,0,0));
  sceneGraph->addChild(translation6);

  double mm = 0.01;
  double epsilon = 0.1;
  SbPolyhedronBox box6_1(280/2*mm,20/2*mm,72/2*mm);
  SbPolyhedron op6(box6_1);
  SbPolyhedronBox box6_2(10/2*mm,76/2*mm,62/2*mm);
  box6_2.Transform(HEPVis::SbRotation(),SbVec3d((-270/2+10)*mm,
                                          ((75+1)/2+10+epsilon)*mm,
                                          0));
  //SbPolyhedron op6_1 = box6_1.add(box6_2);
  op6 = op6.add(box6_2);
  SbPolyhedronBox box6_3(10/2*mm,76/2*mm,62/2*mm);
  box6_3.Transform(HEPVis::SbRotation(),SbVec3d((270/2-10)*mm,
                                          ((75+1)/2+10+epsilon)*mm,
                                          0));
  //SbPolyhedron op6_2 = op6_1.add(box6_3);
  op6 = op6.add(box6_3);

  SbPolyhedronBox box6_4(270/2*mm,20/2*mm,62/2*mm);
  box6_4.Transform(HEPVis::SbRotation(),SbVec3d(0,100*mm,5*mm));
  //SbPolyhedron op6_3 = op6_2.add(box6_4);
  op6 = op6.add(box6_4);

  //SoPolyhedron* polyhedron6 = new SoPolyhedron(op6_3);
  SoPolyhedron* polyhedron6 = new SoPolyhedron(op6);
  polyhedron6->solid.setValue(solid);
  polyhedron6->reducedWireFrame.setValue(reducedWireFrame);
  sceneGraph->addChild(polyhedron6);

  // A U shape :
  SoMaterial* material1 = new SoMaterial;
  material1->diffuseColor.setValue(1, 0, 0);
  sceneGraph->addChild(material1);

  SoTranslation* translation1 = new SoTranslation();
  translation1->translation.setValue(SbVec3f(3.5,0,0));
  sceneGraph->addChild(translation1);

  SbPolyhedronBox box1_1(1,1,1);
  SbPolyhedronBox box1_2(0.5,.5,2);
  //box1_2.Transform(HEPVis::SbRotation(),SbVec3d(1,0,0));
  SoPolyhedron* polyhedron1 = new SoPolyhedron(box1_1.subtract(box1_2));
  polyhedron1->solid.setValue(solid);
  polyhedron1->reducedWireFrame.setValue(reducedWireFrame);
  sceneGraph->addChild(polyhedron1);

  // A lense done with intersection of two shperes :
  SoMaterial* material2 = new SoMaterial;
  material2->diffuseColor.setValue(1, 1, 0);
  sceneGraph->addChild(material2);

  SoTranslation* translation2 = new SoTranslation();
  translation2->translation.setValue(SbVec3f(1.5,0,0));
  sceneGraph->addChild(translation2);

  SbPolyhedronSphere sphere2_1(.01,1,0,2*M_PI,0,M_PI);
  SbPolyhedronSphere sphere2_2(.0F,1,0,2*M_PI,0,M_PI);
  sphere2_2.Transform(HEPVis::SbRotation(),SbVec3d(1.5,0,0));
  SoPolyhedron* polyhedron2 = 
    new SoPolyhedron(sphere2_1.intersect(sphere2_2));
  polyhedron2->solid.setValue(solid);
  polyhedron2->reducedWireFrame.setValue(reducedWireFrame);
  sceneGraph->addChild(polyhedron2);

  // A T with a cylinder hole :
  SoMaterial* material3 = new SoMaterial;
  material3->diffuseColor.setValue(0, 1, 1);
  sceneGraph->addChild(material3);

  SoTranslation* translation3 = new SoTranslation();
  translation3->translation.setValue(SbVec3f(2.5,0,0));
  sceneGraph->addChild(translation3);

  SbPolyhedronBox box3_1(1,1,1);
  SbPolyhedronBox box3_2(2,0.25,1);
  box3_2.Transform(HEPVis::SbRotation(),SbVec3d(0,-1.25,0));
  SbPolyhedron op3_1 = box3_1.add(box3_2);
  SbPolyhedronTubs tubs3(0,0.5,2,0,2*M_PI);
  tubs3.Transform(HEPVis::SbRotation(SbVec3d(0,1,0),M_PI_2),SbVec3d(0,0,0));
  SbPolyhedron op3_2 = op3_1.subtract(tubs3);
  SoPolyhedron* polyhedron3 = new SoPolyhedron(op3_2);
  polyhedron3->solid.setValue(solid);
  polyhedron3->reducedWireFrame.setValue(reducedWireFrame);
  sceneGraph->addChild(polyhedron3);

  // Screw head :
  SoMaterial* material4 = new SoMaterial;
  material4->diffuseColor.setValue(0, 1, 0);
  sceneGraph->addChild(material4);

  SoTranslation* translation4 = new SoTranslation();
  translation4->translation.setValue(SbVec3f(2.5,0,0));
  sceneGraph->addChild(translation4);

  int nstep = 128;
  SbPolyhedronTubs tubs4(0,1,1,0,2*M_PI,nstep);
  SbPolyhedronCone cone4(0,2,0,0,1,nstep);
  cone4.Transform(HEPVis::SbRotation(),SbVec3d(0,0,-1.1));
  SbPolyhedron op4_1 = tubs4.add(cone4);
  SoPolyhedron* polyhedron4 = new SoPolyhedron(op4_1);
  polyhedron4->solid.setValue(solid);
  polyhedron4->reducedWireFrame.setValue(reducedWireFrame);
  sceneGraph->addChild(polyhedron4);

  // A Tube with a transvers hole :
  SoTranslation* translation5 = new SoTranslation();
  translation5->translation.setValue(SbVec3f(3.5,0,0));
  sceneGraph->addChild(translation5);

  SbPolyhedronTubs tubs5_1(0.7,1.5,2,0,2*M_PI);
  SbPolyhedronTubs tubs5_2(  0,0.5,4,0,2*M_PI);
  tubs5_2.Transform(HEPVis::SbRotation(SbVec3d(0,1,0),M_PI/2.),SbVec3d(0,0,0));
  SbPolyhedron op5_1 = tubs5_1.subtract(tubs5_2);
  SoPolyhedron* polyhedron5 = new SoPolyhedron(op5_1);
  polyhedron5->solid.setValue(solid);
  polyhedron5->reducedWireFrame.setValue(reducedWireFrame);
  sceneGraph->addChild(polyhedron5);

  // LHCb Rich2 debugging :
  SoTranslation* translation8 = new SoTranslation();
  translation8->translation.setValue(SbVec3f(3.5,0,0));
  sceneGraph->addChild(translation8);

 {double mm = 0.001;
  double rad = 1;

  //frame->fNDivision = 8;
  //frame->fDivisionLength = (float)1000 * mm;

  double Rh2SphMirrorInnerR = 8600*mm;
  double Rh2SphMirrorOuterR = 8606*mm;

  // Rich2SphMirrorHH :
  double innerRadius = Rh2SphMirrorInnerR;
  double outerRadius = Rh2SphMirrorOuterR;
  double deltaThetaAngle = 0.06*rad;
  SbPolyhedronSphere sphere8_1(innerRadius,
                               outerRadius,
                               0,2*M_PI,0,deltaThetaAngle);

  // Rich2HexTrd0 :
  double sizeX1 = 251*mm;
  double sizeX2 = 753*mm;
  double sizeY1 = 200*mm;
  double sizeY2 = 200*mm;
  double sizeZ = 434.745*mm;
  SbPolyhedronTrd2 trd8_1(sizeX1,sizeX2,sizeY1,sizeY2,sizeZ);
  trd8_1.Transform(HEPVis::SbRotation(SbVec3d(1,0,0),M_PI/2.),SbVec3d(0,0,0));
  trd8_1.Transform(HEPVis::SbRotation(),SbVec3d(0,0,Rh2SphMirrorInnerR));
  // Rich2HexTrd1 :
  sizeX1 = 753*mm;
  sizeX2 = 251*mm;
  sizeY1 = 200*mm;
  sizeY2 = 200*mm;
  sizeZ = 434.745*mm;
  SbPolyhedronTrd2 trd8_2(sizeX1,sizeX2,sizeY1,sizeY2,sizeZ);
  trd8_2.Transform(HEPVis::SbRotation(SbVec3d(1,0,0),M_PI/2.),SbVec3d(0,0,0));
  trd8_2.Transform(HEPVis::SbRotation(),SbVec3d(0,0,Rh2SphMirrorInnerR));
  SbPolyhedron op8_1 = trd8_1.add(trd8_2);
  SbPolyhedron op8_2 = op8_1.add(sphere8_1);
  //FIXME : below does not work.
  //SbPolyhedron op8_1 = sphere8_1.add(trd8_1);
  //SbPolyhedron op8_2 = op8_1.add(trd8_2);

  // Rich2SphMirrorHex :
  //SoPolyhedron* polyhedron8 = new SoPolyhedron(sphere8_1);
  SoPolyhedron* polyhedron8 = new SoPolyhedron(op8_2);
  polyhedron8->solid.setValue(solid);
  polyhedron8->reducedWireFrame.setValue(reducedWireFrame);

  sceneGraph->addChild(polyhedron8);
 }

 {// A lense done with intersection of two shperes :
  SoMaterial* material2 = new SoMaterial;
  material2->diffuseColor.setValue(0, 1, 1);
  sceneGraph->addChild(material2);

  SoTranslation* translation2 = new SoTranslation();
  translation2->translation.setValue(SbVec3f(2,0,0));
  sceneGraph->addChild(translation2);

  SoScale* scale = new SoScale();
  scale->scaleFactor.setValue(SbVec3f(1,2,3));
  sceneGraph->addChild(scale);

  SbPolyhedronSphere sphere(0,1,0,2*M_PI,0,M_PI);
  SoPolyhedron* polyhedron = new SoPolyhedron(sphere);
  polyhedron->solid.setValue(solid);
  polyhedron->reducedWireFrame.setValue(reducedWireFrame);
  sceneGraph->addChild(polyhedron);}

 {// JEC debugging :
  SoTranslation* translation2 = new SoTranslation();
  translation2->translation.setValue(SbVec3f(2,0,0));
  sceneGraph->addChild(translation2);

  SoScale* scale = new SoScale();
  scale->scaleFactor.setValue(SbVec3f(0.01,0.01,0.01));
  sceneGraph->addChild(scale);

  //G4Box* box = new G4Box("Box",20*mm,30*mm,40*mm);
  //G4Tubs* cyl = new G4Tubs("Cylinder",0,50*mm,50*mm,0,twopi);
  //SbPolyhedronTubs tubs(0,10,50,0,2*M_PI);

  SbPolyhedronBox box(20,30,40);
  SbPolyhedronTube tube(0,10,50);
  SbPolyhedron op = box.subtract(tube);

  //SbPolyhedronTube tubs(0,30,40);
  //SbPolyhedronBox box(10,10,50);
  //SbPolyhedron op = tubs.subtract(box);

  SoPolyhedron* polyhedron = new SoPolyhedron(op);
  polyhedron->solid.setValue(solid);
  polyhedron->reducedWireFrame.setValue(reducedWireFrame);
  sceneGraph->addChild(polyhedron);
 }

