{
  if($0=="      ((const SbMatrix*)(G__getstructoffset()))->getValue(libp->para[0].ref?*(SbMat***)libp->para[0].ref:*(SbMat***)(&G__Mlong(libp->para[0])));") {
      print "      ((const SbMatrix*)(G__getstructoffset()))->getValue(libp->para[0].ref?*(SbMat***)libp->para[0].ref:*(SbMat***)(&G__Mlong(libp->para[0])));";
  } else {
    print $0;
  }
}
