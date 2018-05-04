# hapi is for Handy API.

import AIDA

# Do the below try/execpt in case of : 
#   OS> python -i hapi.py
#   >>> import hapi
# In this case the 'import hapi' reexecutes
# the content of hapi and then g_aida is no
# more a singleton.
try:
  import __main__
  g_aida = __main__.g_aida
except AttributeError:
  g_aida = AIDA.createAnalysisFactory()

# in principle the upper g_aida is going
# to be deleted after all the instances
# of the below classes.

class H1D:
  def __init__(self,a_name,a_title,a_n,a_min,a_max):
    fac = g_aida.createTreeFactory()
    tree = fac.create()
    del fac
    fac = g_aida.createHistogramFactory(tree)
    self.m_h = fac.createHistogram1D(a_name,a_title,a_n,a_min,a_max)
    del fac
  def __del__(self):
    del self.m_h
  def fill(self,a_x, a_weight = 1):
    self.m_h.fill(a_x,a_weight)
  #INTERNAL:
  def get_m_h(self):
    return self.m_h;

class H2D:
  def __init__(self,a_name,a_title,a_xn,a_xmin,a_xmax,a_yn,a_ymin,a_ymax):
    fac = g_aida.createTreeFactory()
    tree = fac.create()
    del fac
    fac = g_aida.createHistogramFactory(tree)
    self.m_h = fac.createHistogram2D(a_name,a_title,a_xn,a_xmin,a_xmax,a_yn,a_ymin,a_ymax)
    del fac
  def __del__(self):
    del self.m_h
  def fill(self,a_x,a_y, a_weight = 1):
    self.m_h.fill(a_x,a_y,a_weight)
  #INTERNAL:
  def get_m_h(self):
    return self.m_h;

class Plotter:
  def __init__(self,a_title = ''):
    fac = g_aida.createPlotterFactory()
    self.m_plotter = fac.create('')
    del fac
    self.m_plotter.createRegions(1,1,0)
    self.m_plotter.setTitle(a_title)
    self.m_plotter.show()
  def __del__(self):
    del self.m_plotter
  #INTERNAL:
  def get_m_plotter(self):
    return self.m_plotter;

  def show(self):
    self.m_plotter.show()
  def interact(self):
    self.m_plotter.interact()
  def plot(self,a_histo,a_options = ''):
    region = self.m_plotter.currentRegion()
    region.plot(a_histo.get_m_h(),a_options)
    self.m_plotter.refresh()
    import OnX
    ui = OnX.session().ui()
    if ui != None : OnX.set_PyOS_InputHook(ui)
  def clear(self):
    region = self.m_plotter.currentRegion()
    region.clear()
  def title(self,a_title):
    region = self.m_plotter.currentRegion()
    region.setTitle(a_title)

  def x_label(self,a_label):
    region = self.m_plotter.currentRegion()
    region.style().xAxisStyle().setLabel(a_label)
  def x_log(self):
    region = self.m_plotter.currentRegion()
    region.setParameter('plotter.xAxisLogScale','TRUE')

  def y_label(self,a_label):
    region = self.m_plotter.currentRegion()
    region.style().yAxisStyle().setLabel(a_label)
  def y_log(self):
    region = self.m_plotter.currentRegion()
    region.setParameter('plotter.yAxisLogScale','TRUE')

  def show_grid(self):
    region = self.m_plotter.currentRegion()
    region.setParameter('plotter.gridStyle.visible','TRUE')
  def hide_grid(self):
    region = self.m_plotter.currentRegion()
    region.setParameter('plotter.gridStyle.visible','FALSE')

  def hide_wall(self):
    region = self.m_plotter.currentRegion()
    region.setParameter('plotter.wallStyle.visible','FALSE')

class File:
  def __init__(self,a_file,a_format):
    fac = g_aida.createTreeFactory()
    # 1 = read only
    # 0 = create new false
    self.m_tree = fac.create(a_file,a_format,1,0)
    del fac
  def __del__(self):
    del self.m_tree

  def get(self,a_what):
    return self.m_tree.find(a_what)

  def evaluator(self,a_expression):
    fac = g_aida.createTupleFactory(self.m_tree)
    obj = fac.createEvaluator(a_expression)
    del fac
    return obj

  def filter(self,a_expression):
    fac = g_aida.createTupleFactory(self.m_tree)
    obj = fac.createFilter(a_expression)
    del fac
    return obj


