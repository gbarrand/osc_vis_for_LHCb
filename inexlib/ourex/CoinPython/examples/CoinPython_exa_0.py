
import CoinPython as Inventor

Inventor.SoDB.init()

root = Inventor.SoSeparator()

camera = Inventor.SoPerspectiveCamera()
root.addChild(camera)

scale = Inventor.SoScale()
scale.scaleFactor.setValue(Inventor.SbVec3f(0.1,0.1,0.1))
root.addChild(scale)

Inventor.SoNodeKit.init()
appearanceKit = Inventor.SoAppearanceKit()
root.addChild(appearanceKit)

writeAction = Inventor.SoWriteAction()
writeAction.getOutput().setBinary(Inventor.FALSE)
writeAction.apply(root)

del writeAction
del root
